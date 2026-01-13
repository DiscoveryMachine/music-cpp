
#include <limits>

#include "message/event_id.h"
#include "message/simulation_address.h"
#include "util/validate.h"

namespace music {

void to_json(nlohmann::json &j, const MusicEventId &header) {
  j = {{MusicEventId::kEventIdKey, header.event_id_}};
  j.update(header.sim_address_.ToJson());
}

void from_json(const nlohmann::json &j, MusicEventId &header) {

  std::string j_dump = j.dump();
  j.at(MusicEventId::kEventIdKey).get_to(header.event_id_);
  header.sim_address_ = SimulationAddress::FromJson(j);
}

void MusicEventId::Validate() const {
  if (event_id_ < 0 || event_id_ > std::numeric_limits<unsigned short>::max()) {
    LogError("Invalid value for MusicEventId::event_id: " +
             std::to_string(event_id_));
  }
  sim_address_.Validate();
}
}  // namespace music
