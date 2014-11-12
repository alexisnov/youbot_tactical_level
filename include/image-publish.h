

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>

void send_img()
{

printf("start3\n");

 cv::Mat image = cv::imread("/home/u3/Dropbox/ubuntu_files/img/test.bmp", CV_LOAD_IMAGE_COLOR);
 
ros::NodeHandle nh;
ros::Publisher img_pub = nh.advertise<sensor_msgs::Image>("/camera/image", 1000);

printf("rows %d, cols %d\n", image.rows, image.cols);



ros::Rate loop_rate(1);
    while(ros::ok())
    {
//printf("C\n");
sensor_msgs::Image msg;
  cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg(msg);
img_pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }


}

