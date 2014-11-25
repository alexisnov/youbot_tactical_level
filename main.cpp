//Youbot tactical level commands
//Authors: S. Diane, A. Novoselsky
//Created: 20.10.2014

#include "includes.h"

using namespace luabridge;

static Behaviour* b=NULL;
RosHelper* rh=NULL;
 
lua_State* L;
static bool received_LUA_MSG=false;
std::string LUA_MSG; // команда Lua (принимается извне (из топика ROS))

#define UPDATE_BEHAVIOUR {if(b!=NULL) b->Init(rh); delete ::b; ::b=b;}

<<<<<<< HEAD
void LUA_MoveWithLaser(){ printf("LUA_MoveWithLaser\n"); 
	
	MoveWithLaser* b=new MoveWithLaser();	
		UPDATE_BEHAVIOUR		
}

void LUA_MoveZigZag(float dx, float dy, float perN){printf("LUA_MoveZigZag\n"); 
	
	MoveZigZag* b=new MoveZigZag();	
	for (int i=0;i<perN;i++){
		  b->path.push_back(cv::Point2f(dx,-dy*i));
		  b->path.push_back(cv::Point2f(dx,-dy*(i+1)));
		  b->path.push_back(cv::Point2f(0,-dy*(i+1)));
		  b->path.push_back(cv::Point2f(0,-2*dy*(i+1)));      
		}  		
		UPDATE_BEHAVIOUR		
}
=======
static int mode=-1; // выбор режима
static int last_mode=mode;
static int dx,dy,perN;
>>>>>>> origin/master

void LUA_MoveSpin(){ printf("LUA_MoveSpin\n"); 
	
	MoveSpin* b=new MoveSpin();	
		UPDATE_BEHAVIOUR		
}

void LUA_MoveStop(){ printf("LUA_MoveStop\n"); 
	
	MoveStop* b=new MoveStop();	
		UPDATE_BEHAVIOUR		
}

<<<<<<< HEAD
void LUA_NoMove(){ printf("LUA_MoveStop\n"); 
	
	delete ::b;
}
=======
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
>>>>>>> origin/master



void lua_init()
{
L = luaL_newstate(); 
     
	   luaL_openlibs(L); //io, math, etc.

	   //привязываем функции C++ к одноименным (хотя не обязательно) функциям Lua
		getGlobalNamespace(L)
	.addFunction("LUA_MoveWithLaser", LUA_MoveWithLaser)
	.addFunction("LUA_MoveZigZag", LUA_MoveZigZag)
	.addFunction("LUA_MoveSpin", LUA_MoveSpin)
	.addFunction("LUA_MoveStop", LUA_MoveStop)
	.addFunction("LUA_NoMove", LUA_MoveStop);
	
	
	
	//ЗАГРУЗКА ФАЙЛОВ ЕЩЕ ПРИГОДИТСЯ ...
	    // luaL_dostring(L, "a = 10 + 5"); 
    // lua_getglobal(L, "a"); 
    // int i = lua_tointeger(L, -1); 
    // printf("%d\n", i); 
	// const char* path="/home/u3/ytl/data/lua/test.lua";

	// if (luaL_loadfile(L, path) ) {
    // printf("Lua script not found\n");
	// }
	
	
	// luaL_dofile(L, path);
 
    // lua_pcall(L, 0, 0, 0); //выполнить действия в глобальном пространстве скрипта
	// luaL_dostring(L, "helloWorld()"); 
	// printf("\n");
	
     
  //  lua_close(L); 
}

void topic_behaviour_callback( const std_msgs::String& s )
{
  //std::cout << s.data.c_str() << std::endl;

	//while(b_lock){ros::Rate(100).sleep();}
<<<<<<< HEAD
    LUA_MSG=s.data;
	received_LUA_MSG=true;
	
	// mode = atoi(str.substr(0,1).c_str());
	// if (mode==1){
	 // int i_dx = str.find(":dx")+3;
	 // dx = atoi(str.substr(i_dx,i_dx+1).c_str());
	 // int i_dy = str.find(":dy")+3;
	 // dy = atoi(str.substr(i_dy,i_dy+1).c_str());
	 // int i_pN = str.find(":pN")+3;
	  // perN = atoi(str.substr(i_pN,i_pN+1).c_str());
	// }
    // std::cout <<"Received mode: "<< mode<<" dx: "<< dx<<" dy "<< dy <<" pN "<< perN<< std::endl;
	
	std::cout <<"Received Lua command"<<std::endl;
	
	
=======
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
>>>>>>> origin/master
}

int main(int argc, char **argv)
{ 
    std::cout << "Youbot tactical level started (12)!" << std::endl;
	
	lua_init();	

    rh=new RosHelper(argc, argv);
	rh->tbc=topic_behaviour_callback;
	rh->InitCallbacks();
<<<<<<< HEAD
=======

//send_img();

>>>>>>> origin/master

    double hertz=10;

        ros::Rate loop_rate(hertz);

    while(ros::ok())
    {

<<<<<<< HEAD
		if(received_LUA_MSG)
		{			
		received_LUA_MSG=false;
	luaL_dostring(L, LUA_MSG.c_str());
		}
=======
	if(mode!=last_mode)
	{
		InitBehaviour(mode, rh);
	}

	if(b)
	{
	
>>>>>>> origin/master

		if(b)
		{
		

<<<<<<< HEAD
		  b->Execute(1/hertz);

		}
			ros::spinOnce();
			
		
			loop_rate.sleep();
=======
	}
		
        ros::spinOnce();
        loop_rate.sleep();
>>>>>>> origin/master

    }

    delete b;
delete rh;
lua_close(L); 

  return 0;
}


