#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");

  ros::NodeHandle node;

  ros::service::waitForService("spawn");  //应该是等待第一只乌龟启动完毕
  ros::ServiceClient add_turtle =
    node.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn srv;
  add_turtle.call(srv);  //每调用一次多一只乌龟在地图中，launch文件中使用<node pkg="turtlesim" type="turtlesim_node" name="sim"/>已经启动了一只
  add_turtle.call(srv);

  ros::Publisher turtle_vel =
    node.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);

 // ros::Publisher turtle_vel1 =
 //   node.advertise<geometry_msgs::Twist>("turtle3/cmd_vel", 10);

  tf::TransformListener listener;

  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    tf::StampedTransform transform32;

    try{
#if(0)
      ros::Time past = ros::Time::now() - ros::Duration(5.0);
      listener.waitForTransform("/turtle2", "/turtle1",
                              ros::Time(0), ros::Duration(1.0));

      //获得2到1的变换，也就是相对坐标,也就是以2为坐标原点，from turtle2 to turtle1
      listener.lookupTransform("/turtle2", "/turtle1",    
                               ros::Time(0), transform);  
#else
      //下面，获取当前turtle2到5秒前turtle1的变换
      ros::Time now = ros::Time::now();
      ros::Time past = now - ros::Duration(5.0);  // 5 seconds ago
      listener.waitForTransform("/turtle2", now,
 				"/turtle1", past,
                                "/world",   //specify a frame that does not change over time
                                ros::Duration(1.0));

      //获得2到1的变换，也就是相对坐标,也就是以2为坐标原点，from turtle2 to turtle1
      listener.lookupTransform("/turtle2", now,
 				"/turtle1", past,
                                "/world",   //specify a frame that does not change over time
                                transform);
#endif

    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }


    geometry_msgs::PoseStamped msg;
    msg.header.frame_id = "xxxxx";
    msg.header.stamp = ros::Time::now();
    //////////////////////////////////////////////////////just for test
    tf2::Quaternion q;
    q.setRPY(0, 0, 3.141526 / 2);  //还发生了90度的旋转
    tf2::Transform tmp_tf(q, tf2::Vector3(3, 8, 0));
    //这个函数仅仅是简单的把transform转换成pose
    tf2::toMsg(tmp_tf.inverse(), msg.pose);

    //ROS_INFO("%s %f %f \r\n", msg.header.frame_id.c_str(), msg.pose.position.x, msg.pose.position.y);

   

    geometry_msgs::PoseStamped ident;
    ident.header.frame_id = "turtle1";  //雷达的坐标系
    ident.header.stamp = ros::Time();
    // getIdentity： Return an identity transform.
    // ident.pose: The Transform converted to a geometry_msgs Pose message type

    // tf2::Transform::getIdentity得到一个0变换？？？然后得到的pose是没东西？全是0？？
    tf2::toMsg(tf2::Transform::getIdentity(), ident.pose);  //不知道有什么用

    
    geometry_msgs::Twist vel_msg;
    double y = transform.getOrigin().y();
    double x = transform.getOrigin().x();
    vel_msg.angular.z = x < 0.001 && x > -0.001 ? 0 : 4 * atan2(y, x);  //加入分母项为0检测，避免x过小时候求解得到的角度不稳定，乌龟原地胡乱打转
    ROS_INFO("tf_y:%.2f tf_x:%.2f angular:%.2f", y, x, vel_msg.angular.z);
    vel_msg.linear.x = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
                               pow(transform.getOrigin().y(), 2));
    turtle_vel.publish(vel_msg);
/*
    vel_msg.angular.z = 4.0 * atan2(transform32.getOrigin().y(),
                                    transform32.getOrigin().x());
    vel_msg.linear.x = 0.5 * sqrt(pow(transform32.getOrigin().x(), 2) +
                                  pow(transform32.getOrigin().y(), 2));
    turtle_vel1.publish(vel_msg);
*/
    rate.sleep();
  }
  return 0;
};


