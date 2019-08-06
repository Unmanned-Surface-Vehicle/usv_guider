# USV Guidance System - CPP
A COLREGs-Compliant USV Guider ROS package.

## USV Guider
@brief: Implements a whole USV Guidance System, responsible by mission planning and path planning.  
@currently: disabled.

### USV Mission Planner
@brief: Responsible by reasoning about USV mission and define a plan.  
@currently: Generates hard-coded position GOALs and send them to move_base input.  

## How to Install

    $ cd catkin_ws/src
    $ git clone https://github.com/Unmanned-Surface-Vehicle/usv_guider.git
    $ cd ~/catkin_ws
    $ catkin_make_isolated --install --pkg usv_guider
    $ source install_isolated/setup.bash

## How to Run

The usv_guider package must be executed after the execution of the USV_SIM_LSA, so execute it first:

    $ roslaunch usv_sim movebase_navigation1.launch parse:=true
    $ roslaunch usv_sim movebase_navigation1.launch parse:=false

Then run the current active usv_guider package node:  

    $ rosrun usv_guider usv_mission_planner_node 
  
