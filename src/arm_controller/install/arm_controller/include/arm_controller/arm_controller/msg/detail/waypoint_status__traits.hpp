// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from arm_controller:msg/WaypointStatus.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "arm_controller/msg/waypoint_status.hpp"


#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__TRAITS_HPP_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "arm_controller/msg/detail/waypoint_status__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace arm_controller
{

namespace msg
{

inline void to_flow_style_yaml(
  const WaypointStatus & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const WaypointStatus & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const WaypointStatus & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace arm_controller

namespace rosidl_generator_traits
{

[[deprecated("use arm_controller::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const arm_controller::msg::WaypointStatus & msg,
  std::ostream & out, size_t indentation = 0)
{
  arm_controller::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use arm_controller::msg::to_yaml() instead")]]
inline std::string to_yaml(const arm_controller::msg::WaypointStatus & msg)
{
  return arm_controller::msg::to_yaml(msg);
}

template<>
inline const char * data_type<arm_controller::msg::WaypointStatus>()
{
  return "arm_controller::msg::WaypointStatus";
}

template<>
inline const char * name<arm_controller::msg::WaypointStatus>()
{
  return "arm_controller/msg/WaypointStatus";
}

template<>
struct has_fixed_size<arm_controller::msg::WaypointStatus>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<arm_controller::msg::WaypointStatus>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<arm_controller::msg::WaypointStatus>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__TRAITS_HPP_
