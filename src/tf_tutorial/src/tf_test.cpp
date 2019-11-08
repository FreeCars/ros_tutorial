#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>

#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/String.h"
#include <sstream>

#include <memory>

#include <tf/transform_listener.h>
#include "tf2/LinearMath/Transform.h"
#include "tf2/convert.h"
#include "tf2/utils.h"

#include "tf2_ros/buffer.h"

using namespace std;


int main(int argc, char **argv)
{
   ros::init(argc, argv, "tester");
   ros::NodeHandle n;

   tf::TransformListener listener;
   std::shared_ptr<tf2_ros::Buffer> tf_;

   tf_.reset(new tf2_ros::Buffer());

   ros::Rate loop_rate(1);
   int count = 0;
   while(ros::ok())
   {
       std_msgs::String msg;
       std::stringstream ss;

       ss<< "deb " << count;

       msg.data = ss.str();

       //ROS_INFO("%s", msg.data.c_str());

       
       geometry_msgs::PoseStamped turtle_loc_in_world;

       tf::StampedTransform trans;
       try
        {
            //获得2到1的变换，也就是相对坐标,也就是以2为坐标原点，因为2是targetframe
            listener.lookupTransform("/world", "/turtle1",    
                               ros::Time(0), trans);  
        }
        catch (tf::TransformException &ex) 
        {
            ROS_ERROR("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }
      // tf_->setTransform(trans, "aa");

    /*   geometry_msgs::PoseStamped ident;
       ident.header.frame_id = "turtle1";  //雷达的坐标系
       ident.header.stamp = ros::Time();

       tf2::toMsg(tf2::Transform::getIdentity(), ident.pose);
    */

      geometry_msgs::PointStamped laser_point;
      laser_point.header.frame_id = "turtle1";
      laser_point.header.stamp = ros::Time();

      laser_point.point.x= 0;//赋予一个随机值

      laser_point.point.y= 0;//赋予一个随机值

      laser_point.point.z= 0;//赋予一个随机值

      geometry_msgs::PoseStamped p;

      p.header.frame_id = "turtle1";
      p.header.stamp = ros::Time();

      p.pose.position.x = 2;
      p.pose.position.y = 0;
      p.pose.position.z = 0;

      geometry_msgs::Quaternion q_tmp;
      q_tmp.w=1;
      p.pose.orientation = q_tmp;

      // 四元数转欧拉角
      tf::Matrix3x3 m(trans.getRotation());
      double roll, pitch, yaw;
      m.getRPY(roll, pitch, yaw);      

      // 欧拉角转四元数
      tf::Quaternion q;
      q.setRPY(roll, pitch, yaw);

      //tf_->transform(ident, turtle_loc_in_world, "world");

      geometry_msgs::PointStamped base_point;//实例化一个geometry_msgs::PointStamped类，表示机器人底座
      geometry_msgs::PoseStamped p_world;

      listener.transformPose("world", p, p_world);


    //调用transformlistener的transformpoint函数

      listener.transformPoint("world",laser_point,base_point);//参数目标框架target_frame,stamped_in以及stamped_out。
 //     listener.transformPose

      //输出相关信息
      ROS_INFO("turtle1_pose:(%.2f,%.2f,%.2f) --> world:(%.2f, %.2f, %.2f)at t %.2f" ,
      p.pose.position.x, p.pose.position.y, p.pose.orientation.z,
      p_world.pose.position.x, p.pose.position.y, yaw * 180.0 / 3.141592653,
      p_world.header.stamp.toSec()
      );

      //  ROS_INFO("turtle1: (%.2f, %.2f. %.2f) -----> wordl: (%.2f, %.2f, %.2f) at time %.2f",
      //  laser_point.point.x, laser_point.point.y, laser_point.point.z,
      //  base_point.point.x, base_point.point.y, base_point.point.z, base_point.header.stamp.toSec());
       
       ros::spinOnce();
       loop_rate.sleep();
       ++count;
   }


}
