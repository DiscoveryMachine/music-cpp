// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/command/scenario/scenario_start_request_message.h"
#include "message/entity_id.h"
#include "message/music_message_header.h"

namespace music::test {

TEST_GROUP(ScenarioStartRequestMessageTest){
    // SETUP
};

TEST(ScenarioStartRequestMessageTest, DefaultConstructor) {
  ScenarioStartRequestMessage message;
  LONGS_EQUAL(454013008, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kScenarioStart),
              static_cast<int>(message.GetType()));
  // No new members
}

TEST(ScenarioStartRequestMessageTest, Builder) {
  auto message =
      ScenarioStartRequestMessageBuilder()
          .SetHeader(
              MusicMessageHeaderBuilder().SetExerciseId(2).SetTimestamp(1765571723044).Build())
          .SetOriginId(EntityIdBuilder().SetEntityNumber(0).SetApplication(300).SetSite(20).Build())
          .SetReceiverId(
              EntityIdBuilder().SetEntityNumber(0).SetApplication(200).SetSite(20).Build())
          .SetRequestId(10)
          .Build();

  LONGS_EQUAL(454013008, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kScenarioStart),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(10, message.GetRequestId());
  LONGS_EQUAL(2, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765571723044, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(300, message.GetOriginId().GetApplication());
  LONGS_EQUAL(20, message.GetOriginId().GetSite());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(200, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(20, message.GetReceiverId().GetSite());
}

TEST(ScenarioStartRequestMessageTest, Validate) {
  auto valid_header =
      MusicMessageHeaderBuilder().SetExerciseId(2).SetTimestamp(1765571723044).Build();
  auto valid_origin_id =
      EntityIdBuilder().SetEntityNumber(0).SetApplication(300).SetSite(20).Build();
  auto valid_receiver_id =
      EntityIdBuilder().SetEntityNumber(0).SetApplication(200).SetSite(20).Build();

  auto message = ScenarioStartRequestMessageBuilder()
                     .SetHeader(valid_header)
                     .SetOriginId(valid_origin_id)
                     .SetReceiverId(valid_receiver_id)
                     .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation threw an exception: ") + e.what()).c_str());
  }

  // Set invalid header
  message.SetHeader(MusicMessageHeader());
  CHECK_THROWS(std::exception, message.Validate());

  message.SetHeader(valid_header);

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation threw an exception: ") + e.what()).c_str());
  }

  // Set invalid origin id
  message.SetOriginId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());

  message.SetOriginId(valid_origin_id);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set invalid receiver id
  message.SetReceiverId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());

  message.SetReceiverId(valid_receiver_id);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set invalid request ID
  message.SetRequestId(-1000);
  CHECK_THROWS(std::exception, message.Validate());

  message.SetRequestId(322);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }
}

TEST(ScenarioStartRequestMessageTest, ToJson) {
  auto message =
      ScenarioStartRequestMessageBuilder()
          .SetHeader(
              MusicMessageHeaderBuilder().SetExerciseId(2).SetTimestamp(1765571723044).Build())
          .SetOriginId(EntityIdBuilder().SetEntityNumber(0).SetApplication(300).SetSite(20).Build())
          .SetReceiverId(
              EntityIdBuilder().SetEntityNumber(0).SetApplication(200).SetSite(20).Build())
          .Build();

  nlohmann::json json = message.ToJson();

  LONGS_EQUAL(2, json["header"]["exerciseID"].get<int>());
  LONGS_EQUAL(1765571723044, json["header"]["timestamp"].get<long long>());
  LONGS_EQUAL(0, json["originID"]["entityID"].get<int>());
  LONGS_EQUAL(300, json["originID"]["appID"].get<int>());
  LONGS_EQUAL(20, json["originID"]["siteID"].get<int>());
  LONGS_EQUAL(0, json["receiverID"]["entityID"].get<int>());
  LONGS_EQUAL(200, json["receiverID"]["appID"].get<int>());
  LONGS_EQUAL(20, json["receiverID"]["siteID"].get<int>());
  LONGS_EQUAL(454013008, json["commandIdentifier"].get<int>());
}

TEST(ScenarioStartRequestMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013008,
    "header": {
        "exerciseID": 2,
        "timestamp": 1765571723044
    },
    "originID": {
        "appID": 300,
        "entityID": 0,
        "siteID": 20
    },
    "receiverID": {
        "appID": 200,
        "entityID": 0,
        "siteID": 20
    }
}

)");
}

} // namespace music::test