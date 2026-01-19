// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/entity_id.h"
#include "message/music_response_message.h"

namespace music::test {
TEST_GROUP(MusicResponseMessageTest){
    // DEFINE ANY SETUP HERE
};

TEST(MusicResponseMessageTest, DefaultConstructor) {
  MusicResponseMessage response_message;
  LONGS_EQUAL(std::numeric_limits<long>::min(),
              response_message.GetRequestId());
  EntityId origin_id = response_message.GetOriginId();
  LONGS_EQUAL(std::numeric_limits<int>::min(), origin_id.GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(), origin_id.GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(), origin_id.GetEntityNumber());
  EntityId receiver_id = response_message.GetReceiverId();
  LONGS_EQUAL(std::numeric_limits<int>::min(), receiver_id.GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(), receiver_id.GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(), receiver_id.GetEntityNumber());
}

TEST(MusicResponseMessageTest, SettersAndGetters) {
  MusicResponseMessage response_message;

  EntityId origin_id;
  origin_id.SetSite(123);
  origin_id.SetApplication(456);
  origin_id.SetEntityNumber(789);

  EntityId receiver_id;
  receiver_id.SetSite(012);
  receiver_id.SetApplication(345);
  receiver_id.SetEntityNumber(678);

  response_message.SetOriginId(origin_id);
  response_message.SetReceiverId(receiver_id);
  response_message.SetRequestStatus(RequestStatus::kExecuting);
  response_message.SetRequestId(1234567);

  LONGS_EQUAL(123, response_message.GetOriginId().GetSite());
  LONGS_EQUAL(456, response_message.GetOriginId().GetApplication());
  LONGS_EQUAL(789, response_message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(012, response_message.GetReceiverId().GetSite());
  LONGS_EQUAL(345, response_message.GetReceiverId().GetApplication());
  LONGS_EQUAL(678, response_message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(1234567, response_message.GetRequestId());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kExecuting),
              static_cast<int>(response_message.GetRequestStatus()));
}

TEST(MusicResponseMessageTest, Builder) {
  MusicResponseMessage message =
      MusicResponseMessageBuilder()
          .SetRequestId(73L)
          .SetOriginId(EntityIdBuilder()
                           .SetApplication(1009)
                           .SetEntityNumber(1013)
                           .SetSite(1019)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetApplication(1021)
                             .SetEntityNumber(1031)
                             .SetSite(1033)
                             .Build())
          .SetRequestStatus(RequestStatus::kExecuting)
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(101)
                         .SetTimestamp(1765380158392L)
                         .Build())
          .SetType(MusicMessageType::kCreateEnvironmentResponse)
          .Build();

  LONGS_EQUAL(73L, message.GetRequestId());
  LONGS_EQUAL(1009, message.GetOriginId().GetApplication());
  LONGS_EQUAL(1013, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(1019, message.GetOriginId().GetSite());
  LONGS_EQUAL(1021, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(1031, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(1033, message.GetReceiverId().GetSite());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kExecuting),
              static_cast<int>(message.GetRequestStatus()));
  LONGS_EQUAL(101, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765380158392L, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateEnvironmentResponse),
              static_cast<int>(message.GetType()));
}

