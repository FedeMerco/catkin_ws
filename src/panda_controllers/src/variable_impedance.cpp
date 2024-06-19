#include <ros/ros.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/WrenchStamped.h>

geometry_msgs::Vector3Stamped stiffness;
ros::Publisher pub;
int imped_flag = 0;

void F_ext_callback(geometry_msgs::WrenchStamped F_ext) {
    
    if (imped_flag == 0) {
        stiffness.vector.z = 200;
        imped_flag = 1;
    }

    if (imped_flag != 0) {
        stiffness.vector.z = 200;

        //stiffness.vector.z = 100 * F_ext.wrench.force.z;
    }

    // ROS_INFO_STREAM(stiffness);

    pub.publish(stiffness);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "variable_impedance_command");
    ros::NodeHandle nh;

    ROS_INFO("Modifica impedenza avviata");

    pub = nh.advertise<geometry_msgs::Vector3Stamped>("/cartesian_impedance_controller/desired_stiffness", 10);
    ros::Subscriber sub = nh.subscribe("/franka_state_controller/F_ext", 10, F_ext_callback);

    ros::spin();

    return 0;
}
