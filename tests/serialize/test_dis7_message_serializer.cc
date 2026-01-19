// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <dis7/ActionRequestPdu.h>
#include <dis7/ActionResponsePdu.h>
#include <dis7/DataQueryPdu.h>
#include <dis7/Pdu.h>
#include <dis7/utils/DataStream.h>
#include <dis7/utils/Endian.h>

#include "message/command/entity/transfer_construct_id_message.h"
#include "message/command/scenario/create_environment_response_message.h"
#include "message/command/scenario/finalize_scenario_request_message.h"
#include "message/command/scenario/finalize_scenario_response_message.h"
#include "message/entity_id.h"
#include "message/music_message_header.h"
#include "serialize/dis7_message_serializer.h"
#include "util/music_byte_buffer.h"

// Include CppUTest last to avoid macro conflicts
#include <CppUTest/TestHarness.h>

#include <cstdint>
#include <nlohmann/json_fwd.hpp>

namespace music::test {

namespace {

constexpr uint64_t kMillisPerHour = 3600000ULL;
constexpr uint32_t kDisTimeUnitsPerHour = 0x7FFFFFFFU;

uint32_t EpochMillisToDisTimestampForTest(const uint64_t epoch_millis) {
  const uint64_t millis_since_hour = epoch_millis % kMillisPerHour;
  const double ratio = static_cast<double>(millis_since_hour) /
                       static_cast<double>(kMillisPerHour);
  uint32_t units =
      static_cast<uint32_t>(ratio * static_cast<double>(kDisTimeUnitsPerHour));
  if (units > kDisTimeUnitsPerHour) {
    units = kDisTimeUnitsPerHour;
  }
  return static_cast<uint32_t>((units << 1) | 1U);
}

}  // namespace

TEST_GROUP(Dis7MessageSerializerTest){void setup(){
    // Setup code runs before each test
} void teardown(){
    // Teardown code runs after each test
}};

TEST(Dis7MessageSerializerTest,
     SerializeTransferConstructIdMessage_BufferContainsData) {
  // Create entity IDs for old and new construct IDs
  EntityId old_id = EntityIdBuilder()
                        .SetSite(1)
                        .SetApplication(2)
                        .SetEntityNumber(100)
                        .Build();

  EntityId new_id = EntityIdBuilder()
                        .SetSite(1)
                        .SetApplication(2)
                        .SetEntityNumber(200)
                        .Build();

  // Create origin and receiver IDs for the request message
  EntityId origin_id =
      EntityIdBuilder().SetSite(1).SetApplication(2).SetEntityNumber(0).Build();

  EntityId receiver_id =
      EntityIdBuilder().SetSite(1).SetApplication(2).SetEntityNumber(0).Build();

  // Build the message (without header to avoid potential issues)
  TransferConstructIdMessage message =
      TransferConstructIdMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(200)
                         .SetTimestamp(1766164647036)
                         .Build())
          .SetOldId(old_id)
          .SetNewId(new_id)
          .SetOriginId(origin_id)
          .SetReceiverId(receiver_id)
          .SetRequestId(12345)
          .Build();

  // Validate the message before serialization
  message.Validate();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
}

