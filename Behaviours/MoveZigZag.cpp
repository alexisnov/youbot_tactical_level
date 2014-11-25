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
float frontDist = rh->hokuyoRanges[rh->hokuyoRanges.size()/2];

if(1){
float dr =rh->movePoint(path.at(pointN));
if(dr<=0.01)
   {
   
    if(pointN+1<path.size()){
    pointN++;
	printf("Next point %f , %f\n",path.at(pointN).x, path.at(pointN).y) ;
	}else{
	printf("MoveZigZag Done!\n") ;
	}
    
   }
}else{
  printf("stop, frontDist %f\n",frontDist) ;
  rh->SendControl(0,0,0);//Stop
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
