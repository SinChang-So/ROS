#include "ros/ros.h"
#include "math.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"
#include "nav_msgs/Odometry.h"
#define d_t 0.02
#define s_dis 3
std_msgs::Bool flag_AEB;
std_msgs::Float32 delta_range;
nav_msgs::Odometry current_pos, delta_pos, old_pos;
geometry_msgs::Twist cmd_vel_msg;

float x,y;
double old_sonar_range;
float v_x,v_y;
float delta_x, delta_y;
float aeb_collision_distance = 200.;

void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
	ROS_INFO("Sonar Seq: [%d]", msg->header.seq);
	ROS_INFO("Sonar Range: [%f]", msg->range);
	
	aeb_collision_distance = v_x * (0.7+0.1) * 0.22778 * 2.5; //1m/sec = 0.22778km/h
	
	if(msg -> range <= aeb_collision_distance)
	{
		//ROS_INFO("AEB_Activation!!");
		flag_AEB.data = true;
	}
	else
	{
		flag_AEB.data = false;
	}
	
	
	/*if((msg->range) != old_sonar_range)
	{
		delta_range.data = ((msg->range) - old_sonar_range);
		//printf("\n delta_range : %lf ", delta_range.data);
	}
	old_sonar_range = msg->range;*/
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
	//ROS_INFO("Cmd_vel : linear_x [%f]",msg.linear.x); //speed
	cmd_vel_msg.linear.x = msg.linear.x;
	//ROS_INFO("Cmd_vel : angular_z [%f]",msg.angular.z);//steer
	cmd_vel_msg.angular.z = msg.angular.z;
}
void odomCallback(const nav_msgs::Odometry& msg)
{
	float old_x, old_y;
	float m_dis;
	
	//ROS_INFO("%.2lf %.2lf", msg.pose.pose.position.x, msg.pose.pose.position.y);
	x = msg.pose.pose.position.x;
	y = msg.pose.pose.position.y;
	
	
	if((x != old_x) || (y != old_y))
	{
		delta_x = x - old_x;
		delta_y = y - old_y;
	}
	
	old_x = x;
	old_y = y;
	v_x = delta_x*d_t;
	v_y = delta_y*d_t;
	
	m_dis = sqrt((x*x)+(y*y));
	
	if(m_dis > s_dis) flag_AEB.data = true;
	else flag_AEB.data = false;
	
	/*current_pos.pose.pose.position.x = msg.pose.pose.position.x;
	current_pos.pose.pose.position.y = msg.pose.pose.position.y;
	
	if((current_pos.pose.pose.position.x != old_pos.pose.pose.position.x) || (current_pos.pose.pose.position.y != old_pos.pose.pose.position.y))
	{
		delta_pos.pose.pose.position.x = current_pos.pose.pose.position.x - old_pos.pose.pose.position.x;
		delta_pos.pose.pose.position.y = current_pos.pose.pose.position.y - old_pos.pose.pose.position.y;
	}
	
	old_pos.pose.pose.position.x = current_pos.pose.pose.position.x;
	old_pos.pose.pose.position.y = current_pos.pose.pose.position.y;
	v_x = delta_pos.pose.pose.position.x*d_t;
	v_y = delta_pos.pose.pose.position.y*d_t;
	//printf("%.2lf    %.2lf \n", v_x,v_y);*/
}

int main(int argc, char **argv)
{
	int count = 0;
	
	ros::init(argc, argv,"aeb_controller");
	
	ros::NodeHandle n;
	
	ros::Rate loop_rate(10);
	std::string odom_sub_topic = "/ackermann_steering_controller/odom";
	
	
	// ros::Publisher pub1 = n.advertise<std_msgs::Bool>("/AEB", 1000);
	ros::Publisher aeb_pub = n.advertise<std_msgs::Bool>("/aeb_activation_flag", 1);
	ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("/ackermann_steering_controller/cmd_vel", 10);
	ros::Publisher delta_range_pub = n.advertise<std_msgs::Float32>("/delta_range", 1000);
	
	ros::Subscriber sub = n.subscribe("range", 1000, UltraSonarCallback);
	//ros::Subscriber sub2 = n.subscribe("/RangeSonar1", 1000, UltraSonarCallback2);
	ros::Subscriber cmd_sub = n.subscribe("/cmd_vel", 10, CarControlCallback);
	ros::Subscriber sub_odom = n.subscribe(odom_sub_topic,10,&odomCallback);
	while(ros::ok())
	{
		delta_range_pub.publish(delta_range);
		if((count % 10)==0)
		{
			aeb_pub.publish(flag_AEB);
		}
		
		if(flag_AEB.data == true)
		{
			if(cmd_vel_msg.linear.x>0) cmd_vel_msg.linear.x = 0;
			cmd_pub.publish(cmd_vel_msg);
		}
		else
		{
			cmd_pub.publish(cmd_vel_msg);
		}
		//collision_distance = v_x* (1/10.);
		
		ROS_INFO("Odom : [%6.3f %6.3f] m | Veloctiy : [%6.3f %6.3f] m/s", x, y, v_x, v_y);
		ROS_INFO("Collision Distance : %6.3f", aeb_collision_distance);
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	
	return 0;
}