TEST(MusicResponseMessageTest, Validate) {
  MusicResponseMessage response_message;
  // Should throw an exception because the message is not properly initialized
  CHECK_THROWS(std::exception, response_message.Validate());
  EntityId origin_id;
  origin_id.SetSite(123);
  origin_id.SetApplication(456);
  origin_id.SetEntityNumber(789);

  EntityId receiver_id;
  receiver_id.SetSite(012);
  receiver_id.SetApplication(345);
  receiver_id.SetEntityNumber(678);

  response_message.SetOriginId(origin_id);
  response_message.SetReceiverId(receiver_id);
  response_message.SetRequestStatus(RequestStatus::kPartiallyComplete);
  response_message.SetRequestId(1234567);

  MusicMessageHeader header;
  header.SetExerciseId(42);
  header.SetTimestamp(1765380158392L);
  response_message.SetHeader(header);
  response_message.SetType(MusicMessageType::kCreateEnvironmentResponse);

  // Should not throw an exception because the message is now properly
  // initialized
  try {
    response_message.Validate();
  } catch (std::exception &e) {
    FAIL("Validation threw an exception unexpectedly");
  }

  // Set an invalid request id

  response_message.SetRequestId(-1);
  CHECK_THROWS(std::exception, response_message.Validate());

  response_message.SetRequestId(1234567);
  // Should not throw an exception because the request id is valid again
  try {
    response_message.Validate();
  } catch (std::exception &e) {
    FAIL("Validation threw an exception unexpectedly");
  }

  // Set an invalid origin id
  EntityId invalid_origin_id;
  response_message.SetOriginId(invalid_origin_id);

  // Should throw an exception because the origin id is invalid
  CHECK_THROWS(std::exception, response_message.Validate());

  EntityId valid_origin_id;
  valid_origin_id.SetSite(123);
  valid_origin_id.SetApplication(456);
  valid_origin_id.SetEntityNumber(789);
  response_message.SetOriginId(valid_origin_id);

  // Should not throw an exception because the origin id is now valid
  try {
    response_message.Validate();
  } catch (std::exception &e) {
    FAIL("Validation threw an exception unexpectedly");
  }

  // Set an invalid receiver id
  EntityId invalid_receiver_id;
  response_message.SetReceiverId(invalid_receiver_id);
  // Should throw an exception because the receiver id is invalid
  CHECK_THROWS(std::exception, response_message.Validate());

  EntityId valid_receiver_id;
  valid_receiver_id.SetSite(12);
  valid_receiver_id.SetApplication(345);
  valid_receiver_id.SetEntityNumber(678);
  response_message.SetReceiverId(valid_receiver_id);
  // Should not throw an exception because the receiver id is now valid
  try {
    response_message.Validate();
  } catch (std::exception &e) {
    FAIL("Validation threw an exception unexpectedly");
  }

  // Set an invalid music request status
  response_message.SetRequestStatus(static_cast<RequestStatus>(10));
  // Should throw an exception because the music request status is invalid
  CHECK_THROWS(std::exception, response_message.Validate());

  // Set a valid music request status
  response_message.SetRequestStatus(RequestStatus::kPending);
  // Should not throw an exception because the music request status is now valid
  try {
    response_message.Validate();
  } catch (std::exception &e) {
    FAIL("Validation threw an exception unexpectedly");
  }
}

TEST(MusicResponseMessageTest, ToJson) {
  MusicResponseMessage response_message;

  EntityId origin_id;
  origin_id.SetSite(123);
  origin_id.SetApplication(456);
  origin_id.SetEntityNumber(789);

  EntityId receiver_id;
  receiver_id.SetSite(12);
  receiver_id.SetApplication(345);
  receiver_id.SetEntityNumber(678);

  response_message.SetOriginId(origin_id);
  response_message.SetReceiverId(receiver_id);
  response_message.SetRequestStatus(RequestStatus::kExecuting);
  response_message.SetRequestId(1234567);

  MusicMessageHeader header;
  header.SetExerciseId(42);
  header.SetTimestamp(1765380158392L);
  response_message.SetHeader(header);
  response_message.SetType(MusicMessageType::kCreateEnvironmentResponse);

  nlohmann::json json = response_message.ToJson();
  LONGS_EQUAL(1234567, json[MusicResponseMessage::kRequestIdKey].get<int>());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kExecuting),
              json[MusicResponseMessage::kRequestStatusKey].get<int>());
  LONGS_EQUAL(
      123,
      json[MusicResponseMessage::kOriginIdKey][SimulationAddress::kSiteJsonKey]
          .get<int>());
  LONGS_EQUAL(456, json[MusicResponseMessage::kOriginIdKey]
                       [SimulationAddress::kApplicationJsonKey]
                           .get<int>());
  LONGS_EQUAL(
      789,
      json[MusicResponseMessage::kOriginIdKey][EntityId::kEntityNumberJsonKey]
          .get<int>());
  LONGS_EQUAL(12, json[MusicResponseMessage::kReceiverIdKey]
                      [SimulationAddress::kSiteJsonKey]
                          .get<int>());
  LONGS_EQUAL(345, json[MusicResponseMessage::kReceiverIdKey]
                       [SimulationAddress::kApplicationJsonKey]
                           .get<int>());
  LONGS_EQUAL(
      678,
      json[MusicResponseMessage::kReceiverIdKey][EntityId::kEntityNumberJsonKey]
          .get<int>());
  LONGS_EQUAL(42, json[MusicResponseMessage::kHeaderJsonKey]
                      [MusicMessageHeader::kExerciseIdKey]
                          .get<int>());
  LONGS_EQUAL(1765380158392L, json[MusicResponseMessage::kHeaderJsonKey]
                                  [MusicMessageHeader::kTimestampKey]
                                      .get<long long>());
}

