// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <CppUTest/TestHarness.h>

#include "message/command/constructcontrol/stop_construct_message.h"

namespace music::test {
TEST_GROUP(StopConstructMessageTest){
    // SETUP
};

TEST(StopConstructMessageTest, DefaultConstructor) {
  StopConstructMessage message;

  // Validate type and command identifier
  LONGS_EQUAL(static_cast<long>(MusicMessageType::kStopConstruct),
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(454007001L, message.GetCommandIdentifier());

  // Check origin and receiver IDs are default constructed
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetOriginId().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetEntityNumber());
}

TEST(StopConstructMessageTest, SettersAndGetters) {
  StopConstructMessage message;

  // Set origin ID (issuing entity)
  EntityId origin_id;
  origin_id.SetSite(1);
  origin_id.SetApplication(2);
  origin_id.SetEntityNumber(3);
  message.SetOriginId(origin_id);

  // Set receiver ID (construct to stop)
  EntityId receiver_id;
  receiver_id.SetSite(4);
  receiver_id.SetApplication(5);
  receiver_id.SetEntityNumber(6);
  message.SetReceiverId(receiver_id);

  // Set request ID
  message.SetRequestId(12345);

  // Check command identifier
  LONGS_EQUAL(454007001L, message.GetCommandIdentifier());

  // Check origin ID
  LONGS_EQUAL(1, message.GetOriginId().GetSite());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, message.GetOriginId().GetEntityNumber());

  // Check receiver ID
  LONGS_EQUAL(4, message.GetReceiverId().GetSite());
  LONGS_EQUAL(5, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(6, message.GetReceiverId().GetEntityNumber());

  // Check request ID
  LONGS_EQUAL(12345, message.GetRequestId());
}

TEST(StopConstructMessageTest, Builder) {
  // Build a StopConstructMessage using the builder
  StopConstructMessage message = StopConstructMessageBuilder()
                                     .SetHeader(MusicMessageHeaderBuilder()
                                                    .SetExerciseId(5)
                                                    .SetTimestamp(1234567890L)
                                                    .Build())
                                     .SetOriginId(EntityIdBuilder()
                                                      .SetSite(10)
                                                      .SetApplication(20)
                                                      .SetEntityNumber(30)
                                                      .Build())
                                     .SetReceiverId(EntityIdBuilder()
                                                        .SetSite(40)
                                                        .SetApplication(50)
                                                        .SetEntityNumber(60)
                                                        .Build())
                                     .SetRequestId(99999)
                                     .Build();

  // Check header
  LONGS_EQUAL(5, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1234567890L, message.GetHeader().GetTimestamp());
  // Check origin ID
  LONGS_EQUAL(10, message.GetOriginId().GetSite());
  LONGS_EQUAL(20, message.GetOriginId().GetApplication());
  LONGS_EQUAL(30, message.GetOriginId().GetEntityNumber());

  // Check command identifier
  LONGS_EQUAL(454007001L, message.GetCommandIdentifier());

  // Check request ID
  LONGS_EQUAL(99999, message.GetRequestId());
}
TEST(StopConstructMessageTest, Validate) {
  // Build a full, complete StopConstructMessage

  MusicMessageHeader valid_header = MusicMessageHeaderBuilder()
                                        .SetExerciseId(5)
                                        .SetTimestamp(100000L)
                                        .Build();
  EntityId valid_origin_id = EntityIdBuilder()
                                 .SetSite(10)
                                 .SetApplication(20)
                                 .SetEntityNumber(30)
                                 .Build();
  EntityId valid_receiver_id = EntityIdBuilder()
                                   .SetSite(40)
                                   .SetApplication(50)
                                   .SetEntityNumber(60)
                                   .Build();

  StopConstructMessage message = StopConstructMessageBuilder()
                                     .SetHeader(valid_header)
                                     .SetOriginId(valid_origin_id)
                                     .SetReceiverId(valid_receiver_id)
                                     .SetRequestId(99999)
                                     .Build();

  // Should be valid
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }
  // Set invalid receiver id
  message.SetReceiverId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetReceiverId(valid_receiver_id);
}

TEST(StopConstructMessageTest, ToJson) {
  StopConstructMessage message = StopConstructMessageBuilder()
                                     .SetHeader(MusicMessageHeaderBuilder()
                                                    .SetExerciseId(5)
                                                    .SetTimestamp(100000L)
                                                    .Build())
                                     .SetOriginId(EntityIdBuilder()
                                                      .SetSite(10)
                                                      .SetApplication(20)
                                                      .SetEntityNumber(30)
                                                      .Build())
                                     .SetReceiverId(EntityIdBuilder()
                                                        .SetSite(40)
                                                        .SetApplication(50)
                                                        .SetEntityNumber(60)
                                                        .Build())
                                     .SetRequestId(12345)
                                     .Build();

  nlohmann::json j = message.ToJson();

  // Check command identifier
  LONGS_EQUAL(454007001L, message.GetCommandIdentifier());

  // Check header
  LONGS_EQUAL(5, j["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(100000L, j["header"]["timestamp"].get<long>());

  // Check origin ID
  LONGS_EQUAL(10, j["originID"]["siteID"].get<int>());
  LONGS_EQUAL(20, j["originID"]["appID"].get<int>());
  LONGS_EQUAL(30, j["originID"]["entityID"].get<int>());

  // Check receiver ID
  LONGS_EQUAL(40, j["receiverID"]["siteID"].get<int>());
  LONGS_EQUAL(50, j["receiverID"]["appID"].get<int>());
  LONGS_EQUAL(60, j["receiverID"]["entityID"].get<int>());

  // Check command identifier
  LONGS_EQUAL(454007001L, message.GetCommandIdentifier());
}

TEST(StopConstructMessageTest, FromJson) {
  nlohmann::json j = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454007001,
    "header": {
        "exerciseID": 5,
        "timestamp": 100000
    },
    "originID": {
        "siteID": 10,
        "appID": 20,
        "entityID": 30
    },
    "receiverID": {
        "siteID": 40,
        "appID": 50,
        "entityID": 60
    },
    "requestID": 12345
}
)");

  StopConstructMessage message = StopConstructMessage::FromJson(j);

  // Check command identifier
  LONGS_EQUAL(454007001L, message.GetCommandIdentifier());

  // Check header
  LONGS_EQUAL(5, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(100000L, message.GetHeader().GetTimestamp());

  // Check origin ID
  LONGS_EQUAL(10, message.GetOriginId().GetSite());
  LONGS_EQUAL(20, message.GetOriginId().GetApplication());
  LONGS_EQUAL(30, message.GetOriginId().GetEntityNumber());

  // Check receiver ID
  LONGS_EQUAL(40, message.GetReceiverId().GetSite());
  LONGS_EQUAL(50, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(60, message.GetReceiverId().GetEntityNumber());
}
}  // namespace music::test