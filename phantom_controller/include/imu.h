#ifndef PHANTOM_CONTROLLER_H_
#define PHANTOM_CONTROLLER_H_

#include <ros/ros.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Vector3.h>
#include <sensor_msgs/Imu.h>


class PhantomController
{
public:

	PhantomController( void );
	geometry_msgs::Quaternion  orientation;
	geometry_msgs::Vector3 angular_velocity;
	geometry_msgs::Vector3 linear_acceleration;
        ros::Publisher imu_pub;

private:

	ros::NodeHandle nh;
	ros::Subscriber imu_ori;
	ros::Subscriber imu_gyr;
	ros::Subscriber imu_acc;

	void CallbackOrientation(const geometry_msgs::Quaternion ::ConstPtr& q);
	void CallbackGyro(const geometry_msgs::Vector3::ConstPtr& ypr);
	void CallbackAccelration(const geometry_msgs::Vector3::ConstPtr& aa);
};

#endif // PHANTOM_CONTROLLER_H_
