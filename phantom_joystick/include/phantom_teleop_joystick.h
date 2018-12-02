#ifndef PHANTOM_TELEOP_H_
#define PHANTOM_TELEOP_H_

#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/ByteMultiArray.h>
#include <sensor_msgs/JointState.h>
#include <ros/console.h>
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"


#define PS3_BUTTON_SELECT            0
#define PS3_BUTTON_STICK_LEFT        1
#define PS3_BUTTON_STICK_RIGHT       2
#define PS3_BUTTON_START             3
#define PS3_BUTTON_CROSS_UP          4
#define PS3_BUTTON_CROSS_RIGHT       5
#define PS3_BUTTON_CROSS_DOWN        6
#define PS3_BUTTON_CROSS_LEFT        7
#define PS3_BUTTON_REAR_LEFT_2       8
#define PS3_BUTTON_REAR_RIGHT_2      9
#define PS3_BUTTON_REAR_LEFT_1       10
#define PS3_BUTTON_REAR_RIGHT_1      11
#define PS3_BUTTON_ACTION_TRIANGLE   12
#define PS3_BUTTON_ACTION_CIRCLE     13
#define PS3_BUTTON_ACTION_CROSS      14
#define PS3_BUTTON_ACTION_SQUARE     15
#define PS3_BUTTON_PAIRING           16

#define PS3_AXIS_STICK_LEFT_LEFTWARDS    0
#define PS3_AXIS_STICK_LEFT_UPWARDS      1
#define PS3_AXIS_STICK_RIGHT_LEFTWARDS   2
#define PS3_AXIS_STICK_RIGHT_UPWARDS     3
#define PS3_AXIS_BUTTON_CROSS_UP         4
#define PS3_AXIS_BUTTON_CROSS_RIGHT      5
#define PS3_AXIS_BUTTON_CROSS_DOWN       6
#define PS3_AXIS_BUTTON_CROSS_LEFT       7
#define PS3_AXIS_BUTTON_REAR_LEFT_2      8
#define PS3_AXIS_BUTTON_REAR_RIGHT_2     9
#define PS3_AXIS_BUTTON_REAR_LEFT_1      10
#define PS3_AXIS_BUTTON_REAR_RIGHT_1     11
#define PS3_AXIS_BUTTON_ACTION_TRIANGLE  12
#define PS3_AXIS_BUTTON_ACTION_CIRCLE    13
#define PS3_AXIS_BUTTON_ACTION_CROSS     14
#define PS3_AXIS_BUTTON_ACTION_SQUARE    15
#define PS3_AXIS_ACCELEROMETER_LEFT      16
#define PS3_AXIS_ACCELEROMETER_FORWARD   17
#define PS3_AXIS_ACCELEROMETER_UP        18
#define PS3_AXIS_GYRO_YAW                19

class PhantomTeleopJoystick
{
public:
	PhantomTeleopJoystick( void );
        void publishJoinStates(sensor_msgs::JointState *joint_state);
        sensor_msgs::JointState joint_state_;
	
        unsigned char checksum;
        unsigned char header_1;
        unsigned char header_2;
        unsigned char extra;
	unsigned char buttons;
        unsigned char leftX;
        unsigned char leftY;
        unsigned char rightX;
        unsigned char rightY;
        bool arbotix_con_feed;

        bool NON_TELEOP;
        unsigned char * rxpacket;

private:
        void joyCallback( const sensor_msgs::Joy::ConstPtr &joy );
	int mapa(double x, double in_min, double in_max, double out_min, double out_max);
        int makeword(int lowbyte, int highbyte);

        void Writecm530();
        void Readcm530();
        
	ros::NodeHandle nh_;
	ros::Subscriber joy_sub_;

        ros::Publisher phantom_joint_state;

        int NUMBER_OF_LEGS;
	int NUMBER_OF_LEG_JOINTS;
        double DEADBAND;
        double SCALE;
        double OFFSET;

	ros::Timer timer_Write;
        ros::Timer timer_Read;

	XmlRpc::XmlRpcValue SERVOS;
        std::vector<std::string> servo_map_key_;
        std::vector<std::string> servo_names_;
        std::vector<int> servo_orientation_;
};
#endif // PHANTOM_TELEOP_H_
