#include <ros/ros.h>
#include <sensor_msgs/Joy.h>  
#include <std_msgs/UInt8MultiArray.h>

std_msgs::UInt8MultiArray controller;

void joy_callback(const sensor_msgs::Joy &joy_msg)
{
  // 処理内容を記述
  // joy_msg.axes[5]=gas, joy_msg.axes[2]=brake, joy_msg.axis[0]=steering
  controller.data[0] = ((joy_msg.axes[0]*(-1)*180)+180)/2;
  float gas = joy_msg.axes[5]+93;
  float brake = (joy_msg.axes[2]*21*(-1)+207)/2;
  int throttle = int((gas+brake)/2);
  controller.data[1] = throttle;
  //ROS_INFO("steering:%d",steering);   // スティック0の状態を表示 (-1 ～ 1)
  ROS_INFO("steering:%d",controller.data[0]);  // ボタン0の状態を表示 (0 or 1)
  ROS_INFO("throttle:%d",controller.data[1]);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "joy_pub_node");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("joy", 10, joy_callback);

  ros::Publisher controller_pub = nh.advertise<std_msgs::UInt8MultiArray>("controller", 1);
  ros::Rate loop_rate(100);

  while (ros::ok())
  {
    controller.data.resize(2);
    controller_pub.publish(controller);
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}