//ROS helper functoins
//Authors: S. Diane, ...
//Created: 21.10.2014



 typedef void (*Topic_behaviour_callback)( const std_msgs::String& incoming_msg );

class RosHelper
{
public:

ros::Publisher control_publisher;
std::string topic_control;

ros::Publisher info_publisher;
std::string topic_info;

ros::Subscriber behaviour_subscriber;
std::string topic_behaviour;

Topic_behaviour_callback tbc;

RosHelper(int argc, char **argv);//конструктор, инициализация
~RosHelper();//деструктор
void InitCallbacks(); //вторая часть инициализации
void SendControl(float fwd, float left, float rotLeft); //команда на движение робота
void SendInfo(char* info); //обратная связь, в т.ч. на сервер
};
