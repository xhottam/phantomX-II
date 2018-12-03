#include <phantom_teleop_joystick.h>
#include <std_msgs/ByteMultiArray.h>
#include "LinuxDARwIn.h"


using namespace Robot;
LinuxArbotixPro linux_arbotixpro("/dev/ttyUSB1");
ArbotixPro arbotixpro(&linux_arbotixpro);

//==============================================================================
// Constructor
//==============================================================================

PhantomTeleopJoystick::PhantomTeleopJoystick( void )
{
    ros::param::get( "NON_TELEOP", NON_TELEOP );
    ros::param::get( "NUMBER_OF_LEGS", NUMBER_OF_LEGS );
    ros::param::get( "NUMBER_OF_LEG_SEGMENTS", NUMBER_OF_LEG_JOINTS );
    ros::param::get( "PS3_DEABAND", DEADBAND );
    ros::param::get( "PS3_OFFSET", OFFSET );
    ros::param::get( "PS3_SCALE", SCALE );

    ros::param::get( "SERVOS", SERVOS );

    // Find out how many servos/joints we have
    for( XmlRpc::XmlRpcValue::iterator it = SERVOS.begin(); it != SERVOS.end(); it++ )
    {
        servo_map_key_.push_back( it->first );
    }
    joint_state_.name.resize( servo_map_key_.size() );
    joint_state_.position.resize( servo_map_key_.size() );
    servo_names_.resize( servo_map_key_.size() );
    servo_orientation_.resize( servo_map_key_.size() );
    for( std::size_t i = 0; i < servo_map_key_.size(); i++ )
    {
        ros::param::get( ("/SERVOS/" + static_cast<std::string>( servo_map_key_[i] ) + "/name"), servo_names_[i] );
        ros::param::get( ("/SERVOS/" + static_cast<std::string>( servo_map_key_[i] ) + "/sign"), servo_orientation_[i] );
    }	
    
    
    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &PhantomTeleopJoystick::joyCallback,this);
    phantom_joint_state = nh_.advertise<sensor_msgs::JointState>("/joint_states", 10);
    
    if (NON_TELEOP == false){
	    //linux_arbotixpro.DEBUG_PRINT= true;	    
	    if (arbotixpro.Connect() == true){
                timer_Write = nh_.createTimer(ros::Duration(0.04), boost::bind(&PhantomTeleopJoystick::Writecm530, this));
	    }
    }
   

}

//==============================================================================
// Joint State publisher
//==============================================================================

void PhantomTeleopJoystick::publishJoinStates(sensor_msgs::JointState *joint_state)
{
   
    joint_state->header.stamp = ros::Time::now();
    int i = 0;
    int rxindex = 2;
    for( int leg_index = 0; leg_index < NUMBER_OF_LEGS; leg_index++ ){
        rxindex++;	
        joint_state->name[i] = servo_names_[i];
        joint_state->position[i] = servo_orientation_[i] * (makeword(rxpacket[rxindex++],rxpacket[rxindex++])*M_PI)/180;
        i++;
        joint_state->name[i] = servo_names_[i];
        joint_state->position[i] = servo_orientation_[i] * (makeword(rxpacket[rxindex++],rxpacket[rxindex++])*M_PI)/180;
        i++;
        joint_state->name[i] = servo_names_[i];
        joint_state->position[i] = servo_orientation_[i] * (makeword(rxpacket[rxindex++],rxpacket[rxindex++])*M_PI)/180;
        i++;
    }
    phantom_joint_state.publish( *joint_state );
}
//==============================================================================
// Joystick call reading joystick topics
//==============================================================================

