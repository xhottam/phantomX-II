

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
            result.data = control.ser.readline();
	    size_t index = 0;
	    index = result.data.find("!,PHANTOM:", index);
            if (index == std::string::npos) continue;
            result.data.erase(index,10);
            std::stringstream ss(result.data);
            vect.clear();
            while (std::getline(ss, item, ',')){
	       vect.push_back(item);
	    }
	    if (vect.size() == 9) {
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

