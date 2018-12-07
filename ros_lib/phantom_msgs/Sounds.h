#ifndef _ROS_phantom_msgs_Sounds_h
#define _ROS_phantom_msgs_Sounds_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace phantom_msgs
{

  class Sounds : public ros::Msg
  {
    public:
      typedef bool _torque_type;
      _torque_type torque;
      typedef bool _battery_type;
      _battery_type battery;
      typedef bool _auto_level_type;
      _auto_level_type auto_level;
      typedef bool _stand_type;
      _stand_type stand;
      typedef bool _waiting_type;
      _waiting_type waiting;
      typedef bool _shut_down_type;
      _shut_down_type shut_down;

    Sounds():
      torque(0),
      battery(0),
      auto_level(0),
      stand(0),
      waiting(0),
      shut_down(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_torque;
      u_torque.real = this->torque;
      *(outbuffer + offset + 0) = (u_torque.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->torque);
      union {
        bool real;
        uint8_t base;
      } u_battery;
      u_battery.real = this->battery;
      *(outbuffer + offset + 0) = (u_battery.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->battery);
      union {
        bool real;
        uint8_t base;
      } u_auto_level;
      u_auto_level.real = this->auto_level;
      *(outbuffer + offset + 0) = (u_auto_level.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->auto_level);
      union {
        bool real;
        uint8_t base;
      } u_stand;
      u_stand.real = this->stand;
      *(outbuffer + offset + 0) = (u_stand.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->stand);
      union {
        bool real;
        uint8_t base;
      } u_waiting;
      u_waiting.real = this->waiting;
      *(outbuffer + offset + 0) = (u_waiting.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->waiting);
      union {
        bool real;
        uint8_t base;
      } u_shut_down;
      u_shut_down.real = this->shut_down;
      *(outbuffer + offset + 0) = (u_shut_down.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->shut_down);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_torque;
      u_torque.base = 0;
      u_torque.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->torque = u_torque.real;
      offset += sizeof(this->torque);
      union {
        bool real;
        uint8_t base;
      } u_battery;
      u_battery.base = 0;
      u_battery.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->battery = u_battery.real;
      offset += sizeof(this->battery);
      union {
        bool real;
        uint8_t base;
      } u_auto_level;
      u_auto_level.base = 0;
      u_auto_level.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->auto_level = u_auto_level.real;
      offset += sizeof(this->auto_level);
      union {
        bool real;
        uint8_t base;
      } u_stand;
      u_stand.base = 0;
      u_stand.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->stand = u_stand.real;
      offset += sizeof(this->stand);
      union {
        bool real;
        uint8_t base;
      } u_waiting;
      u_waiting.base = 0;
      u_waiting.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->waiting = u_waiting.real;
      offset += sizeof(this->waiting);
      union {
        bool real;
        uint8_t base;
      } u_shut_down;
      u_shut_down.base = 0;
      u_shut_down.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->shut_down = u_shut_down.real;
      offset += sizeof(this->shut_down);
     return offset;
    }

    const char * getType(){ return "phantom_msgs/Sounds"; };
    const char * getMD5(){ return "9e277e5433232e212c4281205788a826"; };

  };

}
#endif
