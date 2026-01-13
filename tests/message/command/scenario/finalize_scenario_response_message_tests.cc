// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/command/scenario/finalize_scenario_response_message.h"

namespace music::test {

TEST_GROUP(FinalizeScenarioResponseMessageTest){
    // SETUP
};

TEST(FinalizeScenarioResponseMessageTest, DefaultConstructor) {
  FinalizeScenarioResponseMessage message;
  LONGS_EQUAL(454013007, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kFinalizeScenarioResponse),
              static_cast<int>(message.GetType()));
  // No new members
}

TEST(FinalizeScenarioResponseMessageTest, Builder) {
  auto message = FinalizeScenarioResponseMessageBuilder()
                     .SetHeader((MusicMessageHeaderBuilder()
                                     .SetExerciseId(101)
                                     .SetTimestamp(1765561504625)
                                     .Build()))
                     .SetOriginId(EntityIdBuilder()
                                      .SetApplication(100)
                                      .SetSite(300)
                                      .SetEntityNumber(50)
                                      .Build())
                     .SetReceiverId(EntityIdBuilder()
                                        .SetApplication(100)
                                        .SetSite(300)
                                        .SetEntityNumber(52)
                                        .Build())
                     .SetRequestId(502)
                     .SetRequestStatus(RequestStatus::kPartiallyComplete)
                     .Build();

  LONGS_EQUAL(454013007, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kFinalizeScenarioResponse),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(static_cast<int>(RequestStatus::kPartiallyComplete),
              static_cast<int>(message.GetRequestStatus()));
  LONGS_EQUAL(502, message.GetRequestId());
  LONGS_EQUAL(101, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765561504625, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(100, message.GetOriginId().GetApplication());
  LONGS_EQUAL(300, message.GetOriginId().GetSite());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(100, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(300, message.GetReceiverId().GetSite());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
}

TEST(FinalizeScenarioResponseMessageTest, Validate) {
  auto valid_header = MusicMessageHeaderBuilder()
                          .SetExerciseId(101)
                          .SetTimestamp(1765561504625)
                          .Build();
  auto valid_origin_id = EntityIdBuilder()
                             .SetApplication(100)
                             .SetSite(300)
                             .SetEntityNumber(0)
                             .Build();
  auto valid_receiver_id = EntityIdBuilder()
                               .SetApplication(100)
                               .SetSite(300)
                               .SetEntityNumber(0)
                               .Build();
  auto message = FinalizeScenarioResponseMessageBuilder()
                     .SetHeader(valid_header)
                     .SetOriginId(valid_origin_id)
                     .SetReceiverId(valid_receiver_id)
                     .SetRequestId(502)
                     .SetRequestStatus(RequestStatus::kPartiallyComplete)
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

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set origin ID to be invalid
  message.SetOriginId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());

  message.SetOriginId(valid_origin_id);

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set receiver ID to be invalid
  message.SetReceiverId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());

  message.SetReceiverId(valid_receiver_id);

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set Request ID to be invalid
  message.SetRequestId(std::numeric_limits<int>::min());
  CHECK_THROWS(std::exception, message.Validate());

  message.SetRequestId(502);

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }
}
TEST(FinalizeScenarioResponseMessageTest, ToJson) {
  auto message = FinalizeScenarioResponseMessageBuilder()
                     .SetHeader(MusicMessageHeaderBuilder()
                                    .SetExerciseId(2)
                                    .SetTimestamp(1605645402861L)
                                    .Build())
                     .SetOriginId(EntityIdBuilder()
                                      .SetApplication(30)
                                      .SetSite(300)
                                      .SetEntityNumber(0)
                                      .Build())
                     .SetReceiverId(EntityIdBuilder()
                                        .SetApplication(50)
                                        .SetSite(300)
                                        .SetEntityNumber(0)
                                        .Build())
                     .SetRequestId(322)
                     .SetRequestStatus(RequestStatus::kExecuting)
                     .Build();

  nlohmann::json json = message.ToJson();

  LONGS_EQUAL(2, json["header"]["exerciseID"]);
  LONGS_EQUAL(1605645402861L, json["header"]["timestamp"]);
  LONGS_EQUAL(454013007L, json["commandIdentifier"]);
  LONGS_EQUAL(30, json["originID"]["appID"]);
  LONGS_EQUAL(300, json["originID"]["siteID"]);
  LONGS_EQUAL(0, json["originID"]["entityID"]);
  LONGS_EQUAL(50, json["receiverID"]["appID"]);
  LONGS_EQUAL(300, json["receiverID"]["siteID"]);
  LONGS_EQUAL(0, json["receiverID"]["entityID"]);
  LONGS_EQUAL(322, json["requestID"]);
  LONGS_EQUAL(static_cast<int>(RequestStatus::kExecuting), json["status"]);

  nlohmann::json expected_json = nlohmann::json::parse(R"({
  "header": {
    "exerciseID": 2,
    "timestamp": 1605645402861
  },
  "commandIdentifier": 454013007,
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
  "requestID": 322,
  "status": 2
}

)");

  CHECK_TRUE(json == expected_json);
}

TEST(FinalizeScenarioResponseMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"({
  "header": {
    "exerciseID": 2,
    "timestamp": 1605645402861
  },
  "commandIdentifier": 454013007,
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
  "requestID": 322,
  "status": 2
}

)");

  auto message = FinalizeScenarioResponseMessage::FromJson(json);
  LONGS_EQUAL(2, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1605645402861, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(454013007, message.GetCommandIdentifier());
  LONGS_EQUAL(30, message.GetOriginId().GetApplication());
  LONGS_EQUAL(300, message.GetOriginId().GetSite());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(50, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(300, message.GetReceiverId().GetSite());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(322, message.GetRequestId());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kExecuting),
              static_cast<int>(message.GetRequestStatus()));
}

}  // namespace music::test
