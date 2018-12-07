#ifndef _ROS_phantom_msgs_LegJoints_h
#define _ROS_phantom_msgs_LegJoints_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace phantom_msgs
{

  class LegJoints : public ros::Msg
  {
    public:
      typedef float _coxa_type;
      _coxa_type coxa;
      typedef float _femur_type;
      _femur_type femur;
      typedef float _tibia_type;
      _tibia_type tibia;
      typedef float _tarsus_type;
      _tarsus_type tarsus;

    LegJoints():
      coxa(0),
      femur(0),
      tibia(0),
      tarsus(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += serializeAvrFloat64(outbuffer + offset, this->coxa);
      offset += serializeAvrFloat64(outbuffer + offset, this->femur);
      offset += serializeAvrFloat64(outbuffer + offset, this->tibia);
      offset += serializeAvrFloat64(outbuffer + offset, this->tarsus);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->coxa));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->femur));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->tibia));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->tarsus));
     return offset;
    }

    const char * getType(){ return "phantom_msgs/LegJoints"; };
    const char * getMD5(){ return "4281b159c18654b4920aa4a2c525621c"; };

  };

}
#endif
