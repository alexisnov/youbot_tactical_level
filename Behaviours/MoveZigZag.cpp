//Youbot behaviour for zig-zag motion
//Authors: A. Novoselskiy, ...
//Created: 22.10.2014

#include "includes.h"
#define sign(x) (x<0)?-1:1
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
rh->SetManipulator(3.0,2.2,-2.0,3.3,3.0);
//printf("Set Manipulator start position\n");
rh->SetGripper(2.0);
//start motion

//float t=modf(time,60);
 float x=1;
 float dx = rh->path.at(pointN).x-rh->robotX;
 float dy = rh->path.at(pointN).y-rh->robotY;
 float dr = sqrt(pow(dx,2)+pow(dy,2));
 float dtheta = atan2(dy,dx)-rh->theta;
 float vx=0;
 if(dtheta>0.001) {
   vx = (sqrt(pow(dr,2))>x)?sign(dr)*x:x*dr;
 }
 float vy = (dy>x)?sign(dy)*x:x*dy;
 
 float vtheta = (sqrt(pow(dtheta,2))>0.005)?sign(dtheta)*x:x*dtheta;
 //printf("dx %f, dy %f\r\n",dx,dy);
 printf("vx %f, dr %f\r\n",vx,dr);
 printf("dtheta %f, vtheta %f\r\n",dtheta,vtheta);
 rh->SendControl(vx,0,vtheta);
 if(dr<=0.001)
   {
    pointN++;
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
