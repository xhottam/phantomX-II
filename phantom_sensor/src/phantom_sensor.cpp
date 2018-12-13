

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <control.h>
#include <imu.h>

//=============================================================================
// Main
//=============================================================================

int main( int argc, char **argv )
{
    ros::init(argc, argv, "hexapod_sensor");


    // Create class objects
    Control control;
    Imu imu;
    
    control.connect();

    std_msgs::String result;
    std::vector<std::string> vect;
    std::string item;

    ros::AsyncSpinner spinner( 2 ); // Using 2 threads
    spinner.start();
    ros::Rate loop_rate( 50 );
    int rubbish= 0;
    while( ros::ok() )
    {
	if(control.ser.available()){
            rubbish++;
            result.data = control.ser.readline();
	    size_t index = 0;
	    index = result.data.find(control.imu_data_regex, index);
            if (index == std::string::npos) continue;
            result.data.erase(index,control.delete_index);
            std::stringstream ss(result.data);
            vect.clear();
            while (std::getline(ss, item, ',')){
               vect.push_back(item);
             }
            if (vect.size() == control.vector_size && rubbish > 200) {
            	if ( control.IMU_EULER == true ){
	                imu.getImu_Euler(vect,&control.euler);
        	        control.publishImu_Euler(&control.euler);
		}else{
			imu.getImu_Phantom(vect,&control.imu_data);
	                control.publishImu(&control.imu_data);
		}  
            }else{
                ROS_INFO_STREAM("Reads " << vect.size());
            }

/**            index = result.data.find("!ANG:", index);
            if (index == std::string::npos) continue;
            result.data.erase(index,5);
            std::stringstream ss(result.data);
            vect.clear();
            while (std::getline(ss, item, ',')){
               vect.push_back(item);
            }
            if (vect.size() == 3 && rubbish > 200) {
//              for (int i=0; i< vect.size(); i++)
//                    ROS_INFO_STREAM("Read: " << vect.at(i));
            imu.getImu_Euler(vect,&control.euler);
            control.publishImu_Euler(&control.euler);
            }else{      
                ROS_INFO_STREAM("Expect 3 , Reads " << vect.size());
            }
*/
        }
 	contin:;
        loop_rate.sleep();
    }
    control.ser.close();
    return 0;
}

