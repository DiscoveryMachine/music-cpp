// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <exception>

#include "message/command/scenario/finalize_scenario_request_message.h"

namespace music::test {

TEST_GROUP(FinalizeScenarioRequestMessageTest){
    // SETUP
};

TEST(FinalizeScenarioRequestMessageTest, DefaultConstructor) {
  FinalizeScenarioRequestMessage message;
  LONGS_EQUAL(454013006, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kFinalizeScenarioRequest),
              static_cast<int>(message.GetType()));
  // No new members
}

TEST(FinalizeScenarioRequestMessageTest, Builder) {
  auto message = FinalizeScenarioRequestMessageBuilder()
                     .SetHeader(MusicMessageHeaderBuilder()
                                    .SetExerciseId(128)
                                    .SetTimestamp(123456789)
                                    .Build())
                     .SetRequestId(500)
                     .SetOriginId(EntityIdBuilder()
                                      .SetApplication(200)
                                      .SetSite(100)
                                      .SetEntityNumber(400)
                                      .Build())
                     .SetReceiverId(EntityIdBuilder()
                                        .SetApplication(200)
                                        .SetSite(100)
                                        .SetEntityNumber(101)
                                        .Build())
                     .Build();

  LONGS_EQUAL(454013006, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kFinalizeScenarioRequest),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(128, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(123456789, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(500, message.GetRequestId());
  LONGS_EQUAL(200, message.GetOriginId().GetApplication());
  LONGS_EQUAL(100, message.GetOriginId().GetSite());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(200, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(100, message.GetReceiverId().GetSite());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
}

TEST(FinalizeScenarioRequestMessageTest, Validate) {
  auto valid_header = MusicMessageHeaderBuilder()
                          .SetExerciseId(128)
                          .SetTimestamp(123456789)
                          .Build();
  auto valid_origin_id = EntityIdBuilder()
                             .SetApplication(200)
                             .SetSite(100)
                             .SetEntityNumber(0)
                             .Build();
  auto valid_receiver_id = EntityIdBuilder()
                               .SetApplication(200)
                               .SetSite(100)
                               .SetEntityNumber(0)
                               .Build();

  auto message = FinalizeScenarioRequestMessageBuilder()
                     .SetHeader(valid_header)
                     .SetRequestId(500)
                     .SetOriginId(valid_origin_id)
                     .SetReceiverId(valid_receiver_id)
                     .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set header to be invalid
  message.SetHeader(MusicMessageHeader());

  CHECK_THROWS(std::exception, message.Validate());

  message.SetHeader(valid_header);

  // Set origin id to be invalid
  message.SetOriginId(EntityId());

  CHECK_THROWS(std::exception, message.Validate());

  message.SetOriginId(valid_origin_id);

  // Set receiver id to be invalid
  message.SetReceiverId(EntityId());

  CHECK_THROWS(std::exception, message.Validate());

  message.SetReceiverId(valid_receiver_id);

  // Set request id to be invalid
  message.SetRequestId(-5000);
  CHECK_THROWS(std::exception, message.Validate());

  message.SetRequestId(400);

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }
}

TEST(FinalizeScenarioRequestMessageTest, ToJson) {
  auto message = FinalizeScenarioRequestMessageBuilder()
                     .SetHeader(MusicMessageHeaderBuilder()
                                    .SetExerciseId(128)
                                    .SetTimestamp(123456789)
                                    .Build())
                     .SetRequestId(500)
                     .SetOriginId(EntityIdBuilder()
                                      .SetApplication(200)
                                      .SetSite(100)
                                      .SetEntityNumber(400)
                                      .Build())
                     .SetReceiverId(EntityIdBuilder()
                                        .SetApplication(200)
                                        .SetSite(100)
                                        .SetEntityNumber(101)
                                        .Build())
                     .Build();
  nlohmann::json json = message.ToJson();

  LONGS_EQUAL(454013006, json["commandIdentifier"].get<int>());
  LONGS_EQUAL(128, json["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(123456789, json["header"]["timestamp"].get<int>());
  LONGS_EQUAL(500, json["requestID"].get<int>());
  LONGS_EQUAL(200, json["originID"]["appID"].get<int>());
  LONGS_EQUAL(100, json["originID"]["siteID"].get<int>());
  LONGS_EQUAL(0, json["originID"]["entityID"].get<int>());
  LONGS_EQUAL(200, json["receiverID"]["appID"].get<int>());
  LONGS_EQUAL(100, json["receiverID"]["siteID"].get<int>());
  LONGS_EQUAL(0, json["receiverID"]["entityID"].get<int>());
}

TEST(FinalizeScenarioRequestMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
  "header": {
    "exerciseID": 2,
    "timestamp": 1765559767625
  },
  "commandIdentifier": 454013006,
  "originID": {
    "siteID": 300,
    "appID": 30,
    "entityID": 0
  },
  "receiverID": {
    "siteID": 300,
    "appID": 50,
    "entityID": 0
  },
  "requestID": 364
}
)");

  auto message = FinalizeScenarioRequestMessage::FromJson(json);

  LONGS_EQUAL(454013006, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kFinalizeScenarioRequest),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(2, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765559767625, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(364, message.GetRequestId());
  LONGS_EQUAL(30, message.GetOriginId().GetApplication());
  LONGS_EQUAL(300, message.GetOriginId().GetSite());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(50, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(300, message.GetReceiverId().GetSite());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
}

}  // namespace music::test