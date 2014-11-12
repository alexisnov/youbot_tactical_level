//Youbot behaviour for spinning with no linear move
//Authors: S. Diane, ...
//Created: 22.10.2014

#include "includes.h"


void MoveSpin::Execute(float dt)
{	


time+=dt;

char str[100];
sprintf(str, "MoveSpin, time=%.1f", time);
rh->SendInfo(str);


rh->SendControl(0, 0, 0.1);
}
