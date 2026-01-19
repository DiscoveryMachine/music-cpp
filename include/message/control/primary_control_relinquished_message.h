// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_PRIMARY_CONTROL_RELINQUISHED_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_PRIMARY_CONTROL_RELINQUISHED_MESSAGE_H_
#include "message/music_request_message.h"

namespace music {

// Message sent from the primary controller of a construct to the controlled
// construct indicating that it no longer wishes to be the primary controller of
// it. The controlled construct should then update its primary controller
// attribute.
class PrimaryControlRelinquishedMessage : public MusicRequestMessage {
  template <class T>
  friend class PrimaryControlRelinquishedMessageBuilderTemplate;

 public:
  // Constructor
  PrimaryControlRelinquishedMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kPrimaryControlRelinquished;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const PrimaryControlRelinquishedMessage &message);
  friend void from_json(const nlohmann::json &j,
                        PrimaryControlRelinquishedMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static PrimaryControlRelinquishedMessage FromJson(const nlohmann::json &j) {
    PrimaryControlRelinquishedMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(
      std::ostream &os, const PrimaryControlRelinquishedMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "PrimaryControlRelinquishedMessage: " + ToJson().dump();
  }
};

// Template Builder
template <class T>
class PrimaryControlRelinquishedMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<PrimaryControlRelinquishedMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ = std::static_pointer_cast<PrimaryControlRelinquishedMessage>(
        new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

// Concrete Builder
class PrimaryControlRelinquishedMessageBuilder
    : public PrimaryControlRelinquishedMessageBuilderTemplate<
          PrimaryControlRelinquishedMessageBuilder> {
 public:
  PrimaryControlRelinquishedMessageBuilder() {
    SetBuiltMessage(std::make_shared<PrimaryControlRelinquishedMessage>());
  }
  PrimaryControlRelinquishedMessage Build() { return *message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_PRIMARY_CONTROL_RELINQUISHED_MESSAGE_H_