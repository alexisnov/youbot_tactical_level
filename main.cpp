//Youbot tactical level commands
//Authors: S. Diane, ...
//Created: 20.10.2014

#include "includes.h"

//#include "image-publish.h"

static Behaviour* b=NULL;
RosHelper* rh=NULL;


static int mode=-1; // выбор режима
static int last_mode=mode;


void InitBehaviour(int mode, RosHelper* rh)
{
	std::cout << "Switching mode: "<<mode << std::endl;

delete b;
	switch(mode)
	{
	    case 0: b=new MoveWithLaser(); std::cout <<"MoveWithLaser"<< std::endl; break;
	   case 1: b=new MoveZigZag(); std::cout <<"MoveZigZag"<< std::endl; break;
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

	mode=atoi(s.data.c_str());
std::cout <<"Received mode: "<< mode << std::endl;
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


