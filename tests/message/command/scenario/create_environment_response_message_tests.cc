// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <exception>

#include "message/command/scenario/create_environment_response_message.h"

namespace music::test {

TEST_GROUP(CreateEnvironmentResponseMessageTest){
    // SETUP
};

TEST(CreateEnvironmentResponseMessageTest, DefaultConstructor) {
  CreateEnvironmentResponseMessage message;

  // No new members, test type and command identifier
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateEnvironmentResponse),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateEnvironmentResponse)
                  .action_type,
              message.GetCommandIdentifier());
}

// No members to test setters and getters

TEST(CreateEnvironmentResponseMessageTest, Builder) {
  CreateEnvironmentResponseMessage message =
      CreateEnvironmentResponseMessageBuilder().Build();

  // No new members, test type and command identifier
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateEnvironmentResponse),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateEnvironmentResponse)
                  .action_type,
              message.GetCommandIdentifier());
}

TEST(CreateEnvironmentResponseMessageTest, Validate_InvalidFails_ValidPasses) {
  CreateEnvironmentResponseMessage message =
      CreateEnvironmentResponseMessageBuilder().Build();
  CHECK_THROWS(std::exception, message.Validate());
  EntityId valid_entity =
      EntityIdBuilder().SetApplication(1).SetSite(2).SetEntityNumber(0).Build();
  MusicMessageHeader valid_header = MusicMessageHeaderBuilder()
                                        .SetExerciseId(42)
                                        .SetTimestamp(1765548278968)
                                        .Build();
  message.SetHeader(valid_header);
  message.SetOriginId(valid_entity);
  message.SetReceiverId(valid_entity);
  message.SetRequestId(7L);
  message.SetRequestStatus(RequestStatus::kPending);
  // Type and Command Identifier should be set from the default constructor used
  // in the Builder.
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string(
             "Validation should not throw an exception for a valid message: ") +
         std::string(e.what()))
            .c_str());
  }
}

TEST(CreateEnvironmentResponseMessageTest, ToJson) {
  CreateEnvironmentResponseMessage message =
      CreateEnvironmentResponseMessageBuilder().Build();

  nlohmann::json json = message.ToJson();

  // Check for command identifier in JSON
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateEnvironmentResponse)
                  .action_type,
              json[CreateEnvironmentResponseMessage::kCommandIdKey]);
}

TEST(CreateEnvironmentResponseMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
  "header": {
    "exerciseID": 2,
    "timestamp": 1605645402861
  },
  "commandIdentifier": 454013001,
  "originID": {
    "siteID": 300,
    "appID": 30,
    "entityID": 0
  },
  "receiverID": {
    "siteID": 300,
    "appID": 50,
    "entityID": 0
  },
  "requestID": 322,
  "status": 2
}
)");

  CreateEnvironmentResponseMessage message =
      CreateEnvironmentResponseMessage::FromJson(json);

  LONGS_EQUAL(2, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1605645402861L, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(454013001L, message.GetCommandIdentifier());
  LONGS_EQUAL(300, message.GetOriginId().GetSite());
  LONGS_EQUAL(30, message.GetOriginId().GetApplication());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(300, message.GetReceiverId().GetSite());
  LONGS_EQUAL(50, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(322, message.GetRequestId());
  LONGS_EQUAL(2, static_cast<int>(message.GetRequestStatus()));
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateEnvironmentResponse),
              static_cast<int>(message.GetType()));
}

}  // namespace music::test