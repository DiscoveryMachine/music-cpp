// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/command/scenario/parameterize_construct_response_message.h"
#include "message/entity_id.h"

namespace music::test {

TEST_GROUP(ParameterizeConstructResponseMessageTest){
    // SETUP
};

TEST(ParameterizeConstructResponseMessageTest, DefaultConstructor) {
  ParameterizeConstructResponseMessage message;
  LONGS_EQUAL(454013005, message.GetCommandIdentifier());
  LONGS_EQUAL(
      static_cast<int>(MusicMessageType::kParameterizeConstructResponse),
      static_cast<int>(message.GetType()));
  // No new members
}

// No new members to test setters/getters

TEST(ParameterizeConstructResponseMessageTest, Builder) {
  auto message = ParameterizeConstructResponseMessageBuilder()
                     .SetHeader(MusicMessageHeaderBuilder()
                                    .SetExerciseId(2)
                                    .SetTimestamp(1765568811230)
                                    .Build())
                     .SetOriginId(EntityIdBuilder()
                                      .SetEntityNumber(1)
                                      .SetApplication(30)
                                      .SetSite(300)
                                      .Build())
                     .SetReceiverId(EntityIdBuilder()
                                        .SetEntityNumber(0)
                                        .SetApplication(50)
                                        .SetSite(300)
                                        .Build())
                     .SetRequestId(322)
                     .SetRequestStatus(RequestStatus::kExecuting)
                     .Build();

  LONGS_EQUAL(454013005, message.GetCommandIdentifier());
  LONGS_EQUAL(
      static_cast<int>(MusicMessageType::kParameterizeConstructResponse),
      static_cast<int>(message.GetType()));
  LONGS_EQUAL(2, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765568811230, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(1, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(30, message.GetOriginId().GetApplication());
  LONGS_EQUAL(300, message.GetOriginId().GetSite());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(50, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(300, message.GetReceiverId().GetSite());
  LONGS_EQUAL(322, message.GetRequestId());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kExecuting),
              static_cast<int>(message.GetRequestStatus()));
}

TEST(ParameterizeConstructResponseMessageTest, Validate) {
  auto valid_header = MusicMessageHeaderBuilder()
                          .SetExerciseId(2)
                          .SetTimestamp(1765568811230)
                          .Build();
  auto valid_origin_id = EntityIdBuilder()
                             .SetEntityNumber(1)
                             .SetApplication(30)
                             .SetSite(300)
                             .Build();
  auto valid_receiver_id = EntityIdBuilder()
                               .SetEntityNumber(0)
                               .SetApplication(50)
                               .SetSite(300)
                               .Build();

  auto message = ParameterizeConstructResponseMessageBuilder()
                     .SetHeader(valid_header)
                     .SetOriginId(valid_origin_id)
                     .SetReceiverId(valid_receiver_id)
                     .SetRequestId(322)
                     .SetRequestStatus(RequestStatus::kExecuting)
                     .Build();
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set invalid header
  message.SetHeader(MusicMessageHeader());
  CHECK_THROWS(std::exception, message.Validate());

  message.SetHeader(valid_header);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
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

  // Set invalid message status
  message.SetRequestStatus(static_cast<RequestStatus>(-1));
  CHECK_THROWS(std::exception, message.Validate());
  message.SetRequestStatus(static_cast<RequestStatus>(2));
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }
}

TEST(ParameterizeConstructResponseMessageTest, ToJson) {
  auto message = ParameterizeConstructResponseMessageBuilder()
                     .SetHeader(MusicMessageHeaderBuilder()
                                    .SetExerciseId(2)
                                    .SetTimestamp(1765568811230)
                                    .Build())
                     .SetOriginId(EntityIdBuilder()
                                      .SetEntityNumber(1)
                                      .SetApplication(30)
                                      .SetSite(300)
                                      .Build())
                     .SetReceiverId(EntityIdBuilder()
                                        .SetEntityNumber(0)
                                        .SetApplication(50)
                                        .SetSite(300)
                                        .Build())
                     .SetRequestId(322)
                     .SetRequestStatus(RequestStatus::kExecuting)
                     .Build();

  nlohmann::json json = message.ToJson();

  LONGS_EQUAL(454013005, json["commandIdentifier"].get<long>());
  LONGS_EQUAL(2, json["header"]["exerciseID"].get<long>());
  LONGS_EQUAL(1765568811230, json["header"]["timestamp"].get<long>());
  LONGS_EQUAL(1, json["originID"]["entityID"].get<long>());
  LONGS_EQUAL(30, json["originID"]["appID"].get<long>());
  LONGS_EQUAL(300, json["originID"]["siteID"].get<long>());
  LONGS_EQUAL(50, json["receiverID"]["appID"].get<long>());
  LONGS_EQUAL(0, json["receiverID"]["entityID"].get<long>());
  LONGS_EQUAL(300, json["receiverID"]["siteID"].get<long>());
  LONGS_EQUAL(322, json["requestID"].get<long>());
  LONGS_EQUAL(2, json["status"].get<long>());
}

TEST(ParameterizeConstructResponseMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013005,
    "header": {
        "exerciseID": 2,
        "timestamp": 1765568811230
    },
    "originID": {
        "appID": 30,
        "entityID": 1,
        "siteID": 300
    },
    "receiverID": {
        "appID": 50,
        "entityID": 0,
        "siteID": 300
    },
    "requestID": 322,
    "status": 2
}
)");
}

}  // namespace music::test