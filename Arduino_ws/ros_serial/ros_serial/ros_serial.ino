
#include <Servo.h>
#include <ros.h>
#include <std_msgs/UInt16.h>

ros::NodeHandle nh;

Servo servo;

void servoCb( const std_msgs::UInt16& cmd_msg) {
  servo.write(cmd_msg.data);
  //digitalWrite(11, HIGH-digitalRead(11));
}

ros::Subscriber<std_msgs::UInt16> sub("servo", servoCb);

void setup(){
  pinMode(11, OUTPUT);
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);

  servo.attach(9);
}

void loop(){
  nh.spinOnce();
  delay(1);
}
