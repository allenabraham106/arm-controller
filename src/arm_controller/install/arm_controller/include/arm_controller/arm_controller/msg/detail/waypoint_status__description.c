// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from arm_controller:msg/WaypointStatus.idl
// generated code does not contain a copyright notice

#include "arm_controller/msg/detail/waypoint_status__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_arm_controller
const rosidl_type_hash_t *
arm_controller__msg__WaypointStatus__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x63, 0x7e, 0x0a, 0xf0, 0x23, 0xff, 0x24, 0x02,
      0xa6, 0x94, 0xe3, 0xa6, 0x4e, 0xb8, 0x95, 0xad,
      0x4e, 0x9f, 0x7e, 0x22, 0x3e, 0xa9, 0x70, 0xaf,
      0x9e, 0xd7, 0x39, 0xfd, 0xb2, 0x40, 0xf4, 0x17,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char arm_controller__msg__WaypointStatus__TYPE_NAME[] = "arm_controller/msg/WaypointStatus";

// Define type names, field names, and default values
static char arm_controller__msg__WaypointStatus__FIELD_NAME__success[] = "success";
static char arm_controller__msg__WaypointStatus__FIELD_NAME__message[] = "message";

static rosidl_runtime_c__type_description__Field arm_controller__msg__WaypointStatus__FIELDS[] = {
  {
    {arm_controller__msg__WaypointStatus__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {arm_controller__msg__WaypointStatus__FIELD_NAME__message, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
arm_controller__msg__WaypointStatus__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {arm_controller__msg__WaypointStatus__TYPE_NAME, 33, 33},
      {arm_controller__msg__WaypointStatus__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "bool success\n"
  "string message";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
arm_controller__msg__WaypointStatus__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {arm_controller__msg__WaypointStatus__TYPE_NAME, 33, 33},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 27, 27},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
arm_controller__msg__WaypointStatus__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *arm_controller__msg__WaypointStatus__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
