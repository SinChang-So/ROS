#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"
#include "std_msgs/Bool.h"


void ActivationCallback(const std_msgs::Bool::ConstPtr& msg)
{
	 if(msg->data == 1)
	 {
		 ROS_INFO("AEB_Activation!!");
	 }
	 else
	 {
		 ROS_INFO("NOT AEB_Activation!!");
	 }
}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "aeb_controller_sub");
	
	ros::NodeHandle n;
	ros::Rate loop_rate(100);
	
	ros::Subscriber sub = n.subscribe("/bool", 1000, ActivationCallback);
	
	while (ros::ok())
	{
		
		loop_rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
