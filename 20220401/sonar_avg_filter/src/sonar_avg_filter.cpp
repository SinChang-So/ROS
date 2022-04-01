#include "ros/ros.h"
#include "math.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"


int count =0;
double sum = 0.0;
double result = 0.0;
sensor_msgs::Range sonar_average;
void Avg_UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
	//OS_INFO("Sonar Seq: [%d]", msg->header.seq);
	//ROS_INFO("Sonar Range: [%f]", msg->range);
	if (count <5)
	{
		sum +=msg->range;
		count++;
	}
	else 
	{
		result = sum/5;
		count =0;
		sum =0;
		
	}
	printf("%.2lf\n",result);
	sonar_average.range = result;
	
	
}


int main(int argc, char **argv)
{
	ros::init(argc, argv,"sonar_avg_filter_node");
	ros::NodeHandle n;
	ros::Rate loop_rate(10);
	
	ros::Subscriber sub = n.subscribe("range", 1000, Avg_UltraSonarCallback);
	ros::Publisher avg_range_pub = n.advertise<sensor_msgs::Range>("/rang_avg", 1000);
	
	
	while(ros::ok())
	{
		avg_range_pub.publish(sonar_average);
		loop_rate.sleep();
		ros::spinOnce();
	}
}