TEST(Dis7MessageSerializerTest, TransferConstructIdPduMeetsSpec) {
  // Pick IDs so we can verify the serializer overwrites fields as required.
  EntityId old_id = EntityIdBuilder()
                        .SetSite(1)
                        .SetApplication(2)
                        .SetEntityNumber(100)
                        .Build();

  EntityId new_id = EntityIdBuilder()
                        .SetSite(1)
                        .SetApplication(2)
                        .SetEntityNumber(200)
                        .Build();

  // Origin entity number should be forced to 0 by serializer.
  EntityId origin_id = EntityIdBuilder()
                           .SetSite(10)
                           .SetApplication(20)
                           .SetEntityNumber(0)
                           .Build();

  // Receiving EntityID should be forced to all ones by serializer.
  EntityId receiver_id =
      EntityIdBuilder().SetSite(7).SetApplication(8).SetEntityNumber(9).Build();

  TransferConstructIdMessage message =
      TransferConstructIdMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(200)
                         .SetTimestamp(1766164647036)
                         .Build())
          .SetOldId(old_id)
          .SetNewId(new_id)
          .SetOriginId(origin_id)
          .SetReceiverId(receiver_id)
          .SetRequestId(12345)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);

  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());

  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionRequestPdu pdu = DIS::ActionRequestPdu();

  pdu.unmarshal(read_stream);

  // PDU length
  LONGS_EQUAL(72, static_cast<long>(pdu.getLength()));

  // Timestamp is DIS-encoded (absolute mode bit set).
  UNSIGNED_LONGS_EQUAL(1U, static_cast<unsigned long>(pdu.getTimestamp() & 1U));
  UNSIGNED_LONGS_EQUAL(static_cast<unsigned long>(
                           EpochMillisToDisTimestampForTest(1766164647036ULL)),
                       static_cast<unsigned long>(pdu.getTimestamp()));

  // Originating EntityID: site/app from origin, entityNumber forced to 0.
  LONGS_EQUAL(10,
              pdu.getOriginatingEntityID().getSimulationAddress().getSite());
  LONGS_EQUAL(
      20, pdu.getOriginatingEntityID().getSimulationAddress().getApplication());
  LONGS_EQUAL(0, pdu.getOriginatingEntityID().getEntityNumber());

  // Receiving EntityID: all 1s
  LONGS_EQUAL(0xFFFF,
              pdu.getReceivingEntityID().getSimulationAddress().getSite());
  LONGS_EQUAL(
      0xFFFF,
      pdu.getReceivingEntityID().getSimulationAddress().getApplication());
  LONGS_EQUAL(0xFFFF, pdu.getReceivingEntityID().getEntityNumber());

  // RequestID and ActionID.
  LONGS_EQUAL(12345, static_cast<long>(pdu.getRequestID()));
  LONGS_EQUAL(454000002u, pdu.getActionID());

  // Fixed/Variable datum counts.
  LONGS_EQUAL(0u, pdu.getNumberOfFixedDatumRecords());
  LONGS_EQUAL(2u, pdu.getNumberOfVariableDatumRecords());
  LONGS_EQUAL(0, static_cast<long>(pdu.getFixedDatums().size()));
  LONGS_EQUAL(2, static_cast<long>(pdu.getVariableDatums().size()));

  const auto &var0 = pdu.getVariableDatums()[0];
  const auto &var1 = pdu.getVariableDatums()[1];

  // Variable datum IDs 1 and 2
  LONGS_EQUAL(1u, var0.getVariableDatumID());
  LONGS_EQUAL(2u, var1.getVariableDatumID());

  // Length is in bits, padded to 64 bits.
  LONGS_EQUAL(64u, var0.getVariableDatumLength());
  LONGS_EQUAL(64u, var1.getVariableDatumLength());

  // Verify datum payload is EntityID (48-bit) padded to 8 bytes.
  const std::array<unsigned char, 8> expected_old{
      {0x00, 0x01, 0x00, 0x02, 0x00, 0x64, 0x00, 0x00}};
  const std::array<unsigned char, 8> expected_new{
      {0x00, 0x01, 0x00, 0x02, 0x00, 0xC8, 0x00, 0x00}};

  MEMCMP_EQUAL(expected_old.data(), var0.getVariableDatums(),
               expected_old.size());
  MEMCMP_EQUAL(expected_new.data(), var1.getVariableDatums(),
               expected_new.size());
}

TEST(Dis7MessageSerializerTest, SerializeValidMessage_CreateConstructRequest) {
  // Build a full, valid create construct request message
  music::CreateConstructRequestMessage message =
      music::CreateConstructRequestMessageBuilder()
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(200)
                         .SetTimestamp(1767025350080)
                         .Build())
          .SetConstructType("Tank")
          .SetConstructCallsign("Tank Callsign")
          .SetOriginId(EntityIdBuilder()
                           .SetSite(1)
                           .SetApplication(2)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(3)
                             .SetApplication(4)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(54321)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);

  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());

  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionRequestPdu pdu = DIS::ActionRequestPdu();

  pdu.unmarshal(read_stream);

  // Check action type
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateConstructRequest)
                  .action_type,
              pdu.getActionID());
}

