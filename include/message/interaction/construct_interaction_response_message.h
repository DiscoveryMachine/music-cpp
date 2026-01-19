// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_INTERACTION_CONSTRUCT_INTERACTION_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_INTERACTION_CONSTRUCT_INTERACTION_RESPONSE_MESSAGE_H_

#include <memory>
#include <optional>
#include <string>

#include "enum/music_message_type.h"
#include "message/music_response_message.h"
#include "nlohmann/json_fwd.hpp"

namespace music {

// Encapsulate the information required to make a construct interaction
// response.
class ConstructInteractionResponseMessage : public MusicResponseMessage {
  template <class T>
  friend class ConstructInteractionResponseMessageBuilderBase;

 public:
  inline static const std::string kInteractionOptionalDataJsonKey =
      "optionalData";

  // Used in DIS serialization
  inline static long kOptionalDataId = 454119000L;
  ConstructInteractionResponseMessage() {
    type_ = MusicMessageType::kConstructInteractionResponse;
  }

  const std::optional<nlohmann::json> &GetOptionalData() const {
    return optional_data_;
  }

  void SetOptionalData(const nlohmann::json &data) {
    optional_data_ = std::optional<nlohmann::json>(data);
  }

  void SetOptionalData(std::nullopt_t) { optional_data_ = std::nullopt; }

  void Validate() const;

  friend std::ostream &operator<<(
      std::ostream &os, const ConstructInteractionResponseMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "ConstructInteractionResponseMessage: " + ToJson().dump();
  }

  friend void to_json(nlohmann::json &j,
                      const ConstructInteractionResponseMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ConstructInteractionResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static ConstructInteractionResponseMessage FromJson(const nlohmann::json &j) {
    ConstructInteractionResponseMessage message;
    from_json(j, message);
    return message;
  }

 protected:
  RequestStatus status_{RequestStatus::kUnset};
  std::optional<nlohmann::json> optional_data_{};
};

template <class T>
class ConstructInteractionResponseMessageBuilderBase
    : public ResponseMessageBuilder<T> {
 public:
  T &SetOptionalData(const nlohmann::json &data) {
    wip_->optional_data_ = std::optional<nlohmann::json>(data);
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<ConstructInteractionResponseMessage> wip_;
  void SetBuiltMessage(
      std::shared_ptr<ConstructInteractionResponseMessage> resp_mesg) {
    ResponseMessageBuilder<T>::SetBuiltMessage(wip_ = resp_mesg);
  }
};

class ConstructInteractionResponseMessageBuilder
    : public ConstructInteractionResponseMessageBuilderBase<
          ConstructInteractionResponseMessageBuilder> {
 public:
  ConstructInteractionResponseMessageBuilder() {
    SetBuiltMessage(
        wip_ = std::make_shared<ConstructInteractionResponseMessage>());
  }
  ConstructInteractionResponseMessage Build() { return *wip_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_INTERACTION_CONSTRUCT_INTERACTION_RESPONSE_MESSAGE_H_