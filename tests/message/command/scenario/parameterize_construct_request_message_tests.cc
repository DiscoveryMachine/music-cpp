// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/command/scenario/parameterize_construct_request_message.h"

namespace music::test {

TEST_GROUP(ParameterizeConstructRequestMessageTest){
    // SETUP
};

TEST(ParameterizeConstructRequestMessageTest, DefaultConstructor) {
  ParameterizeConstructRequestMessage message;
  LONGS_EQUAL(454013004, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kParameterizeConstructRequest),
              static_cast<int>(message.GetType()));
  // No new members
}

TEST(ParameterizeConstructRequestMessageTest, SettersAndGetters) {
  ParameterizeConstructRequestMessage message;
  nlohmann::json construct_parameters = {{"key", "value"}};
  EntityId ghosted_id;
  ghosted_id.SetEntityNumber(100);
  ghosted_id.SetSite(200);
  ghosted_id.SetApplication(300);
  message.SetConstructParameters(construct_parameters);
  message.SetGhostedId(ghosted_id);
  STRCMP_EQUAL(
      "value",
      message.GetConstructParameters()["key"].get<std::string>().c_str());
  LONGS_EQUAL(100, message.GetGhostedId().value().GetEntityNumber());
  LONGS_EQUAL(200, message.GetGhostedId().value().GetSite());
  LONGS_EQUAL(300, message.GetGhostedId().value().GetApplication());
}

TEST(ParameterizeConstructRequestMessageTest, Builder) {
  auto message = ParameterizeConstructRequestMessageBuilder().Build();
  LONGS_EQUAL(454013004, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kParameterizeConstructRequest),
              static_cast<int>(message.GetType()));
}

TEST(ParameterizeConstructRequestMessageTest, Validate) {
  nlohmann::json valid_construct_parameters = {{"testKey", "testValue"}};
  auto valid_ghosted_id = EntityIdBuilder()
                              .SetEntityNumber(0)
                              .SetApplication(30)
                              .SetSite(300)
                              .Build();
  auto valid_header = MusicMessageHeaderBuilder()
                          .SetExerciseId(2)
                          .SetTimestamp(1605645402861)
                          .Build();
  auto valid_origin_id = EntityIdBuilder()
                             .SetEntityNumber(0)
                             .SetApplication(30)
                             .SetSite(300)
                             .Build();
  auto valid_receiver_id = EntityIdBuilder()
                               .SetEntityNumber(1)
                               .SetApplication(30)
                               .SetSite(300)
                               .Build();
  auto message = ParameterizeConstructRequestMessageBuilder()
                     .SetConstructParameters(valid_construct_parameters)
                     .SetGhostedId(valid_ghosted_id)
                     .SetHeader(valid_header)
                     .SetOriginId(valid_origin_id)
                     .SetReceiverId(valid_receiver_id)
                     .SetRequestId(322)
                     .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set construct parameters to be invalid
  message.SetConstructParameters({});
  CHECK_THROWS(std::exception, message.Validate());
  message.SetConstructParameters(valid_construct_parameters);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set Ghosted ID to be invalid
  message.SetGhostedId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetGhostedId(valid_ghosted_id);
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

  // Set invalid origin ID
  message.SetOriginId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetOriginId(valid_origin_id);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set invalid receiver ID
  message.SetReceiverId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetReceiverId(valid_receiver_id);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }

  // Set invalid request ID
  message.SetRequestId(-1);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetRequestId(322);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(e.what());
  }
}

TEST(ParameterizeConstructRequestMessageTest, ToJson) {
  auto message = ParameterizeConstructRequestMessageBuilder()
                     .SetConstructParameters({{"testKey", "testValue"}})
                     .SetGhostedId(EntityIdBuilder()
                                       .SetEntityNumber(0)
                                       .SetApplication(30)
                                       .SetSite(300)
                                       .Build())
                     .SetHeader(MusicMessageHeaderBuilder()
                                    .SetExerciseId(2)
                                    .SetTimestamp(1605645402861)
                                    .Build())
                     .SetOriginId(EntityIdBuilder()
                                      .SetEntityNumber(0)
                                      .SetApplication(30)
                                      .SetSite(300)
                                      .Build())
                     .SetReceiverId(EntityIdBuilder()
                                        .SetEntityNumber(1)
                                        .SetApplication(30)
                                        .SetSite(300)
                                        .Build())
                     .SetRequestId(322)
                     .Build();

  nlohmann::json json = message.ToJson();
  LONGS_EQUAL(454013004, json["commandIdentifier"].get<long long>());
  STRCMP_EQUAL(
      "testValue",
      json["constructParameters"]["testKey"].get<std::string>().c_str());
  LONGS_EQUAL(0, json["ghostedID"]["entityID"].get<long long>());
  LONGS_EQUAL(30, json["ghostedID"]["appID"].get<long long>());
  LONGS_EQUAL(300, json["ghostedID"]["siteID"].get<long long>());
  LONGS_EQUAL(2, json["header"]["exerciseID"].get<long long>());
  LONGS_EQUAL(1605645402861, json["header"]["timestamp"].get<long long>());
  LONGS_EQUAL(0, json["originID"]["entityID"].get<long long>());
  LONGS_EQUAL(30, json["originID"]["appID"].get<long long>());
  LONGS_EQUAL(300, json["originID"]["siteID"].get<long long>());
  LONGS_EQUAL(1, json["receiverID"]["entityID"].get<long long>());
  LONGS_EQUAL(30, json["receiverID"]["appID"].get<long long>());
  LONGS_EQUAL(300, json["receiverID"]["siteID"].get<long long>());
  LONGS_EQUAL(322, json["requestID"].get<long long>());
}

TEST(ParameterizeConstructRequestMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "commandIdentifier": 454013004,
    "constructParameters": {
        "testKey": "testValue"
    },
    "ghostedID": {
        "appID": 30,
        "entityID": 0,
        "siteID": 300
    },
    "header": {
        "exerciseID": 2,
        "timestamp": 1605645402861
    },
    "originID": {
        "appID": 30,
        "entityID": 0,
        "siteID": 300
    },
    "receiverID": {
        "appID": 30,
        "entityID": 1,
        "siteID": 300
    },
    "requestID": 322
}
)");

  auto message = ParameterizeConstructRequestMessage::FromJson(json);
  LONGS_EQUAL(454013004, message.GetCommandIdentifier());
  STRCMP_EQUAL(
      "testValue",
      std::string(message.GetConstructParameters().at("testKey")).c_str());
  LONGS_EQUAL(0, message.GetGhostedId().value().GetEntityNumber());
  LONGS_EQUAL(30, message.GetGhostedId().value().GetApplication());
  LONGS_EQUAL(300, message.GetGhostedId().value().GetSite());
  LONGS_EQUAL(2, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1605645402861, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(30, message.GetOriginId().GetApplication());
  LONGS_EQUAL(300, message.GetOriginId().GetSite());
  LONGS_EQUAL(1, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(30, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(300, message.GetReceiverId().GetSite());
  LONGS_EQUAL(322, message.GetRequestId());
}

}  // namespace music::test
