//Youbot behaviour for motion towards point {x, y}, avoiding obstacles with laser scanner
//Authors: S. Diane, ...
//Created: 21.10.2014

#include "includes.h"


void print_data(const Urg_driver& urg, const vector<long>& data)
{

    size_t data_n = data.size();
    for (size_t i = 0; i < data_n; ++i) {
        long l = data[i];
cout << l << ", ";	

        //double radian = urg.index2rad(i);
        //long x = static_cast<long>(l * cos(radian));
        //long y = static_cast<long>(l * sin(radian));
        //cout << "(" << x << ", " << y << ") ";
    }
    cout << endl<< endl<< endl;
}


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
