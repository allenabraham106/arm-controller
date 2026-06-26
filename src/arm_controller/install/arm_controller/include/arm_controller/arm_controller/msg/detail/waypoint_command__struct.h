// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from arm_controller:msg/WaypointCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "arm_controller/msg/waypoint_command.h"


#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__STRUCT_H_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Constant 'ADD'.
enum
{
  arm_controller__msg__WaypointCommand__ADD = 0
};

/// Constant 'REMOVE'.
enum
{
  arm_controller__msg__WaypointCommand__REMOVE = 1
};

/// Struct defined in msg/WaypointCommand in the package arm_controller.
typedef struct arm_controller__msg__WaypointCommand
{
  int8_t type;
  int32_t index;
} arm_controller__msg__WaypointCommand;

// Struct for a sequence of arm_controller__msg__WaypointCommand.
typedef struct arm_controller__msg__WaypointCommand__Sequence
{
  arm_controller__msg__WaypointCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} arm_controller__msg__WaypointCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__STRUCT_H_
