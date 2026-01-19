// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <optional>
#include <string>

#include "message/interaction/construct_interaction_response_message.h"
#include "util/validate.h"

namespace music {

static const std::string kErrorMesgPrefix =
    "ConstructInteractionResponseMessage::Validate: ";

void ConstructInteractionResponseMessage::Validate() const {
  MusicResponseMessage::Validate();

  // If optional data is provided, ensure it is valid JSON
  if (optional_data_.has_value() && optional_data_.value().empty()) {
    LogError(kErrorMesgPrefix +
             "Optional data provided is not properly formatted JSON.");
  }
}

void to_json(nlohmann::json &j,
             const ConstructInteractionResponseMessage &req) {
  to_json(j, static_cast<const MusicResponseMessage &>(req));
  if (req.optional_data_.has_value()) {
    j.update(nlohmann::json{
        {ConstructInteractionResponseMessage::kInteractionOptionalDataJsonKey,
         req.optional_data_.value()}});
  }
};

void from_json(const nlohmann::json &j,
               ConstructInteractionResponseMessage &req) {
  from_json(j, static_cast<MusicResponseMessage &>(req));
  if (j.contains(ConstructInteractionResponseMessage::
                     kInteractionOptionalDataJsonKey)) {
    req.optional_data_ = std::optional<nlohmann::json>(j.at(
        ConstructInteractionResponseMessage::kInteractionOptionalDataJsonKey));
  }
}

}  // namespace music