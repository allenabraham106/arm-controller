// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from arm_controller:msg/WaypointStatus.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "arm_controller/msg/waypoint_status.hpp"


#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__BUILDER_HPP_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "arm_controller/msg/detail/waypoint_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace arm_controller
{

namespace msg
{

namespace builder
{

class Init_WaypointStatus_message
{
public:
  explicit Init_WaypointStatus_message(::arm_controller::msg::WaypointStatus & msg)
  : msg_(msg)
  {}
  ::arm_controller::msg::WaypointStatus message(::arm_controller::msg::WaypointStatus::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::arm_controller::msg::WaypointStatus msg_;
};

class Init_WaypointStatus_success
{
public:
  Init_WaypointStatus_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_WaypointStatus_message success(::arm_controller::msg::WaypointStatus::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_WaypointStatus_message(msg_);
  }

private:
  ::arm_controller::msg::WaypointStatus msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::arm_controller::msg::WaypointStatus>()
{
  return arm_controller::msg::builder::Init_WaypointStatus_success();
}

}  // namespace arm_controller

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__BUILDER_HPP_
