// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <exception>
#include <limits>
#include <string>

#include "enum/music_message_type.h"
#include "message/command/scenario/create_construct_response_message.h"
#include "message/music_message_header.h"

namespace music::test {

TEST_GROUP(CreateConstructResponseMessageTest){
    // DEFINE ANY SETUP HERE
};

TEST(CreateConstructResponseMessageTest, DefaultConstructor) {
  CreateConstructResponseMessage message;
  // Create Construct Member - ConstructID
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetConstructId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetConstructId().GetEntityNumber());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetConstructId().GetSite());

  // Class-specific fields (Type and Command Identifier)
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateConstructResponse),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateConstructResponse)
                  .action_type,
              message.GetCommandIdentifier());

  // Music Request Message

  // -- Origin ID
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetOriginId().GetSite());

  // -- Receiver ID
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetReceiverId().GetSite());

  // -- Request ID
  LONGS_EQUAL(std::numeric_limits<long>::min(), message.GetRequestId());

  // -- Music Request Status
  LONGS_EQUAL(static_cast<int>(RequestStatus::kUnset),
              static_cast<int>(message.GetRequestStatus()));

  // Music Message Header
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetHeader().GetExerciseId());
  LONGS_EQUAL(std::numeric_limits<long>::min(),
              message.GetHeader().GetTimestamp());
}

TEST(CreateConstructResponseMessageTest, SettersAndGetters) {
  CreateConstructResponseMessage message;
  EntityId construct_id;
  construct_id.SetApplication(1);
  construct_id.SetEntityNumber(2);
  construct_id.SetSite(3);
  message.SetConstructId(construct_id);

  EntityId origin_id;
  origin_id.SetApplication(4);
  origin_id.SetEntityNumber(5);
  origin_id.SetSite(6);
  message.SetOriginId(origin_id);

  EntityId receiver_id;
  receiver_id.SetApplication(7);
  receiver_id.SetEntityNumber(8);
  receiver_id.SetSite(9);
  message.SetReceiverId(receiver_id);

  message.SetRequestId(12345L);
  message.SetRequestStatus(RequestStatus::kPartiallyComplete);

  MusicMessageHeader header;
  header.SetExerciseId(100);
  header.SetTimestamp(1234567890123L);
  message.SetHeader(header);

  // Verify fields with Getters
  LONGS_EQUAL(1, message.GetConstructId().GetApplication());
  LONGS_EQUAL(2, message.GetConstructId().GetEntityNumber());
  LONGS_EQUAL(3, message.GetConstructId().GetSite());

  LONGS_EQUAL(4, message.GetOriginId().GetApplication());
  LONGS_EQUAL(5, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(6, message.GetOriginId().GetSite());

  LONGS_EQUAL(7, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(8, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(9, message.GetReceiverId().GetSite());

  LONGS_EQUAL(12345L, message.GetRequestId());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kPartiallyComplete),
              static_cast<int>(message.GetRequestStatus()));

  LONGS_EQUAL(100, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1234567890123L, message.GetHeader().GetTimestamp());

  // Verify preset fields (Type and Command Identifier)
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateConstructResponse),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateConstructResponse)
                  .action_type,
              message.GetCommandIdentifier());
}

TEST(CreateConstructResponseMessageTest, Builder) {
  CreateConstructResponseMessage message =
      CreateConstructResponseMessageBuilder()
          .SetConstructId(EntityIdBuilder()
                              .SetApplication(10)
                              .SetEntityNumber(11)
                              .SetSite(12)
                              .Build())
          .SetRequestStatus(RequestStatus::kPending)
          .SetRequestId(1005L)
          .SetOriginId(EntityIdBuilder()
                           .SetApplication(13)
                           .SetEntityNumber(14)
                           .SetSite(15)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetApplication(16)
                             .SetEntityNumber(17)
                             .SetSite(18)
                             .Build())
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(200)
                         .SetTimestamp(9876543210987L)
                         .Build())
          .Build();

  // Verify fields set by Builder
  LONGS_EQUAL(10, message.GetConstructId().GetApplication());
  LONGS_EQUAL(11, message.GetConstructId().GetEntityNumber());
  LONGS_EQUAL(12, message.GetConstructId().GetSite());

  LONGS_EQUAL(13, message.GetOriginId().GetApplication());
  LONGS_EQUAL(0, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(15, message.GetOriginId().GetSite());

  LONGS_EQUAL(16, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(0, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(18, message.GetReceiverId().GetSite());

  LONGS_EQUAL(1005L, message.GetRequestId());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kPending),
              static_cast<int>(message.GetRequestStatus()));

  LONGS_EQUAL(200, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(9876543210987L, message.GetHeader().GetTimestamp());

  // Verify preset fields (Type and Command Identifier)
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateConstructResponse),
              static_cast<int>(message.GetType()));
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateConstructResponse)
                  .action_type,
              message.GetCommandIdentifier());
}

