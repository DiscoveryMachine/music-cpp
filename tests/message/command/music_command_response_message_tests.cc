// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <limits>

#include "message/command/music_command_response_message.h"
#include "message/music_message_header.h"
#include "message/music_response_message.h"
namespace music::test {
TEST_GROUP(MusicCommandResponseMessageTest){
    // DEFINE ANY SETUP HERE
};

TEST(MusicCommandResponseMessageTest, DefaultConstructor) {
  MusicCommandResponseMessage command_response_message;
  LONGS_EQUAL(std::numeric_limits<long>::min(),
              command_response_message.GetCommandIdentifier());
}

TEST(MusicCommandResponseMessageTest, SettersAndGetters) {
  MusicCommandResponseMessage command_response_message;
  command_response_message.SetCommandIdentifier(54321);
  LONGS_EQUAL(54321, command_response_message.GetCommandIdentifier());
}

// TODO Add tests for Builder
TEST(MusicCommandResponseMessageTest, Builder) {
  MusicCommandResponseMessage message =
      MusicCommandResponseMessageBuilder()
          .SetCommandIdentifier(54321)
          .SetRequestStatus(RequestStatus::kExecuting)
          .SetRequestId(98765)
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(42)
                         .SetTimestamp(1765397823237L)
                         .Build())
          .SetType(MusicMessageType::kControlResponse)
          .SetOriginId(EntityIdBuilder()
                           .SetApplication(123)
                           .SetEntityNumber(456)
                           .SetSite(789)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetApplication(987)
                             .SetEntityNumber(654)
                             .SetSite(321)
                             .Build())
          .Build();
  LONGS_EQUAL(54321, message.GetCommandIdentifier());
  LONGS_EQUAL(static_cast<long>(RequestStatus::kExecuting),
              static_cast<long>(message.GetRequestStatus()));
  LONGS_EQUAL(98765, message.GetRequestId());
  LONGS_EQUAL(42, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765397823237L, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(static_cast<long>(MusicMessageType::kControlResponse),
              static_cast<long>(message.GetType()));
  LONGS_EQUAL(123, message.GetOriginId().GetApplication());
  LONGS_EQUAL(456, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(789, message.GetOriginId().GetSite());
  LONGS_EQUAL(987, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(654, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(321, message.GetReceiverId().GetSite());
}

TEST(MusicCommandResponseMessageTest, Validate) {
  MusicCommandResponseMessage command_response_message;
  command_response_message.SetCommandIdentifier(12345);
  command_response_message.SetOriginId(EntityIdBuilder()
                                           .SetEntityNumber(1)
                                           .SetApplication(2)
                                           .SetSite(3)
                                           .Build());
  command_response_message.SetReceiverId(EntityIdBuilder()
                                             .SetEntityNumber(4)
                                             .SetApplication(5)
                                             .SetSite(6)
                                             .Build());
  command_response_message.SetRequestId(55555);
  command_response_message.SetRequestStatus(RequestStatus::kComplete);
  command_response_message.SetHeader(MusicMessageHeaderBuilder()
                                         .SetExerciseId(10)
                                         .SetTimestamp(1765308607186L)
                                         .Build());
  command_response_message.SetType(MusicMessageType::kControlResponse);

  // Should not throw any exceptions
  command_response_message.Validate();

  // Test with invalid command identifier - Validate() will throw
  command_response_message.SetCommandIdentifier(-1);
  CHECK_THROWS(std::exception, command_response_message.Validate());
}

TEST(MusicCommandResponseMessageTest, ToJson) {
  MusicCommandResponseMessage command_response_message;
  command_response_message.SetCommandIdentifier(13579);
  command_response_message.SetOriginId(EntityIdBuilder()
                                           .SetEntityNumber(7)
                                           .SetApplication(8)
                                           .SetSite(9)
                                           .Build());
  command_response_message.SetReceiverId(EntityIdBuilder()
                                             .SetEntityNumber(10)
                                             .SetApplication(11)
                                             .SetSite(12)
                                             .Build());
  command_response_message.SetRequestId(88888);
  command_response_message.SetHeader(MusicMessageHeaderBuilder()
                                         .SetExerciseId(20)
                                         .SetTimestamp(1765308607186L)
                                         .Build());
  command_response_message.SetType(MusicMessageType::kControlResponse);
  command_response_message.SetRequestStatus(RequestStatus::kComplete);

  nlohmann::json j;
  to_json(j, command_response_message);

  LONGS_EQUAL(13579, j[MusicCommandResponseMessage::kCommandIdKey].get<long>());
  LONGS_EQUAL(
      7, j[MusicResponseMessage::kOriginIdKey][EntityId::kEntityNumberJsonKey]
             .get<int>());
  LONGS_EQUAL(8, j[MusicResponseMessage::kOriginIdKey]
                  [SimulationAddress::kApplicationJsonKey]
                      .get<int>());
  LONGS_EQUAL(
      9, j[MusicResponseMessage::kOriginIdKey][SimulationAddress::kSiteJsonKey]
             .get<int>());
  LONGS_EQUAL(
      10,
      j[MusicResponseMessage::kReceiverIdKey][EntityId::kEntityNumberJsonKey]
          .get<int>());
  LONGS_EQUAL(11, j[MusicResponseMessage::kReceiverIdKey]
                   [SimulationAddress::kApplicationJsonKey]
                       .get<int>());
  LONGS_EQUAL(
      12,
      j[MusicResponseMessage::kReceiverIdKey][SimulationAddress::kSiteJsonKey]
          .get<int>());
  LONGS_EQUAL(88888, j[MusicResponseMessage::kRequestIdKey].get<long>());
  LONGS_EQUAL(
      20, j[MusicMessage::kHeaderJsonKey][MusicMessageHeader::kExerciseIdKey]
              .get<int>());
  LONGS_EQUAL(1765308607186L,
              j[MusicMessage::kHeaderJsonKey][MusicMessageHeader::kTimestampKey]
                  .get<long>());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kComplete),
              j[MusicResponseMessage::kRequestStatusKey].get<int>());
}

TEST(MusicCommandResponseMessageTest, FromJson) {
  nlohmann::json j = {{MusicCommandResponseMessage::kCommandIdKey, 24680},
                      {MusicResponseMessage::kOriginIdKey,
                       {{EntityId::kEntityNumberJsonKey, 13},
                        {SimulationAddress::kApplicationJsonKey, 14},
                        {SimulationAddress::kSiteJsonKey, 15}}},
                      {MusicResponseMessage::kReceiverIdKey,
                       {{EntityId::kEntityNumberJsonKey, 16},
                        {SimulationAddress::kApplicationJsonKey, 17},
                        {SimulationAddress::kSiteJsonKey, 18}}},
                      {MusicResponseMessage::kRequestIdKey, 99999},
                      {MusicMessage::kHeaderJsonKey,
                       {{MusicMessageHeader::kExerciseIdKey, 30},
                        {MusicMessageHeader::kTimestampKey, 1765294665905L}}},
                      {MusicMessage::kTypeJsonKey,
                       static_cast<int>(MusicMessageType::kControlResponse)},
                      {MusicResponseMessage::kRequestStatusKey,
                       static_cast<int>(RequestStatus::kAborted)}};
  MusicCommandResponseMessage command_response_message =
      MusicCommandResponseMessage::FromJson(j);
  LONGS_EQUAL(24680, command_response_message.GetCommandIdentifier());
  LONGS_EQUAL(13, command_response_message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(14, command_response_message.GetOriginId().GetApplication());
  LONGS_EQUAL(15, command_response_message.GetOriginId().GetSite());
  LONGS_EQUAL(16, command_response_message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(17, command_response_message.GetReceiverId().GetApplication());
  LONGS_EQUAL(18, command_response_message.GetReceiverId().GetSite());
  LONGS_EQUAL(99999, command_response_message.GetRequestId());
  LONGS_EQUAL(30, command_response_message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765294665905L,
              command_response_message.GetHeader().GetTimestamp());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kAborted),
              static_cast<int>(command_response_message.GetRequestStatus()));
}

TEST(MusicCommandResponseMessageTest, ToString) {
  MusicCommandResponseMessage command_response_message;
  command_response_message.SetCommandIdentifier(11223);
  command_response_message.SetOriginId(EntityIdBuilder()
                                           .SetEntityNumber(1)
                                           .SetApplication(2)
                                           .SetSite(3)
                                           .Build());
  command_response_message.SetReceiverId(EntityIdBuilder()
                                             .SetEntityNumber(4)
                                             .SetApplication(5)
                                             .SetSite(6)
                                             .Build());
  command_response_message.SetRequestId(77777);
  command_response_message.SetHeader(MusicMessageHeaderBuilder()
                                         .SetExerciseId(40)
                                         .SetTimestamp(1765308607186L)
                                         .Build());
  command_response_message.SetType(MusicMessageType::kControlResponse);
  command_response_message.SetRequestStatus(RequestStatus::kPending);

  std::string expected_str =
      R"(MusicCommandResponseMessage: {"commandIdentifier":11223,"header":{"exerciseID":40,"timestamp":1765308607186},"originID":{"appID":2,"entityID":1,"siteID":3},"receiverID":{"appID":5,"entityID":4,"siteID":6},"requestID":77777,"status":1})";
  std::string actual_str = command_response_message.ToString();
  CHECK_EQUAL(expected_str, actual_str);
}
}  // namespace music::test