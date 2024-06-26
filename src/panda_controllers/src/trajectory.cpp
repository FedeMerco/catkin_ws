#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <math.h>

geometry_msgs::PoseStamped msg;
int mov_flag = 0;
int i = 1;
double alpha = 0;

ros::Publisher pub;

geometry_msgs::PoseStamped home;

geometry_msgs::PoseStamped lowPose;

geometry_msgs::PoseStamped forwardPose;

const int waypoints = 50;

geometry_msgs::PoseStamped calcolo_waypoints(int index, geometry_msgs::PoseStamped start, geometry_msgs::PoseStamped goal){
    
    alpha = index/waypoints;

    geometry_msgs::PoseStamped pose;

    pose.pose.position.x = (1 - alpha) * goal.pose.position.x + alpha * start.pose.position.x;
    pose.pose.position.y = (1 - alpha) * goal.pose.position.y + alpha * start.pose.position.y;
    pose.pose.position.z = (1 - alpha) * goal.pose.position.z + alpha * start.pose.position.z;

    return pose;

}


int main(int argc, char** argv) {
    ros::init(argc, argv, "trajectory_command");
    ros::NodeHandle nh;
    ros::Rate rate(5); // 2 Hz

    ROS_INFO("Traiettoria impostata");

    home.pose.position.x = 0.30707093243261097;
    home.pose.position.y = -0.00004975443320201987;
    home.pose.position.z = 0.4871414711004457;

    lowPose.pose.position.x = 0.30707093243261097;
    lowPose.pose.position.y = -0.00004975443320201987;
    lowPose.pose.position.z = 0.482;

    forwardPose.pose.position.x = 0.32;
    forwardPose.pose.position.y = -0.00004975443320201987;
    forwardPose.pose.position.z = 0.482;

    pub = nh.advertise<geometry_msgs::PoseStamped>("/cartesian_impedance_controller/equilibrium_pose", 10);

    while(ros::ok()) {
        if(mov_flag == 0) {

            msg = calcolo_waypoints(i, home, lowPose);
            pub.publish(msg);

            i++;        

            if (i >= waypoints) {
                i = 1;
                mov_flag = 1;
            }

            // ROS_INFO_STREAM("PUNTO RAGGIUNTO: 1");


        }else if (mov_flag == 1){

            msg = calcolo_waypoints(i, lowPose, forwardPose);
            pub.publish(msg);

            i++;        

            if (i >= waypoints) {
                i = 1;
                mov_flag = 2;
            }

            // ROS_INFO_STREAM("PUNTO RAGGIUNTO: 2");

        } else {

            msg = calcolo_waypoints(i, forwardPose, home);
            pub.publish(msg);

            i++;        

            if (i >= waypoints) {
                i = 1;
                mov_flag = 0;
            }

            // ROS_INFO_STREAM("PUNTO RAGGIUNTO: 3");

        }

        rate.sleep();
    }


    return 0;
}