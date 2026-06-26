#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "arm_controller__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__arm_controller__msg__WaypointCommand() -> *const std::ffi::c_void;
}

#[link(name = "arm_controller__rosidl_generator_c")]
extern "C" {
    fn arm_controller__msg__WaypointCommand__init(msg: *mut WaypointCommand) -> bool;
    fn arm_controller__msg__WaypointCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<WaypointCommand>, size: usize) -> bool;
    fn arm_controller__msg__WaypointCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<WaypointCommand>);
    fn arm_controller__msg__WaypointCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<WaypointCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<WaypointCommand>) -> bool;
}

// Corresponds to arm_controller__msg__WaypointCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct WaypointCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub type_: i8,


    // This member is not documented.
    #[allow(missing_docs)]
    pub index: i32,

}

impl WaypointCommand {

    // This constant is not documented.
    #[allow(missing_docs)]
    pub const ADD: i8 = 0;


    // This constant is not documented.
    #[allow(missing_docs)]
    pub const REMOVE: i8 = 1;

}


impl Default for WaypointCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !arm_controller__msg__WaypointCommand__init(&mut msg as *mut _) {
        panic!("Call to arm_controller__msg__WaypointCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for WaypointCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { arm_controller__msg__WaypointCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { arm_controller__msg__WaypointCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { arm_controller__msg__WaypointCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for WaypointCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for WaypointCommand where Self: Sized {
  const TYPE_NAME: &'static str = "arm_controller/msg/WaypointCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__arm_controller__msg__WaypointCommand() }
  }
}


#[link(name = "arm_controller__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__arm_controller__msg__WaypointStatus() -> *const std::ffi::c_void;
}

#[link(name = "arm_controller__rosidl_generator_c")]
extern "C" {
    fn arm_controller__msg__WaypointStatus__init(msg: *mut WaypointStatus) -> bool;
    fn arm_controller__msg__WaypointStatus__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<WaypointStatus>, size: usize) -> bool;
    fn arm_controller__msg__WaypointStatus__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<WaypointStatus>);
    fn arm_controller__msg__WaypointStatus__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<WaypointStatus>, out_seq: *mut rosidl_runtime_rs::Sequence<WaypointStatus>) -> bool;
}

// Corresponds to arm_controller__msg__WaypointStatus
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct WaypointStatus {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub message: rosidl_runtime_rs::String,

}



impl Default for WaypointStatus {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !arm_controller__msg__WaypointStatus__init(&mut msg as *mut _) {
        panic!("Call to arm_controller__msg__WaypointStatus__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for WaypointStatus {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { arm_controller__msg__WaypointStatus__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { arm_controller__msg__WaypointStatus__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { arm_controller__msg__WaypointStatus__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for WaypointStatus {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for WaypointStatus where Self: Sized {
  const TYPE_NAME: &'static str = "arm_controller/msg/WaypointStatus";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__arm_controller__msg__WaypointStatus() }
  }
}


