// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from arm_controller:msg/WaypointCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "arm_controller/msg/waypoint_command.hpp"


#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__STRUCT_HPP_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__arm_controller__msg__WaypointCommand __attribute__((deprecated))
#else
# define DEPRECATED__arm_controller__msg__WaypointCommand __declspec(deprecated)
#endif

namespace arm_controller
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct WaypointCommand_
{
  using Type = WaypointCommand_<ContainerAllocator>;

  explicit WaypointCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->type = 0;
      this->index = 0l;
    }
  }

  explicit WaypointCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->type = 0;
      this->index = 0l;
    }
  }

  // field types and members
  using _type_type =
    int8_t;
  _type_type type;
  using _index_type =
    int32_t;
  _index_type index;

  // setters for named parameter idiom
  Type & set__type(
    const int8_t & _arg)
  {
    this->type = _arg;
    return *this;
  }
  Type & set__index(
    const int32_t & _arg)
  {
    this->index = _arg;
    return *this;
  }

  // constant declarations
  static constexpr int8_t ADD =
    0;
  static constexpr int8_t REMOVE =
    1;

  // pointer types
  using RawPtr =
    arm_controller::msg::WaypointCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_controller::msg::WaypointCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_controller::msg::WaypointCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_controller::msg::WaypointCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_controller::msg::WaypointCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_controller::msg::WaypointCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_controller::msg::WaypointCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_controller::msg::WaypointCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_controller::msg::WaypointCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_controller::msg::WaypointCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_controller__msg__WaypointCommand
    std::shared_ptr<arm_controller::msg::WaypointCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_controller__msg__WaypointCommand
    std::shared_ptr<arm_controller::msg::WaypointCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const WaypointCommand_ & other) const
  {
    if (this->type != other.type) {
      return false;
    }
    if (this->index != other.index) {
      return false;
    }
    return true;
  }
  bool operator!=(const WaypointCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct WaypointCommand_

// alias to use template instance with default allocator
using WaypointCommand =
  arm_controller::msg::WaypointCommand_<std::allocator<void>>;

// constant definitions
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr int8_t WaypointCommand_<ContainerAllocator>::ADD;
#endif  // __cplusplus < 201703L
#if __cplusplus < 201703L
// static constexpr member variable definitions are only needed in C++14 and below, deprecated in C++17
template<typename ContainerAllocator>
constexpr int8_t WaypointCommand_<ContainerAllocator>::REMOVE;
#endif  // __cplusplus < 201703L

}  // namespace msg

}  // namespace arm_controller

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_COMMAND__STRUCT_HPP_
