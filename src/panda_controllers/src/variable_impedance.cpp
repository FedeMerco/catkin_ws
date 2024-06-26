#include <ros/ros.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/WrenchStamped.h>
#include <fstream>
#include <vector>


geometry_msgs::Vector3Stamped stiffness;
ros::Publisher pub;
int imped_flag = 0;

// Questo valore iniziale oltre che da ordinata all'origine funge da valore base per assicurare un minimo di stiffness al robot
double initial_value = 0.505388523211304;
double final_value = 6.92675665037425;
double m = final_value - initial_value;    

double constant_stiffness = 50;
int limiter = 0;
double F_prev = 0;

void F_ext_callback(geometry_msgs::WrenchStamped F_ext) {
    
    if (limiter >= 90) {

        if(abs(F_ext.wrench.force.z - F_prev) >= 0.1){

            F_prev = F_ext.wrench.force.z;

            stiffness.vector.z = m * abs(F_ext.wrench.force.z) + initial_value + constant_stiffness;

            if (stiffness.vector.z >= 1000) {
                stiffness.vector.z = 1000;
            }


            pub.publish(stiffness);
        }

        std::cout << "\nForza: " << F_ext.wrench.force.z << std::endl;
        std::cout << "Stiffness: " << stiffness.vector.z << std::endl;
        limiter = 0;
    }



    ++limiter;
}





int main(int argc, char** argv) {
    ros::init(argc, argv, "variable_impedance_command");
    ros::NodeHandle nh;

    ros::Rate rate(1); // 2 Hz

    ROS_INFO("Modifica impedenza avviata");

    pub = nh.advertise<geometry_msgs::Vector3Stamped>("/cartesian_impedance_controller/desired_stiffness", 10);
    ros::Subscriber sub = nh.subscribe("/franka_state_controller/F_ext", 10, F_ext_callback);

    ros::spin();

    return 0;
}
