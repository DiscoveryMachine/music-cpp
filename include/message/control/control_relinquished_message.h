// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_RELINQUISHED_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_RELINQUISHED_MESSAGE_H_

#include "message/music_request_message.h"
namespace music {

// Message sent from the current controller of a construct to that construct
// indicating that it no longer wishes to be the current controller of it. The
// target construct will send a Control Regained message to its primary
// controller. If there is no primary controller, this construct should return
// to its initial state with no current or primary controller.

class ControlRelinquishedMessage : public MusicRequestMessage {
  template <class T>
  friend class ControlRelinquishedMessageBuilderTemplate;

 public:
  // Constructor
  ControlRelinquishedMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kControlRelinquished;
  }
  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ControlRelinquishedMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlRelinquishedMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ControlRelinquishedMessage FromJson(const nlohmann::json &j) {
    ControlRelinquishedMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const ControlRelinquishedMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ControlRelinquishedMessage: " + ToJson().dump();
  }
};
// Template Builder
template <class T>
class ControlRelinquishedMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlRelinquishedMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ =
        std::static_pointer_cast<ControlRelinquishedMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};

// Concrete Builder
class ControlRelinquishedMessageBuilder
    : public ControlRelinquishedMessageBuilderTemplate<
          ControlRelinquishedMessageBuilder> {
 public:
  ControlRelinquishedMessageBuilder()
      : ControlRelinquishedMessageBuilderTemplate<
            ControlRelinquishedMessageBuilder>() {
    SetBuiltMessage(std::make_shared<ControlRelinquishedMessage>());
  }
  ControlRelinquishedMessage Build() { return *message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_RELINQUISHED_MESSAGE_H_