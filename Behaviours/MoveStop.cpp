//Youbot behaviour for staying in place
//Authors: S. Diane, ...
//Created: 22.10.2014

#include "includes.h"

void MoveStop::Execute(float dt)
{	
time+=dt;

char str[100];
sprintf(str, "MoveStop, time=%.1f", time);
rh->SendInfo(str);

rh->SendControl(0, 0, 0);
}
