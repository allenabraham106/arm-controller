// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from arm_controller:msg/WaypointStatus.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "arm_controller/msg/waypoint_status.h"


#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__STRUCT_H_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/WaypointStatus in the package arm_controller.
typedef struct arm_controller__msg__WaypointStatus
{
  bool success;
  rosidl_runtime_c__String message;
} arm_controller__msg__WaypointStatus;

// Struct for a sequence of arm_controller__msg__WaypointStatus.
typedef struct arm_controller__msg__WaypointStatus__Sequence
{
  arm_controller__msg__WaypointStatus * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_controller__msg__WaypointStatus__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__STRUCT_H_
