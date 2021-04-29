#include <phantom_teleop_joystick.h>
#include <std_msgs/ByteMultiArray.h>
#include <LinuxArbotixPro.h>
#include <ArbotixPro.h>

#define	c1DEC		10
#define _M_PI           3141
#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi


LinuxArbotixPro linux_arbotixpro("/dev/ttyUSB0");
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

    bool loglevel_RPI4_COM= false;
    bool loglevel_CM530_JOINTS= false;

    if (NON_TELEOP == false){
            arbotixpro.DEBUG_JOINTS = loglevel_CM530_JOINTS;
            arbotixpro.DEBUG_COM    = loglevel_RPI4_COM;
            if (arbotixpro.Connect() == true){
                //ROS_INFO("-- PhantomTeleopjoystick constructor (connect CM530) --");
                timer_Write = nh_.createTimer(ros::Duration(0.04), boost::bind(&PhantomTeleopJoystick::Write_Read_CM530, this));
                //ROS_INFO("-- PhantomTeleopjoystick constructor (Timer create to Write_Read_CM530) --");
            }
    }
}

//==============================================================================
// Joint State publisher
//==============================================================================

void PhantomTeleopJoystick::publishJoinStates(sensor_msgs::JointState *joint_state)
{

    //ROS_INFO("-- PhantomTeleopjoystick publishJoinStates START --");
    joint_state->header.stamp = ros::Time::now();
    int i = 0;
    int rxindex = 2;
    int signo = 0;
    for( int leg_index = 0; leg_index < NUMBER_OF_LEGS; leg_index++ ){
        rxindex++;


        if(rxpacket[rxindex++] == 0){
		            signo = -1;
	      }else{
                signo = 1;
	      }
        joint_state->name[i] = servo_names_[i];
        joint_state->position[i] = servo_orientation_[i] * (( signo  * (makeword((int)rxpacket[rxindex++],(int)rxpacket[rxindex++],false,leg_index )/ c1DEC )  * M_PI ) /180);

        i++;
        if(rxpacket[rxindex++] == 0){
                signo = -1;
        }else{
                signo = 1;
        }
        joint_state->name[i] = servo_names_[i];
        joint_state->position[i] = servo_orientation_[i] * ((  signo * ((makeword((int)rxpacket[rxindex++],(int)rxpacket[rxindex++],false,leg_index ) - 100)  / c1DEC )  * M_PI ) /180);

        i++;
        if(rxpacket[rxindex++] == 0){
                signo = -1;
	      }else{
		        signo = 1;
        }
        joint_state->name[i] = servo_names_[i];
        joint_state->position[i] = servo_orientation_[i] * (( signo * ((makeword((int)rxpacket[rxindex++],(int)rxpacket[rxindex++],false,leg_index) - 400 )  / c1DEC )   * M_PI ) /180);

        i++;
    }
    phantom_joint_state.publish( *joint_state );
    //ROS_INFO("-- PhantomTeleopjoystick publishJoinStates END --");
}
//==============================================================================
// Joystick call reading joystick topics
//==============================================================================

void PhantomTeleopJoystick::joyCallback( const sensor_msgs::Joy::ConstPtr &joy )
{
    //ROS_INFO("-- PhantomTeleopjoystick joyCallback START --");
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
    //ROS_INFO("-- PhantomTeleopjoystick joyCallback END --");

}
int PhantomTeleopJoystick::mapa(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int PhantomTeleopJoystick::makeword(int lowbyte, int highbyte,bool feed,int index) {
	unsigned short word;
	word = highbyte;
	word = word << 8;
	word = word + lowbyte;

	if(feed && (index == 0)){
           printf("%i\n",word );
	}
	return (int)word;
}

void PhantomTeleopJoystick::Write_Read_CM530(){
  //ROS_INFO("-- PhantomTeleopjoystick Write_Read_CM530 START --");
  arbotixpro.TxRx_CM530(rightY,rightX,leftY,leftX,buttons,checksum,rxpacket);
  //ROS_INFO("-- PhantomTeleopjoystick Write_Read_CM530 END --");
}




int main(int argc, char** argv)
{
    //ROS_INFO("-- PhantomTeleopjoystick main START --");
    ros::init(argc, argv, "phantom_joystick");
    PhantomTeleopJoystick phantomTeleopJoystick;

    //ros::spin();
/** // our loop will publish at 10Hz
    ros::Rate loop_rate(10);
    ros::spin();
*/

    ros::AsyncSpinner spinner(0); // Using 1 threads
    spinner.start();
    ros::Rate loop_rate(10); // 100 hz
    while ( ros::ok() )
    {
        phantomTeleopJoystick.publishJoinStates(&phantomTeleopJoystick.joint_state_);
        loop_rate.sleep();
    }
    //ROS_INFO("-- PhantomTeleopjoystick main END --");
}
