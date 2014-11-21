//ROS helper functoins (implementation)
//Authors: S. Diane, ...
//Created: 21.10.2014

#include "includes.h"
#define sign(x) ((x<0)?-1:1)


RosHelper::RosHelper(int argc, char **argv)
{
topic_control="/cmd_vel";
topic_info="/tactical_info";
topic_behaviour="/cmd_behaviour";
topic_manipulator="/arm_1/arm_controller/position_command";
topic_gripper="/arm_2/gripper_controller/position_command";
topic_objects="objects";
topic_odom = "/odom";
topic_laser="/scan";

armJointPositions.resize(5);
gripperJointPositions.resize(2);

   ros::init(argc, argv, "youbot_tactical_level");
    ros::start(); // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;

    //Ros communications:
    control_publisher = n.advertise<geometry_msgs::Twist>(topic_control, 1000);	
    info_publisher = n.advertise<std_msgs::String>(topic_info, 1000);	
    manipulator_publisher = n.advertise<brics_actuator::JointPositions>(topic_manipulator, 1000);
    gripper_publisher =  n.advertise<brics_actuator::JointPositions>(topic_gripper, 1000);   
    cube_found=false;
    
	cntLaser=cntOdom=0;
}
float RosHelper::movePoint(cv::Point2f &target)
{//Движение к точке
  float v=0.5;
 float dx = target.x-robotX;
 float dy = target.y-robotY;

 float vx,vy,vtheta=0;
 float dr = sqrt(pow(dx,2)+pow(dy,2));

 float dtheta = angleBetween(target.x, target.y, robotX, robotY, theta);
 
 if(dtheta>0.001) {
   vx = (sqrt(pow(dr,2))>v)?sign(dr)*v:v*dr;
 }
 //float vy = (dy>v)?sign(dy)*v:v*dy;
 
 vtheta = (sqrt(pow(dtheta,2))>0.005)?sign(dtheta)*v:v*dtheta;
 printf("dx %f, dy %f, dr %f, dtheta %f, theta %f\r\n",dx,dy,dr,dtheta,theta);
 printf("vx %f, dr %f\r\n",vx,dr);
 printf("dtheta %f, vtheta %f\r\n",dtheta,vtheta);
 if(fabs(dtheta) >0.02){
  vx=0;
  if (fabs(dtheta) <1){
     vtheta = 0.5*dtheta;
  }else {
     vtheta = 0.5*sign(dtheta);
  }
 }else{
  vtheta = 0.5*dtheta;
  if(fabs(dr)>=0.1){ vx=v;}else {vx=v*dr;}
 }
 if(fabs(dy)>=0.01){ vy=v*sign(dy);}else{vy=0;}
 SendControl(vx,0,vtheta);
 
 return dr;

//float t=modf(time,60);

}
void RosHelper::InitCallbacks()
{
  ros::NodeHandle n;
    behaviour_subscriber = n.subscribe(topic_behaviour, 1000, tbc);	
    objects_subscriber = n.subscribe(topic_objects, 1000, &RosHelper::objectsDetectedCallback, this);
    odom_subscriber=n.subscribe(topic_odom, 1000, &RosHelper::odomCallback, this);
	laser_subscriber=n.subscribe(topic_laser, 1000, &RosHelper::LaserScanCallback, this);
}

RosHelper::~RosHelper()
{
   if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
    wait();
}

float RosHelper::angleBetween(float x, float y, float pt_x, float pt_y, float heading)
{
    float v1_x=x-pt_x;
	float v1_y=y-pt_y;
	float v2_x = cos(heading);
	float v2_y = sin(heading);
	
	float perp_dot = v1_x*v2_y-v1_y*v2_x;
	float dot = v1_x*v2_x+v1_y*v2_y;
	
	//get the signed angle
	double vector_angle = atan2(perp_dot,dot);
	return -1.0*vector_angle;
}

void RosHelper::odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  theta=tf::getYaw(msg->pose.pose.orientation);
  robotX=msg->pose.pose.position.x;
  robotY=msg->pose.pose.position.y;
  //printf("Odom recieved, pose x=%f ,y=%f, a=%f\n",robotX,robotY, theta*180/3.14);
	
	//msg->postion;
}

