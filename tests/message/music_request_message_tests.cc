// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/entity_id.h"
#include "message/music_message_header.h"
#include "message/music_request_message.h"

TEST_GROUP(MusicRequestMessageTest){
    // DEFINE ANY SETUP HERE
};

TEST(MusicRequestMessageTest, DefaultConstructor) {
  music::MusicRequestMessage request_message;
  LONGS_EQUAL(std::numeric_limits<long>::min(), request_message.GetRequestId());
  music::EntityId origin_id = request_message.GetOriginId();
  LONGS_EQUAL(std::numeric_limits<int>::min(), origin_id.GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(), origin_id.GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(), origin_id.GetEntityNumber());
  music::EntityId receiver_id = request_message.GetReceiverId();
  LONGS_EQUAL(std::numeric_limits<int>::min(), receiver_id.GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(), receiver_id.GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(), receiver_id.GetEntityNumber());
}

TEST(MusicRequestMessageTest, SettersAndGetters) {
  music::EntityId origin_id;
  origin_id.SetSite(1);
  origin_id.SetApplication(2);
  origin_id.SetEntityNumber(3);

  music::EntityId receiver_id;
  receiver_id.SetSite(4);
  receiver_id.SetApplication(5);
  receiver_id.SetEntityNumber(6);

  music::MusicRequestMessage request_message;
  request_message.SetOriginId(origin_id);
  request_message.SetReceiverId(receiver_id);
  request_message.SetRequestId(12345);

  LONGS_EQUAL(1, request_message.GetOriginId().GetSite());
  LONGS_EQUAL(2, request_message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, request_message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(4, request_message.GetReceiverId().GetSite());
  LONGS_EQUAL(5, request_message.GetReceiverId().GetApplication());
  LONGS_EQUAL(6, request_message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(12345, request_message.GetRequestId());
}

TEST(MusicRequestMessageTest, Builder) {
  music::MusicRequestMessage message =
      music::MusicRequestMessageBuilder()
          .SetRequestId(2L)
          .SetOriginId(music::EntityIdBuilder()
                           .SetApplication(0)
                           .SetEntityNumber(1)
                           .SetSite(2)
                           .Build())
          .SetReceiverId(music::EntityIdBuilder()
                             .SetApplication(3)
                             .SetEntityNumber(4)
                             .SetSite(5)
                             .Build())
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(100)
                         .SetTimestamp(1765385854934L)
                         .Build())
          .SetType(music::MusicMessageType::kControlRequest)
          .Build();

  LONGS_EQUAL(2L, message.GetRequestId());
  LONGS_EQUAL(0, message.GetOriginId().GetApplication());
  LONGS_EQUAL(1, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(2, message.GetOriginId().GetSite());
  LONGS_EQUAL(3, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(4, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(5, message.GetReceiverId().GetSite());
  LONGS_EQUAL(100, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765385854934L, message.GetHeader().GetTimestamp());
  LONGS_EQUAL(static_cast<int>(music::MusicMessageType::kControlRequest),
              static_cast<int>(message.GetType()));
}

TEST(MusicRequestMessageTest, Validate) {
  music::MusicRequestMessage request_message;
  request_message.SetOriginId(music::EntityIdBuilder()
                                  .SetEntityNumber(1)
                                  .SetApplication(2)
                                  .SetSite(3)
                                  .Build());
  request_message.SetReceiverId(music::EntityIdBuilder()
                                    .SetEntityNumber(4)
                                    .SetApplication(5)
                                    .SetSite(6)
                                    .Build());
  request_message.SetRequestId(55555);
  request_message.SetHeader(music::MusicMessageHeaderBuilder()
                                .SetExerciseId(10)
                                .SetTimestamp(1765308607186L)
                                .Build());
  request_message.SetType(music::MusicMessageType::kControlRequest);

  // Should not throw any exceptions
  request_message.Validate();

  // Test with invalid request ID - Validate() will throw
  request_message.SetRequestId(-1);
  CHECK_THROWS(std::exception, request_message.Validate());

  // Test with invalid origin ID - Validate() will throw
  request_message.SetRequestId(55555);  // Reset to valid
  music::EntityId invalid_origin_id;
  request_message.SetOriginId(invalid_origin_id);
  CHECK_THROWS(std::exception, request_message.Validate());
  request_message.SetOriginId(music::EntityIdBuilder()
                                  .SetEntityNumber(4)
                                  .SetApplication(5)
                                  .SetSite(6)
                                  .Build());
  // Test with invalid receiver ID - Validate() will throw
  music::EntityId invalid_receiver_id;
  request_message.SetReceiverId(invalid_receiver_id);
  CHECK_THROWS(std::exception, request_message.Validate());
  request_message.SetReceiverId(music::EntityIdBuilder()
                                    .SetEntityNumber(4)
                                    .SetApplication(5)
                                    .SetSite(6)
                                    .Build());
  // Test with invalid header - Validate() will throw
  music::MusicMessageHeader invalid_header;
  request_message.SetHeader(invalid_header);
  CHECK_THROWS(std::exception, request_message.Validate());
  music::MusicMessageHeader valid_header = music::MusicMessageHeaderBuilder()
                                               .SetExerciseId(10)
                                               .SetTimestamp(1765308607186L)
                                               .Build();
  request_message.SetHeader(valid_header);
  // Test with invalid type - Validate() will throw
  request_message.SetType(music::MusicMessageType::_count);
  CHECK_THROWS(std::exception, request_message.Validate());
}

TEST(MusicRequestMessageTest, ToJson) {
  music::MusicRequestMessage request_message;
  request_message.SetOriginId(music::EntityIdBuilder()
                                  .SetEntityNumber(1)
                                  .SetApplication(2)
                                  .SetSite(3)
                                  .Build());
  request_message.SetReceiverId(music::EntityIdBuilder()
                                    .SetEntityNumber(4)
                                    .SetApplication(5)
                                    .SetSite(6)
                                    .Build());
  request_message.SetRequestId(55555);
  request_message.SetHeader(music::MusicMessageHeaderBuilder()
                                .SetExerciseId(10)
                                .SetTimestamp(1765308607186L)
                                .Build());
  request_message.SetType(music::MusicMessageType::kControlRequest);

  nlohmann::json json = request_message.ToJson();
  LONGS_EQUAL(55555,
              json[music::MusicRequestMessage::kRequestIdKey].get<long>());
  LONGS_EQUAL(1, json[music::MusicRequestMessage::kOriginIdKey]
                     [music::EntityId::kEntityNumberJsonKey]
                         .get<long>());
  LONGS_EQUAL(2, json[music::MusicRequestMessage::kOriginIdKey]
                     [music::SimulationAddress::kApplicationJsonKey]
                         .get<long>());
  LONGS_EQUAL(3, json[music::MusicRequestMessage::kOriginIdKey]
                     [music::SimulationAddress::kSiteJsonKey]
                         .get<long>());
  LONGS_EQUAL(4, json[music::MusicRequestMessage::kReceiverIdKey]
                     [music::EntityId::kEntityNumberJsonKey]
                         .get<long>());
  LONGS_EQUAL(5, json[music::MusicRequestMessage::kReceiverIdKey]
                     [music::SimulationAddress::kApplicationJsonKey]
                         .get<long>());
  LONGS_EQUAL(6, json[music::MusicRequestMessage::kReceiverIdKey]
                     [music::SimulationAddress::kSiteJsonKey]
                         .get<long>());
  LONGS_EQUAL(10, json[music::MusicMessage::kHeaderJsonKey]
                      [music::MusicMessageHeader::kExerciseIdKey]
                          .get<long>());
  LONGS_EQUAL(1765308607186L, json[music::MusicMessage::kHeaderJsonKey]
                                  [music::MusicMessageHeader::kTimestampKey]
                                      .get<long>());
}

TEST(MusicRequestMessageTest, FromJson) {
  nlohmann::json json = {
      {music::MusicRequestMessage::kRequestIdKey, 4294967295},
      {music::MusicRequestMessage::kOriginIdKey,
       {{music::EntityId::kEntityNumberJsonKey, 1},
        {music::SimulationAddress::kApplicationJsonKey, 2},
        {music::SimulationAddress::kSiteJsonKey, 3}}},
      {music::MusicRequestMessage::kReceiverIdKey,
       {{music::EntityId::kEntityNumberJsonKey, 4},
        {music::SimulationAddress::kApplicationJsonKey, 5},
        {music::SimulationAddress::kSiteJsonKey, 6}}},
      {music::MusicMessage::kHeaderJsonKey,
       {{music::MusicMessageHeader::kExerciseIdKey, 42},
        {music::MusicMessageHeader::kTimestampKey, 1765376538865L}}},
      {music::MusicMessage::kTypeJsonKey,
       static_cast<long>(music::MusicMessageType::kControlRequest)}};
  music::MusicRequestMessage request_message =
      music::MusicRequestMessage::FromJson(json);
  LONGS_EQUAL(4294967295, request_message.GetRequestId());
  LONGS_EQUAL(1, request_message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(2, request_message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, request_message.GetOriginId().GetSite());
  LONGS_EQUAL(4, request_message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(5, request_message.GetReceiverId().GetApplication());
  LONGS_EQUAL(6, request_message.GetReceiverId().GetSite());
  LONGS_EQUAL(42, request_message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765376538865L, request_message.GetHeader().GetTimestamp());
}

TEST(MusicRequestMessageTest, ToString) {
  music::MusicRequestMessage request_message;
  request_message.SetOriginId(music::EntityIdBuilder()
                                  .SetEntityNumber(999)
                                  .SetApplication(888)
                                  .SetSite(777)
                                  .Build());
  request_message.SetReceiverId(music::EntityIdBuilder()
                                    .SetEntityNumber(111)
                                    .SetApplication(222)
                                    .SetSite(333)
                                    .Build());
  request_message.SetRequestId(123456);
  request_message.SetHeader(music::MusicMessageHeaderBuilder()
                                .SetExerciseId(10)
                                .SetTimestamp(1765377435556L)
                                .Build());
  request_message.SetType(music::MusicMessageType::kCreateConstructRequest);

  std::string expected_string =
      "MusicRequestMessage: {\"" +
      std::string(music::MusicMessage::kHeaderJsonKey) +
      "\":" + request_message.GetHeader().ToJson().dump() + ",\"" +
      std::string(music::MusicRequestMessage::kOriginIdKey) +
      "\":" + request_message.GetOriginId().ToJson().dump() + ",\"" +
      std::string(music::MusicRequestMessage::kReceiverIdKey) +
      "\":" + request_message.GetReceiverId().ToJson().dump() + ",\"" +
      std::string(music::MusicRequestMessage::kRequestIdKey) + "\":123456}";

  STRCMP_EQUAL(expected_string.c_str(), request_message.ToString().c_str());
}
