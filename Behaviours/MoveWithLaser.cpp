//Youbot behaviour for motion towards point {x, y}, avoiding obstacles with laser scanner
//Authors: S. Diane, ...
//Created: 21.10.2014

#include "includes.h"



void MoveWithLaser::Init(RosHelper* rh_)
{	
           Behaviour::Init(rh_);
}

float a=0, b=0, c=0;
void MoveWithLaser::Execute(float dt)
{
time+=dt;

rh->SendControl(0.01, 0, 0);
rh->SendInfo("MoveWithLaser - Test");


}
