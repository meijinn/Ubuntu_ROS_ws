#include <ros/ros.h>
#include <sensor_msgs/Joy.h>

void joy_callback(const sensor_msgs::Joy &joy_msg)
{
  // 処理内容を記述
  // 5 gas, 2 brake, 0 steering
  int steering = int(((joy_msg.axes[0]*180)+180)/2);
  float gas = joy_msg.axes[5]+93;
  float brake = (joy_msg.axes[2]*21*(-1)+207)/2;
  int throttle = int((gas+brake)/2);
  ROS_INFO("steering:%d",steering);   // スティック0の状態を表示 (-1 ～ 1)
  ROS_INFO("throttle:%d",throttle);  // ボタン0の状態を表示 (0 or 1)
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "joy_sub_node");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("joy", 10, joy_callback);
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}