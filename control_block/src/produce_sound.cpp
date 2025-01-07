/*#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include <unistd.h> // for sleep function

class SpeakerNode : public rclcpp::Node
{
public:
    SpeakerNode()
        : Node("speaker_node")
    {
        // Create a subscriber for the /utter_message topic
        sub_ = this->create_subscription<std_msgs::msg::Bool>(
            "/utter_message", 10, [this](const std_msgs::msg::Bool::SharedPtr msg) {
                if (msg->data) {
                    RCLCPP_INFO(this->get_logger(), "Sound sent");
                    // If the received data is true, produce a beep that lasts for 0.5 seconds
                    beep();
                }
            });
    }

private:
    void beep()
    {
        // Add code here to produce a beep sound using the internal speakers
        // You can use platform-specific libraries or system calls to achieve this.

        // For example, on Linux systems, you can use the system beep command:
        system("echo -e '\a'");
        
        // For other platforms, you may need to use different methods to produce the beep sound.
        // Be sure to check the documentation for your specific operating system.

        // Wait for 0.5 seconds to complete the beep sound
        usleep(500000); // 500 ms
    }

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_;
};

int main(int argc, char* argv[])
{
    // Initialize the ROS 2 node
    rclcpp::init(argc, argv);

    // Create the SpeakerNode
    auto node = std::make_shared<SpeakerNode>();

    // Spin the node until it is shutdown
    rclcpp::spin(node);

    return 0;
}
*/

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include <cstdlib>

class SpeechNode : public rclcpp::Node
{
public:
    SpeechNode() : Node("speech_node")
    {
        // Create a subscriber for the /utter_message topic
        sub_ = this->create_subscription<std_msgs::msg::Bool>(
            "/utter_message", 10, [this](const std_msgs::msg::Bool::SharedPtr msg) {
                // When a true message is received, call the speak() function
                if (msg->data) {
                    RCLCPP_INFO(this->get_logger(), "Voice message sent");
                    speak("About to move");
                }
            });
    }

private:
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_;

    void speak(const std::string &message)
    {
        // Use the espeak command to speak the given message
        std::string command = "espeak \"" + message + "\"";
        std::system(command.c_str());
    }
};

int main(int argc, char *argv[])
{
    // Initialize the ROS 2 node
    rclcpp::init(argc, argv);

    // Create the SpeechNode
    auto node = std::make_shared<SpeechNode>();

    // Spin the node until it is shutdown
    rclcpp::spin(node);

    // Clean up and shutdown
    rclcpp::shutdown();

    return 0;
}