TEST(CreateConstructResponseMessageTest, Validate) {
  CreateConstructResponseMessage message;
  CHECK_THROWS(std::exception, message.Validate());
  message = CreateConstructResponseMessageBuilder()
                .SetConstructId(EntityIdBuilder()
                                    .SetApplication(10)
                                    .SetEntityNumber(11)
                                    .SetSite(12)
                                    .Build())
                .SetRequestStatus(RequestStatus::kPending)
                .SetRequestId(1005L)
                .SetOriginId(EntityIdBuilder()
                                 .SetApplication(13)
                                 .SetEntityNumber(14)
                                 .SetSite(15)
                                 .Build())
                .SetReceiverId(EntityIdBuilder()
                                   .SetApplication(16)
                                   .SetEntityNumber(17)
                                   .SetSite(18)
                                   .Build())
                .SetHeader(MusicMessageHeaderBuilder()
                               .SetExerciseId(200)
                               .SetTimestamp(9876543210987L)
                               .Build())
                .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Set invalid construct id
  EntityId invalid_construct_id = EntityIdBuilder()
                                      .SetApplication(-1)
                                      .SetEntityNumber(-1)
                                      .SetSite(-1)
                                      .Build();
  message.SetConstructId(invalid_construct_id);
  CHECK_THROWS(std::exception, message.Validate());

  EntityId valid_construct_id = EntityIdBuilder()
                                    .SetApplication(10)
                                    .SetEntityNumber(11)
                                    .SetSite(12)
                                    .Build();
  message.SetConstructId(valid_construct_id);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "a valid construct id: ") +
          e.what())
             .c_str());
  }
}

TEST(CreateConstructResponseMessageTest, ToJson) {
  CreateConstructResponseMessage message =
      CreateConstructResponseMessageBuilder()
          .SetConstructId(EntityIdBuilder()
                              .SetApplication(20)
                              .SetEntityNumber(21)
                              .SetSite(22)
                              .Build())
          .SetRequestStatus(RequestStatus::kPending)
          .SetRequestId(2005L)
          .SetOriginId(EntityIdBuilder()
                           .SetApplication(23)
                           .SetEntityNumber(24)
                           .SetSite(25)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetApplication(26)
                             .SetEntityNumber(27)
                             .SetSite(28)
                             .Build())
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(220)
                         .SetTimestamp(2876543210987L)
                         .Build())
          .Build();

  nlohmann::json json = message.ToJson();
  LONGS_EQUAL(20, json[CreateConstructResponseMessage::kConstructIdKey]
                      [SimulationAddress::kApplicationJsonKey]
                          .get<int>());
  LONGS_EQUAL(21, json[CreateConstructResponseMessage::kConstructIdKey]
                      [EntityId::kEntityNumberJsonKey]
                          .get<int>());
  LONGS_EQUAL(22, json[CreateConstructResponseMessage::kConstructIdKey]
                      [SimulationAddress::kSiteJsonKey]
                          .get<int>());

  LONGS_EQUAL(23, json[CreateConstructResponseMessage::kOriginIdKey]
                      [SimulationAddress::kApplicationJsonKey]
                          .get<int>());
  LONGS_EQUAL(0, json[CreateConstructResponseMessage::kOriginIdKey]
                     [EntityId::kEntityNumberJsonKey]
                         .get<int>());
  LONGS_EQUAL(25, json[CreateConstructResponseMessage::kOriginIdKey]
                      [SimulationAddress::kSiteJsonKey]
                          .get<int>());

  LONGS_EQUAL(26, json[CreateConstructResponseMessage::kReceiverIdKey]
                      [SimulationAddress::kApplicationJsonKey]
                          .get<int>());
  LONGS_EQUAL(0, json[CreateConstructResponseMessage::kReceiverIdKey]
                     [EntityId::kEntityNumberJsonKey]
                         .get<int>());
  LONGS_EQUAL(28, json[CreateConstructResponseMessage::kReceiverIdKey]
                      [SimulationAddress::kSiteJsonKey]
                          .get<int>());

  LONGS_EQUAL(2005L,
              json[CreateConstructResponseMessage::kRequestIdKey].get<long>());
  LONGS_EQUAL(
      static_cast<int>(RequestStatus::kPending),
      static_cast<int>(
          json[CreateConstructResponseMessage::kRequestStatusKey].get<int>()));

  LONGS_EQUAL(220, json[CreateConstructResponseMessage::kHeaderJsonKey]
                       [MusicMessageHeader::kExerciseIdKey]
                           .get<int>());
  LONGS_EQUAL(2876543210987L,
              json[CreateConstructResponseMessage::kHeaderJsonKey]
                  [MusicMessageHeader::kTimestampKey]
                      .get<long>());
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateConstructResponse)
                  .action_type,
              json[CreateConstructResponseMessage::kCommandIdKey].get<int>());
}

