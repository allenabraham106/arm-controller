// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from arm_controller:msg/WaypointStatus.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "arm_controller/msg/waypoint_status.hpp"


#ifndef ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__STRUCT_HPP_
#define ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__arm_controller__msg__WaypointStatus __attribute__((deprecated))
#else
# define DEPRECATED__arm_controller__msg__WaypointStatus __declspec(deprecated)
#endif

namespace arm_controller
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct WaypointStatus_
{
  using Type = WaypointStatus_<ContainerAllocator>;

  explicit WaypointStatus_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit WaypointStatus_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    arm_controller::msg::WaypointStatus_<ContainerAllocator> *;
  using ConstRawPtr =
    const arm_controller::msg::WaypointStatus_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<arm_controller::msg::WaypointStatus_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<arm_controller::msg::WaypointStatus_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      arm_controller::msg::WaypointStatus_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<arm_controller::msg::WaypointStatus_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      arm_controller::msg::WaypointStatus_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<arm_controller::msg::WaypointStatus_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<arm_controller::msg::WaypointStatus_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<arm_controller::msg::WaypointStatus_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__arm_controller__msg__WaypointStatus
    std::shared_ptr<arm_controller::msg::WaypointStatus_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__arm_controller__msg__WaypointStatus
    std::shared_ptr<arm_controller::msg::WaypointStatus_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const WaypointStatus_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const WaypointStatus_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct WaypointStatus_

// alias to use template instance with default allocator
using WaypointStatus =
  arm_controller::msg::WaypointStatus_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace arm_controller

#endif  // ARM_CONTROLLER__MSG__DETAIL__WAYPOINT_STATUS__STRUCT_HPP_
