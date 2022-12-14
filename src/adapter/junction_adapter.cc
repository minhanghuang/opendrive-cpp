#include "opendrive-cpp/adapter/junction_adapter.h"

#include "opendrive-cpp/geometry/core.h"

namespace opendrive {
namespace adapter {

JunctionAdapter::JunctionAdapter() {}

opendrive::Status JunctionAdapter::Run(const element::Junction* ele_junction,
                                       core::Junction::Ptr junction_ptr) {
  ele_junction_ = ele_junction;
  junction_ptr_ = junction_ptr;
  if (!ele_junction_ || !junction_ptr_) {
    set_status(ErrorCode::ADAPTER_JUNCTION_ERROR,
               "Junction Adapter Input Is Null.");
    return status();
  }
  GenerateAttributes().GenerateConnection();
  return status();
}

JunctionAdapter& JunctionAdapter::GenerateAttributes() {
  if (!IsValid()) return *this;
  junction_ptr_->name = ele_junction_->attributes.name;
  junction_ptr_->id = std::to_string(ele_junction_->attributes.id);
  junction_ptr_->type = ele_junction_->attributes.type;
  return *this;
}

JunctionAdapter& JunctionAdapter::GenerateConnection() {
  if (!IsValid()) return *this;
  for (const auto& ele_connection : ele_junction_->connections) {
    core::JunctionConnection connection;
    connection.id = std::to_string(ele_connection.id);
    connection.type = ele_connection.type;
    connection.incoming_road = std::to_string(ele_connection.incoming_road);
    connection.connecting_road = std::to_string(ele_connection.connecting_road);
    connection.contact_point = ele_connection.contact_point;
    for (const auto& lane_link : ele_connection.lane_links) {
      connection.lane_links.push_back(
          {{std::to_string(lane_link.from), std::to_string(lane_link.to)}});
    }
    junction_ptr_->connections.emplace_back(connection);
  }
  return *this;
}

}  // namespace adapter
}  // namespace opendrive