TEST(Dis7MessageSerializerTest, SerializeValidMessage_CreateConstructResponse) {
  // Build a full, valid create construct response message
  music::CreateConstructResponseMessage message =
      music::CreateConstructResponseMessageBuilder()
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(200)
                         .SetTimestamp(1767025350080)
                         .Build())
          .SetConstructId(EntityIdBuilder()
                              .SetSite(1)
                              .SetApplication(2)
                              .SetEntityNumber(123)
                              .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(3)
                           .SetApplication(4)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(5)
                             .SetApplication(6)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(54321)
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);

  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());

  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionResponsePdu pdu = DIS::ActionResponsePdu();

  pdu.unmarshal(read_stream);

  // Check request status
  LONGS_EQUAL(RequestStatus::kComplete, pdu.getRequestStatus());
}

TEST(Dis7MessageSerializerTest,
     SerializeValidMessage_CreateEnvironmentRequest) {
  // Build a full, valid create environment request message
  music::CreateEnvironmentRequestMessage message =
      music::CreateEnvironmentRequestMessageBuilder()
          .SetHeader(music::MusicMessageHeaderBuilder()
                         .SetExerciseId(240)
                         .SetTimestamp(1768000000000)
                         .Build())
          .SetEnvironmentName("Sahara")
          .SetEnvironmentMetadata(
              "{\"sceneTime\":\"11:53:06PM\",\"originLocation\":{\"x\":123, "
              "\"y\": 90.2, \"z\": 0.23}}")
          .SetOriginId(EntityIdBuilder()
                           .SetSite(10)
                           .SetApplication(20)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(30)
                             .SetApplication(40)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(67890)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());
  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionRequestPdu pdu = DIS::ActionRequestPdu();
  pdu.unmarshal(read_stream);
  // Check action type
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kCreateEnvironmentRequest)
                  .action_type,
              pdu.getActionID());
}

TEST(Dis7MessageSerializerTest,
     SerializeValidMessage_CreateEnvironmentResponse) {
  // Build a full, valid create environment response message
  CreateEnvironmentResponseMessage message =
      CreateEnvironmentResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(240)
                         .SetTimestamp(1768000000000)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(10)
                           .SetApplication(20)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(30)
                             .SetApplication(40)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(67890)
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());
  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);

  DIS::ActionResponsePdu pdu = DIS::ActionResponsePdu();
  pdu.unmarshal(read_stream);

  // Check request status
  LONGS_EQUAL(RequestStatus::kComplete, pdu.getRequestStatus());

  // Check request ID
  LONGS_EQUAL(67890, static_cast<long>(pdu.getRequestID()));
}

TEST(Dis7MessageSerializerTest, SerializeValidMessage_FinalizeScenarioRequest) {
  // Build a full, valid finalize scenario request message
  FinalizeScenarioRequestMessage message =
      FinalizeScenarioRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(200)
                         .SetTimestamp(1769000000000)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(15)
                           .SetApplication(25)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(35)
                             .SetApplication(45)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(13579)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());
  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionRequestPdu pdu = DIS::ActionRequestPdu();
  pdu.unmarshal(read_stream);

  // Check action type
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kFinalizeScenarioRequest)
                  .action_type,
              pdu.getActionID());
}

TEST(Dis7MessageSerializerTest,
     SerializeValidMessage_FinalizeScenarioResponse) {
  // Build a full, valid finalize scenario response message
  FinalizeScenarioResponseMessage message =
      FinalizeScenarioResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(245)
                         .SetTimestamp(1769000000000)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(15)
                           .SetApplication(25)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(35)
                             .SetApplication(45)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(13579)
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());
  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionResponsePdu pdu = DIS::ActionResponsePdu();
  pdu.unmarshal(read_stream);

  // Check request status
  LONGS_EQUAL(RequestStatus::kComplete, pdu.getRequestStatus());

  // Check request ID
  LONGS_EQUAL(13579, static_cast<long>(pdu.getRequestID()));
}

TEST(Dis7MessageSerializerTest,
     SerializeValidMessage_ParameterizeConstructRequest) {
  // Build a full, valid parameterize construct request message
  ParameterizeConstructRequestMessage message =
      ParameterizeConstructRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(101)
                         .SetTimestamp(1770000000000)
                         .Build())
          .SetConstructParameters(
              nlohmann::json::parse((R"({"testKey":"testValue"})")))
          .SetGhostedId(EntityIdBuilder()
                            .SetSite(50)
                            .SetApplication(150)
                            .SetEntityNumber(25)
                            .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(50)
                           .SetApplication(60)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(70)
                             .SetApplication(80)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(24680)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());
  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionRequestPdu pdu = DIS::ActionRequestPdu();
  pdu.unmarshal(read_stream);

  // Check action type
  LONGS_EQUAL(MusicMessageTypeHelper::GetInfoForType(
                  MusicMessageType::kParameterizeConstructRequest)
                  .action_type,
              pdu.getActionID());
}

