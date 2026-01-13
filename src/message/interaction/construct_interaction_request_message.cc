// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "enum/interaction_type.h"
#include "message/interaction/construct_interaction_request_message.h"
#include "message/music_request_message.h"
#include "nlohmann/json.hpp"
#include "util/validate.h"

namespace music {

static const std::string kErrorMesgPrefix =
    "ConstructInteractionRequestMessage::Validate: ";

void ConstructInteractionRequestMessage::Validate() const {
  MusicRequestMessage::Validate();

  if (interaction_name_.empty()) {
    LogError(kErrorMesgPrefix + "Interaction name not set.");
  }

  if (interaction_type_ == InteractionType::kUnassigned) {
    LogError(kErrorMesgPrefix + "Interaction type not assigned.");
  }

  // if interaction type is kInteractionRequest, then the interaction data can
  // have multiple or no parameters. If interaction type is kPause, kStop, or
  // kResume, then the interaction data must contain a single item with key of
  // kInteractionId and a uint32_t value
  if ((interaction_type_ == InteractionType::kPause ||
       interaction_type_ == InteractionType::kStop ||
       interaction_type_ == InteractionType::kResume)) {
    if (interaction_data_.size() != 1 &&
        !interaction_data_.contains(
            ConstructInteractionRequestMessage::kInteractionId)) {
      LogError(kErrorMesgPrefix +
               "Interaction data must contain a single item with key of "
               "kInteractionId and a uint32_t value.");
    }
  }
}

void to_json(nlohmann::json &j, const ConstructInteractionRequestMessage &req) {
  to_json(j, static_cast<const MusicRequestMessage &>(req));
  j.update(nlohmann::json{
      {ConstructInteractionRequestMessage::kInteractionNameJsonKey,
       req.interaction_name_},
      {ConstructInteractionRequestMessage::kInteractionTypeJsonKey,
       req.interaction_type_},
      {ConstructInteractionRequestMessage::kInteractionDataJsonKey,
       req.interaction_data_}});
};

void from_json(const nlohmann::json &j,
               ConstructInteractionRequestMessage &req) {
  from_json(j, static_cast<MusicRequestMessage &>(req));
  j.at(ConstructInteractionRequestMessage::kInteractionNameJsonKey)
      .get_to((req.interaction_name_));
  j.at(ConstructInteractionRequestMessage::kInteractionTypeJsonKey)
      .get_to((req.interaction_type_));
  j.at(ConstructInteractionRequestMessage::kInteractionDataJsonKey)
      .get_to((req.interaction_data_));
}

}  // namespace music