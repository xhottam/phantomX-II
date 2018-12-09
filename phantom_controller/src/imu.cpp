#include <imu.h>


PhantomController::PhantomController( void )
{

	imu_ori = nh.subscribe<geometry_msgs::Quaternion>("imu_orientation", 100,&PhantomController::CallbackOrientation,this);
	imu_gyr = nh.subscribe<geometry_msgs::Vector3>("imu_gyro", 100,&PhantomController::CallbackGyro,this);
	imu_acc = nh.subscribe<geometry_msgs::Vector3>("imu_accl", 100,&PhantomController::CallbackAccelration,this);

	imu_pub = nh.advertise<sensor_msgs::Imu>("data", 50);

}

void PhantomController::CallbackOrientation(const geometry_msgs::Quaternion::ConstPtr &q)
{
  orientation.x = q->x;
  orientation.y = q->y;
  orientation.z = q->z;
  orientation.w = 0.0; 
}

void PhantomController::CallbackGyro(const geometry_msgs::Vector3::ConstPtr &ypr)
{
  angular_velocity.x = ypr->x;
  angular_velocity.y = ypr->y;
  angular_velocity.z = ypr->z;  
}

void PhantomController::CallbackAccelration(const geometry_msgs::Vector3::ConstPtr &aa)
{
  linear_acceleration.x = aa->x;
  linear_acceleration.y = aa->y;
  linear_acceleration.z = aa->z;  
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "imu");    

  PhantomController phantomController;
  

  ros::Rate loop_rate(50);

  sensor_msgs::Imu imu;

  while(ros::ok())
  {
    imu.header.stamp = ros::Time::now();
    imu.header.frame_id = "imu";
    imu.orientation = phantomController.orientation;
    imu.angular_velocity = phantomController.angular_velocity;
    imu.linear_acceleration = phantomController.linear_acceleration;

    phantomController.imu_pub.publish(imu);

    ros::spinOnce();

    loop_rate.sleep();
  }
}