void RosHelper::objectsDetectedCallback(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
    
    if(msg->data.size())
    {
	     printf("%d object(s) found! \n",(int)msg->data.size());
       	     cube_found = true;
             for(unsigned int i=0; i<msg->data.size(); i+=12)
                {
                        // get data
                        int id = (int)msg->data[i];
                        float objectWidth = msg->data[i+1];
                        float objectHeight = msg->data[i+2];

                // Find corners OpenCV
				cv::Mat cvHomography(3, 3, CV_32F);
				cvHomography.at<float>(0,0) = msg->data[i+3];
				cvHomography.at<float>(1,0) = msg->data[i+4];
				cvHomography.at<float>(2,0) = msg->data[i+5];
				cvHomography.at<float>(0,1) = msg->data[i+6];
				cvHomography.at<float>(1,1) = msg->data[i+7];
				cvHomography.at<float>(2,1) = msg->data[i+8];
				cvHomography.at<float>(0,2) = msg->data[i+9];
				cvHomography.at<float>(1,2) = msg->data[i+10];
				cvHomography.at<float>(2,2) = msg->data[i+11];
				std::vector<cv::Point2f> inPts, outPts;
				inPts.push_back(cv::Point2f(0,0));
				inPts.push_back(cv::Point2f(objectWidth,0));
				inPts.push_back(cv::Point2f(0,objectHeight));
				inPts.push_back(cv::Point2f(objectWidth,objectHeight));
				cv::perspectiveTransform(inPts, outPts, cvHomography);

				printf("Cube detected at (%f,%f) \n",(outPts.at(0).x-outPts.at(1).x)/2, (outPts.at(0).y-outPts.at(2).y)/2);
								
				}

    }else{
     cube_found=false;
    }
    
}
float RosHelper::get_laserscan_integral_value(std::vector<float> ranges)
{
	 float brR=0, brL=0;
		float m=ranges.size()/2.0;

		int cnt=0;
		for(int i=0;i<ranges.size();i++)
		{
		float x=ranges[i];
		
		if(x<10) continue;
		
		if(x>10000) x=10000;
		
		float br=1/x; //brightness

				if(i<=m) brR+=br;
				if(i>=m) brL+=br;
		cnt++;
		}

		float rot;

		rot=(brR-brL)/(brR+brL+0.01);
		
		return rot;
}
void RosHelper::LaserScanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  cntLaser++;
  /*
  if(cntLaser==2){
  std::stringstream ss;
  ss << ".l#";
  //msg->ranges - Массив Float32 (в Vector`e)
  for(int i=0;i<msg->ranges.size();i++){
    ss << msg->ranges[i] << ";";
  }
  const std::string& tmp = ss.str();
  const char* cstr = tmp.c_str();
  SendInfo(tmp.c_str());
  cntLaser=0;
  }
  */
 
if(cntLaser==2){
float rot=get_laserscan_integral_value(msg->ranges);
hokuyoRanges=msg->ranges;
 char str[100];
  sprintf(str, ".laser_mini#%f",  rot);
SendInfo(str);

  cntLaser=0;
  }
  SendInfo("hi");
 


}

void RosHelper::SendControl(float fwd, float left, float rotLeft)
{
    geometry_msgs::Twist msg;
    msg.linear.x=fwd;
    msg.linear.y=left;
    msg.angular.z=rotLeft;
    control_publisher.publish(msg);

    //std::cout <<"fwd/left/rotLeft: "<<fwd<<"; "<<left<<"; "<<rotLeft<<endl;

}
//Joint values in radians
void RosHelper::SetManipulator(float j1, float j2, float j3,float j4, float j5)
{
    brics_actuator::JointPositions msg;
    
    armJointPositions[0].joint_uri = "arm_joint_1";
    armJointPositions[0].value = j1;
    armJointPositions[0].unit = "rad";
    armJointPositions[1].joint_uri = "arm_joint_2";
    armJointPositions[1].value = j2;
    armJointPositions[1].unit = "rad";
    armJointPositions[2].joint_uri = "arm_joint_3";
    armJointPositions[2].value = j3;
    armJointPositions[2].unit = "rad";
    armJointPositions[3].joint_uri = "arm_joint_4";
    armJointPositions[3].value = j4;
    armJointPositions[3].unit = "rad";
    armJointPositions[4].joint_uri = "arm_joint_5";
    armJointPositions[4].value = j5;
    armJointPositions[4].unit = "rad";
    msg.positions = armJointPositions;
    manipulator_publisher.publish(msg);
}
//Gripper delta in cm
void RosHelper::SetGripper(float delta)
{
	if(delta>=0 && delta<=2){
	brics_actuator::JointPositions msg;
	gripperJointPositions[0].joint_uri = "gripper_finger_joint_l";
    gripperJointPositions[0].value = delta/200;
 gripperJointPositions[0].unit = "m";
    gripperJointPositions[1].joint_uri = "gripper_finger_joint_r";
    gripperJointPositions[1].value = delta/200;
gripperJointPositions[1].unit = "m";
    msg.positions = gripperJointPositions;
    gripper_publisher.publish(msg);
	}
    
}

void RosHelper::SendInfo(const char* info)
{
    std_msgs::String msg;
	std::stringstream ss;
	 ss << info;
	  msg.data = ss.str();

   
    info_publisher.publish(msg);

}


