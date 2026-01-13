// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/construct/state_field_message.h"
#include "util/validate.h"

namespace music {

void StateFieldMessage::Validate() {
  if (state_data_.empty()) {
    LogError("StateFieldMessage::Validate: State data not set.");
  }
  origin_id_.Validate();
  MusicMessage::Validate();
}

void to_json(nlohmann::json &j, const StateFieldMessage &stateField) {
  to_json(j, static_cast<const MusicMessage &>(stateField));
  j.update(
      nlohmann::json{{StateFieldMessage::kStateDataKey, stateField.state_data_},
                     {StateFieldMessage::kOriginIdKey, stateField.origin_id_}});
};

void from_json(const nlohmann::json &j, StateFieldMessage &stateField) {
  from_json(j, static_cast<MusicMessage &>(stateField));
  j.at(StateFieldMessage::kStateDataKey).get_to(stateField.state_data_);
  j.at(StateFieldMessage::kOriginIdKey).get_to(stateField.origin_id_);
}

}  // namespace music