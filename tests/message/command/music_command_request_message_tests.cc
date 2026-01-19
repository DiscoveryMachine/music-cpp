// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/command/music_command_request_message.h"
#include "message/simulation_address.h"

TEST_GROUP(MusicCommandRequestMessageTest){
    // DEFINE ANY SETUP HERE
};

TEST(MusicCommandRequestMessageTest, DefaultConstructor) {
  music::MusicCommandRequestMessage command_request_message;
  LONGS_EQUAL(std::numeric_limits<long>::min(),
              command_request_message.GetCommandIdentifier());
}

TEST(MusicCommandRequestMessageTest, SettersAndGetters) {
  music::MusicCommandRequestMessage command_request_message;
  command_request_message.SetCommandIdentifier(12345);
  LONGS_EQUAL(12345, command_request_message.GetCommandIdentifier());
}

TEST(MusicCommandRequestMessageTest, Validate) {
  music::MusicCommandRequestMessage command_request_message;
  command_request_message.SetCommandIdentifier(67890);
  command_request_message.SetOriginId(music::EntityIdBuilder()
                                          .SetEntityNumber(1)
                                          .SetApplication(2)
                                          .SetSite(3)
                                          .Build());
  command_request_message.SetReceiverId(music::EntityIdBuilder()
                                            .SetEntityNumber(4)
                                            .SetApplication(5)
                                            .SetSite(6)
                                            .Build());
  command_request_message.SetRequestId(55555);
  command_request_message.SetHeader(music::MusicMessageHeaderBuilder()
                                        .SetExerciseId(10)
                                        .SetTimestamp(1765308607186L)
                                        .Build());
  command_request_message.SetType(music::MusicMessageType::kControlRequest);

  // Should not throw any exceptions
  command_request_message.Validate();

  // Test with invalid command identifier - Validate() will throw
  command_request_message.SetCommandIdentifier(-1);
  CHECK_THROWS(std::exception, command_request_message.Validate());
}

