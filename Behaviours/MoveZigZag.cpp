//Youbot behaviour for zig-zag motion
//Authors: A. Novoselskiy, ...
//Created: 22.10.2014

#include "includes.h"
#include <std_msgs/Float32MultiArray.h>
#include <brics_actuator/JointPositions.h>
#include <opencv2/opencv.hpp>
#include <QTransform>
float modf(float a, float b)
{
	while(a>b) a-=b;
return a;
}
void objectsDetectedCallback(const std_msgs::Float32MultiArray & msg)
{
    printf("---\n");
    if(msg.data.size())
    {
                /*for(unsigned int i=0; i<msg.data.size(); i+=12)
                {
                        // get data
                        int id = (int)msg.data[i];
                        float objectWidth = msg.data[i+1];
                        float objectHeight = msg.data[i+2];

                        // Find corners Qt
                        QTransform qtHomography(msg.data[i+3], msg.data[i+4], msg.data[i+5],
                                                                        msg.data[i+6], msg.data[i+7], msg.data[i+8],
                                                                        msg.data[i+9], msg.data[i+10], msg.data[i+11]);

                        QPointF qtTopLeft = qtHomography.map(QPointF(0,0));
                        QPointF qtTopRight = qtHomography.map(QPointF(objectWidth,0));
                        QPointF qtBottomLeft = qtHomography.map(QPointF(0,objectHeight));
                        QPointF qtBottomRight = qtHomography.map(QPointF(objectWidth,objectHeight));
						
                        printf("Object %d detected, Qt corners at (%f,%f) (%f,%f) (%f,%f) (%f,%f)\n",
                                        id,
                                        qtTopLeft.x(), qtTopLeft.y(),
                                        qtTopRight.x(), qtTopRight.y(),
                                        qtBottomLeft.x(), qtBottomLeft.y(),
                                        qtBottomRight.x(), qtBottomRight.y());

                        
                }*/
				
    }
    
}


void MoveZigZag::Execute(float dt)
{	
time+=dt;

char str[100];
sprintf(str, "MoveZigZag, time=%.1f", time);
rh->SendInfo(str);


float t=modf(time,60);

float x=0.1;
if(time<5)
rh->SendControl(x,0, 0); // up
else if(time<15)
rh->SendControl(0,x, 0); // half-right
else if(time<20)
rh->SendControl(x,0, 0);//up
else if(time<40)
rh->SendControl(0,-x, 0); //left
else if(time<45)
rh->SendControl(x,0, 0); // half-right
else if(time<50)
rh->SendControl(x,0, 0); // up
else
rh->SendControl(0,x, 0); // half-right

//rh->SendControl(0.5, 2*sin(time/10), 0);
    ros::NodeHandle objects_handle;
    ros::Subscriber subs;
    subs = objects_handle.subscribe("objects", 1, objectsDetectedCallback);

    ros::spin();
}
