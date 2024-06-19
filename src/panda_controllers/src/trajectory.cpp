#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <math.h>

geometry_msgs::PoseStamped msg;
double tollerance = 0.05;
int sw_flag = 1;
int mov_flag = 0;

// double r = 0.5;
// double r = 0.1;
double r = 0.01; // r1
// double r = 0.001; // r2
// double r = 0.0001; // r3
// double r = 0.00001; // r4

// ciao

ros::Publisher pub;

void ee_pose_callback(geometry_msgs::PoseStamped ee_pose) {
    // Coordinate di spawn del braccio in Gazebo
    // msg.pose.position.x = 0.30707093243261097;
    // msg.pose.position.y = -0.00004975443320201987;
    // msg.pose.position.z = 0.4871414711004457;

    // Movimento sinusoidale lungo z
    //msg.pose.position.z = r * sin(ros::Time::now().toSec()) + 0.4871414711004457;

    if(sw_flag == 0){
        
        msg.pose.position.x = 0.30707093243261097;
        msg.pose.position.y = -0.00004975443320201987;
        msg.pose.position.z = 0.4871414711004457;

        // ros::Duration(3.0).sleep();           

        if(mov_flag == 0){

            sw_flag = 1;
            
        } else {
            
            sw_flag = 3;

        }

        ROS_INFO_STREAM("Passo 0");

        pub.publish(msg);

    } else if(sw_flag == 1){

        msg.pose.position.x = 0.30707093243261097;
        msg.pose.position.y = -0.00004975443320201987;
        msg.pose.position.z = 0.28;

        ros::Duration(3.0).sleep(); 

        // momentaneamente aumento la tolleranza di 0.12, il box aggiunto ha un'altezza di 0.4, per simulare il raggiungimento del punto aumento la tolleranza di 0.4 - 0.28
        if((abs(ee_pose.pose.position.z - msg.pose.position.z)) <= tollerance + 0.12) {
            sw_flag = 2;
        }

        ROS_INFO_STREAM("Passo 1");
        pub.publish(msg);

    } else if(sw_flag == 2){

        msg.pose.position.x = 0.4;
        msg.pose.position.y = -0.00004975443320201987;
        msg.pose.position.z = 0.28;
        
        ros::Duration(3.0).sleep();             

        if((abs(ee_pose.pose.position.x - msg.pose.position.x)) <= tollerance) {
            sw_flag = 0;
            mov_flag = 1;
        } 

        ROS_INFO_STREAM("Passo 2");
        pub.publish(msg);

    } else if(sw_flag == 3) {

        msg.pose.position.x = 0.30707093243261097;
        msg.pose.position.y = -0.00004975443320201987;
        msg.pose.position.z = 0.35;

        ros::Duration(3.0).sleep(); 

        // allo stesso modo del caso precedente aumento la tolleranza di 0.4 - 0.35 = 0.05

        // In una prima prova questa tolleranza non basta, provo ad aumentarla giusto per farlo funzionare

        if((abs(ee_pose.pose.position.z - msg.pose.position.z)) <= tollerance + 0.05) {
            sw_flag = 4;
        }

        ROS_INFO_STREAM("Passo 3");
        pub.publish(msg);

    } else if(sw_flag == 4) {

        msg.pose.position.x = 0.4;
        msg.pose.position.y = -0.00004975443320201987;
        msg.pose.position.z = 0.35;

        ros::Duration(3.0).sleep(); 

        if((abs(ee_pose.pose.position.x - msg.pose.position.x)) <= tollerance) {
            sw_flag = 0;
            mov_flag = 0;
        }

        ROS_INFO_STREAM("Passo 4");
        pub.publish(msg);
    }

    // msg.pose.position.x = 0.30707093243261097;
    // msg.pose.position.y = -0.00004975443320201987;
    // msg.pose.position.z = 0.1;

    // Comando su orientazione end-effector (Ancora non funziona)
    // msg.pose.orientation.x = 0.9999934357993759;
    // msg.pose.orientation.y = -8.222881628074834e-05;
    // msg.pose.orientation.z = 0.0028850600275304727;
    // msg.pose.orientation.w = 5.479228092606895e-05;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "trajectory_command");
    ros::NodeHandle nh;

    ROS_INFO("Traiettoria impostata");

    pub = nh.advertise<geometry_msgs::PoseStamped>("/cartesian_impedance_controller/equilibrium_pose", 10);
    ros::Subscriber sub = nh.subscribe("/cartesian_impedance_controller/franka_ee_pose", 10, ee_pose_callback);

    ros::spin();

    return 0;
}