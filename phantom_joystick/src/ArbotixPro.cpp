/*
 *   ArbotixPro.cpp
 *
 *   Author: ROBOTIS
 *
 */
#include <stdio.h>
#include "ArbotixPro.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>



ArbotixPro::ArbotixPro(PlatformArbotixPro *platform)
{
	m_Platform  = platform;
	DEBUG_COM = false;
        DEBUG_JOINTS= false;
}

ArbotixPro::~ArbotixPro()
{
	Disconnect();
	exit(0);
}

bool ArbotixPro::Connect()
{
	if (m_Platform->OpenPort(DEBUG_COM) == false)
		{
			fprintf(stderr, "\n Fail to open port\n");
			fprintf(stderr, " Arbotix Pro is used by another program or do not have root privileges.\n\n");
			return false;
		}else {
    	                return true;
  	}
}

void ArbotixPro::Disconnect()
{
// do action upon disconnect
	unsigned char txpacket[] = {0xFF, 0xFF, 0xC8, 0x05, 0x03, 0x1A, 0xE0, 0x03, 0x32};
	m_Platform->WritePort(txpacket, 9);
	m_Platform->ClosePort();
}

int ArbotixPro::MakeWord(int lowbyte, int highbyte)
{
        unsigned short word;
        word = highbyte;
        word = word << 8;
        word = word + lowbyte;
        return (int)word;
}

void  ArbotixPro::timestamp()
{
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    printf("%s",asctime( localtime(&ltime) ) );
}

int ArbotixPro::checkSumatory(unsigned char  data[], int length)
{
    int cs = 0;
    for (int i = 2 ; i < length; i++)
    {
        cs += data[i];
    }
    return (int)~cs;
}

void ArbotixPro::TxRx_CM530(int rightY,int rightX,int leftY,int leftX,int buttons, int checksum,unsigned char *rxpacket[]){

				unsigned char txpacket[9];

        txpacket[0]  = 0xFF;
        txpacket[1]  = 0x55;
        txpacket[2]  = rightY;
        txpacket[3]  = rightX;
        txpacket[4]  = leftY;
        txpacket[5]  = leftX;
        txpacket[6]  = buttons;
        txpacket[7]  = 0;
        txpacket[8]  = checksum;

				_TxRx_CM530(txpacket,rxpacket,2);


				if (DEBUG_JOINTS){
				      int rxindex = 2;
				      int signo = 0;
				      for (int j=0; j < 6 ; j++){
								rxindex++;
				        if(rxpacket[rxindex++] == 0){
				                signo = -1;
				        }else{
				                signo = 1;
				        }
				        printf("COXA [%i]: %i",j,signo * ArbotixPro::MakeWord((int)rxpacket[rxindex++],(int)rxpacket[rxindex++]));

				        if(rxpacket[rxindex++] == 0){
				                signo = -1;
				        }else{
				                signo = 1;
				        }
								printf(" | FEMUR[%i]: %i",j,signo * ArbotixPro::MakeWord((int)rxpacket[rxindex++],(int)rxpacket[rxindex++]));

				        if(rxpacket[rxindex++] == 0){
				                signo = -1;
				        }else{
				                signo = 1;
				        }
								printf(" | TIBIA[%i]: %i\n",j,signo * ArbotixPro::MakeWord((int)rxpacket[rxindex++],(int)rxpacket[rxindex++]));
				      }
				  }
}

void ArbotixPro::_TxRx_CM530(unsigned char *txpacket,unsigned char *rxpacket[],int priority){

        if (priority > 1)
                m_Platform->LowPriorityWait();
        if (priority > 0)
                m_Platform->MidPriorityWait();
        m_Platform->HighPriorityWait();

        m_Platform->ClearPort();
        m_Platform->WritePort(txpacket, 9);
        m_Platform->FlushPort();

        int res = 0;
        int get_length = 0;
        int to_length = 63;

        unsigned char read_buf[63];
        memset(&read_buf, '\0', sizeof(read_buf));


				while(1){
				        //int length = m_Platform->ReadPort(&rxpacket[get_length], to_length - get_length);
				        int length = m_Platform->ReadPort(&read_buf[get_length], to_length - get_length);
				        if ( length == -1){
				            printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
				            exit(0);
				        }
				        get_length += length;
				        if (get_length == to_length){
				        	// Find packet header
				          int i;
				          for (i = 0; i < (get_length - 1); i++){
				          	if (read_buf[i] == 0xFF && read_buf[i + 1] == 0xFF){
				          		//printf("Find packet header\n");
				            	//printf("find ->");
				            	//timestamp();
				            	break;
				            }else if (i == (get_length - 2) && read_buf[get_length - 1] == 0xFF){
				            	//printf ("Indice %i rxpacket[%i] == 0\n",i,get_length -1);
				              break;
				            }
				      		}
				          if (i == 0){
				          	// Check checksum
				            //unsigned char checksum = CalculateChecksum(rxpacket);
				            //printf("Chequeamos checksum\n");
				            unsigned char checksum = checkSumatory(read_buf,62);
				            if (read_buf[get_length - 1] == checksum){
				            	//printf("checksum SUCCESS\n");
				              /**for (int j = 0; j < 50; j++){
				              rintf("DATA: %x\n",rxpacket[j]);
				              }*/
				              res = SUCCESS;
				        	}else{
				          	res = RX_CORRUPT;
				            //printf("checksum KO\n");
				          }
				          break;
				        }else {
				        	//printf("try -> ");
				          //timestamp();
				          //printf("get_length --> before ->  %i after->",get_length);
				          for (int j = 0; j < (get_length - i); j++){
				          	read_buf[j] = read_buf[j + i];
				          }
				          get_length -= i;
				         //printf(" = %i\n",get_length);
				        }
							}
				}



        m_Platform->HighPriorityRelease();
        if (priority > 0)
             m_Platform->MidPriorityRelease();
        if (priority > 1)
             m_Platform->LowPriorityRelease();
/*
     printf("---CM530---");
     for (int j = 0; j < 62; j++){
          printf("%i",read_buf[j]);
     }
     printf("\n");
*/

        for (int j = 0; j < 62; j++){
          rxpacket[j]=(unsigned char*)read_buf[j];
        }

}
