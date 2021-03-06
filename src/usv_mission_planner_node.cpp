#include "ros/ros.h"                                  // ROS
#include "std_msgs/String.h"                          // ROS_INFO
#include <nav_msgs/Odometry.h>                        // usv position estimation
#include <geometry_msgs/PoseStamped.h>                // usv_mission_planner goal msg

#include "../include/usv_guider/PriorityQueue.hpp"
#include "../include/usv_guider/Pos.hpp"

#include <sstream>

#define ROS_MAIN_LOOP___TIME_INTERVAL                   2
#define PUBLISH_QUEUE_AMOUNT___USV_MP___GOAL_COMMAND    10
#define SUBSCRIBE_QUEUE_AMOUNT___USV___CURRENT_POSITION 1
#define ACCEPTABLE_RADIUS                               0.5

std::queue<Pos>* Plan_Mission1();
std::queue<Pos>* Plan_Mission2();
std::queue<Pos>* Plan_Mission3();
std::queue<Pos>* Plan_Mission_Diluvio();
std::queue<Pos> *Plan_Mission_Diluvio_Tunning();
std::queue<Pos>* Plan_Mission_Diluvio_HeadOn_Huang2019Generalized();
std::queue<Pos>* Plan_Mission_Diluvio_CrossingRight_Huang2019Generalized();
std::queue<Pos>* Plan_Mission_Diluvio_Overtake();
std::queue<Pos> *Plan_Mission_Diluvio_Left();


void usv_state_callback(const nav_msgs::Odometry::ConstPtr& usv_position_msg);

geometry_msgs::Point usv_current_pos;

int main(int argc, char **argv){
  
  // Basic configuration to start node and configure a publisher
  ros::init(argc, argv, "usv_guider_node"); // Basic iniitalization for ROS node
  ros::NodeHandle n;                        // Handler to interact with ROS
  ros::Publisher  usv_mp_pub = n.advertise<geometry_msgs::PoseStamped>("/diffboat1/move_base_simple/goal", PUBLISH_QUEUE_AMOUNT___USV_MP___GOAL_COMMAND); // Declaration of ROS topic and creation of a publishing handler for usv_mission_planner goal command 
  ros::Subscriber usv_mp_sub  = n.subscribe("/diffboat1/state", SUBSCRIBE_QUEUE_AMOUNT___USV___CURRENT_POSITION, usv_state_callback);
  ros::Rate loop_rate(ROS_MAIN_LOOP___TIME_INTERVAL); 

  // USV Mision Planner position goal command message
  geometry_msgs::PoseStamped      usv_mp_goal_msg;
  usv_mp_goal_msg.header.stamp    = ros::Time::now();
  usv_mp_goal_msg.header.frame_id = "diffboat1/map";  

  // Gets mission plan
  // std::queue<Pos> *path = Plan_Mission_Diluvio();
  // std::queue<Pos> *path = Plan_Mission_Diluvio_HeadOn_Huang2019Generalized();
  // std::queue<Pos> *path = Plan_Mission_Diluvio_CrossingRight_Huang2019Generalized();
  std::queue<Pos> *path = Plan_Mission_Diluvio_Overtake();
  // std::queue<Pos> *path = Plan_Mission_Diluvio_Left();

  // std::queue<Pos> *path = Plan_Mission_Diluvio_Tunning();
  // std::queue<Pos> *path = Plan_Mission2();
  

  // Gets first goal
  Pos next_goal;
  next_goal.x = path->front().x;
  next_goal.y = path->front().y;
  path->pop();

  // Handler to publish usv goal
  geometry_msgs::Point point;
  point.x = next_goal.x;
  point.y = next_goal.y;
  point.z = 0;

  // configure goals orientation
  usv_mp_goal_msg.pose.orientation.x = 0;
  usv_mp_goal_msg.pose.orientation.y = 0;
  usv_mp_goal_msg.pose.orientation.z = 0;
  usv_mp_goal_msg.pose.orientation.w = 1;

  // publish first subgoal
  usv_mp_goal_msg.pose.position = point;
  usv_mp_pub.publish(usv_mp_goal_msg);

  while (ros::ok()){

    ROS_INFO("Current x: %f y: %f",   usv_current_pos.x, usv_current_pos.y);
    ROS_INFO("Next    x: %f y: %f\n", next_goal.x, next_goal.y);

    // Set new subgoal location after reach current subgoal
    if((abs(usv_current_pos.x - next_goal.x) <= ACCEPTABLE_RADIUS) && (abs(usv_current_pos.y - next_goal.y) <= ACCEPTABLE_RADIUS)){ // Tests if achieved subgoal

      if(!path->empty()){

        next_goal = path->front();                  // Gets next subgoal location
        path->pop();                                // Removes from path data structure

        // Update subgoal
        point.x = next_goal.x;
        point.y = next_goal.y;
        usv_mp_goal_msg.pose.position = point;

      }else{

        //restart
        // path = Plan_Mission2();
        // path = Plan_Mission_Diluvio();
        // path = Plan_Mission_Diluvio_HeadOn_Huang2019Generalized();  
        // path = Plan_Mission_Diluvio_CrossingRight_Huang2019Generalized();
        path = Plan_Mission_Diluvio_Overtake();
        // path = Plan_Mission_Diluvio_Left();
        // path = Plan_Mission_Diluvio_Tunning();
            }

    }

    usv_mp_pub.publish(usv_mp_goal_msg);

    // Attending to subscription callbacks
    ros::spinOnce();
    loop_rate.sleep();

  }

  ros::spin();

  return 0;
}


