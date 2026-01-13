// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/command/time/simulation_time_message.h"

namespace music::test {

TEST_GROUP(SimulationTimeMessageTest){
    // SETUP
};

TEST(SimulationTimeMessageTest, DefaultConstructor) {
  SimulationTimeMessage message;
  LONGS_EQUAL(MusicMessageType::kSimulationTime,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(
      MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kSimulationTime)
          .action_type,
      message.GetCommandIdentifier());
  LONGS_EQUAL(std::numeric_limits<long>::min(), message.GetSimTime());
}

TEST(SimulationTimeMessageTest, SettersAndGetters) {
  SimulationTimeMessage message;
  message.SetSimTime(1234567890);
  LONGS_EQUAL(1234567890, message.GetSimTime());

  message.SetSimTime(987654321);
  LONGS_EQUAL(987654321, message.GetSimTime());

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

TEST(SimulationTimeMessageTest, Builder) {
  SimulationTimeMessage message =
      SimulationTimeMessageBuilder()
          .SetSimTime(1234567890)
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
                         .SetTimestamp(1765903162340L)
                         .Build())
          .Build();

  // Check the class-specific fields
  LONGS_EQUAL(MusicMessageType::kSimulationTime,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(
      MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kSimulationTime)
          .action_type,
      message.GetCommandIdentifier());
  LONGS_EQUAL(1234567890, message.GetSimTime());

  // Check one of the super class fields
  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(9, message.GetOriginId().GetSite());
}

TEST(SimulationTimeMessageTest, Validate) {
  // Build a complete, valid message
  SimulationTimeMessage message =
      SimulationTimeMessageBuilder()
          .SetSimTime(1234567890)
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
                         .SetTimestamp(1765903162341L)
                         .Build())
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  // Should pass validation
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set invalid sim time
  message.SetSimTime(-1);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetSimTime(1234567890);

  // Set invalid header
  message.SetHeader(MusicMessageHeader());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetHeader(music::MusicMessageHeaderBuilder()
                        .SetExerciseId(20)
                        .SetTimestamp(1765903162342L)
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

TEST(SimulationTimeMessageTest, ToJson) {
  // Build a complete, valid message
  SimulationTimeMessage message =
      SimulationTimeMessageBuilder()
          .SetSimTime(1234567890)
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
          .SetRequestStatus(RequestStatus::kPending)
          .Build();

  nlohmann::json json = message.ToJson();

  // Check the class-specific fields
  LONGS_EQUAL(1234567890, json["simTime"].get<long>());

  // Check the command identifier
  LONGS_EQUAL(static_cast<long>(MusicMessageTypeHelper::GetInfoForType(
                                    MusicMessageType::kSimulationTime)
                                    .action_type),
              json["commandIdentifier"].get<long>());

  // Check one of the super class fields
  LONGS_EQUAL(1, json["originID"]["entityID"].get<long>());
  LONGS_EQUAL(2, json["originID"]["appID"].get<long>());
  LONGS_EQUAL(3, json["originID"]["siteID"].get<long>());
}

TEST(SimulationTimeMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 3001,
    "header": {
        "exerciseID": 20,
        "timestamp": 1765903162344
    },
    "originID": {
        "entityID": 1,
        "appID": 2,
        "siteID": 3
    },
    "receiverID": {
        "entityID": 4,
        "appID": 5,
        "siteID": 6
    },
    "requestID": 99999,
    "simTime": 1234567890,
    "status": 1
}
)");

  SimulationTimeMessage message = SimulationTimeMessage::FromJson(json);

  // Check the class-specific fields
  LONGS_EQUAL(1234567890, message.GetSimTime());

  // Check one of the super class fields
  LONGS_EQUAL(1, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, message.GetOriginId().GetSite());
}

}  // namespace music::test