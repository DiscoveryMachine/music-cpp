// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <exception>

#include "message/control/construct_primary_control_request_message.h"

namespace music::test {

TEST_GROUP(ConstructPrimaryControlRequestMessageTest){
    // SETUP
};

TEST(ConstructPrimaryControlRequestMessageTest, DefaultConstructor) {
  ConstructPrimaryControlRequestMessage message;

  LONGS_EQUAL(static_cast<long>(MusicMessageType::kPrimaryControlRequest),
              static_cast<long>(message.GetType()));
}

TEST(ConstructPrimaryControlRequestMessageTest, Builder) {
  // Build a ConstructPrimaryControlRequestMessage using the builder
  ConstructPrimaryControlRequestMessage message =
      ConstructPrimaryControlRequestMessageBuilder()
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
          .SetRequestId(42)
          .Build();

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

  // Check request ID
  LONGS_EQUAL(42, message.GetRequestId());
}

TEST(ConstructPrimaryControlRequestMessageTest, Validate) {
  auto valid_header = MusicMessageHeaderBuilder()
                          .SetExerciseId(5)
                          .SetTimestamp(100000L)
                          .Build();

  auto valid_origin_id = EntityIdBuilder()
                             .SetSite(10)
                             .SetApplication(20)
                             .SetEntityNumber(30)
                             .Build();

  auto valid_receiver_id = EntityIdBuilder()
                               .SetSite(40)
                               .SetApplication(50)
                               .SetEntityNumber(60)
                               .Build();

  ConstructPrimaryControlRequestMessage message;
  message.SetHeader(valid_header);
  message.SetOriginId(valid_origin_id);
  message.SetReceiverId(valid_receiver_id);
  message.SetRequestId(42);

  // Should not throw
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Final validation threw an exception: ") +
          std::string(e.what()))
             .c_str());
  }

  // Set invalid header
  message.SetHeader(MusicMessageHeader());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetHeader(valid_header);

  // Set invalid origin id
  message.SetOriginId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetOriginId(valid_origin_id);
}
TEST(ConstructPrimaryControlRequestMessageTest, ToJson) {
  ConstructPrimaryControlRequestMessage message =
      ConstructPrimaryControlRequestMessageBuilder()
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
          .SetRequestId(42)
          .Build();

  nlohmann::json j = message.ToJson();

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

  // Check request ID
  LONGS_EQUAL(42, j["requestID"].get<long>());
}

TEST(ConstructPrimaryControlRequestMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
    {
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
      "requestID": 42
    }
    )");

  ConstructPrimaryControlRequestMessage message =
      ConstructPrimaryControlRequestMessage::FromJson(json);

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

  // Check request ID
  LONGS_EQUAL(42, message.GetRequestId());
}
}  // namespace music::test