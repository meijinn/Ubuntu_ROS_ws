#include <ros/ros.h>
#include <serial/serial.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Empty.h>
#include <std_msgs/UInt16MultiArray.h>

std_msgs::UInt16MultiArray servo;

serial::Serial ser;

void joy_callback(const sensor_msgs::Joy &joy_msg){

    servo.data[0] = ((joy_msg.axes[0]*180)+180)/2;
    float gas = joy_msg.axes[5]+93;
    float brake = (joy_msg.axes[2]*21*(-1)+207)/2;
    int throttle = int((gas+brake)/2);
    servo.data[1] = throttle;
    ROS_INFO("Writing to serial port:%d",servo.data[0]);
    ROS_INFO("Writing to serial port:%d",servo.data[1]);
    ser.write("abc");
}

int main (int argc, char** argv){
    ros::init(argc, argv, "ros_serial");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("joy", 10, joy_callback);
    ros::Publisher servo_pub = nh.advertise<std_msgs::UInt16MultiArray>("servo", 1);

    try//////////////////////////////////////////////////////////////////////////
    {
        ser.setPort("/dev/ttyUSB0");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }else{
        return -1;
    }//////////////////////////////////////////////////////////////////////////////

    ros::Rate loop_rate(100);///////////////////////////////////////////////////////////////////////////////////
    while(ros::ok()){

        servo.data.resize(2);
        servo_pub.publish(servo);
        ros::spinOnce();
        loop_rate.sleep();

    }
    return 0;
}