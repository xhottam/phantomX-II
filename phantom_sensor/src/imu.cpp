#include <imu.h>

int seq;
Imu::Imu( void )
{
	seq = 0;
}


void Imu::getImu_Euler(std::vector<std::string> vect, std_msgs::Float32MultiArray *euler){

	float roll  = std::atof((vect.at(0)).c_str());
        float pitch = std::atof((vect.at(1)).c_str());
        float yaw   = std::atof((vect.at(2)).c_str());
	
	euler->data.clear();
	euler->data.push_back( roll );
        euler->data.push_back( pitch );
        euler->data.push_back( yaw );
         
}
/**           Output angles and linear acceleration and rotational
              velocity. Angles are in degrees, acceleration is
              in units of 1.0 = 1/256 G (9.8/256 m/s^2). Rotational
              velocity is in rad/s. (Output frames have form like
*/
void Imu::getImu_Phantom( std::vector<std::string> vect, sensor_msgs::Imu *imu_data){

	//in AHRS firmware z axis points down, in ROS z axis points up (see REP 103)
        float yaw_deg = -std::atof((vect.at(2)).c_str());
        yaw_deg = yaw_deg;// + imu_yaw_calibration;
        if (yaw_deg > 180.0)
            yaw_deg = yaw_deg - 360.0;
        if (yaw_deg < -180.0)
            yaw_deg = yaw_deg + 360.0;
        float yaw = ToRad(yaw_deg);
        //in AHRS firmware y axis points right, in ROS y axis points left (see REP 103)
        float pitch = ToRad(-std::atof((vect.at(1)).c_str()));
        float roll = ToRad(std::atof((vect.at(0)).c_str()));

        // Publish message
        // AHRS firmware accelerations are negated
        // This means y and z are correct for ROS, but x needs reversing
        imu_data->linear_acceleration.x =  accel_factor(-std::atof((vect.at(6)).c_str()));// * accel_factor
        imu_data->linear_acceleration.y =  accel_factor(std::atof((vect.at(7)).c_str())); // * accel_factor
        imu_data->linear_acceleration.z =  accel_factor(std::atof((vect.at(8)).c_str()));// * accel_factor

        imu_data->angular_velocity.x = std::atof((vect.at(3)).c_str());
	//in AHRS firmware y axis points right, in ROS y axis points left (see REP 103)
        imu_data->angular_velocity.y = -std::atof((vect.at(4)).c_str());
	//inn AHRS firmware z axis points down, in ROS z axis points up (see REP 103) 
	imu_data->angular_velocity.z = -std::atof((vect.at(5)).c_str());
    
	geometry_msgs::Quaternion q  = quaternion_from_euler(roll, pitch, yaw);

	imu_data->orientation.x = q.x;
	imu_data->orientation.y = q.y;
	imu_data->orientation.z = q.z;
	imu_data->orientation.w = q.w;
	
        imu_data->header.stamp= ros::Time::now();
	imu_data->header.frame_id = "base_link";
	imu_data->header.seq = seq++;


}
geometry_msgs::Quaternion Imu::quaternion_from_euler(float roll, float pitch, float yaw){

    geometry_msgs::Quaternion q;

    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * cp * sr - sy * sp * cr;
    q.y = sy * cp * sr + cy * sp * cr;
    q.z = sy * cp * cr - cy * sp * sr;

    return q;


}