void PhantomTeleopJoystick::joyCallback( const sensor_msgs::Joy::ConstPtr &joy )
{
    ros::Time current_time = ros::Time::now();

    buttons = 0;
    leftX = 128;
    leftY = 128;
    rightX = 128;
    rightY = 128;

    if( joy->buttons[PS3_BUTTON_ACTION_TRIANGLE] == 1 )
    {
     	 buttons += 8; 
    }
    if( joy->buttons[PS3_BUTTON_ACTION_CIRCLE] == 1 )
    {
     	 buttons += 32; 
    }
    if( joy->buttons[PS3_BUTTON_ACTION_CROSS] == 1 )
    {
     	 buttons += 1; 
    }
    if( joy->buttons[PS3_BUTTON_ACTION_SQUARE] == 1 )
    {
     	 buttons += 16; 
    }   
    if( joy->buttons[PS3_BUTTON_REAR_LEFT_1] == 1 )
    {
     	 buttons += 128; 
    }
    if( joy->buttons[PS3_BUTTON_REAR_RIGHT_1] == 1 )
    {
     	 buttons += 64; 
    }
    if( joy->buttons[PS3_BUTTON_STICK_RIGHT] == 1 )
    {
     	 buttons += 2; 
    }

    
    if( joy->axes[PS3_AXIS_STICK_LEFT_LEFTWARDS] >  DEADBAND || joy->axes[PS3_AXIS_STICK_LEFT_LEFTWARDS] < -DEADBAND)
    {
	 //ROS_DEBUG("%.6f" ,joy->axes[PS3_AXIS_STICK_LEFT_LEFTWARDS]);
	
         leftX = mapa(joy->axes[PS3_AXIS_STICK_LEFT_LEFTWARDS],-1.0,1.0,0.0,253.0);
         //ROS_DEBUG("%.6f" ,joy->axes[PS3_AXIS_STICK_LEFT_LEFTWARDS] );
         //ROS_DEBUG("leftX %i", leftX);
    }
    if( joy->axes[PS3_AXIS_STICK_LEFT_UPWARDS] >  DEADBAND || joy->axes[PS3_AXIS_STICK_LEFT_UPWARDS] < -DEADBAND)
   {
         leftY = mapa(joy->axes[PS3_AXIS_STICK_LEFT_UPWARDS],-1.0,1.0,0.0,253.0);
    }
    if( joy->axes[PS3_AXIS_STICK_RIGHT_LEFTWARDS] >  DEADBAND || joy->axes[PS3_AXIS_STICK_RIGHT_LEFTWARDS] < -DEADBAND)
    {
         rightX = mapa(joy->axes[PS3_AXIS_STICK_RIGHT_LEFTWARDS],-1.0,1.0,0.0,253.0);
    }
    if( joy->axes[PS3_AXIS_STICK_RIGHT_UPWARDS] >  DEADBAND || joy->axes[PS3_AXIS_STICK_RIGHT_UPWARDS] < -DEADBAND)
    {
         rightY = mapa(joy->axes[PS3_AXIS_STICK_RIGHT_UPWARDS],-1.0,1.0,0.0,253.0);
    }

    checksum = (255 - (rightY+rightX+leftY+leftX+buttons)%256);

}
int PhantomTeleopJoystick::mapa(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int PhantomTeleopJoystick::makeword(int lowbyte, int highbyte) {
	unsigned short word;
	word = highbyte;
	word = word << 8;
	word = word + lowbyte;
	return (int)word;
}

void PhantomTeleopJoystick::Writecm530(){
	arbotixpro.WriteCM530(rightY,rightX,leftY,leftX,buttons,checksum);
}



int main(int argc, char** argv)
{
    ros::init(argc, argv, "phantom_joystick");
    PhantomTeleopJoystick phantomTeleopJoystick;

    //ros::spin();

/**    // our loop will publish at 10Hz
    ros::Rate loop_rate(5);
 
    ros::spin();
*/    

    ros::AsyncSpinner spinner(1); // Using 1 threads
    spinner.start();

    ros::Rate loop_rate(0.8); // 100 hz
    while ( ros::ok() )
    {  
        phantomTeleopJoystick.rxpacket = arbotixpro.ReadCM530();
        phantomTeleopJoystick.publishJoinStates(&phantomTeleopJoystick.joint_state_);
        loop_rate.sleep();
    }
}
