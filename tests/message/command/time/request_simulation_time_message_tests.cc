// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/command/time/request_simulation_time_message.h"

namespace music::test {

TEST_GROUP(RequestSimulationTimeMessageTest){
    // SETUP
};

TEST(RequestSimulationTimeMessageTest, DefaultConstructor) {
  RequestSimulationTimeMessage message;
  LONGS_EQUAL(MusicMessageType::kRequestSimulationTime,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kRequestSimulationTime)
                  .action_type,
              message.GetCommandIdentifier());
}

TEST(RequestSimulationTimeMessageTest, SettersAndGetters) {
  RequestSimulationTimeMessage message;

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

TEST(RequestSimulationTimeMessageTest, Builder) {
  RequestSimulationTimeMessage message =
      RequestSimulationTimeMessageBuilder()
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
                         .SetTimestamp(1765308607186L)
                         .Build())
          .Build();

  // Check the class-specific fields
  LONGS_EQUAL(MusicMessageType::kRequestSimulationTime,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kRequestSimulationTime)
                  .action_type,
              message.GetCommandIdentifier());

  // Check one of the super class fields
  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(9, message.GetOriginId().GetSite());
}

TEST(RequestSimulationTimeMessageTest, Validate) {
  // Build a full, valid message
  EntityId valid_origin_id =
      EntityIdBuilder().SetEntityNumber(1).SetApplication(2).SetSite(3).Build();
  EntityId valid_receiver_id =
      EntityIdBuilder().SetEntityNumber(4).SetApplication(5).SetSite(6).Build();
  MusicMessageHeader valid_header = MusicMessageHeaderBuilder()
                                        .SetExerciseId(20)
                                        .SetTimestamp(1765308607186L)
                                        .Build();
  RequestSimulationTimeMessage message = RequestSimulationTimeMessageBuilder()
                                             .SetOriginId(valid_origin_id)
                                             .SetReceiverId(valid_receiver_id)
                                             .SetRequestId(99999)
                                             .SetHeader(valid_header)
                                             .Build();

  // Validate should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set invalid origin id
  message.SetOriginId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetOriginId(valid_origin_id);

  // Set invalid receiver id
  message.SetReceiverId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetReceiverId(valid_receiver_id);

  // Final check should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Final validation threw an exception: ") +
          std::string(e.what()))
             .c_str());
  }
}

TEST(RequestSimulationTimeMessageTest, ToJson) {
  // Build a full, valid message
  RequestSimulationTimeMessage message =
      RequestSimulationTimeMessageBuilder()
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(1)
                           .SetApplication(2)
                           .SetSite(3)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(4)
                             .SetApplication(5)
                             .SetSite(6)
                             .Build())
          .SetRequestId(99999)
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(20)
                         .SetTimestamp(1765308607186L)
                         .Build())
          .Build();

  nlohmann::json json = message.ToJson();

  // Check header
  LONGS_EQUAL(20, json["header"]["exerciseID"].get<long>());
  LONGS_EQUAL(1765308607186L, json["header"]["timestamp"].get<long>());
  // Check origin ID
  LONGS_EQUAL(3, json["originID"]["siteID"].get<long>());
  LONGS_EQUAL(2, json["originID"]["appID"].get<long>());
  LONGS_EQUAL(1, json["originID"]["entityID"].get<long>());
  // Check receiver ID
  LONGS_EQUAL(6, json["receiverID"]["siteID"].get<long>());
  LONGS_EQUAL(5, json["receiverID"]["appID"].get<long>());
  LONGS_EQUAL(4, json["receiverID"]["entityID"].get<long>());
  // Check request ID
  LONGS_EQUAL(99999, json["requestID"].get<long>());
  // Check command identifier
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kRequestSimulationTime)
                  .action_type,
              json["commandIdentifier"].get<long>());
}

TEST(RequestSimulationTimeMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454009000,
    "header": {
        "exerciseID": 20,
        "timestamp": 1765308607186
    },
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

  RequestSimulationTimeMessage message =
      RequestSimulationTimeMessage::FromJson(json);

  // Check one of the super class fields
  LONGS_EQUAL(1, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, message.GetOriginId().GetSite());

  // Check command identifier
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kRequestSimulationTime)
                  .action_type,
              message.GetCommandIdentifier());
}
}  // namespace music::test