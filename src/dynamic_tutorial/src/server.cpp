#include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <dynamic_tutorial/tutorialConfig.h>

#include <dynamic_tutorial/test.h>

void callback(dynamic_tutorial::tutorialConfig &config, uint32_t level) {
  ROS_INFO("Reconfigure Request: %d %f %s %s %d my=%d", 
            config.int_param, config.double_param, 
            config.str_param.c_str(), 
            config.bool_param?"True":"False", 
            config.size, 
            sum(config.int_param, 100)
            );
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "dynamic_tutorial");

  dynamic_reconfigure::Server<dynamic_tutorial::tutorialConfig> server;
  dynamic_reconfigure::Server<dynamic_tutorial::tutorialConfig>::CallbackType f;

  f = boost::bind(&callback, _1, _2);
  server.setCallback(f);

  ROS_INFO("Spinning node");
  ros::spin();
  return 0;
}


