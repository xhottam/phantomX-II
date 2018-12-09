/*

MinIMU-9-Arduino-AHRS
Pololu MinIMU-9 + Arduino AHRS (Attitude and Heading Reference System)

Copyright (c) 2011-2016 Pololu Corporation.
http://www.pololu.com/

MinIMU-9-Arduino-AHRS is based on sf9domahrs by Doug Weibel and Jose Julio:
http://code.google.com/p/sf9domahrs/

sf9domahrs is based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose
Julio and Doug Weibel:
http://code.google.com/p/ardu-imu/

MinIMU-9-Arduino-AHRS is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.

MinIMU-9-Arduino-AHRS is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License along
with MinIMU-9-Arduino-AHRS. If not, see <http://www.gnu.org/licenses/>.

*/

// Uncomment the following line to use a MinIMU-9 v5 or AltIMU-10 v5. Leave commented for older IMUs (up through v4).
//#define IMU_V5

// Uncomment the below line to use this axis definition:
   // X axis pointing forward
   // Y axis pointing to the right
   // and Z axis pointing down.
// Positive pitch : nose up
// Positive roll : right wing down
// Positive yaw : clockwise
int SENSOR_SIGN[9] = {1,1,1,-1,-1,-1,1,1,1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer
// Uncomment the below line to use this axis definition:
   // X axis pointing forward
   // Y axis pointing to the left
   // and Z axis pointing up.
// Positive pitch : nose down
// Positive roll : right wing down
// Positive yaw : counterclockwise
//int SENSOR_SIGN[9] = {1,-1,-1,-1,1,1,1,-1,-1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer

// tested with Arduino Uno with ATmega328 and Arduino Duemilanove with ATMega168

#define DEFAULT_FREQUENCY     50 /*Hz*/
#define GYROSCOPE (unsigned)     0
#define ACCELEROMETER (unsigned) 1
#define MAGNETOMETER (unsigned)  2
#define DATA_LENGTH (unsigned)   1

#include <Wire.h>
#include <ros.h>


unsigned long loop_rate;


#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Quaternion.h>


ros::NodeHandle nh;

geometry_msgs::Quaternion orientation;
ros::Publisher imu_ori("imu_orientation",&orientation);

geometry_msgs::Vector3 angular_velocity;
ros::Publisher imu_gyr("imu_gyro",&angular_velocity);

geometry_msgs::Vector3 linear_acceleration;
ros::Publisher imu_acc("imu_accl",&linear_acceleration);

//// accelerometer: 8 g sensitivity
//// 3.9 mg/digit; 1 g = 256
#define GRAVITY 256  //this equivalent to 1G in the raw data coming from the accelerometer
//
//#define ToRad(x) ((x)*0.01745329252)  // *pi/180
//#define ToDeg(x) ((x)*57.2957795131)  // *180/pi
//
//// gyro: 2000 dps full scale
//// 70 mdps/digit; 1 dps = 0.07
//#define Gyro_Gain_X 0.07 //X axis Gyro gain
//#define Gyro_Gain_Y 0.07 //Y axis Gyro gain
//#define Gyro_Gain_Z 0.07 //Z axis Gyro gain
//#define Gyro_Scaled_X(x) ((x)*ToRad(Gyro_Gain_X)) //Return the scaled ADC raw data of the gyro in radians for second
//#define Gyro_Scaled_Y(x) ((x)*ToRad(Gyro_Gain_Y)) //Return the scaled ADC raw data of the gyro in radians for second
//#define Gyro_Scaled_Z(x) ((x)*ToRad(Gyro_Gain_Z)) //Return the scaled ADC raw data of the gyro in radians for second
//
//// LSM303/LIS3MDL magnetometer calibration constants; use the Calibrate example from
//// the Pololu LSM303 or LIS3MDL library to find the right values for your board
//
#define M_X_MIN -1000
#define M_Y_MIN -1000
#define M_Z_MIN -1000
#define M_X_MAX +1000
#define M_Y_MAX +1000
#define M_Z_MAX +1000


float G_Dt=0.02;    // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

long timer=0;   //general purpuse timer
long timer_old;
long timer24=0; //Second timer used to print values
int AN[6]; //array that stores the gyro and accelerometer data
int AN_OFFSET[6]={0,0,0,0,0,0}; //Array that stores the Offset of the sensors

int gyro_x;
int gyro_y;
int gyro_z;
int accel_x;
int accel_y;
int accel_z;
int magnetom_x;
int magnetom_y;
int magnetom_z;
float c_magnetom_x;
float c_magnetom_y;
float c_magnetom_z;
float MAG_Heading;

// Euler angles
float roll;
float pitch;
float yaw;


unsigned int counter=0;


void setup()
{
  
  nh.initNode();

  nh.advertise(imu_ori);
  nh.advertise(imu_gyr);
  nh.advertise(imu_acc);
  
  I2C_Init();
  
  delay(500);

  Accel_Init();
  Compass_Init();
  Gyro_Init();

  delay(20);

  for(int i=0;i<32;i++)    // We take some readings...
    {
    Read_Gyro();
    Read_Accel();
    for(int y=0; y<6; y++)   // Cumulate values
      AN_OFFSET[y] += AN[y];
    delay(20);
    }

  for(int y=0; y<6; y++)
    AN_OFFSET[y] = AN_OFFSET[y]/32;

  AN_OFFSET[5]-=GRAVITY*SENSOR_SIGN[5];

//  Serial.println("Offset:");
//  for(int y=0; y<6; y++)
//    Serial.println(AN_OFFSET[y]);

  counter=0;
  loop_rate = 1000 / DEFAULT_FREQUENCY;
}

void loop() //Main Loop
{

    counter++;
 
    timer=millis();

    // *** DCM algorithm
    // Data adquisition
    Read_Gyro();   // This read gyro data
    Read_Accel();     // Read I2C accelerometer

    if (counter > 5)  // Read compass data at 10Hz... (5 loop runs)
    {
      counter=0;
      Read_Compass();    // Read I2C magnetometer
      Compass_Heading(); // Calculate magnetic heading
    }

            orientation.x = magnetom_x;
            orientation.y = magnetom_y;
            orientation.z = magnetom_z;
            imu_ori.publish(&orientation);
                        
            angular_velocity.x = gyro_x;
            angular_velocity.y = gyro_y;
            angular_velocity.z = gyro_z;            
            imu_gyr.publish(&angular_velocity);
            
            linear_acceleration.x = accel_x;
            linear_acceleration.y = accel_y;
            linear_acceleration.z = accel_z;
            imu_acc.publish(&linear_acceleration);
            
            nh.spinOnce();
 
  while ((millis() - timer) < loop_rate);
}
