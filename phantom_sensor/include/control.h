#ifndef CONTROL
#define CONTROL_H_

#include <ros/ros.h>
#include <serial/serial.h>
#include <sensor_msgs/Imu.h>



class Control
{
    public:

	Control( void );
	void publishImu( const sensor_msgs::Imu &imu_data );
        int MASTER_LOOP_RATE;  // Master loop rate

	sensor_msgs::Imu imu_data;
	serial::Serial ser;


    private:
	int connect();

	ros::Time current_time_imu;

       // Topics we are publishing
        ros::Publisher imu_pub_;

	// Node Handle
        ros::NodeHandle nh_;

};

#endif // CONTROL_H_
