// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/control/construct_control_request_message.h"

namespace music::test {
TEST_GROUP(ConstructControlRequestMessageTest){
    // SETUP
};

TEST(ConstructControlRequestMessageTest, DefaultConstructor) {
  ConstructControlRequestMessage message;
  LONGS_EQUAL(MusicMessageType::kControlRequest, message.GetType());
  LONGS_EQUAL(std::numeric_limits<long>::min(), message.GetRequestId());
  CHECK_TRUE(message.GetContext().empty());
}

TEST(ConstructControlRequestMessageTest, SettersAndGetters) {
  ConstructControlRequestMessage message;
  EntityId test_entity;
  test_entity.SetSite(1);
  test_entity.SetApplication(2);
  test_entity.SetEntityNumber(3);

  message.SetTargetConstruct(test_entity);
  message.SetContext("com.discoverymachine.context.Arrest");

  EntityId retrieved_entity = message.GetTargetConstruct();
  LONGS_EQUAL(1, retrieved_entity.GetSite());
  LONGS_EQUAL(2, retrieved_entity.GetApplication());
  LONGS_EQUAL(3, retrieved_entity.GetEntityNumber());
  STRCMP_EQUAL("com.discoverymachine.context.Arrest",
               message.GetContext().c_str());
}

TEST(ConstructControlRequestMessageTest, Validation) {
  ConstructControlRequestMessage message;

  // Default message should be invalid due to empty context and uninitialized
  // EntityId
  CHECK_THROWS(std::exception, message.Validate());

  // Set valid context and TargetConstruct
  EntityId valid_target_construct;
  valid_target_construct.SetSite(1);
  valid_target_construct.SetApplication(1);
  valid_target_construct.SetEntityNumber(1);

  message.SetTargetConstruct(valid_target_construct);
  std::string valid_context = "com.discoverymachine.context.Arrest";
  message.SetContext(valid_context);

  MusicMessageHeader valid_header =
      MusicMessageHeaderBuilder().SetExerciseId(1).SetTimestamp(1000L).Build();
  message.SetHeader(valid_header);

  EntityId valid_origin;
  valid_origin.SetSite(2);
  valid_origin.SetApplication(2);
  valid_origin.SetEntityNumber(2);
  message.SetOriginId(valid_origin);

  EntityId valid_receiver;
  valid_receiver.SetSite(3);
  valid_receiver.SetApplication(3);
  valid_receiver.SetEntityNumber(3);
  message.SetReceiverId(valid_receiver);

  message.SetRequestId(1);

  // Now the message should be valid
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL("Validation should have passed but threw an exception.");
  }

  // Check invalid context
  message.SetContext("");
  CHECK_THROWS(std::exception, message.Validate());
  message.SetContext(valid_context);

  // Check invalid TargetConstruct
  message.SetTargetConstruct(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetTargetConstruct(valid_target_construct);

  // Check invalid header
  message.SetHeader(MusicMessageHeader());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetHeader(valid_header);
}

TEST(ConstructControlRequestMessageTest, Builder) {
  ConstructControlRequestMessage message =
      ConstructControlRequestMessageBuilder()
          .SetTargetConstruct(EntityIdBuilder()
                                  .SetSite(1)
                                  .SetApplication(2)
                                  .SetEntityNumber(3)
                                  .Build())
          .SetContext("com.discoverymachine.context.Arrest")
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(1)
                         .SetTimestamp(1000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(4)
                           .SetApplication(5)
                           .SetEntityNumber(6)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(7)
                             .SetApplication(8)
                             .SetEntityNumber(9)
                             .Build())
          .SetRequestId(42)
          .Build();
}

TEST(ConstructControlRequestMessageTest, ToJson) {
  ConstructControlRequestMessage message =
      ConstructControlRequestMessageBuilder()
          .SetTargetConstruct(EntityIdBuilder()
                                  .SetSite(1)
                                  .SetApplication(2)
                                  .SetEntityNumber(3)
                                  .Build())
          .SetContext("com.discoverymachine.context.Arrest")
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(1)
                         .SetTimestamp(1000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(4)
                           .SetApplication(5)
                           .SetEntityNumber(6)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(7)
                             .SetApplication(8)
                             .SetEntityNumber(9)
                             .Build())
          .SetRequestId(42)
          .Build();

  nlohmann::json json = message.ToJson();

  // Check header
  LONGS_EQUAL(1, json["header"]["exerciseID"].get<long>());
  LONGS_EQUAL(1000L, json["header"]["timestamp"].get<long>());

  // Check targetConstruct and context
  LONGS_EQUAL(1, json["targetConstruct"]["siteID"].get<int>());
  LONGS_EQUAL(2, json["targetConstruct"]["appID"].get<int>());
  LONGS_EQUAL(3, json["targetConstruct"]["entityID"].get<int>());
  STRCMP_EQUAL("com.discoverymachine.context.Arrest",
               json["context"].get<std::string>().c_str());

  // Check origin ID
  LONGS_EQUAL(4, json["originID"]["siteID"].get<int>());
  LONGS_EQUAL(5, json["originID"]["appID"].get<int>());
  LONGS_EQUAL(6, json["originID"]["entityID"].get<int>());

  // Check receiver ID
  LONGS_EQUAL(7, json["receiverID"]["siteID"].get<int>());
  LONGS_EQUAL(8, json["receiverID"]["appID"].get<int>());
  LONGS_EQUAL(9, json["receiverID"]["entityID"].get<int>());

  // Check request ID
  LONGS_EQUAL(42, json["requestID"].get<long>());
}

TEST(ConstructControlRequestMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
  {
    "context": "com.discoverymachine.context.Arrest",
    "header": {
      "exerciseID": 1,
      "timestamp": 1000
    },
    "originID": {
      "siteID": 4,
      "appID": 5,
      "entityID": 6
    },
    "receiverID": {
      "siteID": 7,
      "appID": 8,
      "entityID": 9
    },
    "requestID": 42,
    "targetConstruct": {
      "siteID": 1,
      "appID": 2,
      "entityID": 3
    }
  })");

  ConstructControlRequestMessage message =
      ConstructControlRequestMessage::FromJson(json);

  // Check header
  LONGS_EQUAL(1, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1000L, message.GetHeader().GetTimestamp());

  // Check targetConstruct and context
  EntityId target_construct = message.GetTargetConstruct();
  LONGS_EQUAL(1, target_construct.GetSite());
  LONGS_EQUAL(2, target_construct.GetApplication());
  LONGS_EQUAL(3, target_construct.GetEntityNumber());
  STRCMP_EQUAL("com.discoverymachine.context.Arrest",
               message.GetContext().c_str());

  // Check origin ID
  EntityId origin_id = message.GetOriginId();
  LONGS_EQUAL(4, origin_id.GetSite());
  LONGS_EQUAL(5, origin_id.GetApplication());
  LONGS_EQUAL(6, origin_id.GetEntityNumber());

  // Check receiver ID
  EntityId receiver_id = message.GetReceiverId();
  LONGS_EQUAL(7, receiver_id.GetSite());
  LONGS_EQUAL(8, receiver_id.GetApplication());
  LONGS_EQUAL(9, receiver_id.GetEntityNumber());

  // Check request ID
  LONGS_EQUAL(42, message.GetRequestId());
}

}  // namespace music::test