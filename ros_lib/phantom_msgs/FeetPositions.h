#ifndef _ROS_phantom_msgs_FeetPositions_h
#define _ROS_phantom_msgs_FeetPositions_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "phantom_msgs/Pose.h"

namespace phantom_msgs
{

  class FeetPositions : public ros::Msg
  {
    public:
      phantom_msgs::Pose foot[6];

    FeetPositions():
      foot()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      for( uint8_t i = 0; i < 6; i++){
      offset += this->foot[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      for( uint8_t i = 0; i < 6; i++){
      offset += this->foot[i].deserialize(inbuffer + offset);
      }
     return offset;
    }

    const char * getType(){ return "phantom_msgs/FeetPositions"; };
    const char * getMD5(){ return "d12724c3b1519cfb275eb5b1d0e25de2"; };

  };

}
#endif