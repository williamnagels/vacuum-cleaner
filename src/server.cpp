#include "map.h"
#include "movement.h"
#include "route-planning/spiral_planner.h"
#include <vacuumcleaner/cleaningAction.h>
#include <thread>

#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>

namespace
{
  template <typename T>
  auto GetParameter(ros::NodeHandle const& node, std::string const& parameter, T const& default_value) -> T
  {
    T parameter_temp;    
    if (not node.param(std::string("/server/vacuumcleaner/") + parameter, parameter_temp, default_value))
    {
      ROS_INFO_STREAM("parameter \""<<parameter<<"\" not found, using default:\""<<default_value<<"\"");
    }
    return parameter_temp;
  }
}

class CleaningAction
{
private:
  ros::NodeHandle _node_handle;
  actionlib::SimpleActionServer<vacuumcleaner::cleaningAction> _action_server;
  std::string _action_name;
  vacuumcleaner::cleaningFeedback _feedback;
  double _robot_radius_in_meter;
  Map _map;
  SpiralPlanner _spiral_planner;
  Movement _movement;
public:

  CleaningAction(std::string const& name)
    :_action_server(_node_handle, name,false)
    ,_action_name(name)
    ,_robot_radius_in_meter(GetParameter(_node_handle, "robot_radius", 0.1))
    ,_map(_node_handle, GetParameter(_node_handle, "map_topic", std::string("map")))
    ,_spiral_planner(_robot_radius_in_meter, M_PI/GetParameter(_node_handle, "spiral_delta_denominator", 8))
    ,_movement(_spiral_planner, [this](auto coordinates){_map.OnPositionChanged(coordinates);})
  {
    _action_server.registerGoalCallback(std::bind(std::mem_fn(&CleaningAction::OnGoal), this));
    _action_server.start();
  }

  void OnGoal()
  {

  }
};


int main(int argc, char** argv)
{
  ros::init(argc, argv, "cleaning");
  CleaningAction cleaningAction("cleaning");
  ros::spin();

  return 0;
}
