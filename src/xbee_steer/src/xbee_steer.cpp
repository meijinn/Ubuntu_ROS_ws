#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/UInt16MultiArray.h>

std_msgs::UInt16MultiArray servo; //standard messageをUint16MultiArray型でservoという名前をつける。

void joy_callback(const sensor_msgs::Joy &joy_msg)
{
  // joy_msg.axis[1]=gas, joy_msg.axis[2]=brake, joy_msg.axis[0]=steering
  servo.data[0] = ((joy_msg.axes[0]*(-1)*180)+180)/2; //servo.data[0]にステアリング入力を記述
  //throttle入力をgasとbrakeに分ける。
  float gas = joy_msg.axes[1]*(-1)+93; //servo.data[0]にステアリング入力を 
  float brake = (joy_msg.axes[2]*21+207)/2;
  int throttle = int((gas+brake)/2);
  servo.data[1] = throttle; //servo.data[1]にステアリング入力を記述
  ROS_INFO("steering:%d",servo.data[0]);  // arduino側に送信される、steeringの入力値をプリント
  ROS_INFO("throttle:%d", servo.data[1]); // arduino側に送信される、throttleの入力値をプリント
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "joy_pub_node");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("joy", 10, joy_callback);

  ros::Publisher servo_pub = nh.advertise<std_msgs::UInt16MultiArray>("servo", 1);
  ros::Rate loop_rate(100); //100Hzで動作するように設定

  while (ros::ok())
  {
    servo.data.resize(2); //servo配列のサイズを2に設定
    servo_pub.publish(servo); //servoの配列データservo.data[0],servo.data[1]をpublish
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}