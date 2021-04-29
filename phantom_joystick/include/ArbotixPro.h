#ifndef _ARBOTIXPRO_H_
#define _ARBOTIXPRO_H_

	class PlatformArbotixPro
	{
		public:
			/////////// Need to implement below methods (Platform porting) //////////////
			// Port control
			virtual bool OpenPort(bool loglevel) = 0;
			virtual bool SetBaud(int baud) = 0;
			virtual void ClosePort() = 0;
			virtual void ClearPort() = 0;
			virtual int WritePort(unsigned char* packet, int numPacket) = 0;
			virtual int ReadPort(unsigned char* packet, int numPacket) = 0;
			virtual void FlushPort() = 0;

			// Using semaphore
			virtual void LowPriorityWait() = 0;
			virtual void MidPriorityWait() = 0;
			virtual void HighPriorityWait() = 0;
			virtual void LowPriorityRelease() = 0;
			virtual void MidPriorityRelease() = 0;
			virtual void HighPriorityRelease() = 0;

			// Using timeout
			virtual void SetPacketTimeout(int lenPacket) = 0;
			virtual bool IsPacketTimeout() = 0;
			virtual double GetPacketTime() = 0;
			virtual void SetUpdateTimeout(int msec) = 0;
			virtual bool IsUpdateTimeout() = 0;
			virtual double GetUpdateTime() = 0;

			virtual void Sleep(int Miliseconds) = 0;
			//////////////////////////////////////////////////////////////////////////////
	};
	class ArbotixPro
	{
		public:
			enum
			{
				SUCCESS,
				TX_CORRUPT,
				TX_FAIL,
				RX_FAIL,
				RX_TIMEOUT,
				RX_CORRUPT
			};

			enum
			{
				INPUT_VOLTAGE   = 1,
				ANGLE_LIMIT     = 2,
				OVERHEATING     = 4,
				RANGE           = 8,
				CHECKSUM        = 16,
				OVERLOAD        = 32,
				INSTRUCTION     = 64
			};

			enum
			{
				P_MODEL_NUMBER_L		= 0,
				P_MODEL_NUMBER_H		= 1,
				P_VERSION				= 2,
				P_ID					= 3,
				P_BAUD_RATE				= 4,
				P_RETURN_DELAY_TIME		= 5,
				P_RETURN_LEVEL			= 16,
				P_DXL_POWER				= 24,
				P_LED_PANNEL			= 25,
				P_LED_HEAD_L			= 26,
				P_LED_HEAD_H			= 27,
				P_LED_EYE_L				= 28,
				P_LED_EYE_H				= 29,
				P_BUTTON				= 30,
				P_GYRO_Z_L				= 38,
				P_GYRO_Z_H				= 39,
				P_GYRO_Y_L				= 40,
				P_GYRO_Y_H				= 41,
				P_GYRO_X_L				= 42,
				P_GYRO_X_H				= 43,
				P_ACCEL_Y_L				= 44,
				P_ACCEL_Y_H				= 45,
				P_ACCEL_X_L				= 46,
				P_ACCEL_X_H				= 47,
				P_ACCEL_Z_L				= 48,
				P_ACCEL_Z_H				= 49,
				P_VOLTAGE				= 50,
				P_LEFT_MIC_L			= 51,
				P_LEFT_MIC_H			= 52,
				P_ADC2_L				= 53,
				P_ADC2_H				= 54,
				P_ADC3_L				= 55,
				P_ADC3_H				= 56,
				P_ADC4_L				= 57,
				P_ADC4_H				= 58,
				P_ADC5_L				= 59,
				P_ADC5_H				= 60,
				P_ADC6_L				= 61,
				P_ADC6_H				= 62,
				P_ADC7_L				= 63,
				P_ADC7_H				= 64,
				P_ADC8_L				= 65,
				P_ADC8_H				= 66,
				P_RIGHT_MIC_L			= 67,
				P_RIGHT_MIC_H			= 68,
				P_ADC10_L				= 69,
				P_ADC10_H				= 70,
				P_ADC11_L				= 71,
				P_ADC11_H				= 72,
				P_ADC12_L				= 73,
				P_ADC12_H				= 74,
				P_ADC13_L				= 75,
				P_ADC13_H				= 76,
				P_ADC14_L				= 77,
				P_ADC14_H				= 78,
				P_ADC15_L				= 79,
				P_ADC15_H				= 80,
				MAXNUM_ADDRESS
			};

			enum
			{
				ID_CM			= 200,
				ID_BROADCAST	= 254
			};

		private:
      PlatformArbotixPro *m_Platform;
			static const int RefreshTime = 6; //msec
      unsigned char rx_Odroid_packet[50];

			int TxRxPacket(unsigned char *txpacket, unsigned char *rxpacket, int priority);
      int checkSumatory(unsigned char  data[], int length);
      void timestamp();
     void _TxRx_CM530(unsigned char *txpacket,unsigned char *rxpacket[],int priority);

		public:
			bool DEBUG_COM;
      bool DEBUG_JOINTS;

			ArbotixPro(PlatformArbotixPro *platform);
			~ArbotixPro();

			bool Connect();
			bool ChangeBaud(int baud);
			void Disconnect();


			void TxRx_CM530(int rightY,int rightX,int leftY,int leftX,int buttons, int checksum,unsigned char *rxpacket[]);


			// Utility
			static int MakeWord(int lowbyte, int highbyte);
	};

#endif
