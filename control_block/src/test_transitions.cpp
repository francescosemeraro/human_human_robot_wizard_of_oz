#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/bool.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "trajectory_msgs/msg/joint_trajectory_point.hpp"
#include "builtin_interfaces/msg/duration.hpp"

class JointTrajectoryControllerNode : public rclcpp::Node
{
public:
    JointTrajectoryControllerNode()
        : Node("joint_trajectory_controller_node")
    {
        this->declare_parameter("condition", "not set");
        condition_ = this->get_parameter("condition").as_string();
        std::string message = "Condition is ";
        RCLCPP_INFO(this->get_logger(), (message + condition_).c_str());

        // Create a subscriber for the label topic
        sub_ = this->create_subscription<std_msgs::msg::Int32>(
            "/label", 1, [this](const std_msgs::msg::Int32::SharedPtr label) {
                // Callback function to handle the received label value
                // Define the pre-recorded joint positions based on the label value
                if (previous_label != label->data){
                    previous_label = label->data;
                    switch (label->data) {
                        case 0: // R-R
                            jointValues = {3.75, -0.445, 1.13, -2.27, 1.17, -1.40};
                            //jointValues = {4.95, -2.09, -1.81, -0.715, -1.58, -0.907}; //FOR DEMO
                            RCLCPP_INFO(this->get_logger(), "Command to R-R pose sent");
                            publishJointConfiguration(jointValues, label->data);
                            break;
                        case 2: // P-W1
                            if (case1Count == 0) {
                                jointValues = {4.01, -0.169, 1.12, -2.53, 1.57, -1.66};
                                //jointValues = {4.97, -1.57, -2.15, -1.48, -2.23, -3.23}; //FOR DEMO
                                RCLCPP_INFO(this->get_logger(), "Command to first P-W pose sent");
                                publishJointConfiguration(jointValues, label->data);
                            }
                            else if (case1Count == 1) {
                                jointValues = {4.09, -0.494, 1.13, -2.21, 1.57, -1.72};
                                //jointValues = {4.97, -1.57, -2.15, -1.48, -2.23, -3.23}; /FOR DEMO
                                RCLCPP_INFO(this->get_logger(), "Command to second P-W pose sent");
                                publishJointConfiguration(jointValues, label->data);
                            }
                            case1Count++;
                            break;
                        case 3: // R-W reactive
                            if (condition_ == "adaptive") {
                                std_msgs::msg::Bool signal;
                                signal.data = true;
                                pub_2->publish(signal); //UNCOMMENT AFTER DEMO
                                if (!timer_) {
                                    // Set timer for 5 seconds
                                    timer_ = this->create_wall_timer(std::chrono::seconds(5), [this, data = label->data]() {
                                        // After 5 seconds, set the joint configuration
                                        jointValues = {3.75, -0.445, 1.13, -2.27, 1.17, -3.14};
                                        //jointValues = {5.23, -1.8, -2.15, -1.05, -1.88, -0.61}; //FOR DEMO
                                        RCLCPP_INFO(this->get_logger(), "Command to R-W pose sent");
                                        publishJointConfiguration(jointValues, data);
                                        timer_ = nullptr;
                                    });
                                }
                            }
                            else {
                                jointValues = {3.75, -0.445, 1.13, -2.27, 1.17, -3.14};
                                RCLCPP_INFO(this->get_logger(), "Command to R-W pose sent");
                                publishJointConfiguration(jointValues, label->data);
                            }
                            break;
                        case 4: // W-P 
                            jointValues = {3.91, -0.447, 1.13, -2.26, 1.55, -3.14};
                            RCLCPP_INFO(this->get_logger(), "Command to W-P pose sent");
                            publishJointConfiguration(jointValues, label->data);
                            break;
                        case 5: // W-R
                            if (condition_ == "adaptive") {
                                if (!timer_) {
                                    std_msgs::msg::Bool signal;
                                    signal.data = true;
                                    pub_2->publish(signal);
                                    // Set timer for 5 seconds
                                    timer_ = this->create_wall_timer(std::chrono::seconds(5), [this, data = label->data]() {
                                        // After 5 seconds, set the joint configuration
                                        jointValues = {3.92, -0.440, 1.13, -2.26, 1.55, -2.36};
                                        RCLCPP_INFO(this->get_logger(), "Command to W-R pose sent");
                                        publishJointConfiguration(jointValues, data);
                                        timer_ = nullptr;
                                    });
                                }
                            }
                            else {
                                jointValues = {3.92, -0.440, 1.13, -2.26, 1.55, -2.36};
                                RCLCPP_INFO(this->get_logger(), "Command to W-R pose sent");
                                publishJointConfiguration(jointValues, label->data);
                            }
                            break;
                        case 6: // Default starting position
                            jointValues = {3.93, -0.942, 2.18, -2.84, 1.57, -1.52};
                            case1Count = 0;
                            RCLCPP_INFO(this->get_logger(), "Command to P-P pose sent");
                            publishJointConfiguration(jointValues, label->data);
                            break;
                    }
                }
            });

        // Create a publisher for the joint trajectory topic
        pub_1 = this->create_publisher<trajectory_msgs::msg::JointTrajectory>(
            "/scaled_joint_trajectory_controller/joint_trajectory", 1);
        pub_2 = this->create_publisher<std_msgs::msg::Bool>(
            "/utter_message", 1);
    }

private:
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_;
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr pub_1;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_2;
    int case1Count = 0;
    int previous_label = 100;
    rclcpp::TimerBase::SharedPtr timer_;
    std::string condition_;
    std::vector<double> jointValues;


    void publishJointConfiguration(const std::vector<double> jointValues, int32_t data)
    {
        trajectory_msgs::msg::JointTrajectory msg;
        msg.joint_names =  {"shoulder_pan_joint", "shoulder_lift_joint", "elbow_joint",
                                    "wrist_1_joint", "wrist_2_joint", "wrist_3_joint"};
        trajectory_msgs::msg::JointTrajectoryPoint point;
        point.positions = jointValues;
        builtin_interfaces::msg::Duration time_from_start;
        time_from_start.sec = 4;
        point.time_from_start = time_from_start;
        msg.points.push_back(point);
        if (data != 1){
            pub_1->publish(msg);
        }
    }
};

int main(int argc, char* argv[])
{
    // Initialize the ROS 2 node
    rclcpp::init(argc, argv);

    // Create the JointTrajectoryControllerNode
    auto node = std::make_shared<JointTrajectoryControllerNode>();

    // Spin the node until it is shutdown
    rclcpp::spin(node);

    return 0;
}


