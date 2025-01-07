#include "rclcpp/rclcpp.hpp"
#include "interface_msgs/msg/raw_datapoint.hpp"
#include "visualization_msgs/msg/marker_array.hpp"
#include "std_msgs/msg/int32.hpp"
#include "rosbag2_cpp/writer.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;

class DataRecorderNode : public rclcpp::Node
{
public:
    DataRecorderNode()
        : Node("data_recorder_node")
    {
        this->declare_parameter("bag_file_name","sample");
        std::string bag_file_name = this->get_parameter("bag_file_name").as_string();
        //std::vector<rclcpp::Parameter> all_new_parameters{rclcpp::Parameter("bag_file_name")};
        //this->set_parameters(all_new_parameters);
        
        // Create the bag writer
        writer_=std::make_unique<rosbag2_cpp::Writer>();
        writer_->open(bag_file_name);

        // Create the subscribers
        marker_subscriber_ = create_subscription<visualization_msgs::msg::MarkerArray>(
            "/body_tracking_data", 10, [this](const visualization_msgs::msg::MarkerArray::SharedPtr msg) {
                handleMarkerArray(msg);
            });

        int_subscriber_ = create_subscription<std_msgs::msg::Int32>(
            "/label", 10, [this](const std_msgs::msg::Int32::SharedPtr msg) {
                handleInt32(msg);
            });

        raw_datapoint_publisher_ = this->create_publisher<interface_msgs::msg::RawDatapoint>("/recorder/raw_datapoint", 10);

        raw_datapoint_subscriber_ = create_subscription<interface_msgs::msg::RawDatapoint>(
                "/recorder/raw_datapoint", 10, std::bind(&DataRecorderNode::handleRawDatapoint, this, _1));
    }


private:
    void handleMarkerArray(const visualization_msgs::msg::MarkerArray::SharedPtr msg)
    {
        // Combine the marker array and int data into a raw datapoint message
        interface_msgs::msg::RawDatapoint raw_datapoint;
        visualization_msgs::msg::MarkerArray empty;
        if (*msg != empty){
            raw_datapoint.marker_array = *msg;
            raw_datapoint.label = int_data_;

            raw_datapoint_publisher_->publish(raw_datapoint);
        }
    }

    void handleRawDatapoint(std::shared_ptr<rclcpp::SerializedMessage> msg) const
    {        
        rclcpp::Time time_stamp = this->now();

        writer_->write(msg, "raw_datapoint", "interface_msgs/msg/RawDatapoint", time_stamp);

        //RCLCPP_INFO(get_logger(), "Received and recorded a raw datapoint");
    }

    void handleInt32(const std_msgs::msg::Int32::SharedPtr msg)
    {
        if (msg->data != int_data_){
            RCLCPP_INFO(get_logger(), "Label updated to %i", msg->data);

            int_data_ = msg->data;
        }
    }

    rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr marker_subscriber_;
    rclcpp::Subscription<interface_msgs::msg::RawDatapoint>::SharedPtr raw_datapoint_subscriber_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr int_subscriber_;
    rclcpp::Publisher<interface_msgs::msg::RawDatapoint>::SharedPtr raw_datapoint_publisher_;
    std::unique_ptr<rosbag2_cpp::Writer> writer_;
    int int_data_ = -1;  
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<DataRecorderNode>());

    rclcpp::shutdown();

    return 0;
}
