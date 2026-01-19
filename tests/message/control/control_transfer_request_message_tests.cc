// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>
#include <Utest.h>

#include "message/control/control_transfer_request_message.h"

namespace music::test {

TEST_GROUP(ControlTransferRequestMessageTest){
    // SETUP
};

TEST(ControlTransferRequestMessageTest, Validate) {
  auto valid_header =
      MusicMessageHeaderBuilder().SetExerciseId(1).SetTimestamp(1000L).Build();

  auto valid_origin_id =
      EntityIdBuilder().SetSite(1).SetApplication(2).SetEntityNumber(3).Build();

  auto valid_receiver_id =
      EntityIdBuilder().SetSite(4).SetApplication(5).SetEntityNumber(6).Build();

  auto valid_proposed_controller =
      EntityIdBuilder().SetSite(7).SetApplication(8).SetEntityNumber(9).Build();

  auto valid_target_construct = EntityIdBuilder()
                                    .SetSite(10)
                                    .SetApplication(11)
                                    .SetEntityNumber(12)
                                    .Build();

  auto valid_context = nlohmann::json::parse("{\"testKey\":\"Test Value\"}");

  ControlTransferRequestMessage message =
      ControlTransferRequestMessageBuilder()
          .SetHeader(valid_header)
          .SetOriginId(valid_origin_id)
          .SetReceiverId(valid_receiver_id)
          .SetRequestId(10)
          .SetProposedController(valid_proposed_controller)
          .SetTargetConstruct(valid_target_construct)
          .SetContext(valid_context)
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

  // Set invalid proposed controller
  message.SetProposedController(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetProposedController(valid_proposed_controller);
}

TEST(ControlTransferRequestMessageTest, ToJson) {
  ControlTransferRequestMessage message =
      ControlTransferRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(1)
                         .SetTimestamp(1000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(1)
                           .SetApplication(2)
                           .SetEntityNumber(3)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(4)
                             .SetApplication(5)
                             .SetEntityNumber(6)
                             .Build())
          .SetRequestId(10)
          .SetProposedController(EntityIdBuilder()
                                     .SetSite(7)
                                     .SetApplication(8)
                                     .SetEntityNumber(9)
                                     .Build())
          .SetTargetConstruct(EntityIdBuilder()
                                  .SetSite(10)
                                  .SetApplication(11)
                                  .SetEntityNumber(12)
                                  .Build())
          .SetContext(nlohmann::json::parse("{\"testKey\":\"Test Value\"}"))
          .Build();
  nlohmann::json json = message.ToJson();

  // Check header
  LONGS_EQUAL(1, json["header"]["exerciseID"].get<long>());
  LONGS_EQUAL(1000L, json["header"]["timestamp"].get<long>());
  // Check origin ID
  LONGS_EQUAL(1, json["originID"]["siteID"].get<int>());
  LONGS_EQUAL(2, json["originID"]["appID"].get<int>());
  LONGS_EQUAL(3, json["originID"]["entityID"].get<int>());
  // Check receiver ID
  LONGS_EQUAL(4, json["receiverID"]["siteID"].get<int>());
  LONGS_EQUAL(5, json["receiverID"]["appID"].get<int>());
  LONGS_EQUAL(6, json["receiverID"]["entityID"].get<int>());
  // Check request ID
  LONGS_EQUAL(10, json["requestID"].get<long>());
  // Check proposed controller
  LONGS_EQUAL(7, json["proposedController"]["siteID"].get<int>());
  LONGS_EQUAL(8, json["proposedController"]["appID"].get<int>());
  LONGS_EQUAL(9, json["proposedController"]["entityID"].get<int>());
  // Check target construct
  LONGS_EQUAL(10, json["targetConstruct"]["siteID"].get<int>());
  LONGS_EQUAL(11, json["targetConstruct"]["appID"].get<int>());
  LONGS_EQUAL(12, json["targetConstruct"]["entityID"].get<int>());
  // Check context
  STRCMP_EQUAL("{\"testKey\":\"Test Value\"}", json["context"].dump().c_str());
}

TEST(ControlTransferRequestMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
    {
      "context": {"testKey":"Test Value"},
      "header": {
        "exerciseID": 1,
        "timestamp": 1000
      },
      "originID": {
        "siteID": 1,
        "appID": 2,
        "entityID": 3
      },
      "proposedController": {
        "siteID": 7,
        "appID": 8,
        "entityID": 9
      },
      "receiverID": {
        "siteID": 4,
        "appID": 5,
        "entityID": 6
      },
      "requestID": 10,
      "targetConstruct": {
        "siteID": 10,
        "appID": 11,
        "entityID": 12
      }
    }
  )");

  ControlTransferRequestMessage message =
      ControlTransferRequestMessage::FromJson(json);

  // Check header
  LONGS_EQUAL(1, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1000L, message.GetHeader().GetTimestamp());
  // Check origin ID
  LONGS_EQUAL(1, message.GetOriginId().GetSite());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, message.GetOriginId().GetEntityNumber());
  // Check receiver ID
  LONGS_EQUAL(4, message.GetReceiverId().GetSite());
  LONGS_EQUAL(5, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(6, message.GetReceiverId().GetEntityNumber());
  // Check request ID
  LONGS_EQUAL(10, message.GetRequestId());
  // Check proposed controller
  LONGS_EQUAL(7, message.GetProposedController().GetSite());
  LONGS_EQUAL(8, message.GetProposedController().GetApplication());
  LONGS_EQUAL(9, message.GetProposedController().GetEntityNumber());
  // Check target construct
  LONGS_EQUAL(10, message.GetTargetConstruct().GetSite());
  LONGS_EQUAL(11, message.GetTargetConstruct().GetApplication());
  LONGS_EQUAL(12, message.GetTargetConstruct().GetEntityNumber());
  // Check context
  STRCMP_EQUAL("{\"testKey\":\"Test Value\"}",
               message.GetContext().dump().c_str());
}
}  // namespace music::test