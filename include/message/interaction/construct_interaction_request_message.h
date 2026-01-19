// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_INTERACTION_CONSTRUCT_INTERACTION_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_INTERACTION_CONSTRUCT_INTERACTION_REQUEST_MESSAGE_H_

#include <memory>
#include <string>

#include "enum/interaction_type.h"
#include "enum/music_message_type.h"
#include "message/music_request_message.h"

namespace music {

// Encapsulate the information required to make a construct interaction request.
class ConstructInteractionRequestMessage : public MusicRequestMessage {
  template <class T>
  friend class ConstructInteractionRequestMessageBuilderBase;

 public:
  inline static const std::string kInteractionNameJsonKey = "interactionName";
  inline static const std::string kInteractionTypeJsonKey = "interactionType";
  inline static const std::string kInteractionDataJsonKey = "interactionData";

  // Used in DIS serialization
  inline static const long kInteractionNameId = 454110000L;
  inline static const std::string kInteractionId = "interactionID";

  ConstructInteractionRequestMessage() {
    type_ = MusicMessageType::kConstructInteractionRequest;
  }

  const std::string &GetInteractionName() const { return interaction_name_; }
  const InteractionType GetInteractionType() const { return interaction_type_; }
  const nlohmann::json &GetInteractionData() const { return interaction_data_; }

  void SetInteractionName(const std::string &name) { interaction_name_ = name; }
  void SetInteractionType(InteractionType type) { interaction_type_ = type; }
  void SetInteractionData(const nlohmann::json &data) {
    interaction_data_ = data;
  }

  void Validate() const;

  friend std::ostream &operator<<(
      std::ostream &os, const ConstructInteractionRequestMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "ConstructInteractionRequestMessage: " + ToJson().dump();
  }

  friend void to_json(nlohmann::json &j,
                      const ConstructInteractionRequestMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ConstructInteractionRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static ConstructInteractionRequestMessage FromJson(const nlohmann::json &j) {
    ConstructInteractionRequestMessage message;
    from_json(j, message);
    return message;
  }

 protected:
  std::string interaction_name_{};
  InteractionType interaction_type_{InteractionType::kUnassigned};
  nlohmann::json interaction_data_{};
};

template <class T>
class ConstructInteractionRequestMessageBuilderBase
    : public RequestMessageBuilder<T> {
 public:
  T &SetInteractionName(const std::string &name) {
    wip_->interaction_name_ = name;
    return static_cast<T &>(*this);
  }

  T &SetInteractionType(InteractionType type) {
    wip_->interaction_type_ = type;
    return static_cast<T &>(*this);
  }

  T &SetInteractionData(const nlohmann::json &data) {
    wip_->interaction_data_ = data;
    return static_cast<T &>(*this);
  }

 protected:
  void SetBuiltMessage(
      std::shared_ptr<ConstructInteractionRequestMessage> req_mesg) {
    RequestMessageBuilder<T>::SetBuiltMessage(wip_ = req_mesg);
  }
  std::shared_ptr<ConstructInteractionRequestMessage> wip_;
};

class ConstructInteractionRequestMessageBuilder
    : public ConstructInteractionRequestMessageBuilderBase<
          ConstructInteractionRequestMessageBuilder> {
 public:
  ConstructInteractionRequestMessageBuilder() {
    SetBuiltMessage(wip_ =
                        std::make_shared<ConstructInteractionRequestMessage>());
  }
  ConstructInteractionRequestMessage Build() { return *wip_; }
};

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_INTERACTION_CONSTRUCT_INTERACTION_REQUEST_MESSAGE_H_