TEST(Dis7MessageSerializerTest,
     SerializeValidMessage_ParameterizeConstructResponse) {
  // Build a full, valid parameterize construct response message
  ParameterizeConstructResponseMessage message =
      ParameterizeConstructResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(101)
                         .SetTimestamp(1770000000000)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(50)
                           .SetApplication(60)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(70)
                             .SetApplication(80)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(24680)
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());
  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionResponsePdu pdu = DIS::ActionResponsePdu();
  pdu.unmarshal(read_stream);

  // Check request status
  LONGS_EQUAL(RequestStatus::kComplete, pdu.getRequestStatus());

  // Check request ID
  LONGS_EQUAL(24680, static_cast<long>(pdu.getRequestID()));
}

TEST(Dis7MessageSerializerTest,
     SerializeValidMessage_ScenarioStartRequestMessage) {
  // Build a full, valid scenario start request message
  ScenarioStartRequestMessage message =
      ScenarioStartRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(80)
                         .SetTimestamp(1771000000000)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(5)
                           .SetApplication(10)
                           .SetEntityNumber(0)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(15)
                             .SetApplication(20)
                             .SetEntityNumber(0)
                             .Build())
          .SetRequestId(11223)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  std::vector<char> bytes = buffer.GetDataAt(buffer.GetLength());
  DIS::DataStream read_stream =
      DIS::DataStream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::ActionRequestPdu pdu = DIS::ActionRequestPdu();
  pdu.unmarshal(read_stream);

  // Check action type
  LONGS_EQUAL(
      MusicMessageTypeHelper::GetInfoForType(MusicMessageType::kScenarioStart)
          .action_type,
      pdu.getActionID());
}

TEST(Dis7MessageSerializerTest, SerializeInvalidMessageFails) {
  // Create a message with invalid data
  TransferConstructIdMessage message;

  // Set entity IDs with invalid values (negative site)
  EntityId invalid_old_id;
  invalid_old_id.SetSite(-1);  // Invalid site
  invalid_old_id.SetApplication(2);
  invalid_old_id.SetEntityNumber(100);

  message.SetOldId(invalid_old_id);

  // Attempt to validate should throw
  bool validation_failed = false;
  try {
    message.Validate();
  } catch (const std::runtime_error &e) {
    validation_failed = true;
  }

  CHECK_TRUE(validation_failed);
}

TEST(Dis7MessageSerializerTest,
     Test_SerializeConstructDataMessage_DataIsPresent) {
  ConstructDataMessage construct_message =
      ConstructDataMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(2)
                         .SetTimestamp(1000L)
                         .Build())
          .SetType(MusicMessageType::kConstructData)
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(1)
                           .SetApplication(2)
                           .SetSite(3)
                           .Build())
          .SetPrimaryControllerId(EntityIdBuilder()
                                      .SetEntityNumber(1)
                                      .SetApplication(1)
                                      .SetSite(1)
                                      .Build())
          .SetCurrentControllerId(EntityIdBuilder()
                                      .SetEntityNumber(1)
                                      .SetApplication(1)
                                      .SetSite(1)
                                      .Build())
          .SetConstructInformationRecord(
              ConstructInformationRecordBuilder()
                  .SetConstructCategoryType(ConstructCategoryType::kAggregate)
                  .SetConstructRenderType(
                      ConstructRenderType::kGhostedConstruct)
                  .Build())
          .SetGhostedId(EntityIdBuilder()
                            .SetEntityNumber(1)
                            .SetApplication(1)
                            .SetSite(1)
                            .Build())
          .SetCallsign("Test callsign")
          .SetConstructName("Test Construct Name")
          .SetInteractionRecord("test interaction record")
          .Build();

  Dis7MessageSerializer ser = Dis7MessageSerializer();

  MusicByteBuffer serialized_buffer =
      ser.SerializeMusicMessage(construct_message);
  CHECK_TRUE(serialized_buffer.GetLength() > 0);

  // Make sure it can also be deserialized
  DIS::Pdu base_pdu = DIS::Pdu();
  DIS::DataStream stream(
      serialized_buffer.GetDataAt(serialized_buffer.GetLength()).data(),
      serialized_buffer.GetLength(), DIS::Endian::BIG);
  base_pdu.unmarshal(stream);

  CHECK_EQUAL(base_pdu.getProtocolVersion(), 7);
}

