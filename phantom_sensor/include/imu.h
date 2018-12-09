
#ifndef IMU_H_
#define IMU_H_

#include <ros/ros.h>
#include <sensor_msgs/Imu.h>




class Imu
{
    public:
        Imu( void );
        void getImu( sensor_msgs::Imu *imu_data);
    private:
};

#endif // IMU_H_
