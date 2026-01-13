// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/command/time/set_simulation_time_message.h"

namespace music::test {

TEST_GROUP(SetSimulationTimeMessageTest){
    // SETUP
};

TEST(SetSimulationTimeMessageTest, DefaultConstructor) {
  SetSimulationTimeMessage message;
  LONGS_EQUAL(MusicMessageType::kSetSimulationTime,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kSetSimulationTime)
                  .action_type,
              message.GetCommandIdentifier());
  LONGS_EQUAL(std::numeric_limits<long>::min(), message.GetNewTime());
}

TEST(SetSimulationTimeMessageTest, SettersAndGetters) {
  SetSimulationTimeMessage message;
  message.SetNewTime(1625078400);
  LONGS_EQUAL(1625078400, message.GetNewTime());

  message.SetNewTime(1625164800);
  LONGS_EQUAL(1625164800, message.GetNewTime());

  // Check one of the super class setters and getters
  music::EntityId origin_id = music::EntityIdBuilder()
                                  .SetEntityNumber(1)
                                  .SetApplication(2)
                                  .SetSite(3)
                                  .Build();
  message.SetOriginId(origin_id);
  LONGS_EQUAL(1, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, message.GetOriginId().GetSite());
}

TEST(SetSimulationTimeMessageTest, Builder) {
  SetSimulationTimeMessage message =
      SetSimulationTimeMessageBuilder()
          .SetNewTime(1234567890)
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
                         .SetTimestamp(1765903162342L)
                         .Build())
          .Build();

  // Check the class-specific fields
  LONGS_EQUAL(MusicMessageType::kSetSimulationTime,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kSetSimulationTime)
                  .action_type,
              message.GetCommandIdentifier());
  LONGS_EQUAL(1234567890, message.GetNewTime());

  // Check one of the super class fields
  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(9, message.GetOriginId().GetSite());
}

TEST(SetSimulationTimeMessageTest, Validate) {
  // Build a complete, valid message
  SetSimulationTimeMessage message =
      SetSimulationTimeMessageBuilder()
          .SetNewTime(1234567890)
          .SetOriginId(music::EntityIdBuilder()
                           .SetEntityNumber(1)
                           .SetApplication(2)
                           .SetSite(3)
                           .Build())
          .SetReceiverId(music::EntityIdBuilder()
                             .SetEntityNumber(4)
                             .SetApplication(5)
                             .SetSite(6)
                             .Build())
          .SetRequestId(99999)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(20)
                         .SetTimestamp(1765903162343L)
                         .Build())
          .Build();

  // Should pass validation
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set invalid new time
  message.SetNewTime(-1);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetNewTime(1234567890);

  // Set invalid header
  message.SetHeader(MusicMessageHeader());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetHeader(music::MusicMessageHeaderBuilder()
                        .SetExerciseId(20)
                        .SetTimestamp(1765903162344L)
                        .Build());

  // final check should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Final validation threw an exception: ") +
          std::string(e.what()))
             .c_str());
  }
}

TEST(SetSimulationTimeMessageTest, ToJson) {
  // Build a complete, valid message
  SetSimulationTimeMessage message =
      SetSimulationTimeMessageBuilder()
          .SetNewTime(1234567890)
          .SetOriginId(music::EntityIdBuilder()
                           .SetEntityNumber(1)
                           .SetApplication(2)
                           .SetSite(3)
                           .Build())
          .SetReceiverId(music::EntityIdBuilder()
                             .SetEntityNumber(4)
                             .SetApplication(5)
                             .SetSite(6)
                             .Build())
          .SetRequestId(99999)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(20)
                         .SetTimestamp(1765903162345L)
                         .Build())
          .Build();

  nlohmann::json json = message.ToJson();

  // Check the class-specific fields
  LONGS_EQUAL(1234567890, json["newTime"].get<long>());

  // Check the command identifier
  LONGS_EQUAL(static_cast<long>(MusicMessageTypeHelper::GetInfoForType(
                                    MusicMessageType::kSetSimulationTime)
                                    .action_type),
              json["commandIdentifier"].get<long>());

  // Check one of the super class fields
  LONGS_EQUAL(1, json["originID"]["entityID"].get<long>());
  LONGS_EQUAL(2, json["originID"]["appID"].get<long>());
  LONGS_EQUAL(3, json["originID"]["siteID"].get<long>());
}

TEST(SetSimulationTimeMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454009000,
    "header": {
        "exerciseID": 20,
        "timestamp": 1765903162346
    },
    "newTime": 1234567890,
    "originID": {
        "appID": 2,
        "entityID": 1,
        "siteID": 3
    },
    "receiverID": {
        "appID": 5,
        "entityID": 4,
        "siteID": 6
    },
    "requestID": 99999
}
)");

  SetSimulationTimeMessage set_sim_time_message =
      SetSimulationTimeMessage::FromJson(json);
  LONGS_EQUAL(1234567890, set_sim_time_message.GetNewTime());
  LONGS_EQUAL(1, set_sim_time_message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(2, set_sim_time_message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, set_sim_time_message.GetOriginId().GetSite());
  LONGS_EQUAL(4, set_sim_time_message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(5, set_sim_time_message.GetReceiverId().GetApplication());
  LONGS_EQUAL(6, set_sim_time_message.GetReceiverId().GetSite());
  LONGS_EQUAL(99999, set_sim_time_message.GetRequestId());
  LONGS_EQUAL(20, set_sim_time_message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765903162346L, set_sim_time_message.GetHeader().GetTimestamp());
}

}  // namespace music::test