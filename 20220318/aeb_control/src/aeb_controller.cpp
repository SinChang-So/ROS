#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"
#include "std_msgs/Bool.h"

std_msgs::Bool flag_AEB;
float range_n = 0.0;

void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
	range_n = msg->range;
	//ROS_INFO("Sonar Seq: [%d]", msg->header.seq);
	//ROS_INFO("Sonar Range: [%f]", range_n);
	//range_n = msg->range;

}
	

/*
void UltraSonarCallback1(const sensor_msgs::Range::ConstPtr& msg)
{
	ROS_INFO("Sonar1 Seq: [%d]", msg->header.seq);
	ROS_INFO("Sonar1 Range: [%f]", msg->range);
	
}
*/
int main(int argc, char **argv)
{
	int count = 0;
	ros::init(argc, argv, "aeb_controller");
	
	ros::NodeHandle n;
	ros::Rate loop_rate(1);
	
	ros::Subscriber sub = n.subscribe("range", 1000,UltraSonarCallback);
	ros::Publisher pub = n.advertise<std_msgs::Bool>("/bool",1000);
	//ros::Subscriber sub1 = n.subscribe("/RangeSonar1", 1000,UltraSonarCallback);
	
	while (ros::ok())
	{
		ROS_INFO("Sonar Range!!!: [%f]", range_n);
		if(range_n <=1.0)
		{
			//ROS_INFO("AEB_Activation!!");
			flag_AEB.data = true;
			pub.publish(flag_AEB);
		}
		else
		{
			flag_AEB.data = false;
			pub.publish(flag_AEB);
		}
		
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	return 0;
}
