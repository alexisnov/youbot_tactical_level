//Youbot behaviour for zig-zag motion
//Authors: A. Novoselskiy, ...
//Created: 22.10.2014

#include "includes.h"
#define sign(x) ((x<0)?-1:1)
//#define abs(x) x*sign(x)
float modf(float a, float b)
{
	while(a>b) a-=b;
        return a;
}



void MoveZigZag::Execute(float dt)
{	
time+=dt;

char str[100];
sprintf(str, "MoveZigZag, time=%.1f", time);
rh->SendInfo(str);
//Set Manipulator in front position looking down
//rh->SetManipulator(3.0,2.2,-2.0,3.3,3.0);
//printf("Set Manipulator start position\n");
rh->SetGripper(2.0);
//start motion
/*
 double HectorPathFollower::headingDiff(double x, double y, double pt_x, double pt_y, double heading)
  {
double v1_x = x - pt_x;
    double v1_y = y - pt_y;
    double v2_x = cos(heading);
    double v2_y = sin(heading);

    double perp_dot = v1_x * v2_y - v1_y * v2_x;
    double dot = v1_x * v2_x + v1_y * v2_y;

    //get the signed angle
    double vector_angle = atan2(perp_dot, dot);

    return -1.0 * vector_angle;
	}
*/
float v=0.5;
 float dx = rh->path.at(pointN).x-rh->robotX;
 float dy = rh->path.at(pointN).y-rh->robotY;
 float vx,vy,vtheta=0;
 float dr = sqrt(pow(dx,2)+pow(dy,2));
 float dtheta = atan2(dy,dx)-rh->theta;
 
 if(dtheta>0.001) {
   vx = (sqrt(pow(dr,2))>v)?sign(dr)*v:v*dr;
 }
 //float vy = (dy>v)?sign(dy)*v:v*dy;
 
 vtheta = (sqrt(pow(dtheta,2))>0.005)?sign(dtheta)*v:v*dtheta;
 printf("dx %f, dy %f, dr %f, dtheta %f, theta %f\r\n",dx,dy,dr,dtheta,rh->theta);
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
 rh->SendControl(vx,0,vtheta);

//float t=modf(time,60);

 if(dr<=0.01)
   {
   
    if(pointN+1<rh->path.size()){
    pointN++;
	}
    printf("Next point %f , %f\n",rh->path.at(pointN).x, rh->path.at(pointN).y) ;
   }

if (!rh->cube_found){
	
  /*if(time<5)
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
  */
	if(detect_time!=0){
		detect_time=0;
	}
}else{
	if(detect_time==0){
		detect_time=time;
	}
	float dt = time-detect_time;
	rh->SendControl(0,0,0);//Stop
	rh->SetManipulator(3.0,2.2,-2.0,3.3,3.0);
	if(dt>5){
	rh->SetGripper(0);//Close
	}
	if(dt>10)
	{
		rh->SetManipulator(3.0,2.2,-2.0,3.3,3.0);
	}
}   

}
