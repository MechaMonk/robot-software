#include <ros/ros.h>
#include <seeker_msgs/MineInfo.h>
#include <seeker_msgs/MineMap.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "map_builder");

    ros::NodeHandle node;

    ros::Publisher pub = node.advertise<seeker_msgs::MineMap>("map", 1000);
    seeker_msgs::MineMap map;

    auto on_new_mine_detection = boost::function<void(const seeker_msgs::MineInfo::ConstPtr&)>(
        [&pub, &map](const seeker_msgs::MineInfo::ConstPtr& msg) {
            ROS_INFO("I've seen it first, that mine is mine!");

            map.mines.push_back(*msg);
            pub.publish(map);
        }
    );
    ros::Subscriber sub = node.subscribe("mine_detection", 1000, on_new_mine_detection);

    ros::spin();

    return 0;
}
