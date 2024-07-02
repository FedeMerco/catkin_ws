#include <ros/ros.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/WrenchStamped.h>
#include <fstream>
#include <vector>


geometry_msgs::Vector3Stamped stiffness;
ros::Publisher pub;
ros::Subscriber sub;

double a = 0.02353;
double b = 0.00472;

double controller_stiff_max = 200;
double F_max = 10;
double muscle_stiff_max = a + b*log(F_max);

double mapped_stiff;

double F_prev = 0;


geometry_msgs::WrenchStamped F_ext;


void F_ext_callback(geometry_msgs::WrenchStamped F_external) {

    F_ext = F_external;

}


void timer_callback(const ros::TimerEvent&) {


    if(abs(F_ext.wrench.force.z - F_prev) >= 0.1){

        F_prev = F_ext.wrench.force.z;

        mapped_stiff = a + b * log(abs(F_ext.wrench.force.z));

        stiffness.vector.z = (controller_stiff_max/muscle_stiff_max)*mapped_stiff;

        if (stiffness.vector.z >= 200) {

            stiffness.vector.z = 200;

        } else if (stiffness.vector.z <= 10) {

            stiffness.vector.z = 30;
            
        }


        pub.publish(stiffness);
    }

    std::cout << "\nForza: " << F_ext.wrench.force.z << std::endl;
    std::cout << "Stiffness: " << stiffness.vector.z << std::endl;

}



int main(int argc, char** argv) {
    ros::init(argc, argv, "variable_impedance_command");
    ros::NodeHandle nh;

    ros::Rate rate(1); // 1 Hz

    ROS_INFO("Modifica impedenza avviata");

    pub = nh.advertise<geometry_msgs::Vector3Stamped>("/cartesian_impedance_controller/desired_stiffness", 10);

    sub = nh.subscribe("/franka_state_controller/F_ext", 10, F_ext_callback);

    ros::Timer timer1 = nh.createTimer(ros::Duration(1), timer_callback);

    ros::spin();

    return 0;
}
