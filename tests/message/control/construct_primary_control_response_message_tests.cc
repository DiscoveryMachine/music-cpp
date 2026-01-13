// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/control/construct_primary_control_response_message.h"

namespace music::test {
TEST_GROUP(ConstructPrimaryControlResponseMessageTest){
    // SETUP
};

TEST(ConstructPrimaryControlResponseMessageTest, DefaultConstructor) {
  ConstructPrimaryControlResponseMessage message;

  LONGS_EQUAL(static_cast<long>(MusicMessageType::kPrimaryControlResponse),
              static_cast<long>(message.GetType()));
}

TEST(ConstructPrimaryControlResponseMessageTest, Builder) {
  // Build a ConstructPrimaryControlResponseMessage using the builder
  ConstructPrimaryControlResponseMessage message =
      ConstructPrimaryControlResponseMessageBuilder()
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
          .SetRequestStatus(RequestStatus::kExecuting)
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

  // Check request status
  LONGS_EQUAL(static_cast<long>(RequestStatus::kExecuting),
              static_cast<long>(message.GetRequestStatus()));
}

TEST(ConstructPrimaryControlResponseMessageTest, Validate) {
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

  ConstructPrimaryControlResponseMessage message =
      ConstructPrimaryControlResponseMessageBuilder()
          .SetHeader(valid_header)
          .SetOriginId(valid_origin_id)
          .SetReceiverId(valid_receiver_id)
          .SetRequestId(42)
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();
  // This should not throw an exception
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

  // Set invalid status
  message.SetRequestStatus(static_cast<RequestStatus>(-1));
  CHECK_THROWS(std::exception, message.Validate());
  message.SetRequestStatus(RequestStatus::kComplete);
}

TEST(ConstructPrimaryControlResponseMessageTest, ToJson) {
  ConstructPrimaryControlResponseMessage message =
      ConstructPrimaryControlResponseMessageBuilder()
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
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  nlohmann::json json = message.ToJson();

  // Check header
  LONGS_EQUAL(5, json["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(100000L, json["header"]["timestamp"].get<long>());
  // Check origin ID
  LONGS_EQUAL(10, json["originID"]["siteID"].get<int>());
  LONGS_EQUAL(20, json["originID"]["appID"].get<int>());
  LONGS_EQUAL(30, json["originID"]["entityID"].get<int>());

  // Check receiver ID
  LONGS_EQUAL(40, json["receiverID"]["siteID"].get<int>());
  LONGS_EQUAL(50, json["receiverID"]["appID"].get<int>());
  LONGS_EQUAL(60, json["receiverID"]["entityID"].get<int>());

  // Check request ID
  LONGS_EQUAL(42, json["requestID"].get<long>());

  // Check request status
  LONGS_EQUAL(static_cast<long>(RequestStatus::kComplete),
              static_cast<long>(json["status"].get<RequestStatus>()));
}

TEST(ConstructPrimaryControlResponseMessageTest, FromJson) {
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
      "requestID": 42,
      "status": 2
    }
  )");

  ConstructPrimaryControlResponseMessage message =
      ConstructPrimaryControlResponseMessage::FromJson(json);

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

  // Check request status
  LONGS_EQUAL(static_cast<long>(RequestStatus::kExecuting),
              static_cast<long>(message.GetRequestStatus()));
}

}  // namespace music::test