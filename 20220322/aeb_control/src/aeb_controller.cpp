#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"

std_msgs::Bool flag_AEB;
std_msgs::Float32 delta_range;
double old_sonar_range = 0;
geometry_msgs::Twist cmd_vel_msg;

void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
	ROS_INFO("Sonar Seq: [%d]", msg->header.seq);
	ROS_INFO("Sonar Range: [%f]", msg->range);
	
	if(msg -> range <= 1.0)
	{
		ROS_INFO("AEB_Activation!!");
		flag_AEB.data = true;
	}
	else
	{
		flag_AEB.data = false;
	}
	if((msg->range) != old_sonar_range)
	{
		delta_range.data = ((msg->range) - old_sonar_range);
		printf("\n delta_range : %lf ", delta_range.data);
	}
	old_sonar_range = msg->range;
}

/*void UltraSonarCallback2(const sensor_msgs::Range::ConstPtr& msg)
{
	ROS_INFO("Sonar2 Seq: [%d]", msg->header.seq);
	ROS_INFO("Sonar2 Range: [%f]", msg->range);
	
	if(msg -> range <= 1.0)
	{
		ROS_INFO("AEB_Activation!! ");
		flag_AEB.data = true;
	}
	else
	{
		flag_AEB.data = false;
	}
}*/


void CarControlCallback(const geometry_msgs::Twist& msg)
{
	ROS_INFO("Cmd_vel : linear_x [%f]",msg.linear.x); //speed
	cmd_vel_msg.linear.x = msg.linear.x;
	ROS_INFO("Cmd_vel : angular_z [%f]",msg.angular.z);//steer
	cmd_vel_msg.angular.z = msg.angular.z;
}

int main(int argc, char **argv)
{
	int count = 0;
	
	ros::init(argc, argv,"aeb_controller");
	
	ros::NodeHandle n;
	
	ros::Rate loop_rate(10);
	
	// ros::Publisher pub1 = n.advertise<std_msgs::Bool>("/AEB", 1000);
	ros::Publisher aeb_pub = n.advertise<std_msgs::Bool>("/aeb_activation_flag", 1);
	ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("/ackermann_steering_controller/cmd_vel", 10);
	ros::Publisher delta_range_pub = n.advertise<std_msgs::Float32>("/delta_range", 1000);
	 
	
	ros::Subscriber sub = n.subscribe("range", 1000, UltraSonarCallback);
	//ros::Subscriber sub2 = n.subscribe("/RangeSonar1", 1000, UltraSonarCallback2);
	ros::Subscriber cmd_sub = n.subscribe("/cmd_vel", 10, CarControlCallback);
	while(ros::ok())
	{
		delta_range_pub.publish(delta_range);
		if((count % 10)==0)
		{
			aeb_pub.publish(flag_AEB);
		}
		
		if(flag_AEB.data == true)
		{
			cmd_vel_msg.linear.x = 0;
			cmd_pub.publish(cmd_vel_msg);
		}
		else
		{
			cmd_pub.publish(cmd_vel_msg);
		}
		
		
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	
	return 0;
}
