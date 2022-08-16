
#include <Servo.h>
#include <ros.h>
#include <std_msgs/UInt16MultiArray.h>

Servo servo;
Servo speedcontroller;

int TRIG = 3;
int ECHO = 2;
const int BRAKE = 93;


double duration = 0;
double distance = 0;
double speed_of_sound = 331.5 + 0.6 * 25; // 25℃の気温の想定

ros::NodeHandle nh;

void servoCb( const std_msgs::UInt16MultiArray& cmd_msg) {
  servo.write(cmd_msg.data[0]);
  
  float dist = getRange_Ultrasound();
  
  if(dist < 20){
    cmd_msg.data[1] = BRAKE;
  }
  speedcontroller.write(cmd_msg.data[1]); 
}

ros::Subscriber<std_msgs::UInt16MultiArray> sub("servo", servoCb);

float getRange_Ultrasound()
{
  pinMode(TRIG, OUTPUT);
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  pinMode(ECHO, INPUT);
  duration = pulseIn(ECHO, HIGH);
  
  if (duration > 0) {
    duration = duration / 2; // 往路にかかった時間
    distance = duration * speed_of_sound * 100 / 1000000;
    return distance;
  }
}

void setup(){
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);
  
  servo.attach(10);
  speedcontroller.attach(6);
}


void loop(){
  nh.spinOnce();
  delay(1);
}
