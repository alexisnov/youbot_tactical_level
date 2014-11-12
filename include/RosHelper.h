//ROS helper functoins
//Authors: S. Diane, ...
//Created: 21.10.2014



 typedef void (*Topic_behaviour_callback)(const std_msgs::String& incoming_msg );
 //typedef void (*Topic_objects_callback)(const std_msgs::Float32MultiArray& incoming_msg );
 //typedef void (*Topic_odom_callback)(const geometry_msgs::Pose& incoming_msg );
 

 
class RosHelper
{
public:

ros::Publisher control_publisher;
std::string topic_control;

ros::Publisher info_publisher;
std::string topic_info;

ros::Publisher manipulator_publisher;//Команды на манипулятор
std::string topic_manipulator;

ros::Publisher gripper_publisher;//Команды на схват
std::string topic_gripper;

ros::Subscriber behaviour_subscriber;
std::string topic_behaviour;

ros::Subscriber objects_subscriber;//массив найденных объектов
std::string topic_objects;

ros::Subscriber odom_subscriber; //Данные одометрии
std::string topic_odom;


Topic_behaviour_callback tbc;
//Topic_odom_callback odomCallback;
//Topic_objects_callback objectsDetectedCallback;

std::vector <brics_actuator::JointValue> armJointPositions;
std::vector <brics_actuator::JointValue> gripperJointPositions;

bool cube_found;//Flag for cube detection
 std::vector<cv::Point2f> path;//path of robot
 double theta,robotX,robotY;//Robot odometry parameters
RosHelper(int argc, char **argv);//конструктор, инициализация
~RosHelper();//деструктор
void InitCallbacks(); //вторая часть инициализации
void SendControl(float fwd, float left, float rotLeft); //команда на движение робота
void SetManipulator(float j1, float j2, float j3,float j4, float j5);//команды на звенья манипулятора (в радианах)
void odomCallback(const nav_msgs::Odometry::ConstPtr& msg);
void objectsDetectedCallback(const std_msgs::Float32MultiArray::ConstPtr& msg);
void SetGripper(float delta);//команды на схват в см
void SendInfo(char* info); //обратная связь, в т.ч. на сервер
};