void usv_state_callback(const nav_msgs::Odometry::ConstPtr& usv_position_msg){

  usv_current_pos = usv_position_msg->pose.pose.position;
  // ROS_INFO("USV current position: X: %f, Y: %f", usv_current_pos.x, usv_current_pos.y);
  // ROS_INFO("C_x - N_x: %f - %f --- C_y - N_y: %f - %f", usv_current_pos.x, next_goal.x, usv_current_pos.y, next_goal.y);

}

std::queue<Pos>* Plan_Mission1(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  test.x = (double) 13;
  test.y = (double) 2;
  path->push(test);

  test.x = (double) 16;
  test.y = (double) 3;
  path->push(test);

  test.x = (double) 16;
  test.y = (double) 9;
  path->push(test);

  test.x = (double) 5;
  test.y = (double) 9;
  path->push(test);

  test.x = (double) 5;
  test.y = (double) 3;
  path->push(test);  

  return path;
  
}

std::queue<Pos>* Plan_Mission2(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  test.x = (double) 18;
  test.y = (double) 5.5;
  path->push(test);

  test.x = (double) 2;
  test.y = (double) 5;
  path->push(test);
  
  // test.x = (double) 2;
  // test.y = (double) 5;
  // path->push(test);
  
  // test.x = (double) 18;
  // test.y = (double) 5.5;
  // path->push(test);

  return path;
}

std::queue<Pos>* Plan_Mission3(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  test.x = (double) 5;
  test.y = (double) 3;
  path->push(test);

  test.x = (double) 16;
  test.y = (double) 2;
  path->push(test);

  test.x = (double) 16;
  test.y = (double) 8;
  path->push(test);

  test.x = (double) 4;
  test.y = (double) 8;
  path->push(test);

  test.x = (double) 2;
  test.y = (double) 5;
  path->push(test);

  test.x = (double) 16;
  test.y = (double) 2;
  path->push(test);

  test.x = (double) 4;
  test.y = (double) 8;
  path->push(test);

  test.x = (double) 5;
  test.y = (double) 3;
  path->push(test);

  test.x = (double) 16;
  test.y = (double) 8;
  path->push(test);

  test.x = (double) 2;
  test.y = (double) 5;
  path->push(test);


  return path;
  
}

std::queue<Pos>* Plan_Mission_Diluvio(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  test.x = (double) 478;
  test.y = (double) 115;
  path->push(test);

  test.x = (double) 460;
  test.y = (double) 115;
  path->push(test);
  
  // test.x = (double) 2;
  // test.y = (double) 5;
  // path->push(test);
  
  // test.x = (double) 18;
  // test.y = (double) 5.5;
  // path->push(test);

  return path;
}

std::queue<Pos>* Plan_Mission_Diluvio_Tunning(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  test.x = (double) 466;
  test.y = (double) 115;
  path->push(test);

  test.x = (double) 446;
  test.y = (double) 105;
  path->push(test);
  
  // test.x = (double) 2;
  // test.y = (double) 5;
  // path->push(test);
  
  // test.x = (double) 18;
  // test.y = (double) 5.5;
  // path->push(test);

  return path;
}

std::queue<Pos>* Plan_Mission_Diluvio_HeadOn_Huang2019Generalized(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  // test.x = (double) 462;
  // test.y = (double) 107.5;
  // path->push(test);
  
  // test.x = (double) 480;
  // test.y = (double) 107.5;
  // path->push(test);

  // test.x = (double) 460;
  // test.y = (double) 107.5;
  // path->push(test);

  test.x = (double) 480;
  test.y = (double) 109;
  path->push(test);

  // test.x = (double) 460;
  // test.y = (double) 112;
  // path->push(test);


  // test.x = (double) 446;
  // test.y = (double) 105;
  // path->push(test);
  
  // test.x = (double) 2;
  // test.y = (double) 5;
  // path->push(test);
  
  // test.x = (double) 18;
  // test.y = (double) 5.5;
  // path->push(test);

  return path;
}

std::queue<Pos>* Plan_Mission_Diluvio_CrossingRight_Huang2019Generalized(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  test.x = (double) 410;
  test.y = (double) 133;
  path->push(test);

  // test.x = (double) 446;
  // test.y = (double) 105;
  // path->push(test);
  
  // test.x = (double) 2;
  // test.y = (double) 5;
  // path->push(test);
  
  // test.x = (double) 18;
  // test.y = (double) 5.5;
  // path->push(test);

  return path;
}

std::queue<Pos>* Plan_Mission_Diluvio_Overtake(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  // test.x = (double) 600;
  // test.y = (double) 107.5;
  // path->push(test);

  test.x = (double) 550;
  test.y = (double) 107.5;
  path->push(test);

  // test.x = (double) 446;
  // test.y = (double) 105;
  // path->push(test);
  
  // test.x = (double) 2;
  // test.y = (double) 5;
  // path->push(test);
  
  // test.x = (double) 18;
  // test.y = (double) 5.5;
  // path->push(test);

  return path;
}

std::queue<Pos>* Plan_Mission_Diluvio_Left(){

  std::queue<Pos> *path = new std::queue<Pos>();
  Pos test;

  test.x = (double) 410;
  test.y = (double) 133;
  path->push(test);
  
  // test.x = (double) 18;
  // test.y = (double) 5.5;
  // path->push(test);

  return path;
}
