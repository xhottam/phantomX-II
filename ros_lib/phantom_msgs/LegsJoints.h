#ifndef _ROS_phantom_msgs_LegsJoints_h
#define _ROS_phantom_msgs_LegsJoints_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "phantom_msgs/LegJoints.h"

namespace phantom_msgs
{

  class LegsJoints : public ros::Msg
  {
    public:
      phantom_msgs::LegJoints leg[6];

    LegsJoints():
      leg()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      for( uint8_t i = 0; i < 6; i++){
      offset += this->leg[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      for( uint8_t i = 0; i < 6; i++){
      offset += this->leg[i].deserialize(inbuffer + offset);
      }
     return offset;
    }

    const char * getType(){ return "phantom_msgs/LegsJoints"; };
    const char * getMD5(){ return "77281f1f9c4cd29d21908d690db99185"; };

  };

}
#endif