// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from arm_controller:msg/WaypointCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "arm_controller/msg/waypoint_command.hpp"


#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__TRAITS_HPP_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "arm_controller/msg/detail/waypoint_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace arm_controller
{

namespace msg
{

inline void to_flow_style_yaml(
  const WaypointCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: type
  {
    out << "type: ";
    rosidl_generator_traits::value_to_yaml(msg.type, out);
    out << ", ";
  }

  // member: index
  {
    out << "index: ";
    rosidl_generator_traits::value_to_yaml(msg.index, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const WaypointCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: type
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "type: ";
    rosidl_generator_traits::value_to_yaml(msg.type, out);
    out << "\n";
  }

  // member: index
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "index: ";
    rosidl_generator_traits::value_to_yaml(msg.index, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const WaypointCommand & msg, bool use_flow_style = false)
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
  const arm_controller::msg::WaypointCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  arm_controller::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use arm_controller::msg::to_yaml() instead")]]
inline std::string to_yaml(const arm_controller::msg::WaypointCommand & msg)
{
  return arm_controller::msg::to_yaml(msg);
}

template<>
inline const char * data_type<arm_controller::msg::WaypointCommand>()
{
  return "arm_controller::msg::WaypointCommand";
}

template<>
inline const char * name<arm_controller::msg::WaypointCommand>()
{
  return "arm_controller/msg/WaypointCommand";
}

template<>
struct has_fixed_size<arm_controller::msg::WaypointCommand>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<arm_controller::msg::WaypointCommand>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<arm_controller::msg::WaypointCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__TRAITS_HPP_
