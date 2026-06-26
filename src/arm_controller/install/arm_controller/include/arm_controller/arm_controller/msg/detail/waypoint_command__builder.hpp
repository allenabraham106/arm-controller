// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from arm_controller:msg/WaypointCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "arm_controller/msg/waypoint_command.hpp"


#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__BUILDER_HPP_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "arm_controller/msg/detail/waypoint_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace arm_controller
{

namespace msg
{

namespace builder
{

class Init_WaypointCommand_index
{
public:
  explicit Init_WaypointCommand_index(::arm_controller::msg::WaypointCommand & msg)
  : msg_(msg)
  {}
  ::arm_controller::msg::WaypointCommand index(::arm_controller::msg::WaypointCommand::_index_type arg)
  {
    msg_.index = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_controller::msg::WaypointCommand msg_;
};

class Init_WaypointCommand_type
{
public:
  Init_WaypointCommand_type()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_WaypointCommand_index type(::arm_controller::msg::WaypointCommand::_type_type arg)
  {
    msg_.type = std::move(arg);
    return Init_WaypointCommand_index(msg_);
  }

private:
  ::arm_controller::msg::WaypointCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_controller::msg::WaypointCommand>()
{
  return arm_controller::msg::builder::Init_WaypointCommand_type();
}

}  // namespace arm_controller

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__BUILDER_HPP_
