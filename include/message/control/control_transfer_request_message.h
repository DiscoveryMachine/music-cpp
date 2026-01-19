// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_TRANSFER_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_CONTROL_CONTROL_TRANSFER_REQUEST_MESSAGE_H_

#include "message/music_request_message.h"
namespace music {

// Message sent by a construct to the target constructs primary controller
// indicating that the sender wishes to transfer control to another non-primary
// controller construct.The primary *controller would then decide to send a
// Control Transfer Response with status complete or aborted. In either case,
// this response will be sent to the requestor construct. On granting the
// request, the primary controller will send a Set Current Controller which will
// send a Control Transfer *Response with status complete.

class ControlTransferRequestMessage : public MusicRequestMessage {
  template <class T>
  friend class ControlTransferRequestMessageBuilderTemplate;

 public:
  // Constructor
  ControlTransferRequestMessage() : MusicRequestMessage() {
    type_ = MusicMessageType::kControlTransferRequest;
  }

  // JSON Keys and IDs
  static inline const std::string kTargetConstructKey = "targetConstruct";
  static inline const std::string kContextKey = "context";
  static inline const std::string kProposedControllerKey = "proposedController";
  static inline const long kProposedControllerId = 454210003L;
  static inline const long kContextId = 454210002L;
  static inline const long kTargetConstructId = 454210001L;

  // Setters/getters
  const EntityId &GetTargetConstruct() const { return target_construct_; }
  const nlohmann::json &GetContext() const { return context_; }
  const EntityId &GetProposedController() const { return proposed_controller_; }

  void SetTargetConstruct(const EntityId &new_construct_id) {
    target_construct_ = new_construct_id;
  }
  void SetContext(const nlohmann::json &new_context) { context_ = new_context; }
  void SetProposedController(const EntityId &new_controller_id) {
    proposed_controller_ = new_controller_id;
  }

  // Validate
  void Validate() const;

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const ControlTransferRequestMessage &message);
  friend void from_json(const nlohmann::json &j,
                        ControlTransferRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static ControlTransferRequestMessage FromJson(const nlohmann::json &j) {
    ControlTransferRequestMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(
      std::ostream &os, const ControlTransferRequestMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "ControlTransferRequestMessage: " + ToJson().dump();
  }

 protected:
  nlohmann::json context_{};
  EntityId target_construct_{};
  EntityId proposed_controller_{};
};
// Template Builder
template <class T>
class ControlTransferRequestMessageBuilderTemplate
    : public RequestMessageBuilder<T> {
 protected:
  std::shared_ptr<ControlTransferRequestMessage> message_;
  void SetBuiltMessage(std::shared_ptr<MusicRequestMessage> new_message) {
    message_ =
        std::static_pointer_cast<ControlTransferRequestMessage>(new_message);
    RequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }

 public:
  T &SetTargetConstruct(EntityId new_construct_id) {
    message_->target_construct_ = new_construct_id;
    return static_cast<T &>(*this);
  }
  T &SetContext(nlohmann::json new_context) {
    message_->context_ = new_context;
    return static_cast<T &>(*this);
  }
  T &SetProposedController(EntityId new_controller_id) {
    message_->proposed_controller_ = new_controller_id;
    return static_cast<T &>(*this);
  }
};

// Concrete Builder
class ControlTransferRequestMessageBuilder
    : public ControlTransferRequestMessageBuilderTemplate<
          ControlTransferRequestMessageBuilder> {
 public:
  ControlTransferRequestMessageBuilder() {
    SetBuiltMessage(std::make_shared<ControlTransferRequestMessage>());
  }
  ControlTransferRequestMessage Build() { return *message_; }
};

}  // namespace music
#endif