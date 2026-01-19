// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <nlohmann/json_fwd.hpp>

#include "message/command/scenario/create_environment_request_message.h"

namespace music::test {

TEST_GROUP(CreateEnvironmentRequestMessageTest){
    // SETUP
};

TEST(CreateEnvironmentRequestMessageTest, DefaultConstructor) {
  CreateEnvironmentRequestMessage message;
  STRCMP_EQUAL("", message.GetEnvironmentName().c_str());
  CHECK_TRUE(message.GetEnvironmentMetadata().empty());

  // Check command identifier and type
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateEnvironmentRequest),
              static_cast<int>(message.GetType()));

  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kCreateEnvironmentRequest)
                  .action_type,
              message.GetCommandIdentifier());
}

TEST(CreateEnvironmentRequestMessageTest, SettersAndGetters) {
  CreateEnvironmentRequestMessage message;
  message.SetEnvironmentName("Tundra");
  nlohmann::json environment_json = {{"key", "test json"}};
  message.SetEnvironmentMetadata(environment_json);

  STRCMP_EQUAL("Tundra", message.GetEnvironmentName().c_str());
  STRCMP_EQUAL("test json", std::string(message.GetEnvironmentMetadata().at("key")).c_str());
}

TEST(CreateEnvironmentRequestMessageTest, Builder) {
  CreateEnvironmentRequestMessage message = CreateEnvironmentRequestMessageBuilder()
                                                .SetEnvironmentName("Rainforest")
                                                .SetEnvironmentMetadata({{"key", "test value"}})
                                                .Build();
  STRCMP_EQUAL("Rainforest", message.GetEnvironmentName().c_str());
  STRCMP_EQUAL("test value", std::string(message.GetEnvironmentMetadata().at("key")).c_str());
}

TEST(CreateEnvironmentRequestMessageTest, Validate) {
  CreateEnvironmentRequestMessage message;
  CHECK_THROWS(std::exception, message.Validate());

  nlohmann::json valid_metadata = {{"key", "value"}};
  std::string valid_name = "Savannah";
  message =
      CreateEnvironmentRequestMessageBuilder()
          .SetEnvironmentName(valid_name)
          .SetEnvironmentMetadata(valid_metadata)
          .SetOriginId(
              music::EntityIdBuilder().SetApplication(4).SetEntityNumber(5).SetSite(6).Build())
          .SetReceiverId(
              music::EntityIdBuilder().SetApplication(10).SetEntityNumber(11).SetSite(12).Build())
          .SetRequestId(67890)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(123)
                         .SetTimestamp(9876543210987L)
                         .Build())
          .Build();
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL("Validation should not throw for a valid message");
  }

  // Set bad environment name
  message.SetEnvironmentName("");
  CHECK_THROWS(std::exception, message.Validate());

  message.SetEnvironmentName(valid_name);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL("Validation should not throw for a valid message after resetting the "
         "name");
  }

  // Set bad environment metadata
  message.SetEnvironmentMetadata(nlohmann::json::object());
  CHECK_THROWS(std::exception, message.Validate());

  message.SetEnvironmentMetadata(valid_metadata);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL("Validation should not throw for a valid message after resetting the "
         "metadata");
  }
}

TEST(CreateEnvironmentRequestMessageTest, ToJson) {
  nlohmann::json valid_metadata = {{"key", "value2"}};
  std::string valid_name = "Rocky Mountains";
  auto message =
      CreateEnvironmentRequestMessageBuilder()
          .SetEnvironmentName(valid_name)
          .SetEnvironmentMetadata(valid_metadata)
          .SetOriginId(
              music::EntityIdBuilder().SetApplication(4).SetEntityNumber(5).SetSite(6).Build())
          .SetReceiverId(
              music::EntityIdBuilder().SetApplication(10).SetEntityNumber(11).SetSite(12).Build())
          .SetRequestId(67890)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(123)
                         .SetTimestamp(9876543210987L)
                         .Build())
          .Build();
  nlohmann::json json = message.ToJson();

  STRCMP_EQUAL("Rocky Mountains",
               std::string(json.at(CreateEnvironmentRequestMessage::kEnvironmentNameKey)).c_str());
  STRCMP_EQUAL(
      "value2",
      std::string(json.at(CreateEnvironmentRequestMessage::kEnvironmentMetadataKey).at("key"))
          .c_str());
}

TEST(CreateEnvironmentRequestMessageTest, FromJson) {
  nlohmann::json json = {
      {CreateEnvironmentRequestMessage::kEnvironmentNameKey, "Rocky Mountains"},
      {CreateEnvironmentRequestMessage::kEnvironmentMetadataKey, {{"key", "value2"}}},
      {CreateEnvironmentRequestMessage::kCommandIdKey,
       MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kCreateEnvironmentRequest)
           .action_type},
      {music::MusicRequestMessage::kOriginIdKey,
       {{music::SimulationAddress::kApplicationJsonKey, 7},
        {music::EntityId::kEntityNumberJsonKey, 8},
        {music::SimulationAddress::kSiteJsonKey, 9}}},
      {music::MusicRequestMessage::kReceiverIdKey,
       {{music::SimulationAddress::kApplicationJsonKey, 13},
        {music::EntityId::kEntityNumberJsonKey, 14},
        {music::SimulationAddress::kSiteJsonKey, 15}}},
      {music::MusicRequestMessage::kRequestIdKey, 123456},
      {music::MusicMessage::kHeaderJsonKey,
       {{music::MusicMessageHeader::kExerciseIdKey, 200},
        {music::MusicMessageHeader::kTimestampKey, 123321456654789L}}},
      {music::MusicMessage::kTypeJsonKey, music::MusicMessageType::kCreateEnvironmentRequest}};

  auto message = CreateEnvironmentRequestMessage::FromJson(json);
  STRCMP_EQUAL("Rocky Mountains", message.GetEnvironmentName().c_str());
  STRCMP_EQUAL("value2", std::string(message.GetEnvironmentMetadata().at("key")).c_str());
}

} // namespace music::test