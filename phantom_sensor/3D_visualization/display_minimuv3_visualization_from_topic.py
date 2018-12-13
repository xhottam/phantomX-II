#!/usr/bin/env python
# MinIMU9ArduinoAHRS
# Pololu MinIMU-9 + Arduino AHRS (Attitude and Heading Reference System)

# Copyright (c) 2011 Pololu Corporation.
# http://www.pololu.com/

# MinIMU9ArduinoAHRS is based on sf9domahrs by Doug Weibel and Jose Julio:
# http://code.google.com/p/sf9domahrs/

# sf9domahrs is based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose
# Julio and Doug Weibel:
# http://code.google.com/p/ardu-imu/

# MinIMU9ArduinoAHRS is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your option)
# any later version.

# MinIMU9ArduinoAHRS is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
# more details.

# You should have received a copy of the GNU Lesser General Public License along
# with MinIMU9ArduinoAHRS. If not, see <http://www.gnu.org/licenses/>.

################################################################################

# This is a test/3D visualization program for the Pololu MinIMU-9 + Arduino
# AHRS, based on "Test for Razor 9DOF IMU" by Jose Julio, copyright 2009.

# This script needs VPython, pyserial and pywin modules

# First Install Python 2.6.4 (Python 2.7 also works)
# Install pywin from http://sourceforge.net/projects/pywin32/
# Install pyserial from http://sourceforge.net/projects/pyserial/files/
# Install VPython from http://vpython.org/contents/download_windows.html

import rospy

from visual import *
import serial
import string
import math
import wx
from sensor_msgs.msg import Imu
from std_msgs.msg import Float32MultiArray
from tf.transformations import euler_from_quaternion

from time import time

grad2rad = 3.141592/180.0

#Create shutdown hook to kill visual displays
def shutdown_hook():
    #print "Killing displays"
    wx.Exit()

#register shutdown hook
rospy.on_shutdown(shutdown_hook)

# Main scene
scene=display(title="Pololu MinIMU-9 + Arduino AHRS")
scene.range=(1.2,1.2,1.2)
#scene.forward = (0,-1,-0.25)
scene.forward = (1,0,-0.25)
scene.up=(0,0,1)

# Second scene (Roll, Pitch, Yaw)
scene2 = display(title='Pololu MinIMU-9 + Arduino AHRS',x=0, y=0, width=500, height=200,center=(0,0,0), background=(0,0,0))
scene2.range=(1,1,1)
scene.width=500
scene.y=200

scene2.select()
#Roll, Pitch, Yaw
cil_roll = cylinder(pos=(-0.4,0,0),axis=(0.2,0,0),radius=0.01,color=color.red)
cil_roll2 = cylinder(pos=(-0.4,0,0),axis=(-0.2,0,0),radius=0.01,color=color.red)
cil_pitch = cylinder(pos=(0.1,0,0),axis=(0.2,0,0),radius=0.01,color=color.green)
cil_pitch2 = cylinder(pos=(0.1,0,0),axis=(-0.2,0,0),radius=0.01,color=color.green)
#cil_course = cylinder(pos=(0.6,0,0),axis=(0.2,0,0),radius=0.01,color=color.blue)
#cil_course2 = cylinder(pos=(0.6,0,0),axis=(-0.2,0,0),radius=0.01,color=color.blue)
arrow_course = arrow(pos=(0.6,0,0),color=color.cyan,axis=(-0.2,0,0), shaftwidth=0.02, fixedwidth=1)

#Roll,Pitch,Yaw labels
label(pos=(-0.4,0.3,0),text="Roll",box=0,opacity=0)
label(pos=(0.1,0.3,0),text="Pitch",box=0,opacity=0)
label(pos=(0.55,0.3,0),text="Yaw",box=0,opacity=0)
label(pos=(0.6,0.22,0),text="N",box=0,opacity=0,color=color.yellow)
label(pos=(0.6,-0.22,0),text="S",box=0,opacity=0,color=color.yellow)
label(pos=(0.38,0,0),text="W",box=0,opacity=0,color=color.yellow)
label(pos=(0.82,0,0),text="E",box=0,opacity=0,color=color.yellow)
label(pos=(0.75,0.15,0),height=7,text="NE",box=0,color=color.yellow)
label(pos=(0.45,0.15,0),height=7,text="NW",box=0,color=color.yellow)
label(pos=(0.75,-0.15,0),height=7,text="SE",box=0,color=color.yellow)
label(pos=(0.45,-0.15,0),height=7,text="SW",box=0,color=color.yellow)

