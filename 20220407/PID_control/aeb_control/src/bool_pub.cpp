#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Bool.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::Bool::ConstPtr& msg)
{
	if(msg->data == true)	ROS_INFO("AEB_Activation!!");
	else if(msg->data == false)	ROS_INFO("NO_AEB_Activation!!");
}

int main(int argc, char **argv)
{
	int count = 0;
	ros::init(argc, argv, "bool_pub");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/aeb_activation_flag", 1, chatterCallback);
	ros::Rate loop_rate(10); //10
	
	while(ros::ok())
	{	
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	return 0;
}
