#include <ros/ros.h>

#include "tf2/LinearMath/Transform.h"
#include "tf2/convert.h"
#include "tf2/utils.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/message_filter.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2_ros/transform_listener.h"

#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"

#include <turtlesim/Spawn.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "test_listener");

  ros::NodeHandle node;

  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);

  geometry_msgs::PoseStamped ident;
  ident.header.frame_id = "aa";//baselink
  ident.header.stamp = ros::Time::now();
  tf2::toMsg(tf2::Transform::getIdentity(), ident.pose);//ident is all 0

  geometry_msgs::PoseStamped odom_pose;
  //odom_pose.header.stamp = ros::Time::now();
  //odom_pose.header.frame_id = "world";


  ros::Rate rate(10.0);
  while (node.ok())
  {
    geometry_msgs::TransformStamped transformStamped;
  //  tf2::toMsg(transformStamped.transform, odom_pose.pose);
    try{
  //    transformStamped = tfBuffer.lookupTransform("world", "turtle1",
   //                            ros::Time(0));

       //下面方法坐标变换，结果还是坐标，ident是原点坐标，求原点坐标在world的坐标
       tfBuffer.transform(ident, odom_pose, "aa");
      // 下面方法获取的是变换，world to aa的变换
      // transformStamped = tfBuffer.lookupTransform("world", "aa", ros::Time(0));
    }
    catch (tf2::TransformException &ex) {
      ROS_WARN("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
    ROS_INFO("frame:%s x:%.1f y:%.1f z:%.1f; \r\n --ident: x:%.1f y:%.1f z:%.1f;ox:%.1f oy:%.1f oz:%.1f, ow:%.1f;",
             odom_pose.header.frame_id.c_str(), odom_pose.pose.position.x, odom_pose.pose.position.y, odom_pose.pose.position.z,
             ident.pose.position.x, ident.pose.position.y, ident.pose.position.z,
             ident.pose.orientation.x, ident.pose.orientation.y,ident.pose.orientation.z,ident.pose.orientation.w
         );

    rate.sleep();
  }
  return 0;
};



