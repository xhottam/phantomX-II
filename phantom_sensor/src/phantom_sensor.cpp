

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <control.h>

//=============================================================================
// Main
//=============================================================================

int main( int argc, char **argv )
{
    ros::init(argc, argv, "hexapod_sensor");

    // Create class objects
    Control control;

    control.connect();

    std_msgs::String result;
    std::vector<std::string> vect;
    std::string item;

    ros::AsyncSpinner spinner( 2 ); // Using 2 threads
    spinner.start();
    ros::Rate loop_rate( 10 );
    while( ros::ok() )
    {
        if(control.ser.available()){
            //ROS_INFO_STREAM("Reading from serial port");
            result.data = control.ser.readline();
	    size_t index = 0;
            /* Locate the substring to replace. */
	    index = result.data.find("!,DCM:", index);
            if (index == std::string::npos) continue;
            result.data.erase(index,6);
            //ROS_INFO_STREAM(result.data);
            //!,DCM:-37.43,-16.24,43.51,0.70,-0.42,-0.58,0.66,0.69,0.29,0.28,-0.58,0.76
            std::stringstream ss(result.data);
            vect.clear();
            while (std::getline(ss, item, ',')){
	       vect.push_back(item);
	    }
	    if (vect.size() == 12) {
		for (int i=0; i< vect.size(); i++)
                    ROS_INFO_STREAM("Read: " << vect.at(i));
            }else{	
		ROS_INFO_STREAM("Reads: " << vect.size());
	    }
        }
 	contin:;
        loop_rate.sleep();
    }
    return 0;
}

