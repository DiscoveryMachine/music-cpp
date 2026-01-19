// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <CppUTest/TestHarness.h>

#include "message/command/simulation/display_messages_message.h"

namespace music::test {

TEST_GROUP(DisplayMessagesMessageTest){
    // SETUP
};

TEST(DisplayMessagesMessageTest, DefaultConstructor) {
  DisplayMessagesMessage message;
  LONGS_EQUAL(MusicMessageType::kDisplayMessages,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(
      MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kDisplayMessages)
          .action_type,
      message.GetCommandIdentifier());
  LONGS_EQUAL(std::numeric_limits<long>::min(), message.GetTimeout());
  LONGS_EQUAL(0, message.GetMessages().size());
}

TEST(DisplayMessagesMessageTest, SettersAndGetters) {
  DisplayMessagesMessage message;

  std::vector<std::string> test_messages = {"First line of message",
                                            "Second line of message",
                                            "Third line of message"};
  message.SetMessages(test_messages);
  message.SetTimeout(10);

  LONGS_EQUAL(3, message.GetMessages().size());
  STRCMP_EQUAL("First line of message", message.GetMessages()[0].c_str());
  STRCMP_EQUAL("Second line of message", message.GetMessages()[1].c_str());
  STRCMP_EQUAL("Third line of message", message.GetMessages()[2].c_str());
  LONGS_EQUAL(10, message.GetTimeout());
}

TEST(DisplayMessagesMessageTest, Builder) {
  DisplayMessagesMessage message =
      DisplayMessagesMessageBuilder()
          .SetMessages({"Line 1", "Line 2", "Line 3"})
          .SetTimeout(5)
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
                         .SetTimestamp(1765907966262L)
                         .Build())
          .Build();

  LONGS_EQUAL(3, message.GetMessages().size());
  STRCMP_EQUAL("Line 1", message.GetMessages()[0].c_str());
  STRCMP_EQUAL("Line 2", message.GetMessages()[1].c_str());
  STRCMP_EQUAL("Line 3", message.GetMessages()[2].c_str());
  LONGS_EQUAL(5, message.GetTimeout());

  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(9, message.GetOriginId().GetSite());

  LONGS_EQUAL(10, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(11, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(12, message.GetReceiverId().GetSite());

  LONGS_EQUAL(99999, message.GetRequestId());

  LONGS_EQUAL(20, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765907966262L, message.GetHeader().GetTimestamp());
}

TEST(DisplayMessagesMessageTest, Validate) {
  // Build a full, valid message
  DisplayMessagesMessage message =
      DisplayMessagesMessageBuilder()
          .SetMessages({"Line 1", "Line 2", "Line 3"})
          .SetTimeout(5)
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
                         .SetTimestamp(1765907966262L)
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
  // Set invalid timeout
  message.SetTimeout(-2);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetTimeout(5);
  // Set invalid origin id
  message.SetOriginId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetOriginId(music::EntityIdBuilder()
                          .SetEntityNumber(7)
                          .SetApplication(8)
                          .SetSite(9)
                          .Build());
}

TEST(DisplayMessagesMessageTest, ToJson) {
  // Build a full, valid message
  DisplayMessagesMessage message =
      DisplayMessagesMessageBuilder()
          .SetMessages({"Line 1", "Line 2", "Line 3"})
          .SetTimeout(5)
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
                         .SetTimestamp(1765907966262L)
                         .Build())
          .Build();
  nlohmann::json json = message.ToJson();

  LONGS_EQUAL(5, json["timeout"].get<long>());
  LONGS_EQUAL(3, json["messages"].size());

  STRCMP_EQUAL("Line 1", json["messages"][0].get<std::string>().c_str());
  STRCMP_EQUAL("Line 2", json["messages"][1].get<std::string>().c_str());
  STRCMP_EQUAL("Line 3", json["messages"][2].get<std::string>().c_str());

  // Check the command identifier
  LONGS_EQUAL(
      MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kDisplayMessages)
          .action_type,
      json["commandIdentifier"].get<long>());

  // Check some of the super class fields
  LONGS_EQUAL(
      7, json["originID"][music::EntityId::kEntityNumberJsonKey].get<long>());
  LONGS_EQUAL(8, json["originID"][music::SimulationAddress::kApplicationJsonKey]
                     .get<long>());
  LONGS_EQUAL(
      9, json["originID"][music::SimulationAddress::kSiteJsonKey].get<long>());
  LONGS_EQUAL(
      10,
      json["receiverID"][music::EntityId::kEntityNumberJsonKey].get<long>());
  LONGS_EQUAL(11,
              json["receiverID"][music::SimulationAddress::kApplicationJsonKey]
                  .get<long>());
  LONGS_EQUAL(
      12,
      json["receiverID"][music::SimulationAddress::kSiteJsonKey].get<long>());
  LONGS_EQUAL(99999, json["requestID"].get<long>());
}

TEST(DisplayMessagesMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454004005,
    "header": {
        "exerciseID": 20,
        "timestamp": 1765907966262
    },
    "messages": [
        "Line 1",
        "Line 2",
        "Line 3"
    ],
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
    "requestID": 99999,
    "timeout": 5
}
)");

  DisplayMessagesMessage message = DisplayMessagesMessage::FromJson(json);

  LONGS_EQUAL(5, message.GetTimeout());
  LONGS_EQUAL(3, message.GetMessages().size());
  STRCMP_EQUAL("Line 1", message.GetMessages()[0].c_str());
  STRCMP_EQUAL("Line 2", message.GetMessages()[1].c_str());
  STRCMP_EQUAL("Line 3", message.GetMessages()[2].c_str());

  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(9, message.GetOriginId().GetSite());

  LONGS_EQUAL(10, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(11, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(12, message.GetReceiverId().GetSite());

  LONGS_EQUAL(99999, message.GetRequestId());

  LONGS_EQUAL(20, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765907966262L, message.GetHeader().GetTimestamp());
}

}  // namespace music::test
