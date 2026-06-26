// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from arm_controller:msg/WaypointCommand.idl
// generated code does not contain a copyright notice

#include "arm_controller/msg/detail/waypoint_command__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_arm_controller
const rosidl_type_hash_t *
arm_controller__msg__WaypointCommand__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xcf, 0x73, 0xe0, 0x29, 0x96, 0xa5, 0x07, 0x8b,
      0x52, 0x7e, 0x14, 0x19, 0xf8, 0x70, 0x2d, 0xa0,
      0x9f, 0x77, 0x2f, 0x86, 0x42, 0x25, 0x2a, 0x46,
      0xad, 0x5e, 0x96, 0xdd, 0xe9, 0xab, 0x90, 0x97,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char arm_controller__msg__WaypointCommand__TYPE_NAME[] = "arm_controller/msg/WaypointCommand";

// Define type names, field names, and default values
static char arm_controller__msg__WaypointCommand__FIELD_NAME__type[] = "type";
static char arm_controller__msg__WaypointCommand__FIELD_NAME__index[] = "index";

static rosidl_runtime_c__type_description__Field arm_controller__msg__WaypointCommand__FIELDS[] = {
  {
    {arm_controller__msg__WaypointCommand__FIELD_NAME__type, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT8,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {arm_controller__msg__WaypointCommand__FIELD_NAME__index, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
arm_controller__msg__WaypointCommand__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {arm_controller__msg__WaypointCommand__TYPE_NAME, 34, 34},
      {arm_controller__msg__WaypointCommand__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "int8 ADD=0\n"
  "int8 REMOVE=1\n"
  "int8 type\n"
  "int32 index";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
arm_controller__msg__WaypointCommand__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {arm_controller__msg__WaypointCommand__TYPE_NAME, 34, 34},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 46, 46},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
arm_controller__msg__WaypointCommand__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *arm_controller__msg__WaypointCommand__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
