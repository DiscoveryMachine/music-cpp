// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_ENTITY_TRANSFER_CONSTRUCT_ID_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_ENTITY_TRANSFER_CONSTRUCT_ID_MESSAGE_H_

#include <memory>
#include <nlohmann/json.hpp>

#include "message/command/music_command_request_message.h"
#include "message/entity_id.h"

namespace music {
class TransferConstructIdMessage : public MusicCommandRequestMessage {
  template <class T>
  friend class TransferConstructIdMessageBuilderTemplate;

 public:
  TransferConstructIdMessage() {
    type_ = MusicMessageType::kTransferConstructId;
    command_identifier_ =
        MusicMessageTypeHelper::GetInfoForType(type_).action_type;
  }

  // Serialization Keys
  inline static const std::string kOldIdKey = "oldID";
  inline static const std::string kNewIdKey = "newID";
  inline static const long kOldId = 1L;
  inline static const long kNewId = 2L;

  // Serialization
  friend void from_json(const nlohmann::json &jsn,
                        TransferConstructIdMessage &msg);
  friend void to_json(nlohmann::json &jsn,
                      const TransferConstructIdMessage &msg);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static TransferConstructIdMessage FromJson(const nlohmann::json &j) {
    TransferConstructIdMessage message;
    from_json(j, message);
    return message;
  }

  // ToString and output (testing)
  friend std::ostream &operator<<(std::ostream &os,
                                  const TransferConstructIdMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "MusicTransferConstructIdMessage: " + ToJson().dump();
  }

  // Getters
  const EntityId &GetOldId() const { return old_id_; }
  const EntityId &GetNewId() const { return new_id_; }

  // Setters
  void SetOldId(const EntityId &old_id) { old_id_ = old_id; }
  void SetNewId(const EntityId &new_id) { new_id_ = new_id; }

  // Validate
  void Validate() const;

 protected:
  EntityId old_id_{};
  EntityId new_id_{};
};

// Template Builder
template <class T>
class TransferConstructIdMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 public:
  T &SetOldId(EntityId old_id) {
    this->command_request_message_->old_id_ = old_id;
    return static_cast<T &>(*this);
  }

  T &SetNewId(EntityId new_id) {
    this->command_request_message_->new_id_ = new_id;
    return static_cast<T &>(*this);
  }

 protected:
  void SetBuiltMessage(std::shared_ptr<TransferConstructIdMessage> message) {
    this->command_request_message_ = message;
    // Call parent's SetBuiltMessage to properly initialize the inheritance
    // chain
    CommandRequestMessageBuilder<T>::SetBuiltMessage(message);
  }
  std::shared_ptr<TransferConstructIdMessage> command_request_message_;
};

// Concrete Builder
class TransferConstructIdMessageBuilder
    : public TransferConstructIdMessageBuilderTemplate<
          TransferConstructIdMessageBuilder> {
 public:
  TransferConstructIdMessageBuilder() {
    SetBuiltMessage(std::make_shared<TransferConstructIdMessage>());
  }

  TransferConstructIdMessage Build() { return *command_request_message_; }
};

}  // namespace music

#endif