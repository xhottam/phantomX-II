#include <control.h>


Control::Control( void )
{

        ros::param::get( "MASTER_LOOP_RATE", MASTER_LOOP_RATE );
        (ros::param::get( "IMU_EULER", IMU_EULER ))?:IMU_EULER = true;
        (ros::param::get( "IMU_PHANTOM", IMU_PHANTOM ))?:IMU_PHANTOM = false;

	       

        //Topics we are publishing
        current_time_imu = ros::Time::now();

        if ( IMU_EULER ){
		imu_pub_euler = nh_.advertise<std_msgs::Float32MultiArray>( "imu_euler", 1 );
                imu_data_regex = "!ANG:";
                delete_index = 5; 
                vector_size  = 3;
	}
	if ( IMU_PHANTOM ){
		imu_pub_ = nh_.advertise<sensor_msgs::Imu>( "imu_phantom", 5 );
                 imu_data_regex = "!,PHANTOM:";
                 delete_index = 10;
                 vector_size  = 9;
	}
        
	
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

void Control::publishImu( const sensor_msgs::Imu *imu_data ){
                                                  
	imu_pub_.publish(*imu_data);
}
void Control::publishImu_Euler( const std_msgs::Float32MultiArray *euler ){

	imu_pub_euler.publish(*euler);
}