TEST(CreateConstructResponseMessageTest, FromJson) {
  nlohmann::json json = {
      {CreateConstructResponseMessage::kConstructIdKey,
       {{SimulationAddress::kApplicationJsonKey, 20},
        {EntityId::kEntityNumberJsonKey, 21},
        {SimulationAddress::kSiteJsonKey, 22}}},
      {CreateConstructResponseMessage::kCommandIdKey,
       MusicMessageTypeHelper::GetInfoForType(
           MusicMessageType::kCreateConstructResponse)
           .action_type},
      {CreateConstructResponseMessage::kTypeJsonKey,
       static_cast<int>(MusicMessageType::kCreateConstructResponse)},
      {CreateConstructResponseMessage::kRequestIdKey, 2005L},
      {CreateConstructResponseMessage::kRequestStatusKey,
       static_cast<int>(RequestStatus::kPending)},
      {CreateConstructResponseMessage::kOriginIdKey,
       {{SimulationAddress::kApplicationJsonKey, 23},
        {EntityId::kEntityNumberJsonKey, 24},
        {SimulationAddress::kSiteJsonKey, 25}}},
      {CreateConstructResponseMessage::kReceiverIdKey,
       {{SimulationAddress::kApplicationJsonKey, 26},
        {EntityId::kEntityNumberJsonKey, 27},
        {SimulationAddress::kSiteJsonKey, 28}}},
      {CreateConstructResponseMessage::kHeaderJsonKey,
       {{MusicMessageHeader::kExerciseIdKey, 220},
        {MusicMessageHeader::kTimestampKey, 2876543210987L}}}};

  CreateConstructResponseMessage message =
      CreateConstructResponseMessage::FromJson(json);
  LONGS_EQUAL(20, message.GetConstructId().GetApplication());
  LONGS_EQUAL(21, message.GetConstructId().GetEntityNumber());
  LONGS_EQUAL(22, message.GetConstructId().GetSite());

  LONGS_EQUAL(23, message.GetOriginId().GetApplication());
  LONGS_EQUAL(24, message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(25, message.GetOriginId().GetSite());

  LONGS_EQUAL(26, message.GetReceiverId().GetApplication());
  LONGS_EQUAL(27, message.GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(28, message.GetReceiverId().GetSite());

  LONGS_EQUAL(2005L, message.GetRequestId());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kPending),
              static_cast<int>(message.GetRequestStatus()));

  LONGS_EQUAL(220, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(2876543210987L, message.GetHeader().GetTimestamp());

  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateConstructResponse)
                  .action_type,
              message.GetCommandIdentifier());
}

}  // namespace music::test