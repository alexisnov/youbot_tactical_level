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

           hl=new HokuyoLaser(0,0);


}

float a=0, b=0, c=0;
void MoveWithLaser::Execute(float dt)
{
time+=dt;

char str[100];

vector<long> data=hl->GetValues();
//print_data(*hl->urg, data);

float DR=0, DL=0;
int data_n=data.size();
float m=data_n/2.0;


float minD=100500;
float Threshold=1000;

int cnt=0;
for(int i=0;i<data_n;i++)
{
float x=data[i];
if(x<1) continue;

	if(x<minD )minD=x;
		if(i<=m) DR+=x;
		if(i>=m) DL+=x;
cnt++;
}

float rot, avg_D, danger;

rot=(DR-DL)/(DR+DL+0.01);
avg_D=(DR+DL)/cnt;
//danger=1-avg_D/Threshold; if(danger<0) danger=0;if(danger>1) danger=1;


   // std::cout <<"MoveWithLaser: avg_D/danger/rot="<<avg_D<<"/"<<danger<<"/"<<rot<<endl;

float a_=0.1, b_=rot/10;
//a_=0.7*a+0.3*a_;b_=0.7*b+0.3*b_;
rh->SendControl(a_, b_, 0);
a=a_; b=b_;

sprintf(str, "time=%.1f, avg_D=%.1f, rot=%.1f", time, avg_D, rot);
rh->SendInfo(str);


}
