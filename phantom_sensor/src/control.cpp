#include <control.h>


Control::Control( void )
{

        ros::param::get( "MASTER_LOOP_RATE", MASTER_LOOP_RATE );
        //Topics we are publishing
        current_time_imu = ros::Time::now();
        imu_pub_ = nh_.advertise<sensor_msgs::Imu>( "/imu", 10 );

	
}

int Control::connect(){

    try
    {
        ser.setPort("/dev/ttyUSB1");
        ser.setBaudrate(57600);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }else{
        return -1;
    }
}

void Control::publishImu( const sensor_msgs::Imu &imu_data ){
}

