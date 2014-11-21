//Youbot tactical level commands
//Authors: S. Diane, ...
//Created: 20.10.2014

#include "includes.h"

//#include "image-publish.h"

static Behaviour* b=NULL;
RosHelper* rh=NULL;


static int mode=-1; // выбор режима
static int last_mode=mode;
static int dx,dy,perN;

void InitBehaviour(int mode, RosHelper* rh)
{
	std::cout << "Switching mode: "<<mode << std::endl;

delete b;
	switch(mode)
	{
	    case 0: b=new MoveWithLaser(); std::cout <<"MoveWithLaser"<< std::endl; break;
	   case 1: b=new MoveZigZag(); std::cout <<"MoveZigZag"<< std::endl;  
		for (int i=0;i<perN;i++){
		  b->path.push_back(cv::Point2f(dx,-dy*i));
		  b->path.push_back(cv::Point2f(dx,-dy*(i+1)));
		  b->path.push_back(cv::Point2f(0,-dy*(i+1)));
		  b->path.push_back(cv::Point2f(0,-2*dy*(i+1)));      
		}   
	   break;
	    case 2: b=new MoveSpin(); std::cout <<"MoveSpin"<< std::endl; break;
	    case 3: b=new MoveStop(); std::cout <<"MoveStop"<< std::endl; break;
	    default: b=NULL; std::cout <<"Wrong mode"<< std::endl; break;
	}

if(b!=NULL) b->Init(rh);

	last_mode=mode;
}

void topic_behaviour_callback( const std_msgs::String& s )
{
  //std::cout << s.data.c_str() << std::endl;

	//while(b_lock){ros::Rate(100).sleep();}
    std::string str=s.data;
	
	mode = atoi(str.substr(0,1).c_str());
	if (mode==1){
	 int i_dx = str.find(":dx")+3;
	 dx = atoi(str.substr(i_dx,i_dx+1).c_str());
	 int i_dy = str.find(":dy")+3;
	 dy = atoi(str.substr(i_dy,i_dy+1).c_str());
	 int i_pN = str.find(":pN")+3;
	  perN = atoi(str.substr(i_pN,i_pN+1).c_str());
	}
    std::cout <<"Received mode: "<< mode<<" dx: "<< dx<<" dy "<< dy <<" pN "<< perN<< std::endl;
}

int main(int argc, char **argv)
{ 



    std::cout << "Hello World (11)!" << std::endl;

    rh=new RosHelper(argc, argv);
	rh->tbc=topic_behaviour_callback;
	rh->InitCallbacks();

//send_img();


    double hertz=10;

        ros::Rate loop_rate(hertz);


    while(ros::ok())
    {

	if(mode!=last_mode)
	{
		InitBehaviour(mode, rh);
	}

	if(b)
	{
	

	  b->Execute(1/hertz);

	}
		
        ros::spinOnce();
        loop_rate.sleep();

    }

    delete b;
delete rh;


  return 0;
}


