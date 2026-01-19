// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_CONSTRUCTCONTROL_REMOVE_CONSTRUCT_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_CONSTRUCTCONTROL_REMOVE_CONSTRUCT_MESSAGE_H_

#include "message/command/music_command_request_message.h"
#include "message/entity_id.h"
namespace music {

// This command notifies a simulation that a construct should be removed. The
// most common use case for this command is if a simulation removes its own
// entity and needs to notify all other simulations that it was removed. In this
// event, the receiver_id_ will be all Sites, all Apps, and an entity ID of 0
// (Note: The origin_id_ will be the simulation that owns the construct that
// is being removed).

// Command Identifier : 454007002

class RemoveConstructMessage : public MusicCommandRequestMessage {
  template <class T>
  friend class RemoveConstructMessageBuilderTemplate;

 public:
  // Constructor
  RemoveConstructMessage() {
    type_ = MusicMessageType::kRemoveConstruct;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kRemoveConstruct)
                              .action_type;
  }
  // Validate
  void Validate() const;

  // Serialization Keys
  inline static const std::string kRemovedConstructKey = "removedConstruct";
  inline static const long kRemovedConstructId = 1L;

  // Getters
  const EntityId &GetRemovedConstruct() const { return removed_construct_; }

  // Setters
  void SetRemovedConstruct(const EntityId &removed_construct) {
    removed_construct_ = removed_construct;
  }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const RemoveConstructMessage &message);
  friend void from_json(const nlohmann::json &j,
                        RemoveConstructMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }
  static RemoveConstructMessage FromJson(const nlohmann::json &j) {
    RemoveConstructMessage message;
    from_json(j, message);
    return message;
  }
  // Overload stream operator for easy testing and logging
  friend std::ostream &operator<<(std::ostream &os,
                                  const RemoveConstructMessage &message) {
    os << message.ToString();
    return os;
  }
  std::string ToString() const {
    return "RemoveConstructMessage: " + ToJson().dump();
  }

 protected:
  // The unique identifier of the construct to be removed.
  EntityId removed_construct_{};
};
// Template builder class
template <class T>
class RemoveConstructMessageBuilderTemplate
    : public CommandRequestMessageBuilder<T> {
 public:
  T &SetRemovedConstruct(const EntityId &removed_construct) {
    remove_construct_message_->removed_construct_ = removed_construct;
    return static_cast<T &>(*this);
  }

 protected:
  std::shared_ptr<RemoveConstructMessage> remove_construct_message_;
  void SetBuiltMessage(
      std::shared_ptr<MusicCommandRequestMessage> new_message) {
    remove_construct_message_ =
        std::static_pointer_cast<RemoveConstructMessage>(new_message);
    CommandRequestMessageBuilder<T>::SetBuiltMessage(new_message);
  }
};
// Concrete builder
class RemoveConstructMessageBuilder
    : public RemoveConstructMessageBuilderTemplate<
          RemoveConstructMessageBuilder> {
 public:
  RemoveConstructMessageBuilder()
      : RemoveConstructMessageBuilderTemplate<RemoveConstructMessageBuilder>() {
    this->SetBuiltMessage(std::make_shared<RemoveConstructMessage>());
  }
  RemoveConstructMessage Build() { return *remove_construct_message_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_CONSTRUCTCONTROL_REMOVE_CONSTRUCT_MESSAGE_H_