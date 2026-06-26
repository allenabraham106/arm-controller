// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from arm_controller:msg/WaypointCommand.idl
// generated code does not contain a copyright notice
#include "arm_controller/msg/detail/waypoint_command__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
arm_controller__msg__WaypointCommand__init(arm_controller__msg__WaypointCommand * msg)
{
  if (!msg) {
    return false;
  }
  // type
  // index
  return true;
}

void
arm_controller__msg__WaypointCommand__fini(arm_controller__msg__WaypointCommand * msg)
{
  if (!msg) {
    return;
  }
  // type
  // index
}

bool
arm_controller__msg__WaypointCommand__are_equal(const arm_controller__msg__WaypointCommand * lhs, const arm_controller__msg__WaypointCommand * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // type
  if (lhs->type != rhs->type) {
    return false;
  }
  // index
  if (lhs->index != rhs->index) {
    return false;
  }
  return true;
}

bool
arm_controller__msg__WaypointCommand__copy(
  const arm_controller__msg__WaypointCommand * input,
  arm_controller__msg__WaypointCommand * output)
{
  if (!input || !output) {
    return false;
  }
  // type
  output->type = input->type;
  // index
  output->index = input->index;
  return true;
}

arm_controller__msg__WaypointCommand *
arm_controller__msg__WaypointCommand__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_controller__msg__WaypointCommand * msg = (arm_controller__msg__WaypointCommand *)allocator.allocate(sizeof(arm_controller__msg__WaypointCommand), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(arm_controller__msg__WaypointCommand));
  bool success = arm_controller__msg__WaypointCommand__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
arm_controller__msg__WaypointCommand__destroy(arm_controller__msg__WaypointCommand * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    arm_controller__msg__WaypointCommand__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
arm_controller__msg__WaypointCommand__Sequence__init(arm_controller__msg__WaypointCommand__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_controller__msg__WaypointCommand * data = NULL;

  if (size) {
    data = (arm_controller__msg__WaypointCommand *)allocator.zero_allocate(size, sizeof(arm_controller__msg__WaypointCommand), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = arm_controller__msg__WaypointCommand__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        arm_controller__msg__WaypointCommand__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
arm_controller__msg__WaypointCommand__Sequence__fini(arm_controller__msg__WaypointCommand__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      arm_controller__msg__WaypointCommand__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

arm_controller__msg__WaypointCommand__Sequence *
arm_controller__msg__WaypointCommand__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  arm_controller__msg__WaypointCommand__Sequence * array = (arm_controller__msg__WaypointCommand__Sequence *)allocator.allocate(sizeof(arm_controller__msg__WaypointCommand__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = arm_controller__msg__WaypointCommand__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
arm_controller__msg__WaypointCommand__Sequence__destroy(arm_controller__msg__WaypointCommand__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    arm_controller__msg__WaypointCommand__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
arm_controller__msg__WaypointCommand__Sequence__are_equal(const arm_controller__msg__WaypointCommand__Sequence * lhs, const arm_controller__msg__WaypointCommand__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!arm_controller__msg__WaypointCommand__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
arm_controller__msg__WaypointCommand__Sequence__copy(
  const arm_controller__msg__WaypointCommand__Sequence * input,
  arm_controller__msg__WaypointCommand__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(arm_controller__msg__WaypointCommand);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    arm_controller__msg__WaypointCommand * data =
      (arm_controller__msg__WaypointCommand *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!arm_controller__msg__WaypointCommand__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          arm_controller__msg__WaypointCommand__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!arm_controller__msg__WaypointCommand__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
