// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from arm_controller:msg/WaypointCommand.idl
// generated code does not contain a copyright notice
#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "arm_controller/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "arm_controller/msg/detail/waypoint_command__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_arm_controller
bool cdr_serialize_arm_controller__msg__WaypointCommand(
  const arm_controller__msg__WaypointCommand * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_arm_controller
bool cdr_deserialize_arm_controller__msg__WaypointCommand(
  eprosima::fastcdr::Cdr &,
  arm_controller__msg__WaypointCommand * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_arm_controller
size_t get_serialized_size_arm_controller__msg__WaypointCommand(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_arm_controller
size_t max_serialized_size_arm_controller__msg__WaypointCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_arm_controller
bool cdr_serialize_key_arm_controller__msg__WaypointCommand(
  const arm_controller__msg__WaypointCommand * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_arm_controller
size_t get_serialized_size_key_arm_controller__msg__WaypointCommand(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_arm_controller
size_t max_serialized_size_key_arm_controller__msg__WaypointCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_arm_controller
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, arm_controller, msg, WaypointCommand)();

#ifdef __cplusplus
}
#endif

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
