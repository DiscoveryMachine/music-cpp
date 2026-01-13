// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_CONSTRUCT_REQUEST_MESSAGE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_COMMAND_SCENARIO_CREATE_CONSTRUCT_REQUEST_MESSAGE_H_

#include "message/command/music_command_request_message.h"
#include "message/location.h"
#include "message/orientation.h"
namespace music {

class CreateConstructRequestMessage : public MusicCommandRequestMessage {
  friend class CreateConstructRequestMessageBuilder;

 public:
  // Constructor
  CreateConstructRequestMessage() {
    type_ = MusicMessageType::kCreateConstructRequest;
    command_identifier_ = MusicMessageTypeHelper::GetInfoForType(
                              MusicMessageType::kCreateConstructRequest)
                              .action_type;
  }

  // Validate
  void Validate() const;

  // Static constants
  inline static const long kConstructTypeId = 1L;
  inline static const long kCallsignId = 2L;
  inline static const long kCommandIdentifier = 454013002L;
  inline static const long kLocationId = 3L;
  inline static const long kOrientationId = 4L;

  // JSON Keys
  inline static const std::string kConstructTypeKey = "constructType";
  inline static const std::string kConstructCallsignKey = "constructCallsign";
  inline static const std::string kConstructLocationKey = "constructLocation";
  inline static const std::string kConstructOrientationKey =
      "constructOrientation";

  // Getters
  const std::string &GetConstructType() const { return construct_type_; }
  const std::string &GetConstructCallsign() const { return callsign_; }
  const std::optional<Location> &GetConstructLocation() const {
    return construct_location_;
  }
  const std::optional<Orientation> &GetConstructOrientation() const {
    return construct_orientation_;
  }

  // Setters
  void SetConstructType(const std::string &construct_type) {
    construct_type_ = construct_type;
  }
  void SetConstructCallsign(const std::string &callsign) {
    callsign_ = callsign;
  }
  void SetConstructLocation(const Location &construct_location) {
    construct_location_ = construct_location;
  }
  // Use overloaded setters (one valid/one null)
  void SetConstructLocation(std::nullopt_t) {
    construct_location_ = std::nullopt;
  }
  void SetConstructOrientation(const Orientation &construct_orientation) {
    construct_orientation_ = construct_orientation;
  }
  void SetConstructOrientation(std::nullopt_t) {
    construct_orientation_ = std::nullopt;
  }

  // JSON serialization
  friend void to_json(nlohmann::json &j,
                      const CreateConstructRequestMessage &message);

  friend void from_json(const nlohmann::json &j,
                        CreateConstructRequestMessage &message);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static CreateConstructRequestMessage FromJson(const nlohmann::json &j) {
    CreateConstructRequestMessage message;
    from_json(j, message);
    return message;
  }

  friend std::ostream &operator<<(
      std::ostream &os, const CreateConstructRequestMessage &message) {
    os << message.ToString();
    return os;
  }

  std::string ToString() const {
    return "CreateConstructRequestMessage: " + ToJson().dump();
  }

 protected:
  // The full type name of the construct to be created. This type is the
  // combination of the constructs package and name fields or
  // "com.discoverymachine.entity.Merchant"
  std::string construct_type_{};

  // The callsign that should be assigned to the created construct.
  std::string callsign_{};

  // The location the construct should be placed at. (Required only for physical
  // constructs)
  std::optional<Location> construct_location_{};

  // The orientation of the construct to be created. (Required only for physical
  // constructs)
  std::optional<Orientation> construct_orientation_{};
};

// Builder
class CreateConstructRequestMessageBuilder
    : public CommandRequestMessageBuilder<
          CreateConstructRequestMessageBuilder> {
 public:
  CreateConstructRequestMessageBuilder() {
    create_construct_message_ =
        std::make_shared<CreateConstructRequestMessage>();
    SetBuiltMessage(create_construct_message_);
  }

  // Setters that return 'this' (the builder itself)
  CreateConstructRequestMessageBuilder &SetConstructType(
      const std::string &construct_type) {
    create_construct_message_->SetConstructType(construct_type);
    return *this;
  }
  CreateConstructRequestMessageBuilder &SetConstructCallsign(
      const std::string &callsign) {
    create_construct_message_->SetConstructCallsign(callsign);
    return *this;
  }
  CreateConstructRequestMessageBuilder &SetConstructLocation(
      const Location &construct_location) {
    create_construct_message_->SetConstructLocation(construct_location);
    return *this;
  }
  CreateConstructRequestMessageBuilder &SetConstructOrientation(
      const Orientation &construct_orientation) {
    create_construct_message_->SetConstructOrientation(construct_orientation);
    return *this;
  }

  CreateConstructRequestMessage Build() {
    EnforceSystemToSystemEntities();
    return *create_construct_message_;
  }

 protected:
  std::shared_ptr<CreateConstructRequestMessage> create_construct_message_;
  void SetBuiltMessage(
      std::shared_ptr<CreateConstructRequestMessage> new_message) {
    create_construct_message_ = new_message;
    CommandRequestMessageBuilder<
        CreateConstructRequestMessageBuilder>::SetBuiltMessage(new_message);
  }

  void EnforceSystemToSystemEntities() {
    // enforce origin and receiver entity numbers to be 0
    auto new_origin_id = create_construct_message_->GetOriginId();
    new_origin_id.SetEntityNumber(0);
    auto new_receiver_id = create_construct_message_->GetReceiverId();
    new_receiver_id.SetEntityNumber(0);
    create_construct_message_->SetOriginId(new_origin_id);
    create_construct_message_->SetReceiverId(new_receiver_id);
  }
};
}  // namespace music
#endif