L1 = label(pos=(-0.4,0.22,0),text="-",box=0,opacity=0)
L2 = label(pos=(0.1,0.22,0),text="-",box=0,opacity=0)
L3 = label(pos=(0.7,0.3,0),text="-",box=0,opacity=0)

# Main scene objects
scene.select()
# Reference axis (x,y,z)
arrow(color=color.green,axis=(1,0,0), shaftwidth=0.02, fixedwidth=1)
arrow(color=color.green,axis=(0,-1,0), shaftwidth=0.02 , fixedwidth=1)
arrow(color=color.green,axis=(0,0,-1), shaftwidth=0.02, fixedwidth=1)
# labels
label(pos=(0,0,0.8),text="Pololu MinIMU-9 + Arduino AHRS",box=0,opacity=0)
label(pos=(1,0,0),text="X",box=0,opacity=0)
label(pos=(0,-1,0),text="Y",box=0,opacity=0)
label(pos=(0,0,-1),text="Z",box=0,opacity=0)
# IMU object
platform = box(length=1, height=0.05, width=1, color=color.blue)
p_line = box(length=1,height=0.08,width=0.1,color=color.yellow)
plat_arrow = arrow(color=color.green,axis=(1,0,0), shaftwidth=0.06, fixedwidth=1)

rospy.init_node("display_3D_visualization_node")

imu_euler   = rospy.get_param('~IMU_EULER', true)
imu_phantom = rospy.get_param('~IMU_PHANTOM', false)

def processIMU_Phantom(imuMsg):

    quaternion = (
      imuMsg.orientation.x,
      imuMsg.orientation.y,
      imuMsg.orientation.z,
      imuMsg.orientation.w)
    
    (roll,pitch,yaw) = euler_from_quaternion(quaternion)

    axis=(cos(pitch)*cos(yaw),-cos(pitch)*sin(yaw),sin(pitch)) 
    up=(sin(roll)*sin(yaw)+cos(roll)*sin(pitch)*cos(yaw),sin(roll)*cos(yaw)-cos(roll)*sin(pitch)*sin(yaw),-cos(roll)*cos(pitch))
    platform.axis=axis
    platform.up=up
    platform.length=1.0
    platform.width=0.65
    plat_arrow.axis=axis
    plat_arrow.up=up
    plat_arrow.length=0.8
    p_line.axis=axis
    p_line.up=up
    cil_roll.axis=(0.2*cos(roll),0.2*sin(roll),0)
    cil_roll2.axis=(-0.2*cos(roll),-0.2*sin(roll),0)
    cil_pitch.axis=(0.2*cos(pitch),0.2*sin(pitch),0)
    cil_pitch2.axis=(-0.2*cos(pitch),-0.2*sin(pitch),0)
    arrow_course.axis=(0.2*sin(yaw),0.2*cos(yaw),0)
    L1.text = str(float(roll))
    L2.text = str(float(pitch))
    L3.text = str(float(yaw))        

def processIMU_Euler(imuMsg):

    roll  = imuMsg.data[0]*grad2rad
    pitch = imuMsg.data[1]*grad2rad
    yaw   = imuMsg.data[2]*grad2rad

    axis=(cos(pitch)*cos(yaw),-cos(pitch)*sin(yaw),sin(pitch))
    up=(sin(roll)*sin(yaw)+cos(roll)*sin(pitch)*cos(yaw),sin(roll)*cos(yaw)-cos(roll)*sin(pitch)*sin(yaw),-cos(roll)*cos(pitch))
    platform.axis=axis
    platform.up=up
    platform.length=1.0
    platform.width=0.65
    plat_arrow.axis=axis
    plat_arrow.up=up
    plat_arrow.length=0.8
    p_line.axis=axis
    p_line.up=up
    cil_roll.axis=(0.2*cos(roll),0.2*sin(roll),0)
    cil_roll2.axis=(-0.2*cos(roll),-0.2*sin(roll),0)
    cil_pitch.axis=(0.2*cos(pitch),0.2*sin(pitch),0)
    cil_pitch2.axis=(-0.2*cos(pitch),-0.2*sin(pitch),0)
    arrow_course.axis=(0.2*sin(yaw),0.2*cos(yaw),0)
    L1.text = str(float(roll))
    L2.text = str(float(pitch))
    L3.text = str(float(yaw))


if imu_euler:
	rospy.loginfo("Opening IMU_Euler...")
	sub = rospy.Subscriber('imu_euler', Float32MultiArray, processIMU_Euler)
else:
	rospy.loginfo("Opening IMU_Phantom...")
	sub = rospy.Subscriber('imu_phantom', Imu, processIMU_Phantom)
rospy.spin()

