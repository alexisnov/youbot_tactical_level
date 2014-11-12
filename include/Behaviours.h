//Youbot behaviour interface and classes
//Authors: S. Diane, ...
//Created: 21.10.2014

#include "includes.h"

class Behaviour {
public:
RosHelper* rh;
float time;

    virtual ~Behaviour() {} // destructor, use it to call destructor of the inherit classes
    virtual void Execute(float dt){}
virtual void Init(RosHelper* rh_){rh=rh_; time=0;}
};


//Youbot behaviour for motion towards point {x, y}, avoiding obstacles with laser scanner
class MoveWithLaser:public Behaviour {
public:
HokuyoLaser* hl;
~MoveWithLaser(){if(hl!=NULL) delete hl;}
    void Execute(float dt);
void Init(RosHelper* rh_);
};

//Youbot behaviour for zig-zag motion
class MoveZigZag:public Behaviour {

public:  
    void Execute(float dt);
};


//Youbot behaviour for rotation at place
class MoveSpin:public Behaviour {

public:  
    void Execute(float dt);
};

//Youbot behaviour for doing nothing
class MoveStop:public Behaviour {

public:  
    void Execute(float dt);
};


