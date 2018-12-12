#ifndef CONTROL
#define CONTROL_H_

#include <ros/ros.h>
#include <serial/serial.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Float32MultiArray.h>



class Control
{
    public:

	Control( void );
	void publishImu( const sensor_msgs::Imu *imu_data );
        void publishImu_Euler( const std_msgs::Float32MultiArray *euler );
        int connect();
        int MASTER_LOOP_RATE;  // Master loop rate

	sensor_msgs::Imu imu_data;
        std_msgs::Float32MultiArray euler;
	serial::Serial ser;


    private:

	ros::Time current_time_imu;
       // Topics we are publishing
        ros::Publisher imu_pub_;
        ros::Publisher imu_pub_euler;
	// Node Handle
        ros::NodeHandle nh_;

};

#endif // CONTROL_H_
