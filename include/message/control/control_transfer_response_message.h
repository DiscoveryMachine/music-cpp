// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_TRANSFER_RESPONSE_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_TRANSFER_RESPONSE_MESSAGE_H_

#include "message/music_response_message.h"
namespace music {

// Response to a Request to gain current control of a construct.
class ControlTransferResponseMessage : public MusicResponseMessage {
  template <class T>
  friend class ControlTransferResponseMessageBuilderTemplate;

 public:
  // Constructor
  ControlTransferResponseMessage() : MusicResponseMessage() {
    type_ = MusicMessageType::kControlTransferResponse;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ControlTransferResponseMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlTransferResponseMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ControlTransferResponseMessage FromJson(const nlohmann::json &j) {
    ControlTransferResponseMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(
      std::ostream &os, const ControlTransferResponseMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ControlTransferResponseMessage: " + ToJson().dump();
  }
};
// Template Builder
template <class T>
class ControlTransferResponseMessageBuilderTemplate
    : public ResponseMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlTransferResponseMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicResponseMessage> new_message) {
    message_ =
        std::static_pointer_cast<ControlTransferResponseMessage>(new_message);
    ResponseMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete Builder
class ControlTransferResponseMessageBuilder
    : public ControlTransferResponseMessageBuilderTemplate<
          ControlTransferResponseMessageBuilder> {
 public:
  ControlTransferResponseMessageBuilder() {
    SetBuiltMessage(std::make_shared<ControlTransferResponseMessage>());
  }
  ControlTransferResponseMessage Build() { return *message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_TRANSFER_RESPONSE_MESSAGE_H_