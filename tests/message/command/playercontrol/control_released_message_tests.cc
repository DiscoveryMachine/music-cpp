// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#include <CppUTest/TestHarness.h>

#include "message/command/playercontrol/control_released_message.h"

namespace music::test {
TEST_GROUP(ControlReleasedMessageTest){
    // SETUP
};

TEST(ControlReleasedMessageTest, DefaultConstructor) {
  ControlReleasedMessage message;
  LONGS_EQUAL(MusicMessageType::kControlReleased,
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(
      MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kControlReleased)
          .action_type,
      message.GetCommandIdentifier());
}

TEST(ControlReleasedMessageTest, SettersAndGetters) {
  ControlReleasedMessage message;

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

TEST(ControlReleasedMessageTest, Builder) {
  ControlReleasedMessage message = ControlReleasedMessageBuilder()
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
                                       .SetRequestId(67890)
                                       .Build();

  LONGS_EQUAL(9, message.GetOriginId().GetSite());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(12, message.GetReceiverId().GetSite());
  LONGS_EQUAL(11, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(10, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(67890, message.GetRequestId());
}

TEST(ControlReleasedMessageTest, Validate) {
  EntityId valid_origin_id =
      EntityIdBuilder().SetEntityNumber(7).SetApplication(8).SetSite(9).Build();
  EntityId valid_receiver_id = EntityIdBuilder()
                                   .SetEntityNumber(10)
                                   .SetApplication(11)
                                   .SetSite(12)
                                   .Build();
  MusicMessageHeader valid_header = MusicMessageHeaderBuilder()
                                        .SetExerciseId(20)
                                        .SetTimestamp(1765894109000)
                                        .Build();

  ControlReleasedMessage message = ControlReleasedMessageBuilder()
                                       .SetHeader(valid_header)
                                       .SetOriginId(valid_origin_id)
                                       .SetReceiverId(valid_receiver_id)
                                       .SetRequestId(99999)
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

TEST(ControlReleasedMessageTest, ToJson) {
  ControlReleasedMessage message =
      ControlReleasedMessageBuilder()
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
          .SetRequestId(67890)
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(20)
                         .SetTimestamp(1765910664820)
                         .Build())
          .Build();

  nlohmann::json json = message.ToJson();

  // Check header
  LONGS_EQUAL(20, json["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(1765910664820, json["header"]["timestamp"].get<long>());

  // Check origin ID
  LONGS_EQUAL(9, json["originID"]["siteID"].get<long>());
  LONGS_EQUAL(8, json["originID"]["appID"].get<long>());
  LONGS_EQUAL(7, json["originID"]["entityID"].get<long>());
}

TEST(ControlReleasedMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454999002,
    "header": {
        "exerciseID": 20,
        "timestamp": 1765910664820
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
    "requestID": 67890
}
)");

  ControlReleasedMessage message = ControlReleasedMessage::FromJson(json);

  // Check command identifier
  LONGS_EQUAL(
      MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kControlReleased)
          .action_type,
      message.GetCommandIdentifier());

  // Check header
  LONGS_EQUAL(20, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765910664820, message.GetHeader().GetTimestamp());
  // Check origin ID
  LONGS_EQUAL(9, message.GetOriginId().GetSite());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  // Check receiver ID
  LONGS_EQUAL(12, message.GetReceiverId().GetSite());
  LONGS_EQUAL(11, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(10, message.GetReceiverId().GetEntityNumber());
  // Check request ID
  LONGS_EQUAL(67890, message.GetRequestId());
}

}  // namespace music::test