TEST(Dis7MessageSerializerTest,
     Test_SerializeConstructInteractionRequestMessage_DataIsPresent) {
  MusicMessageHeader header;
  EntityId origin_id;
  EntityId receiver_id;
  ConstructInteractionRequestMessage test_mesg;
  header = MusicMessageHeaderBuilder()
               .SetExerciseId(3)
               .SetTimestamp(1234567890L)
               .Build();

  origin_id.SetSite(1);
  origin_id.SetApplication(2);
  origin_id.SetEntityNumber(3);

  receiver_id.SetSite(4);
  receiver_id.SetApplication(5);
  receiver_id.SetEntityNumber(6);

  test_mesg =
      ConstructInteractionRequestMessageBuilder()
          .SetHeader(header)
          .SetOriginId(origin_id)
          .SetReceiverId(receiver_id)
          .SetRequestId(1)
          .SetInteractionName("Test Interaction")
          .SetInteractionType(InteractionType::kRequest)
          .SetInteractionData(nlohmann::json{{"Key1", "Value1"}, {"Key2", 2}})
          .Build();

  Dis7MessageSerializer serializer = Dis7MessageSerializer();

  MusicByteBuffer buff = serializer.SerializeMusicMessage(test_mesg);
  DIS::ActionRequestPdu pdu = DIS::ActionRequestPdu();
  CHECK_TRUE(buff.GetLength() > 0);

  // Attempt to deserialize back to the DIS pdu
  DIS::DataStream stream =
      DIS::DataStream(buff.GetDataAt(buff.GetLength()).data(), buff.GetLength(),
                      DIS::Endian::BIG);
  pdu.unmarshal(stream);

  CHECK_EQUAL(7, pdu.getProtocolVersion());
  CHECK_EQUAL(2, pdu.getNumberOfVariableDatumRecords());
}

TEST(Dis7MessageSerializerTest,
     Test_SerializeConstructInteractionResponseMessage_DataIsPresent) {
  MusicMessageHeader header;
  EntityId origin_id;
  EntityId receiver_id;
  ConstructInteractionResponseMessage test_mesg;

  header = MusicMessageHeaderBuilder()
               .SetExerciseId(3)
               .SetTimestamp(1234567890L)
               .Build();

  origin_id.SetSite(1);
  origin_id.SetApplication(2);
  origin_id.SetEntityNumber(3);

  receiver_id.SetSite(4);
  receiver_id.SetApplication(5);
  receiver_id.SetEntityNumber(6);

  const nlohmann::json &optional_data =
      nlohmann::json{{"Optional data key", "Optional data value"}};

  test_mesg = ConstructInteractionResponseMessageBuilder()
                  .SetHeader(header)
                  .SetOriginId(origin_id)
                  .SetReceiverId(receiver_id)
                  .SetRequestId(1)
                  .SetRequestStatus(RequestStatus::kExecuting)
                  .SetOptionalData(optional_data)
                  .Build();

  Dis7MessageSerializer serializer = Dis7MessageSerializer();

  MusicByteBuffer buff = serializer.SerializeMusicMessage(test_mesg);
  DIS::ActionResponsePdu pdu = DIS::ActionResponsePdu();
  CHECK_TRUE(buff.GetLength() > 0);

  // Attempt to deserialize back to the DIS pdu
  DIS::DataStream stream =
      DIS::DataStream(buff.GetDataAt(buff.GetLength()).data(), buff.GetLength(),
                      DIS::Endian::BIG);
  pdu.unmarshal(stream);

  CHECK_EQUAL(7, pdu.getProtocolVersion());
  CHECK_EQUAL(1,
              pdu.getNumberOfVariableDatumRecords());  // Should have the
                                                       // 'optional' data field
}
}  // namespace music::test
