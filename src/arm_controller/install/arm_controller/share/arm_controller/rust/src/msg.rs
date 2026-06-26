#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to arm_controller__msg__WaypointCommand

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::WaypointCommand::default())
  }
}

impl rosidl_runtime_rs::Message for WaypointCommand {
  type RmwMsg = super::msg::rmw::WaypointCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        type_: msg.type_,
        index: msg.index,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      type_: msg.type_,
      index: msg.index,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      type_: msg.type_,
      index: msg.index,
    }
  }
}


