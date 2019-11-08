#include "ros/ros.h"
#include "test/AddTwoInts.h"

bool add(test::AddTwoInts::Request  &req,
         test::AddTwoInts::Response &res)  //注意到res是引用
{
  res.sum = req.a + req.b;
  ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("sending back response: [%ld]", (long int)res.sum);
  return true;  //返回true，client会收到这个true
} 

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_two_ints_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("add_two_ints", add);
  ROS_INFO("Ready to add two ints.");
  ros::spin();

  return 0;
}



