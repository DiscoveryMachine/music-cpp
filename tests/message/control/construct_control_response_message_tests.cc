// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/control/construct_control_response_message.h"

namespace music::test {
TEST_GROUP(ConstructControlResponseMessageTest){
    // SETUP
};

TEST(ConstructControlResponseMessageTest, DefaultConstructor) {
  ConstructControlResponseMessage message;

  LONGS_EQUAL(static_cast<long>(MusicMessageType::kControlResponse),
              static_cast<long>(message.GetType()));
}

TEST(ConstructControlResponseMessageTest, Builder) {
  // Build a ConstructControlResponseMessage using the builder
  ConstructControlResponseMessage message =
      ConstructControlResponseMessageBuilder()
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

  // Validate fields
  LONGS_EQUAL(5, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(100000L, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(10, message.GetOriginId().GetSite());
  LONGS_EQUAL(20, message.GetOriginId().GetApplication());
  LONGS_EQUAL(30, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(40, message.GetReceiverId().GetSite());
  LONGS_EQUAL(50, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(60, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(12345, message.GetRequestId());
}

TEST(ConstructControlResponseMessageTest, Validate) {
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

  ConstructControlResponseMessage message =
      ConstructControlResponseMessageBuilder()
          .SetHeader(valid_header)
          .SetOriginId(valid_origin_id)
          .SetReceiverId(valid_receiver_id)
          .SetRequestId(12345)
          .SetRequestStatus(RequestStatus::kExecuting)
          .Build();

  // Message should be valid
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
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
  // Set invalid receiver id
  message.SetReceiverId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetReceiverId(valid_receiver_id);
  // Set invalid request id
  message.SetRequestId(-1);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetRequestId(12345);
  // Set invalid status
  message.SetRequestStatus(static_cast<RequestStatus>(-1));
  CHECK_THROWS(std::exception, message.Validate());
  message.SetRequestStatus(RequestStatus::kComplete);

  // Final check should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Final validation threw an exception: ") +
          std::string(e.what()))
             .c_str());
  }
}

TEST(ConstructControlResponseMessageTest, ToJson) {
  ConstructControlResponseMessage message =
      ConstructControlResponseMessageBuilder()
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
          .SetRequestStatus(RequestStatus::kComplete)
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
  LONGS_EQUAL(12345, j["requestID"].get<long>());
  // Check status
  LONGS_EQUAL(static_cast<long>(RequestStatus::kComplete),
              j["status"].get<long>());
}

TEST(ConstructControlResponseMessageTest, FromJson) {
  nlohmann::json j = nlohmann::json::parse(R"(
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
          "requestID": 12345,
          "status": 5
        }
        )");

  ConstructControlResponseMessage message =
      ConstructControlResponseMessage::FromJson(j);

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
  LONGS_EQUAL(12345, message.GetRequestId());
  // Check status
  LONGS_EQUAL(static_cast<long>(RequestStatus::kAborted),
              static_cast<long>(message.GetRequestStatus()));
}
}  // namespace music::test
