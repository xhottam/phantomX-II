

#include <ros/ros.h>

//=============================================================================
// Main
//=============================================================================

int main( int argc, char **argv )
{
    ros::init(argc, argv, "hexapod_sensor");


    ros::AsyncSpinner spinner( 2 ); // Using 2 threads
    spinner.start();
    ros::Rate loop_rate( 100 );
    while( ros::ok() )
    {
        loop_rate.sleep();
    }
    return 0;
}