TEST(MusicResponseMessageTest, FromJson) {
  nlohmann::json json = {
      {MusicResponseMessage::kHeaderJsonKey,
       {{MusicMessageHeader::kExerciseIdKey, 42},
        {MusicMessageHeader::kTimestampKey, 1765380158392L}}},
      {MusicResponseMessage::kTypeJsonKey,
       static_cast<int>(MusicMessageType::kCreateEnvironmentResponse)},
      {MusicResponseMessage::kRequestIdKey, 1234567},
      {MusicResponseMessage::kRequestStatusKey,
       static_cast<int>(RequestStatus::kExecuting)},
      {MusicResponseMessage::kOriginIdKey,
       {{SimulationAddress::kSiteJsonKey, 123},
        {SimulationAddress::kApplicationJsonKey, 456},
        {EntityId::kEntityNumberJsonKey, 789}}},
      {MusicResponseMessage::kReceiverIdKey,
       {{SimulationAddress::kSiteJsonKey, 12},
        {SimulationAddress::kApplicationJsonKey, 345},
        {EntityId::kEntityNumberJsonKey, 678}}}};
  MusicResponseMessage response_message = MusicResponseMessage::FromJson(json);
  LONGS_EQUAL(42, response_message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765380158392L, response_message.GetHeader().GetTimestamp());
  LONGS_EQUAL(1234567, response_message.GetRequestId());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kExecuting),
              static_cast<int>(response_message.GetRequestStatus()));
  LONGS_EQUAL(123, response_message.GetOriginId().GetSite());
  LONGS_EQUAL(456, response_message.GetOriginId().GetApplication());
  LONGS_EQUAL(789, response_message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(12, response_message.GetReceiverId().GetSite());
  LONGS_EQUAL(345, response_message.GetReceiverId().GetApplication());
  LONGS_EQUAL(678, response_message.GetReceiverId().GetEntityNumber());
}

TEST(MusicResponseMessageTest, ToString) {
  MusicResponseMessage response_message;

  EntityId origin_id;
  origin_id.SetSite(123);
  origin_id.SetApplication(456);
  origin_id.SetEntityNumber(789);

  EntityId receiver_id;
  receiver_id.SetSite(12);
  receiver_id.SetApplication(345);
  receiver_id.SetEntityNumber(678);

  response_message.SetOriginId(origin_id);
  response_message.SetReceiverId(receiver_id);
  response_message.SetRequestStatus(RequestStatus::kComplete);
  response_message.SetRequestId(1234567);

  MusicMessageHeader header;
  header.SetExerciseId(42);
  header.SetTimestamp(1765380158392L);
  response_message.SetHeader(header);
  response_message.SetType(MusicMessageType::kCreateEnvironmentResponse);

  std::string expected_string =
      "MusicResponseMessage: {\"" + MusicResponseMessage::kHeaderJsonKey +
      "\":{\"" + MusicMessageHeader::kExerciseIdKey + "\":42,\"" +
      MusicMessageHeader::kTimestampKey + "\":1765380158392},\"" +
      MusicResponseMessage::kOriginIdKey + "\":{\"" +
      SimulationAddress::kApplicationJsonKey + "\":456,\"" +
      EntityId::kEntityNumberJsonKey + "\":789,\"" +
      SimulationAddress::kSiteJsonKey + "\":123},\"" +
      MusicResponseMessage::kReceiverIdKey + "\":{\"" +
      SimulationAddress::kApplicationJsonKey + "\":345,\"" +
      EntityId::kEntityNumberJsonKey + "\":678,\"" +
      SimulationAddress::kSiteJsonKey + "\":12},\"" +
      MusicResponseMessage::kRequestIdKey + "\":1234567,\"" +
      MusicResponseMessage::kRequestStatusKey + "\":4}";

  std::string message_string = response_message.ToString();
  STRCMP_EQUAL(expected_string.c_str(), message_string.c_str());
}
}  // namespace music::test