TEST(MusicCommandRequestMessageTest, Builder) {
  music::MusicCommandRequestMessage message =
      music::MusicCommandRequestMessageBuilder()
          .SetCommandIdentifier(24680)
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
                         .SetTimestamp(1765308607186L)
                         .Build())
          .SetType(music::MusicMessageType::kControlRequest)
          .Build();

  LONGS_EQUAL(24680, message.GetCommandIdentifier());
  LONGS_EQUAL(7, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(8, message.GetOriginId().GetApplication());
  LONGS_EQUAL(9, message.GetOriginId().GetSite());
  LONGS_EQUAL(10, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(11, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(12, message.GetReceiverId().GetSite());
  LONGS_EQUAL(99999, message.GetRequestId());
  LONGS_EQUAL(20, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765308607186L, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(static_cast<int>(music::MusicMessageType::kControlRequest),
              static_cast<int>(message.GetType()));
}

TEST(MusicCommandRequestMessageTest, ToJson) {
  music::MusicCommandRequestMessage command_request_message;
  command_request_message.SetCommandIdentifier(24680);
  command_request_message.SetOriginId(music::EntityIdBuilder()
                                          .SetEntityNumber(7)
                                          .SetApplication(8)
                                          .SetSite(9)
                                          .Build());
  command_request_message.SetReceiverId(music::EntityIdBuilder()
                                            .SetEntityNumber(10)
                                            .SetApplication(11)
                                            .SetSite(12)
                                            .Build());
  command_request_message.SetRequestId(99999);
  command_request_message.SetHeader(music::MusicMessageHeaderBuilder()
                                        .SetExerciseId(20)
                                        .SetTimestamp(1765308607186L)
                                        .Build());
  command_request_message.SetType(music::MusicMessageType::kControlRequest);

  nlohmann::json j = command_request_message.ToJson();

  LONGS_EQUAL(24680,
              j[music::MusicCommandRequestMessage::kCommandIdKey].get<long>());
  LONGS_EQUAL(7, j[music::MusicRequestMessage::kOriginIdKey]
                  [music::EntityId::kEntityNumberJsonKey]
                      .get<long>());
  LONGS_EQUAL(8, j[music::MusicRequestMessage::kOriginIdKey]
                  [music::SimulationAddress::kApplicationJsonKey]
                      .get<long>());
  LONGS_EQUAL(9, j[music::MusicRequestMessage::kOriginIdKey]
                  [music::SimulationAddress::kSiteJsonKey]
                      .get<long>());
  LONGS_EQUAL(10, j[music::MusicRequestMessage::kReceiverIdKey]
                   [music::EntityId::kEntityNumberJsonKey]
                       .get<long>());
  LONGS_EQUAL(11, j[music::MusicRequestMessage::kReceiverIdKey]
                   [music::SimulationAddress::kApplicationJsonKey]
                       .get<long>());
  LONGS_EQUAL(12, j[music::MusicRequestMessage::kReceiverIdKey]
                   [music::SimulationAddress::kSiteJsonKey]
                       .get<long>());
  LONGS_EQUAL(99999, j[music::MusicRequestMessage::kRequestIdKey].get<long>());
  LONGS_EQUAL(20, j[music::MusicMessage::kHeaderJsonKey]
                   [music::MusicMessageHeader::kExerciseIdKey]
                       .get<long>());
  LONGS_EQUAL(1765308607186L, j[music::MusicMessage::kHeaderJsonKey]
                               [music::MusicMessageHeader::kTimestampKey]
                                   .get<long>());
}

TEST(MusicCommandRequestMessageTest, FromJson) {
  nlohmann::json j = {
      {music::MusicCommandRequestMessage::kCommandIdKey, 13579},
      {music::MusicRequestMessage::kOriginIdKey,
       {{music::EntityId::kEntityNumberJsonKey, 13},
        {music::SimulationAddress::kApplicationJsonKey, 14},
        {music::SimulationAddress::kSiteJsonKey, 15}}},
      {music::MusicRequestMessage::kReceiverIdKey,
       {{music::EntityId::kEntityNumberJsonKey, 16},
        {music::SimulationAddress::kApplicationJsonKey, 17},
        {music::SimulationAddress::kSiteJsonKey, 18}}},
      {music::MusicRequestMessage::kRequestIdKey, 88888},
      {music::MusicMessage::kHeaderJsonKey,
       {{music::MusicMessageHeader::kExerciseIdKey, 30},
        {music::MusicMessageHeader::kTimestampKey, 1765308607186L}}},
      {music::MusicMessage::kTypeJsonKey,
       static_cast<int>(music::MusicMessageType::kControlRequest)}};

  music::MusicCommandRequestMessage command_request_message =
      music::MusicCommandRequestMessage::FromJson(j);
  LONGS_EQUAL(13579, command_request_message.GetCommandIdentifier());
  LONGS_EQUAL(13, command_request_message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(14, command_request_message.GetOriginId().GetApplication());
  LONGS_EQUAL(15, command_request_message.GetOriginId().GetSite());
  LONGS_EQUAL(16, command_request_message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(17, command_request_message.GetReceiverId().GetApplication());
  LONGS_EQUAL(18, command_request_message.GetReceiverId().GetSite());
  LONGS_EQUAL(88888, command_request_message.GetRequestId());
  LONGS_EQUAL(30, command_request_message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765308607186L,
              command_request_message.GetHeader().GetTimestamp());
}

TEST(MusicCommandRequestMessageTest, ToString) {
  music::MusicCommandRequestMessage command_request_message;
  command_request_message.SetCommandIdentifier(11223);
  command_request_message.SetOriginId(music::EntityIdBuilder()
                                          .SetEntityNumber(19)
                                          .SetApplication(20)
                                          .SetSite(21)
                                          .Build());
  command_request_message.SetReceiverId(music::EntityIdBuilder()
                                            .SetEntityNumber(22)
                                            .SetApplication(23)
                                            .SetSite(24)
                                            .Build());
  command_request_message.SetRequestId(77777);
  command_request_message.SetHeader(music::MusicMessageHeaderBuilder()
                                        .SetExerciseId(40)
                                        .SetTimestamp(1765308607186L)
                                        .Build());
  command_request_message.SetType(music::MusicMessageType::kControlRequest);

  // JSON string will have each key in alphabetical order.
  std::string expected_substring =
      "MusicCommandRequestMessage: "
      "{\"commandIdentifier\":11223,\"header\":{\"exerciseID\":40,"
      "\"timestamp\":"
      "1765308607186},\"originID\":{\"appID\":20,\"entityID\":19,\"siteID\":21}"
      ",\"receiverID\":{\"appID\":23,\"entityID\":22,\"siteID\":24},"
      "\"requestID\":77777}";

  std::string to_string_output = command_request_message.ToString();
  STRCMP_CONTAINS(expected_substring.c_str(), to_string_output.c_str());
}