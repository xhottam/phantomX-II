
#ifndef IMU_H_
#define IMU_H_

#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Quaternion.h>

#define ToRad(x) ((x)*0.01745329252)  // *pi/180



class Imu
{
    public:
        Imu( void );
        void getImu( std::vector<std::string> vect, sensor_msgs::Imu *imu_data);
    private:
        geometry_msgs::Quaternion quaternion_from_euler(float roll, float pitch, float yaw);
       
};

#endif // IMU_H_
