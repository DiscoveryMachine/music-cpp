// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <CppUTest/TestHarness.h>

#include "message/command/playercontrol/control_initiated_message.h"

namespace music::test {

TEST_GROUP(ControlInitiatedMessageTest){
    // SETUP
};

TEST(ControlInitiatedMessageTest, DefaultConstructor) {
  ControlInitiatedMessage message;
  LONGS_EQUAL(MusicMessageType::kControlInitiated,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kControlInitiated)
                  .action_type,
              message.GetCommandIdentifier());
}

TEST(ControlInitiatedMessageTest, SettersAndGetters) {
  ControlInitiatedMessage message;

  message.SetOriginId(music::EntityIdBuilder()
                          .SetEntityNumber(1)
                          .SetApplication(2)
                          .SetSite(3)
                          .Build());
  message.SetReceiverId(music::EntityIdBuilder()
                            .SetEntityNumber(4)
                            .SetApplication(5)
                            .SetSite(6)
                            .Build());
  message.SetRequestId(12345);

  LONGS_EQUAL(3, message.GetOriginId().GetSite());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(1, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(6, message.GetReceiverId().GetSite());
  LONGS_EQUAL(5, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(4, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(12345, message.GetRequestId());
}

TEST(ControlInitiatedMessageTest, Builder) {
  ControlInitiatedMessage message =
      ControlInitiatedMessageBuilder()
          .SetOriginId(music::EntityIdBuilder()
                           .SetEntityNumber(7)
                           .SetApplication(8)
                           .SetSite(9)
                           .Build())
          .SetReceiverId(music::EntityIdBuilder()
                             .SetEntityNumber(10)
                             .SetApplication(11)
                             .SetSite(12)
                             .Build())
          .SetRequestId(99999)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(20)
                         .SetTimestamp(1765894109000)
                         .Build())
          .Build();

  LONGS_EQUAL(9, message.GetOriginId().GetSite());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(12, message.GetReceiverId().GetSite());
  LONGS_EQUAL(11, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(10, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(99999, message.GetRequestId());
  LONGS_EQUAL(20, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765894109000, message.GetHeader().GetTimestamp());
}
TEST(ControlInitiatedMessageTest, Validate) {
  // Build a full, valid message
  ControlInitiatedMessage message =
      ControlInitiatedMessageBuilder()
          .SetOriginId(music::EntityIdBuilder()
                           .SetEntityNumber(7)
                           .SetApplication(8)
                           .SetSite(9)
                           .Build())
          .SetReceiverId(music::EntityIdBuilder()
                             .SetEntityNumber(10)
                             .SetApplication(11)
                             .SetSite(12)
                             .Build())
          .SetRequestId(99999)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(20)
                         .SetTimestamp(1765894109000)
                         .Build())
          .Build();

  // Validate should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }
}

TEST(ControlInitiatedMessageTest, ToJson) {
  ControlInitiatedMessage message =
      ControlInitiatedMessageBuilder()
          .SetOriginId(music::EntityIdBuilder()
                           .SetEntityNumber(7)
                           .SetApplication(8)
                           .SetSite(9)
                           .Build())
          .SetReceiverId(music::EntityIdBuilder()
                             .SetEntityNumber(10)
                             .SetApplication(11)
                             .SetSite(12)
                             .Build())
          .SetRequestId(99999)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(20)
                         .SetTimestamp(1765894109000)
                         .Build())
          .Build();

  nlohmann::json json = message.ToJson();

  // Check header
  LONGS_EQUAL(20, json["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(1765894109000, json["header"]["timestamp"].get<long>());

  // Check command identifier
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kControlInitiated)
                  .action_type,
              json["commandIdentifier"].get<long>());

  // Check origin ID
  LONGS_EQUAL(9, json["originID"]["siteID"].get<long>());
  LONGS_EQUAL(8, json["originID"]["appID"].get<long>());
  LONGS_EQUAL(7, json["originID"]["entityID"].get<long>());
}

TEST(ControlInitiatedMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454999001,
    "header": {
        "exerciseID": 20,
        "timestamp": 1765894109000
    },
    "originID": {
        "appID": 8,
        "entityID": 7,
        "siteID": 9
    },
    "receiverID": {
        "appID": 11,
        "entityID": 10,
        "siteID": 12
    },
    "requestID": 99999
}
)");

  ControlInitiatedMessage message = ControlInitiatedMessage::FromJson(json);

  // Check command identifier
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kControlInitiated)
                  .action_type,
              message.GetCommandIdentifier());

  // Check header
  LONGS_EQUAL(20, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765894109000, message.GetHeader().GetTimestamp());
  // Check origin ID
  LONGS_EQUAL(9, message.GetOriginId().GetSite());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  // Check receiver ID
  LONGS_EQUAL(12, message.GetReceiverId().GetSite());
  LONGS_EQUAL(11, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(10, message.GetReceiverId().GetEntityNumber());
  // Check request ID
  LONGS_EQUAL(99999, message.GetRequestId());
}

}  // namespace music::test
