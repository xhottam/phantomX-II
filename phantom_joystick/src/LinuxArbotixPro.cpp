#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include "LinuxArbotixPro.h"
#include <errno.h>


LinuxArbotixPro::LinuxArbotixPro(const char* name)
{
	DEBUG_COM = false;
	m_Socket_fd = -1;
	m_PacketStartTime = 0;
	m_PacketWaitTime = 0;
	m_UpdateStartTime = 0;
	m_UpdateWaitTime = 0;
	m_ByteTransferTime = 0;

	sem_init(&m_LowSemID, 0, 1);
	sem_init(&m_MidSemID, 0, 1);
	sem_init(&m_HighSemID, 0, 1);

	SetPortName(name);
}

LinuxArbotixPro::~LinuxArbotixPro()
{
	ClosePort();
}

void LinuxArbotixPro::SetPortName(const char* name)
{
	strcpy(m_PortName, name);
}

bool LinuxArbotixPro::OpenPort(bool loglevel)
{
	struct termios newtio;
	struct serial_struct serinfo;
	//double baudrate = 1000000.0; //bps (1Mbps)
        double baudrate = 57600.0; //bps (1Mbps)
	DEBUG_COM = loglevel;

	ClosePort();

	if (DEBUG_COM == true){
		printf("\n%s open ", m_PortName);
		printf(" \n%s", baudrate);
  }
	if ((m_Socket_fd = open(m_PortName, O_RDWR )) < 0)
		goto UART_OPEN_ERROR;

	if (DEBUG_COM == true)
		printf("success!\n");

	// You must set 38400bps!
	memset(&newtio, 0, sizeof(newtio));
        newtio.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
        newtio.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
        newtio.c_cflag &= ~CSIZE; // Clear all bits that set the data size
        newtio.c_cflag |= CS8; // 8 bits per byte (most common)
        newtio.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
        newtio.c_cflag |= CREAD | CLOCAL; // Turn on REA

        newtio.c_lflag &= ~ICANON;
        newtio.c_lflag &= ~ECHO; // Disable echo
        newtio.c_lflag &= ~ECHOE; // Disable erasure
        newtio.c_lflag &= ~ECHONL; // Disable new-line echo
        newtio.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
        newtio.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
        newtio.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

        newtio.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        newtio.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
        // newtio.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
        // newtio.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

        newtio.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
        newtio.c_cc[VMIN] = 0;

        // Set in/out baud rate to be 9600
        cfsetispeed(&newtio, B57600);
        cfsetospeed(&newtio, B57600);

	if (tcsetattr(m_Socket_fd, TCSANOW, &newtio) != 0 ){
		if (DEBUG_COM == true){
  		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		}
        }

/*
        if (ioctl(m_Socket_fd, TIOCGSERIAL, &serinfo) < 0)
                goto UART_OPEN_ERROR;

        //udevadm info -a -p  $(udevadm info -q path -n /dev/ttyUSB0) | grep latency
        //ATTRS{latency_timer}=="16" TO  ATTRS{latency_timer}=="1"
        serinfo.flags |= ASYNC_LOW_LATENCY;

        if (ioctl(m_Socket_fd, TIOCSSERIAL, &serinfo) < 0)
                {
                        if (DEBUG_COM == true)
                                printf("failed!\n");
                        goto UART_OPEN_ERROR;
                }
*/

        tcflush(m_Socket_fd, TCIFLUSH);
	m_ByteTransferTime = (1000.0 / baudrate) * 12.0;
	return true;

UART_OPEN_ERROR:
	if (DEBUG_COM == true)
		printf("failed!\n");
	ClosePort();
	return false;
}

bool LinuxArbotixPro::SetBaud(int baud)
{
	struct serial_struct serinfo;
	int baudrate = (int)(2000000.0f / (float)(baud + 1));

	if (m_Socket_fd == -1)
		return false;

	if (ioctl(m_Socket_fd, TIOCGSERIAL, &serinfo) < 0)
		{
			fprintf(stderr, "Cannot get serial info\n");
			return false;
		}

	serinfo.flags &= ~ASYNC_SPD_MASK;
	serinfo.flags |= ASYNC_SPD_CUST;
	serinfo.custom_divisor = serinfo.baud_base / baudrate;

	if (ioctl(m_Socket_fd, TIOCSSERIAL, &serinfo) < 0)
		{
			fprintf(stderr, "Cannot set serial info\n");
			return false;
		}

	ClosePort();
	OpenPort(false);

	m_ByteTransferTime = (float)((1000.0f / baudrate) * 12.0f * 8);

	return true;
}

void LinuxArbotixPro::ClosePort()
{
	if (m_Socket_fd != -1)
		close(m_Socket_fd);
	m_Socket_fd = -1;
}

void LinuxArbotixPro::FlushPort()
{
	if (m_Socket_fd != -1)
		tcdrain(m_Socket_fd);
}

void LinuxArbotixPro::ClearPort()
{
	tcflush(m_Socket_fd, TCIFLUSH);
}

int LinuxArbotixPro::WritePort(unsigned char* packet, int numPacket)
{
	return write(m_Socket_fd, packet, numPacket);
}

int LinuxArbotixPro::ReadPort(unsigned char* packet, int numPacket)
{
	return read(m_Socket_fd, packet, numPacket);
}

void LinuxArbotixPro::LowPriorityWait()
{
	sem_wait(&m_LowSemID);
}

void LinuxArbotixPro::MidPriorityWait()
{
	sem_wait(&m_MidSemID);
}

void LinuxArbotixPro::HighPriorityWait()
{
	sem_wait(&m_HighSemID);
}

void LinuxArbotixPro::LowPriorityRelease()
{
	sem_post(&m_LowSemID);
}

void LinuxArbotixPro::MidPriorityRelease()
{
	sem_post(&m_MidSemID);
}

void LinuxArbotixPro::HighPriorityRelease()
{
	sem_post(&m_HighSemID);
}

double LinuxArbotixPro::GetCurrentTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return ((double)tv.tv_sec * 1000.0 + (double)tv.tv_usec / 1000.0);
}

void LinuxArbotixPro::SetPacketTimeout(int lenPacket)
{
	m_PacketStartTime = GetCurrentTime();
	m_PacketWaitTime = m_ByteTransferTime * (double)lenPacket + 5.0;
}

bool LinuxArbotixPro::IsPacketTimeout()
{
	if (GetPacketTime() > m_PacketWaitTime)
		return true;

	return false;
}

double LinuxArbotixPro::GetPacketTime()
{
	double time;

	time = GetCurrentTime() - m_PacketStartTime;
	if (time < 0.0)
		m_PacketStartTime = GetCurrentTime();

	return time;
}

void LinuxArbotixPro::SetUpdateTimeout(int msec)
{
	m_UpdateStartTime = GetCurrentTime();
	m_UpdateWaitTime = msec;
}

bool LinuxArbotixPro::IsUpdateTimeout()
{
	if (GetUpdateTime() > m_UpdateWaitTime)
		return true;

	return false;
}

double LinuxArbotixPro::GetUpdateTime()
{
	double time;

	time = GetCurrentTime() - m_UpdateStartTime;
	if (time < 0.0)
		m_UpdateStartTime = GetCurrentTime();

	return time;
}

void LinuxArbotixPro::Sleep(int Miliseconds)
{
	double time = GetCurrentTime();

	do
		{
			usleep(Miliseconds * 1000);
		}
	while (GetCurrentTime() - time < (double)Miliseconds);
}
