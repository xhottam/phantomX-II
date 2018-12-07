#ifndef _ROS_phantom_msgs_Pose_h
#define _ROS_phantom_msgs_Pose_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/Point.h"
#include "phantom_msgs/RPY.h"

namespace phantom_msgs
{

  class Pose : public ros::Msg
  {
    public:
      typedef geometry_msgs::Point _position_type;
      _position_type position;
      typedef phantom_msgs::RPY _orientation_type;
      _orientation_type orientation;

    Pose():
      position(),
      orientation()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->position.serialize(outbuffer + offset);
      offset += this->orientation.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->position.deserialize(inbuffer + offset);
      offset += this->orientation.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "phantom_msgs/Pose"; };
    const char * getMD5(){ return "f1b4a886328450f637af10145259080b"; };

  };

}
#endif
