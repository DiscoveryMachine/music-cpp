// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <dis7/ActionRequestPdu.h>
#include <dis7/DataQueryPdu.h>
#include <dis7/Pdu.h>
#include <dis7/utils/DataStream.h>
#include <dis7/utils/Endian.h>

#include <asio/deferred.hpp>
#include <cstdlib>
#include <exception>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>

#include "config/config.h"
#include "enum/construct_category_type.h"
#include "enum/construct_render_type.h"
#include "enum/dead_reckoning_algorithm_type.h"
#include "enum/music_message_type.h"
#include "message/Orientation.h"
#include "message/command/entity/transfer_construct_id_message.h"
#include "message/command/scenario/create_environment_response_message.h"
#include "message/command/scenario/finalize_scenario_request_message.h"
#include "message/command/scenario/finalize_scenario_response_message.h"
#include "message/command/time/request_simulation_time_message.h"
#include "message/command/time/set_simulation_time_message.h"
#include "message/command/time/simulation_time_message.h"
#include "message/construct/construct_data_message.h"
#include "message/construct/damage.h"
#include "message/construct/dead_reckoning.h"
#include "message/control/construct_primary_control_response_message.h"
#include "message/control/control_regained_message.h"
#include "message/control/control_transfer_response_message.h"
#include "message/entity_id.h"
#include "message/interaction/construct_interaction_request_message.h"
#include "message/music_message_header.h"
#include "message/perception/perception_data_message.h"
#include "message/waypoint/waypoint_data_message.h"
#include "message/waypoint/waypoint_record.h"
#include "serialize/dis7_message_deserializer.h"
#include "serialize/dis7_message_serializer.h"
#include "util/music_byte_buffer.h"

// Include CppUTest last to avoid macro conflicts
#include <CppUTest/TestHarness.h>

namespace music::test {

namespace {

constexpr long long kMillisPerHour = 3600000LL;

long long MillisSinceHour(const long long epoch_millis) {
  const long long m = epoch_millis % kMillisPerHour;
  return m >= 0 ? m : (m + kMillisPerHour);
}

void CheckSameMillisSinceHour(const long long expected_epoch_millis,
                              const long long actual_epoch_millis,
                              const long long tolerance_millis = 2LL) {
  const long long expected = MillisSinceHour(expected_epoch_millis);
  const long long actual = MillisSinceHour(actual_epoch_millis);
  CHECK_TRUE(std::llabs(expected - actual) <= tolerance_millis);
}
}  // namespace

TEST_GROUP(Dis7MessageDeserializerTest){};

TEST(Dis7MessageDeserializerTest, Test_SerializeBasicMessage_DataPeekWorks) {
  // Build a full, valid create construct request message
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
                                      .SetEntityNumber(4)
                                      .SetApplication(5)
                                      .SetSite(6)
                                      .Build())
          .SetCurrentControllerId(EntityIdBuilder()
                                      .SetEntityNumber(7)
                                      .SetApplication(8)
                                      .SetSite(9)
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
          .SetCallsign("Test Callsign")
          .SetConstructName("Test Construct Name")
          .SetInteractionRecord(
              "land, fireWeaponAtTarget, fireWeaponAtLocation, moveToLocation, "
              "moveOnRoute, setEndOfRouteAction, setEquipmentQuantity, "
              "assignFormation")
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(construct_message);
  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);

  int protocol_version = (int)buffer.ReadValueSequential<unsigned char>();
  int exercise_id = (int)buffer.ReadValueSequential<unsigned char>();

  LONGS_EQUAL(7, protocol_version);
  LONGS_EQUAL(2, exercise_id);
}

TEST(Dis7MessageDeserializerTest, ConstructDataMessage_RoundTrip_Ghosted) {
  // Build a full, valid create construct request message
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
                                      .SetEntityNumber(4)
                                      .SetApplication(5)
                                      .SetSite(6)
                                      .Build())
          .SetCurrentControllerId(EntityIdBuilder()
                                      .SetEntityNumber(7)
                                      .SetApplication(8)
                                      .SetSite(9)
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
          .SetCallsign("Test")
          .SetConstructName("Test")
          .SetInteractionRecord("land")
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(construct_message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      72, buffer.GetLength());  // Ghosted Construct with 1B Callsign/Construct
                                // Name/Interaction Name is 72B

  // Deserialize the message
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto deserialized_construct_message =
      std::dynamic_pointer_cast<ConstructDataMessage>(parsed);
  CHECK_TRUE(deserialized_construct_message != nullptr);

  // Verify all fields match
  LONGS_EQUAL(MusicMessageType::kConstructData,
              static_cast<int>(deserialized_construct_message->GetType()));
  LONGS_EQUAL(2, deserialized_construct_message->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      1000L, deserialized_construct_message->GetHeader().GetTimestamp());
  LONGS_EQUAL(3, deserialized_construct_message->GetOriginId().GetSite());
  LONGS_EQUAL(2,
              deserialized_construct_message->GetOriginId().GetApplication());
  LONGS_EQUAL(1,
              deserialized_construct_message->GetOriginId().GetEntityNumber());
  // Primary Controller ID
  LONGS_EQUAL(
      6, deserialized_construct_message->GetPrimaryControllerId().GetSite());
  LONGS_EQUAL(5, deserialized_construct_message->GetPrimaryControllerId()
                     .GetApplication());
  LONGS_EQUAL(4, deserialized_construct_message->GetPrimaryControllerId()
                     .GetEntityNumber());
  // Current Controller ID
  LONGS_EQUAL(
      9, deserialized_construct_message->GetCurrentControllerId().GetSite());
  LONGS_EQUAL(8, deserialized_construct_message->GetCurrentControllerId()
                     .GetApplication());
  LONGS_EQUAL(7, deserialized_construct_message->GetCurrentControllerId()
                     .GetEntityNumber());
  // Construct Information Record
  LONGS_EQUAL(static_cast<int>(ConstructCategoryType::kAggregate),
              static_cast<int>(deserialized_construct_message
                                   ->GetConstructInformationRecord()
                                   .GetConstructCategory()));
  LONGS_EQUAL(static_cast<int>(ConstructRenderType::kGhostedConstruct),
              static_cast<int>(deserialized_construct_message
                                   ->GetConstructInformationRecord()
                                   .GetConstructRender()));
  // Ghosted ID
  LONGS_EQUAL(1, deserialized_construct_message->GetGhostedId()->GetSite());
  LONGS_EQUAL(1,
              deserialized_construct_message->GetGhostedId()->GetApplication());
  LONGS_EQUAL(
      1, deserialized_construct_message->GetGhostedId()->GetEntityNumber());
  // Callsign, Construct Name, Interaction Record
  STRCMP_EQUAL("Test", deserialized_construct_message->GetCallsign().c_str());
  STRCMP_EQUAL("Test",
               deserialized_construct_message->GetConstructName().c_str());
  STRCMP_EQUAL("land",
               deserialized_construct_message->GetInteractionRecord().c_str());
}

TEST(Dis7MessageDeserializerTest, ConstructDataMessage_RoundTrip_Physical) {
  ConstructDataMessage construct_message =
      ConstructDataMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(5)
                         .SetTimestamp(123456789L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(1)
                           .SetApplication(2)
                           .SetEntityNumber(3)
                           .Build())
          .SetPrimaryControllerId(EntityIdBuilder()
                                      .SetSite(4)
                                      .SetApplication(5)
                                      .SetEntityNumber(6)
                                      .Build())
          .SetCurrentControllerId(EntityIdBuilder()
                                      .SetSite(7)
                                      .SetApplication(8)
                                      .SetEntityNumber(9)
                                      .Build())
          .SetConstructInformationRecord(
              ConstructInformationRecordBuilder()
                  .SetConstructCategoryType(ConstructCategoryType::kAggregate)
                  .SetConstructRenderType(
                      ConstructRenderType::kRenderedPhysical)
                  .Build())
          .SetForce(Force::kOpposing)
          .SetEntityType(EntityTypeBuilder()
                             .SetKind(1)
                             .SetDomain(2)
                             .SetCountry(225)
                             .SetCategory(3)
                             .SetSubcategory(4)
                             .SetSpecific(5)
                             .SetExtra(6)
                             .Build())
          .SetLocation(LocationBuilder().SetX(1.0).SetY(2.0).SetZ(3.0).Build())
          .SetOrientation(OrientationBuilder()
                              .SetPhi(0.1)
                              .SetTheta(0.2)
                              .SetPsi(0.3)
                              .Build())
          .SetVelocity(VectorBuilder().SetX(1.0).SetY(2.0).SetZ(3.0).Build())
          .SetDeadReckoning(
              DeadReckoningBuilder()
                  .SetAlgorithmType(DeadReckoningAlgorithmType::kStatic)
                  .SetAngularVelocity(
                      VectorBuilder().SetX(4.0).SetY(5.0).SetZ(6.0).Build())
                  .SetLinearAcceleration(
                      VectorBuilder().SetX(7).SetY(8).SetZ(9).Build())
                  .Build())
          .SetDamage(MusicDamageBuilder()
                         .SetCatastrophicDamage(0.1)
                         .SetMobilityDamage(0.2)
                         .SetIsFirepowerDisabled(0)
                         .Build())
          .SetCallsign("Test")
          .SetConstructName("Test")
          .SetInteractionRecord("land")
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(construct_message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      176,
      buffer.GetLength());  // Rendered Physical Construct with 1B
                            // Callsign/Construct Name/Interaction Name is 176B

  // Deserialize the message
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto deserialized_construct_message =
      std::dynamic_pointer_cast<ConstructDataMessage>(parsed);
  CHECK_TRUE(deserialized_construct_message != nullptr);

  // Verify all fields match
  LONGS_EQUAL(5, deserialized_construct_message->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      123456789L, deserialized_construct_message->GetHeader().GetTimestamp());
  LONGS_EQUAL(1, deserialized_construct_message->GetOriginId().GetSite());
  LONGS_EQUAL(2,
              deserialized_construct_message->GetOriginId().GetApplication());
  LONGS_EQUAL(3,
              deserialized_construct_message->GetOriginId().GetEntityNumber());

  LONGS_EQUAL(
      4, deserialized_construct_message->GetPrimaryControllerId().GetSite());
  LONGS_EQUAL(5, deserialized_construct_message->GetPrimaryControllerId()
                     .GetApplication());
  LONGS_EQUAL(6, deserialized_construct_message->GetPrimaryControllerId()
                     .GetEntityNumber());

  LONGS_EQUAL(
      7, deserialized_construct_message->GetCurrentControllerId().GetSite());
  LONGS_EQUAL(8, deserialized_construct_message->GetCurrentControllerId()
                     .GetApplication());
  LONGS_EQUAL(9, deserialized_construct_message->GetCurrentControllerId()
                     .GetEntityNumber());

  LONGS_EQUAL(ConstructCategoryType::kAggregate,
              deserialized_construct_message->GetConstructInformationRecord()
                  .GetConstructCategory());
  LONGS_EQUAL(ConstructRenderType::kRenderedPhysical,
              deserialized_construct_message->GetConstructInformationRecord()
                  .GetConstructRender());
  LONGS_EQUAL(Force::kOpposing,
              deserialized_construct_message->GetForce().value());
  LONGS_EQUAL(
      1, deserialized_construct_message->GetEntityType().value().GetKind());
  LONGS_EQUAL(
      2, deserialized_construct_message->GetEntityType().value().GetDomain());
  LONGS_EQUAL(
      225,
      deserialized_construct_message->GetEntityType().value().GetCountry());
  LONGS_EQUAL(
      3, deserialized_construct_message->GetEntityType().value().GetCategory());
  LONGS_EQUAL(
      4,
      deserialized_construct_message->GetEntityType().value().GetSubcategory());
  LONGS_EQUAL(
      5, deserialized_construct_message->GetEntityType().value().GetSpecific());
  LONGS_EQUAL(
      6, deserialized_construct_message->GetEntityType().value().GetExtra());

  DOUBLES_EQUAL(1.0,
                deserialized_construct_message->GetLocation().value().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.0,
                deserialized_construct_message->GetLocation().value().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(3.0,
                deserialized_construct_message->GetLocation().value().GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(
      0.1, deserialized_construct_message->GetOrientation().value().GetPhi(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      0.2, deserialized_construct_message->GetOrientation().value().GetTheta(),
      kDoublePrecision);
  DOUBLES_EQUAL(
      0.3, deserialized_construct_message->GetOrientation().value().GetPsi(),
      kDoublePrecision);
  DOUBLES_EQUAL(1.0,
                deserialized_construct_message->GetVelocity().value().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.0,
                deserialized_construct_message->GetVelocity().value().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(3.0,
                deserialized_construct_message->GetVelocity().value().GetZ(),
                kDoublePrecision);
  LONGS_EQUAL(DeadReckoningAlgorithmType::kStatic,
              deserialized_construct_message->GetDeadReckoning()
                  .value()
                  .GetDeadReckAlgorithm());
  DOUBLES_EQUAL(4.0,
                deserialized_construct_message->GetDeadReckoning()
                    .value()
                    .GetAngularVelocity()
                    .GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(5.0,
                deserialized_construct_message->GetDeadReckoning()
                    .value()
                    .GetAngularVelocity()
                    .GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(6.0,
                deserialized_construct_message->GetDeadReckoning()
                    .value()
                    .GetAngularVelocity()
                    .GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(7.0,
                deserialized_construct_message->GetDeadReckoning()
                    .value()
                    .GetLinearAcceleration()
                    .GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(8.0,
                deserialized_construct_message->GetDeadReckoning()
                    .value()
                    .GetLinearAcceleration()
                    .GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(9.0,
                deserialized_construct_message->GetDeadReckoning()
                    .value()
                    .GetLinearAcceleration()
                    .GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(0.1,
                deserialized_construct_message->GetDamage()
                    .value()
                    .GetCatastrophicDamage(),
                kDoublePrecision);
  DOUBLES_EQUAL(
      0.2,
      deserialized_construct_message->GetDamage().value().GetMobilityDamage(),
      kDoublePrecision);
  CHECK_FALSE(deserialized_construct_message->GetDamage()
                  .value()
                  .IsFirepowerDisabled());
  STRCMP_EQUAL("Test", deserialized_construct_message->GetCallsign().c_str());
  STRCMP_EQUAL("Test",
               deserialized_construct_message->GetConstructName().c_str());
  STRCMP_EQUAL("land",
               deserialized_construct_message->GetInteractionRecord().c_str());
}

TEST(Dis7MessageDeserializerTest, EventMessage_RoundTrip) {
  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(5)
                                  .SetTimestamp(123456789L)
                                  .Build();

  EntityId origin_id =
      EntityIdBuilder().SetSite(1).SetApplication(2).SetEntityNumber(3).Build();

  std::string event_type = "TestEventType";

  nlohmann::json event_data =
      nlohmann::json::parse(R"({"key1":"value1","key2":42})");

  EventMessage event_message = EventMessageBuilder()
                                   .SetHeader(header)
                                   .SetOriginId(origin_id)
                                   .SetEventType(event_type)
                                   .SetEventData(event_data)
                                   .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(event_message);
  CHECK_TRUE(buffer.GetLength() > 0);

  // Deserialize the message
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto deserialized_event_message =
      std::dynamic_pointer_cast<EventMessage>(parsed);
  CHECK_TRUE(deserialized_event_message != nullptr);

  // Verify fields match
  LONGS_EQUAL(5, deserialized_event_message->GetHeader().GetExerciseId());
  LONGS_EQUAL(MusicMessageType::kEvent, deserialized_event_message->GetType());
  CheckSameMillisSinceHour(
      123456789L, deserialized_event_message->GetHeader().GetTimestamp());
  LONGS_EQUAL(1, deserialized_event_message->GetOriginId().GetSite());
  LONGS_EQUAL(2, deserialized_event_message->GetOriginId().GetApplication());
  LONGS_EQUAL(3, deserialized_event_message->GetOriginId().GetEntityNumber());
  STRCMP_EQUAL(event_type.c_str(),
               deserialized_event_message->GetEventType().c_str());
  STRCMP_EQUAL(event_data.dump().c_str(),
               deserialized_event_message->GetEventData().dump().c_str());
}

TEST(Dis7MessageDeserializerTest, CollisionMessage_RoundTrip) {
  Location location = LocationBuilder().SetX(5.0).SetY(10.0).SetZ(15.0).Build();

  Vector velocity = VectorBuilder().SetX(2.0).SetY(4.0).SetZ(6.0).Build();

  MusicEventId event_id =
      MusicEventIdBuilder()
          .SetSimulationAddress(
              SimulationAddressBuilder().SetSite(10).SetApplication(20).Build())
          .SetEventId(30)
          .Build();

  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(5)
                                  .SetTimestamp(123456789L)
                                  .Build();

  EntityId colliding_id = EntityIdBuilder()
                              .SetSite(10)
                              .SetApplication(20)
                              .SetEntityNumber(30)
                              .Build();

  EntityId issuing_id = EntityIdBuilder()
                            .SetSite(40)
                            .SetApplication(50)
                            .SetEntityNumber(60)
                            .Build();

  CollisionMessage collision_message =
      CollisionMessageBuilder()
          .SetHeader(header)
          .SetMass(2000.0)
          .SetCollisionType(CollisionType::kElastic)
          .SetCollisionLocation(location)
          .SetCollisionVelocity(velocity)
          .SetCollisionEventId(event_id)
          .SetCollidingId(colliding_id)
          .SetIssuingId(issuing_id)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(collision_message);
  CHECK_TRUE(buffer.GetLength() > 0);

  // Deserialize the message
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto deserialized_collision_message =
      std::dynamic_pointer_cast<CollisionMessage>(parsed);
  CHECK_TRUE(deserialized_collision_message != nullptr);

  // Verify fields match
  LONGS_EQUAL(MusicMessageType::kCollision,
              static_cast<int>(deserialized_collision_message->GetType()));
  LONGS_EQUAL(
      static_cast<int>(CollisionType::kElastic),
      static_cast<int>(deserialized_collision_message->GetCollisionType()));
  DOUBLES_EQUAL(2000.0, deserialized_collision_message->GetMass(),
                kDoublePrecision);
  DOUBLES_EQUAL(5.0,
                deserialized_collision_message->GetCollisionLocation().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(10.0,
                deserialized_collision_message->GetCollisionLocation().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(15.0,
                deserialized_collision_message->GetCollisionLocation().GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(2.0,
                deserialized_collision_message->GetCollisionVelocity().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(4.0,
                deserialized_collision_message->GetCollisionVelocity().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(6.0,
                deserialized_collision_message->GetCollisionVelocity().GetZ(),
                kDoublePrecision);
  LONGS_EQUAL(10, deserialized_collision_message->GetCollisionEventId()
                      .GetSimulationAddress()
                      .GetSite());
  LONGS_EQUAL(20, deserialized_collision_message->GetCollisionEventId()
                      .GetSimulationAddress()
                      .GetApplication());
  LONGS_EQUAL(
      30, deserialized_collision_message->GetCollisionEventId().GetEventId());
  LONGS_EQUAL(10, deserialized_collision_message->GetCollidingId().GetSite());
  LONGS_EQUAL(
      20, deserialized_collision_message->GetCollidingId().GetApplication());
  LONGS_EQUAL(
      30, deserialized_collision_message->GetCollidingId().GetEntityNumber());
  LONGS_EQUAL(40, deserialized_collision_message->GetIssuingId().GetSite());
  LONGS_EQUAL(50,
              deserialized_collision_message->GetIssuingId().GetApplication());
  LONGS_EQUAL(60,
              deserialized_collision_message->GetIssuingId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, ConstructControlRequestMessage_RoundTrip) {
  ConstructControlRequestMessage message =
      ConstructControlRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(1)
                         .SetTimestamp(1000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(4)
                           .SetApplication(5)
                           .SetEntityNumber(6)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(7)
                             .SetApplication(8)
                             .SetEntityNumber(9)
                             .Build())
          .SetRequestId(42)
          .SetTargetConstruct(EntityIdBuilder()
                                  .SetSite(10)
                                  .SetApplication(11)
                                  .SetEntityNumber(12)
                                  .Build())
          .SetContext("com.discoverymachine.context.Arrest")
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto control_request =
      std::dynamic_pointer_cast<ConstructControlRequestMessage>(parsed);
  CHECK_TRUE(control_request != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlRequest),
              static_cast<int>(control_request->GetType()));

  LONGS_EQUAL(1, control_request->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      1000LL,
      static_cast<long long>(control_request->GetHeader().GetTimestamp()));
  LONGS_EQUAL(42L, control_request->GetRequestId());
  LONGS_EQUAL(4, control_request->GetOriginId().GetSite());
  LONGS_EQUAL(5, control_request->GetOriginId().GetApplication());
  LONGS_EQUAL(6, control_request->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(7, control_request->GetReceiverId().GetSite());
  LONGS_EQUAL(8, control_request->GetReceiverId().GetApplication());
  LONGS_EQUAL(9, control_request->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(10, control_request->GetTargetConstruct().GetSite());
  LONGS_EQUAL(11, control_request->GetTargetConstruct().GetApplication());
  LONGS_EQUAL(12, control_request->GetTargetConstruct().GetEntityNumber());
  STRCMP_EQUAL("com.discoverymachine.context.Arrest",
               control_request->GetContext().c_str());
}

TEST(Dis7MessageDeserializerTest, ConstructControlResponseMessage_RoundTrip) {
  ConstructControlResponseMessage message =
      ConstructControlResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(10)
                         .SetTimestamp(2000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(13)
                           .SetApplication(14)
                           .SetEntityNumber(15)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(16)
                             .SetApplication(17)
                             .SetEntityNumber(18)
                             .Build())
          .SetRequestId(84)
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto control_response =
      std::dynamic_pointer_cast<ConstructControlResponseMessage>(parsed);
  CHECK_TRUE(control_response != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlResponse),
              static_cast<int>(control_response->GetType()));

  LONGS_EQUAL(10, control_response->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      2000LL,
      static_cast<long long>(control_response->GetHeader().GetTimestamp()));
  LONGS_EQUAL(84L, control_response->GetRequestId());
  LONGS_EQUAL(13, control_response->GetOriginId().GetSite());
  LONGS_EQUAL(14, control_response->GetOriginId().GetApplication());
  LONGS_EQUAL(15, control_response->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(16, control_response->GetReceiverId().GetSite());
  LONGS_EQUAL(17, control_response->GetReceiverId().GetApplication());
  LONGS_EQUAL(18, control_response->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kComplete),
              static_cast<int>(control_response->GetRequestStatus()));
}

TEST(Dis7MessageDeserializerTest, StateFieldMessage_RoundTrip) {
  nlohmann::json state_data = nlohmann::json::parse("{\"stateKey\":123}");
  StateFieldMessage message = StateFieldMessageBuilder()
                                  .SetHeader(MusicMessageHeaderBuilder()
                                                 .SetExerciseId(15)
                                                 .SetTimestamp(2500L)
                                                 .Build())
                                  .SetOriginId(EntityIdBuilder()
                                                   .SetSite(19)
                                                   .SetApplication(20)
                                                   .SetEntityNumber(21)
                                                   .Build())
                                  .SetStateData(state_data)
                                  .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(40, buffer.GetLength());  // State Field PDU takes 32B, assuming
                                        // 8B for state_data. This value would
                                        // take 16B which is +8B or 40B total

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto state_field_message =
      std::dynamic_pointer_cast<StateFieldMessage>(parsed);
  CHECK_TRUE(state_field_message != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kStateField),
              static_cast<int>(state_field_message->GetType()));

  LONGS_EQUAL(15, state_field_message->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      2500LL,
      static_cast<long long>(state_field_message->GetHeader().GetTimestamp()));
  LONGS_EQUAL(19, state_field_message->GetOriginId().GetSite());
  LONGS_EQUAL(20, state_field_message->GetOriginId().GetApplication());
  LONGS_EQUAL(21, state_field_message->GetOriginId().GetEntityNumber());
  STRCMP_EQUAL(state_data.dump().c_str(),
               state_field_message->GetStateData().dump().c_str());
}

TEST(Dis7MessageDeserializerTest,
     ConstructPrimaryControlRequestMessage_RoundTrip) {
  ConstructPrimaryControlRequestMessage message =
      ConstructPrimaryControlRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(20)
                         .SetTimestamp(3000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(21)
                           .SetApplication(22)
                           .SetEntityNumber(23)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(24)
                             .SetApplication(25)
                             .SetEntityNumber(26)
                             .Build())
          .SetRequestId(126)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40, buffer.GetLength());  // Primary Control Request Message PDU takes 40B

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto primary_control_request =
      std::dynamic_pointer_cast<ConstructPrimaryControlRequestMessage>(parsed);
  CHECK_TRUE(primary_control_request != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kPrimaryControlRequest),
              static_cast<int>(primary_control_request->GetType()));

  LONGS_EQUAL(20, primary_control_request->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      3000LL, static_cast<long long>(
                  primary_control_request->GetHeader().GetTimestamp()));
  LONGS_EQUAL(126L, primary_control_request->GetRequestId());
  LONGS_EQUAL(21, primary_control_request->GetOriginId().GetSite());
  LONGS_EQUAL(22, primary_control_request->GetOriginId().GetApplication());
  LONGS_EQUAL(23, primary_control_request->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(24, primary_control_request->GetReceiverId().GetSite());
  LONGS_EQUAL(25, primary_control_request->GetReceiverId().GetApplication());
  LONGS_EQUAL(26, primary_control_request->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest,
     ConstructPrimaryControlResponseMessage_RoundTrip) {
  ConstructPrimaryControlResponseMessage message =
      ConstructPrimaryControlResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(30)
                         .SetTimestamp(4000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(27)
                           .SetApplication(28)
                           .SetEntityNumber(29)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(30)
                             .SetApplication(31)
                             .SetEntityNumber(32)
                             .Build())
          .SetRequestId(168)
          .SetRequestStatus(RequestStatus::kPending)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      48,
      buffer.GetLength());  // Primary Control Response Message PDU takes 40B +
                            // 8B for our FixedDatum containing ActionID

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto primary_control_response =
      std::dynamic_pointer_cast<ConstructPrimaryControlResponseMessage>(parsed);
  CHECK_TRUE(primary_control_response != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kPrimaryControlResponse),
              static_cast<int>(primary_control_response->GetType()));

  LONGS_EQUAL(30, primary_control_response->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      4000LL, static_cast<long long>(
                  primary_control_response->GetHeader().GetTimestamp()));
  LONGS_EQUAL(168L, primary_control_response->GetRequestId());
  LONGS_EQUAL(27, primary_control_response->GetOriginId().GetSite());
  LONGS_EQUAL(28, primary_control_response->GetOriginId().GetApplication());
  LONGS_EQUAL(29, primary_control_response->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(30, primary_control_response->GetReceiverId().GetSite());
  LONGS_EQUAL(31, primary_control_response->GetReceiverId().GetApplication());
  LONGS_EQUAL(32, primary_control_response->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kPending),
              static_cast<int>(primary_control_response->GetRequestStatus()));
}

TEST(Dis7MessageDeserializerTest, DeserializeControlGrantedMessage_RoundTrip) {
  ControlGrantedMessage message = ControlGrantedMessageBuilder()
                                      .SetHeader(MusicMessageHeaderBuilder()
                                                     .SetExerciseId(40)
                                                     .SetTimestamp(5000L)
                                                     .Build())
                                      .SetOriginId(EntityIdBuilder()
                                                       .SetSite(33)
                                                       .SetApplication(34)
                                                       .SetEntityNumber(35)
                                                       .Build())
                                      .SetReceiverId(EntityIdBuilder()
                                                         .SetSite(36)
                                                         .SetApplication(37)
                                                         .SetEntityNumber(38)
                                                         .Build())
                                      .SetRequestId(210)
                                      .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40, buffer.GetLength());  // Primary Control Request Message PDU takes 40B

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);

  CHECK_TRUE(parsed != nullptr);
  const auto control_granted =
      std::dynamic_pointer_cast<ControlGrantedMessage>(parsed);
  // Check each field
  CHECK_TRUE(control_granted != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlGranted),
              static_cast<int>(control_granted->GetType()));
  LONGS_EQUAL(40, control_granted->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      5000LL,
      static_cast<long long>(control_granted->GetHeader().GetTimestamp()));
  LONGS_EQUAL(210L, control_granted->GetRequestId());
  LONGS_EQUAL(33, control_granted->GetOriginId().GetSite());
  LONGS_EQUAL(34, control_granted->GetOriginId().GetApplication());
  LONGS_EQUAL(35, control_granted->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(36, control_granted->GetReceiverId().GetSite());
  LONGS_EQUAL(37, control_granted->GetReceiverId().GetApplication());
  LONGS_EQUAL(38, control_granted->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, ControlLostMessage_RoundTrip) {
  ControlLostMessage message = ControlLostMessageBuilder()
                                   .SetHeader(MusicMessageHeaderBuilder()
                                                  .SetExerciseId(50)
                                                  .SetTimestamp(6000L)
                                                  .Build())
                                   .SetOriginId(EntityIdBuilder()
                                                    .SetSite(39)
                                                    .SetApplication(40)
                                                    .SetEntityNumber(41)
                                                    .Build())
                                   .SetReceiverId(EntityIdBuilder()
                                                      .SetSite(42)
                                                      .SetApplication(43)
                                                      .SetEntityNumber(44)
                                                      .Build())
                                   .SetRequestId(252)
                                   .Build();
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  CHECK_TRUE(buffer.GetLength() > 0);
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto control_lost =
      std::dynamic_pointer_cast<ControlLostMessage>(parsed);
  CHECK_TRUE(control_lost != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlLost),
              static_cast<int>(control_lost->GetType()));
  LONGS_EQUAL(50, control_lost->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      6000LL, static_cast<long long>(control_lost->GetHeader().GetTimestamp()));
  LONGS_EQUAL(252L, control_lost->GetRequestId());
  LONGS_EQUAL(39, control_lost->GetOriginId().GetSite());
  LONGS_EQUAL(40, control_lost->GetOriginId().GetApplication());
  LONGS_EQUAL(41, control_lost->GetOriginId().GetEntityNumber());

  LONGS_EQUAL(42, control_lost->GetReceiverId().GetSite());
  LONGS_EQUAL(43, control_lost->GetReceiverId().GetApplication());
  LONGS_EQUAL(44, control_lost->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, ControlReclamationMessage_RoundTrip) {
  ControlReclamationMessage message =
      ControlReclamationMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(60)
                         .SetTimestamp(7000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(45)
                           .SetApplication(46)
                           .SetEntityNumber(47)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(48)
                             .SetApplication(49)
                             .SetEntityNumber(50)
                             .Build())
          .SetRequestId(294)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40,
      buffer.GetLength());  // Primary Control Reclamation Message PDU takes 40B
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto control_reclamation =
      std::dynamic_pointer_cast<ControlReclamationMessage>(parsed);
  CHECK_TRUE(control_reclamation != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlReclamation),
              static_cast<int>(control_reclamation->GetType()));
  LONGS_EQUAL(60, control_reclamation->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      7000LL,
      static_cast<long long>(control_reclamation->GetHeader().GetTimestamp()));
  LONGS_EQUAL(294L, control_reclamation->GetRequestId());
  LONGS_EQUAL(45, control_reclamation->GetOriginId().GetSite());
  LONGS_EQUAL(46, control_reclamation->GetOriginId().GetApplication());
  LONGS_EQUAL(47, control_reclamation->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(48, control_reclamation->GetReceiverId().GetSite());
  LONGS_EQUAL(49, control_reclamation->GetReceiverId().GetApplication());
  LONGS_EQUAL(50, control_reclamation->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, ControlRegainedMessage_RoundTrip) {
  ControlRegainedMessage message = ControlRegainedMessageBuilder()
                                       .SetHeader(MusicMessageHeaderBuilder()
                                                      .SetExerciseId(70)
                                                      .SetTimestamp(8000L)
                                                      .Build())
                                       .SetOriginId(EntityIdBuilder()
                                                        .SetSite(51)
                                                        .SetApplication(52)
                                                        .SetEntityNumber(53)
                                                        .Build())
                                       .SetReceiverId(EntityIdBuilder()
                                                          .SetSite(54)
                                                          .SetApplication(55)
                                                          .SetEntityNumber(56)
                                                          .Build())
                                       .SetRequestId(336)
                                       .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40,
      buffer.GetLength());  // Primary Control Regained Message PDU takes 40B
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto control_regained =
      std::dynamic_pointer_cast<ControlRegainedMessage>(parsed);
  CHECK_TRUE(control_regained != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlRegained),
              static_cast<int>(control_regained->GetType()));
  LONGS_EQUAL(70, control_regained->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      8000LL,
      static_cast<long long>(control_regained->GetHeader().GetTimestamp()));
  LONGS_EQUAL(336L, control_regained->GetRequestId());
  LONGS_EQUAL(51, control_regained->GetOriginId().GetSite());
  LONGS_EQUAL(52, control_regained->GetOriginId().GetApplication());
  LONGS_EQUAL(53, control_regained->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(54, control_regained->GetReceiverId().GetSite());
  LONGS_EQUAL(55, control_regained->GetReceiverId().GetApplication());
  LONGS_EQUAL(56, control_regained->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, ControlRelinquishedMessage_RoundTrip) {
  ControlRelinquishedMessage message =
      ControlRelinquishedMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(80)
                         .SetTimestamp(9000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(57)
                           .SetApplication(58)
                           .SetEntityNumber(59)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(60)
                             .SetApplication(61)
                             .SetEntityNumber(62)
                             .Build())
          .SetRequestId(378)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40,
      buffer
          .GetLength());  // Primary Control Relinquished Message PDU takes 40B
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto control_relinquished =
      std::dynamic_pointer_cast<ControlRelinquishedMessage>(parsed);
  CHECK_TRUE(control_relinquished != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlRelinquished),
              static_cast<int>(control_relinquished->GetType()));
  LONGS_EQUAL(80, control_relinquished->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      9000LL,
      static_cast<long long>(control_relinquished->GetHeader().GetTimestamp()));
  LONGS_EQUAL(378L, control_relinquished->GetRequestId());
  LONGS_EQUAL(57, control_relinquished->GetOriginId().GetSite());
  LONGS_EQUAL(58, control_relinquished->GetOriginId().GetApplication());
  LONGS_EQUAL(59, control_relinquished->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(60, control_relinquished->GetReceiverId().GetSite());
  LONGS_EQUAL(61, control_relinquished->GetReceiverId().GetApplication());
  LONGS_EQUAL(62, control_relinquished->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, ControlTransferGrantedMessage_RoundTrip) {
  ControlTransferGrantedMessage message =
      ControlTransferGrantedMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(90)
                         .SetTimestamp(10000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(63)
                           .SetApplication(64)
                           .SetEntityNumber(65)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(66)
                             .SetApplication(67)
                             .SetEntityNumber(68)
                             .Build())
          .SetRequestId(420)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(40,
              buffer.GetLength());  // Primary Control Transfer Granted Message
                                    // PDU takes 40B
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto control_transfer_granted =
      std::dynamic_pointer_cast<ControlTransferGrantedMessage>(parsed);
  CHECK_TRUE(control_transfer_granted != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlTransferGranted),
              static_cast<int>(control_transfer_granted->GetType()));
  LONGS_EQUAL(90, control_transfer_granted->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      10000LL, static_cast<long long>(
                   control_transfer_granted->GetHeader().GetTimestamp()));
  LONGS_EQUAL(420L, control_transfer_granted->GetRequestId());
  LONGS_EQUAL(63, control_transfer_granted->GetOriginId().GetSite());
  LONGS_EQUAL(64, control_transfer_granted->GetOriginId().GetApplication());
  LONGS_EQUAL(65, control_transfer_granted->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(66, control_transfer_granted->GetReceiverId().GetSite());
  LONGS_EQUAL(67, control_transfer_granted->GetReceiverId().GetApplication());
  LONGS_EQUAL(68, control_transfer_granted->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, ControlTransferRequestMessage_RoundTrip) {
  ControlTransferRequestMessage message =
      ControlTransferRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(100)
                         .SetTimestamp(11000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(69)
                           .SetApplication(70)
                           .SetEntityNumber(71)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(72)
                             .SetApplication(73)
                             .SetEntityNumber(74)
                             .Build())
          .SetTargetConstruct(EntityIdBuilder()
                                  .SetSite(75)
                                  .SetApplication(76)
                                  .SetEntityNumber(77)
                                  .Build())
          .SetProposedController(EntityIdBuilder()
                                     .SetSite(78)
                                     .SetApplication(79)
                                     .SetEntityNumber(80)
                                     .Build())
          .SetContext(nlohmann::json::parse(
              "{\"context\":\"com.discoverymachine.context.Arrest\"}"))
          .SetRequestId(462)
          .Build();

  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Validation should not throw an exception after setting "
                      "all required fields: ") +
          e.what())
             .c_str());
  }
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto control_transfer_request =
      std::dynamic_pointer_cast<ControlTransferRequestMessage>(parsed);
  CHECK_TRUE(control_transfer_request != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlTransferRequest),
              static_cast<int>(control_transfer_request->GetType()));
  LONGS_EQUAL(100, control_transfer_request->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      11000LL, static_cast<long long>(
                   control_transfer_request->GetHeader().GetTimestamp()));
  LONGS_EQUAL(462L, control_transfer_request->GetRequestId());
  LONGS_EQUAL(69, control_transfer_request->GetOriginId().GetSite());
  LONGS_EQUAL(70, control_transfer_request->GetOriginId().GetApplication());
  LONGS_EQUAL(71, control_transfer_request->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(72, control_transfer_request->GetReceiverId().GetSite());
  LONGS_EQUAL(73, control_transfer_request->GetReceiverId().GetApplication());
  LONGS_EQUAL(74, control_transfer_request->GetReceiverId().GetEntityNumber());
  STRCMP_EQUAL("{\"context\":\"com.discoverymachine.context.Arrest\"}",
               control_transfer_request->GetContext().dump().c_str());
  LONGS_EQUAL(75, control_transfer_request->GetTargetConstruct().GetSite());
  LONGS_EQUAL(76,
              control_transfer_request->GetTargetConstruct().GetApplication());
  LONGS_EQUAL(77,
              control_transfer_request->GetTargetConstruct().GetEntityNumber());
  LONGS_EQUAL(78, control_transfer_request->GetProposedController().GetSite());
  LONGS_EQUAL(
      79, control_transfer_request->GetProposedController().GetApplication());
  LONGS_EQUAL(
      80, control_transfer_request->GetProposedController().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, ControlTransferResponseMessage_RoundTrip) {
  ControlTransferResponseMessage message =
      ControlTransferResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(110)
                         .SetTimestamp(12000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(75)
                           .SetApplication(76)
                           .SetEntityNumber(77)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(78)
                             .SetApplication(79)
                             .SetEntityNumber(80)
                             .Build())
          .SetRequestId(504)
          .SetRequestStatus(RequestStatus::kAborted)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto control_transfer_response =
      std::dynamic_pointer_cast<ControlTransferResponseMessage>(parsed);
  CHECK_TRUE(control_transfer_response != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kControlTransferResponse),
              static_cast<int>(control_transfer_response->GetType()));
  LONGS_EQUAL(110, control_transfer_response->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      12000LL, static_cast<long long>(
                   control_transfer_response->GetHeader().GetTimestamp()));
  LONGS_EQUAL(504L, control_transfer_response->GetRequestId());
  LONGS_EQUAL(75, control_transfer_response->GetOriginId().GetSite());
  LONGS_EQUAL(76, control_transfer_response->GetOriginId().GetApplication());
  LONGS_EQUAL(77, control_transfer_response->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(78, control_transfer_response->GetReceiverId().GetSite());
  LONGS_EQUAL(79, control_transfer_response->GetReceiverId().GetApplication());
  LONGS_EQUAL(80, control_transfer_response->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kAborted),
              static_cast<int>(control_transfer_response->GetRequestStatus()));
}

TEST(Dis7MessageDeserializerTest, PrimaryControlGrantedMessage_RoundTrip) {
  PrimaryControlGrantedMessage message =
      PrimaryControlGrantedMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(120)
                         .SetTimestamp(13000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(81)
                           .SetApplication(82)
                           .SetEntityNumber(83)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(84)
                             .SetApplication(85)
                             .SetEntityNumber(86)
                             .Build())
          .SetRequestId(546)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40,
      buffer.GetLength());  // Primary Control Granted Message PDU takes 40B
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto primary_control_granted =
      std::dynamic_pointer_cast<PrimaryControlGrantedMessage>(parsed);
  CHECK_TRUE(primary_control_granted != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kPrimaryControlGranted),
              static_cast<int>(primary_control_granted->GetType()));
  LONGS_EQUAL(120, primary_control_granted->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      13000LL, static_cast<long long>(
                   primary_control_granted->GetHeader().GetTimestamp()));
  LONGS_EQUAL(546L, primary_control_granted->GetRequestId());
  LONGS_EQUAL(81, primary_control_granted->GetOriginId().GetSite());
  LONGS_EQUAL(82, primary_control_granted->GetOriginId().GetApplication());
  LONGS_EQUAL(83, primary_control_granted->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(84, primary_control_granted->GetReceiverId().GetSite());
  LONGS_EQUAL(85, primary_control_granted->GetReceiverId().GetApplication());
  LONGS_EQUAL(86, primary_control_granted->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, PrimaryControlRelinquishedMessage_RoundTrip) {
  PrimaryControlRelinquishedMessage message =
      PrimaryControlRelinquishedMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(130)
                         .SetTimestamp(14000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(87)
                           .SetApplication(88)
                           .SetEntityNumber(89)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(90)
                             .SetApplication(91)
                             .SetEntityNumber(92)
                             .Build())
          .SetRequestId(588)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(40, buffer.GetLength());  // Primary Control Relinquished Message
                                        // PDU takes 40B
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto primary_control_relinquished =
      std::dynamic_pointer_cast<PrimaryControlRelinquishedMessage>(parsed);
  CHECK_TRUE(primary_control_relinquished != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kPrimaryControlRelinquished),
              static_cast<int>(primary_control_relinquished->GetType()));
  LONGS_EQUAL(130, primary_control_relinquished->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      14000LL, static_cast<long long>(
                   primary_control_relinquished->GetHeader().GetTimestamp()));
  LONGS_EQUAL(588L, primary_control_relinquished->GetRequestId());
  LONGS_EQUAL(87, primary_control_relinquished->GetOriginId().GetSite());
  LONGS_EQUAL(88, primary_control_relinquished->GetOriginId().GetApplication());
  LONGS_EQUAL(89,
              primary_control_relinquished->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(90, primary_control_relinquished->GetReceiverId().GetSite());
  LONGS_EQUAL(91,
              primary_control_relinquished->GetReceiverId().GetApplication());
  LONGS_EQUAL(92,
              primary_control_relinquished->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, SetCurrentControllerMessage_RoundTrip) {
  SetCurrentControllerMessage message =
      SetCurrentControllerMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(140)
                         .SetTimestamp(15000L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(93)
                           .SetApplication(94)
                           .SetEntityNumber(95)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetSite(96)
                             .SetApplication(97)
                             .SetEntityNumber(98)
                             .Build())
          .SetRequestId(630)
          .SetCurrentControllerId(EntityIdBuilder()
                                      .SetSite(99)
                                      .SetApplication(100)
                                      .SetEntityNumber(101)
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

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      56, buffer.GetLength());  // Set Current Controller Message PDU takes 56B
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);
  const auto set_current_controller =
      std::dynamic_pointer_cast<SetCurrentControllerMessage>(parsed);
  CHECK_TRUE(set_current_controller != nullptr);

  // Check each field
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kSetCurrentController),
              static_cast<int>(set_current_controller->GetType()));
  LONGS_EQUAL(140, set_current_controller->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      15000LL, static_cast<long long>(
                   set_current_controller->GetHeader().GetTimestamp()));
  LONGS_EQUAL(630L, set_current_controller->GetRequestId());
  LONGS_EQUAL(93, set_current_controller->GetOriginId().GetSite());
  LONGS_EQUAL(94, set_current_controller->GetOriginId().GetApplication());
  LONGS_EQUAL(95, set_current_controller->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(96, set_current_controller->GetReceiverId().GetSite());
  LONGS_EQUAL(97, set_current_controller->GetReceiverId().GetApplication());
  LONGS_EQUAL(98, set_current_controller->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(99, set_current_controller->GetCurrentControllerId().GetSite());
  LONGS_EQUAL(
      100, set_current_controller->GetCurrentControllerId().GetApplication());
  LONGS_EQUAL(
      101, set_current_controller->GetCurrentControllerId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest, DeserializeTransferConstructId_RoundTrip) {
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

  EntityId origin_id = EntityIdBuilder()
                           .SetSite(10)
                           .SetApplication(20)
                           .SetEntityNumber(0)
                           .Build();
  EntityId receiver_id = EntityIdBuilder()
                             .SetSite(30)
                             .SetApplication(40)
                             .SetEntityNumber(0)
                             .Build();

  TransferConstructIdMessage message =
      TransferConstructIdMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(200)
                         .SetTimestamp(1234L)
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
  LONGS_EQUAL(
      72, buffer.GetLength());  // Transfer Construct ID Message PDU takes 72B

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto transfer =
      std::dynamic_pointer_cast<TransferConstructIdMessage>(parsed);
  CHECK_TRUE(transfer != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kTransferConstructId),
              static_cast<int>(transfer->GetType()));

  // Check all fields
  LONGS_EQUAL(MusicMessageType::kTransferConstructId,
              static_cast<int>(transfer->GetType()));
  LONGS_EQUAL(200, transfer->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      1234LL, static_cast<long long>(transfer->GetHeader().GetTimestamp()));
  LONGS_EQUAL(12345L, transfer->GetRequestId());
  LONGS_EQUAL(origin_id.GetSite(), transfer->GetOriginId().GetSite());
  LONGS_EQUAL(origin_id.GetApplication(),
              transfer->GetOriginId().GetApplication());
  LONGS_EQUAL(origin_id.GetEntityNumber(),
              transfer->GetOriginId().GetEntityNumber());

  // Receiver ID should be all 1s
  LONGS_EQUAL(0xFFFF, transfer->GetReceiverId().GetSite());
  LONGS_EQUAL(0xFFFF, transfer->GetReceiverId().GetApplication());
  LONGS_EQUAL(0xFFFF, transfer->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(1, transfer->GetOldId().GetSite());
  LONGS_EQUAL(2, transfer->GetOldId().GetApplication());
  LONGS_EQUAL(100, transfer->GetOldId().GetEntityNumber());
  LONGS_EQUAL(1, transfer->GetNewId().GetSite());
  LONGS_EQUAL(2, transfer->GetNewId().GetApplication());
  LONGS_EQUAL(200, transfer->GetNewId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest,
     DeserializeConstructInteractionRequest_RoundTrip) {
  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(3)
                                  .SetTimestamp(1234567890L)
                                  .Build();

  EntityId origin_id =
      EntityIdBuilder().SetSite(1).SetApplication(2).SetEntityNumber(3).Build();
  EntityId receiver_id =
      EntityIdBuilder().SetSite(4).SetApplication(5).SetEntityNumber(6).Build();
  nlohmann::json interaction_data = {{"Test Key", "Test Value"}};

  ConstructInteractionRequestMessage message =
      ConstructInteractionRequestMessageBuilder()
          .SetHeader(header)
          .SetOriginId(origin_id)
          .SetReceiverId(receiver_id)
          .SetRequestId(1)
          .SetInteractionName("Test Interaction")
          .SetInteractionType(InteractionType::kRequest)
          .SetInteractionData(interaction_data)
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(104, buffer.GetLength());  // Construct Interaction Request
                                         // Message PDU takes 72B + 32B for an
                                         // extra 16B of Interaction Name and
                                         // 16B of Interaction Data, or 104 B

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto interaction =
      std::dynamic_pointer_cast<ConstructInteractionRequestMessage>(parsed);
  CHECK_TRUE(interaction != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kConstructInteractionRequest),
              static_cast<int>(interaction->GetType()));

  // Verify all fields
  LONGS_EQUAL(MusicMessageType::kConstructInteractionRequest,
              static_cast<int>(interaction->GetType()));
  LONGS_EQUAL(3, interaction->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      1234567890LL,
      static_cast<long long>(interaction->GetHeader().GetTimestamp()));
  LONGS_EQUAL(1L, interaction->GetRequestId());
  LONGS_EQUAL(origin_id.GetSite(), interaction->GetOriginId().GetSite());
  LONGS_EQUAL(origin_id.GetApplication(),
              interaction->GetOriginId().GetApplication());
  LONGS_EQUAL(origin_id.GetEntityNumber(),
              interaction->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(receiver_id.GetSite(), interaction->GetReceiverId().GetSite());
  LONGS_EQUAL(receiver_id.GetApplication(),
              interaction->GetReceiverId().GetApplication());
  LONGS_EQUAL(receiver_id.GetEntityNumber(),
              interaction->GetReceiverId().GetEntityNumber());
  STRCMP_EQUAL("Test Interaction", interaction->GetInteractionName().c_str());
  LONGS_EQUAL(static_cast<int>(InteractionType::kRequest),
              static_cast<int>(interaction->GetInteractionType()));
  nlohmann::json expected_data = {{"Test Key", "Test Value"}};
  STRCMP_EQUAL(expected_data.dump().c_str(),
               interaction->GetInteractionData().dump().c_str());
}

TEST(Dis7MessageDeserializerTest, CreateConstructRequestMessage_RoundTrip) {
  MusicMessageHeader header = MusicMessageHeaderBuilder()
                                  .SetExerciseId(5)
                                  .SetTimestamp(123456789L)
                                  .Build();

  EntityId origin_id = EntityIdBuilder()
                           .SetSite(11)
                           .SetApplication(22)
                           .SetEntityNumber(33)
                           .Build();
  EntityId receiver_id = EntityIdBuilder()
                             .SetSite(44)
                             .SetApplication(55)
                             .SetEntityNumber(66)
                             .Build();

  CreateConstructRequestMessage message =
      CreateConstructRequestMessageBuilder()
          .SetHeader(header)
          .SetOriginId(origin_id)
          .SetReceiverId(receiver_id)
          .SetRequestId(2)
          .SetConstructType("Helicopter")
          .SetConstructCallsign("Eagle One")
          .SetConstructLocation(
              LocationBuilder().SetX(100.0).SetY(200.0).SetZ(50.0).Build())
          .SetConstructOrientation(OrientationBuilder()
                                       .SetPsi(1.0)
                                       .SetTheta(0.0)
                                       .SetPhi(3.0)
                                       .Build())
          .Build();

  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      136,
      buffer.GetLength());  // Create Construct Request Message PDU takes 104B +
                            // 32B for an extra 16B of Construct Type (over 8B)
                            // and 16B of Construct Callsign (over 8B), or 136 B

  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto construct_request =
      std::dynamic_pointer_cast<CreateConstructRequestMessage>(parsed);
  CHECK_TRUE(construct_request != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateConstructRequest),
              static_cast<int>(construct_request->GetType()));

  // Verify all fields
  LONGS_EQUAL(5, construct_request->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      123456789LL,
      static_cast<long long>(construct_request->GetHeader().GetTimestamp()));
  LONGS_EQUAL(2L, construct_request->GetRequestId());
  LONGS_EQUAL(11, construct_request->GetOriginId().GetSite());
  LONGS_EQUAL(22, construct_request->GetOriginId().GetApplication());
  LONGS_EQUAL(0, construct_request->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(44, construct_request->GetReceiverId().GetSite());
  LONGS_EQUAL(55, construct_request->GetReceiverId().GetApplication());
  LONGS_EQUAL(0, construct_request->GetReceiverId().GetEntityNumber());
  STRCMP_EQUAL("Helicopter", construct_request->GetConstructType().c_str());
  STRCMP_EQUAL("Eagle One", construct_request->GetConstructCallsign().c_str());
  DOUBLES_EQUAL(100.0, construct_request->GetConstructLocation().value().GetX(),
                kDoublePrecision);
  DOUBLES_EQUAL(200.0, construct_request->GetConstructLocation().value().GetY(),
                kDoublePrecision);
  DOUBLES_EQUAL(50.0, construct_request->GetConstructLocation().value().GetZ(),
                kDoublePrecision);
  DOUBLES_EQUAL(1.0,
                construct_request->GetConstructOrientation().value().GetPsi(),
                kDoublePrecision);
  DOUBLES_EQUAL(0.0,
                construct_request->GetConstructOrientation().value().GetTheta(),
                kDoublePrecision);
  DOUBLES_EQUAL(3.0,
                construct_request->GetConstructOrientation().value().GetPhi(),
                kDoublePrecision);
}

TEST(Dis7MessageDeserializerTest, CreateEnvironmentRequestMessage_RoundTrip) {
  // Build a full, valid create environment request message
  nlohmann::json metadata = {{"weather", "sunny"},
                             {"event ", "Bilbo Baggins' birthday"}};
  CreateEnvironmentRequestMessage message =
      CreateEnvironmentRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(7)
                         .SetTimestamp(123456789L)
                         .Build())
          .SetEnvironmentName("The Shire")
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(50)
                           .SetApplication(40)
                           .SetSite(20)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(20)
                             .SetApplication(50)
                             .SetSite(20)
                             .Build())
          .SetRequestId(3)
          .SetEnvironmentMetadata(metadata)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(128,
              buffer.GetLength());  // Create Environment Request Message PDU
                                    // takes 72B + 40B for the json metadata and
                                    // +16B for the Environment Name = 128B

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto environment_request =
      std::dynamic_pointer_cast<CreateEnvironmentRequestMessage>(parsed);
  CHECK_TRUE(environment_request != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateEnvironmentRequest),
              static_cast<int>(environment_request->GetType()));

  // Verify all fields
  LONGS_EQUAL(7, environment_request->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      123456789LL,
      static_cast<long long>(environment_request->GetHeader().GetTimestamp()));
  LONGS_EQUAL(3L, environment_request->GetRequestId());
  LONGS_EQUAL(20, environment_request->GetOriginId().GetSite());
  LONGS_EQUAL(40, environment_request->GetOriginId().GetApplication());
  // The origin entity number should always be 0 since the system itself sends
  // the message.
  LONGS_EQUAL(0, environment_request->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(20, environment_request->GetReceiverId().GetSite());
  LONGS_EQUAL(50, environment_request->GetReceiverId().GetApplication());
  // The receiver entity number should always be 0 since the target is the
  // system itself.
  LONGS_EQUAL(0, environment_request->GetReceiverId().GetEntityNumber());
  STRCMP_EQUAL("The Shire", environment_request->GetEnvironmentName().c_str());
  STRCMP_EQUAL("sunny", environment_request->GetEnvironmentMetadata()
                            .at("weather")
                            .get<std::string>()
                            .c_str());
  STRCMP_EQUAL("Bilbo Baggins' birthday",
               environment_request->GetEnvironmentMetadata()
                   .at("event ")
                   .get<std::string>()
                   .c_str());
}

TEST(Dis7MessageDeserializerTest, FinalizeScenarioRequestMessage_RoundTrip) {
  // Build a full, valid finalize scenario request message
  FinalizeScenarioRequestMessage message =
      FinalizeScenarioRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(9)
                         .SetTimestamp(987654321L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(60)
                           .SetApplication(70)
                           .SetSite(80)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(80)
                             .SetApplication(70)
                             .SetSite(60)
                             .Build())
          .SetRequestId(4)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40,
      buffer.GetLength());  // Finalize Scenario Request Message PDU takes 40B

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto finalize_request =
      std::dynamic_pointer_cast<FinalizeScenarioRequestMessage>(parsed);
  CHECK_TRUE(finalize_request != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kFinalizeScenarioRequest),
              static_cast<int>(finalize_request->GetType()));

  // Verify all fields
  LONGS_EQUAL(9, finalize_request->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      987654321LL,
      static_cast<long long>(finalize_request->GetHeader().GetTimestamp()));
  LONGS_EQUAL(4L, finalize_request->GetRequestId());
  LONGS_EQUAL(80, finalize_request->GetOriginId().GetSite());
  LONGS_EQUAL(70, finalize_request->GetOriginId().GetApplication());
  LONGS_EQUAL(0, finalize_request->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(60, finalize_request->GetReceiverId().GetSite());
  LONGS_EQUAL(70, finalize_request->GetReceiverId().GetApplication());
  LONGS_EQUAL(0, finalize_request->GetReceiverId().GetEntityNumber());
}

TEST(Dis7MessageDeserializerTest,
     ParameterizeConstructRequestMessage_RoundTrip) {
  // Build a full, valid parameterize construct request message
  ParameterizeConstructRequestMessage message =
      ParameterizeConstructRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(11)
                         .SetTimestamp(192837465L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(100)
                           .SetSite(110)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(110)
                             .SetApplication(100)
                             .SetSite(90)
                             .Build())
          .SetRequestId(5)
          .SetConstructParameters(
              nlohmann::json::parse((R"({"param1":"testValue","param2":42})")))
          .SetGhostedId(EntityIdBuilder()
                            .SetEntityNumber(25)
                            .SetApplication(150)
                            .SetSite(120)
                            .Build())
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      104, buffer.GetLength());  // Parameterize Construct Request
                                 // Message PDU takes 72B + 32B more for a
                                 // longer Construct Parameters (above 8B of
                                 // data, padded to the next 8B interval) = 104B

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto parameterize_request =
      std::dynamic_pointer_cast<ParameterizeConstructRequestMessage>(parsed);
  CHECK_TRUE(parameterize_request != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kParameterizeConstructRequest),
              static_cast<int>(parameterize_request->GetType()));

  // Verify all fields
  LONGS_EQUAL(11, parameterize_request->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      192837465LL,
      static_cast<long long>(parameterize_request->GetHeader().GetTimestamp()));
  LONGS_EQUAL(5L, parameterize_request->GetRequestId());
  LONGS_EQUAL(110, parameterize_request->GetOriginId().GetSite());
  LONGS_EQUAL(100, parameterize_request->GetOriginId().GetApplication());
  LONGS_EQUAL(0, parameterize_request->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(90, parameterize_request->GetReceiverId().GetSite());
  LONGS_EQUAL(100, parameterize_request->GetReceiverId().GetApplication());
  LONGS_EQUAL(110, parameterize_request->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(120, parameterize_request->GetGhostedId().value().GetSite());
  LONGS_EQUAL(150,
              parameterize_request->GetGhostedId().value().GetApplication());
  LONGS_EQUAL(25,
              parameterize_request->GetGhostedId().value().GetEntityNumber());
  STRCMP_EQUAL("testValue", parameterize_request->GetConstructParameters()
                                .at("param1")
                                .get<std::string>()
                                .c_str());
  LONGS_EQUAL(
      42,
      parameterize_request->GetConstructParameters().at("param2").get<int>());
}

TEST(Dis7MessageDeserializerTest, ScenarioStartRequestMessage_RoundTrip) {
  // Build a full, valid scenario start request message
  ScenarioStartRequestMessage message =
      ScenarioStartRequestMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(13)
                         .SetTimestamp(5647382910L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(300)
                           .SetSite(400)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(0)
                             .SetApplication(400)
                             .SetSite(300)
                             .Build())
          .SetRequestId(6)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40, buffer.GetLength());  // Scenario Start Request Message PDU takes 40B

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto scenario_start_request =
      std::dynamic_pointer_cast<ScenarioStartRequestMessage>(parsed);
  CHECK_TRUE(scenario_start_request != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kScenarioStart),
              static_cast<int>(scenario_start_request->GetType()));
}

TEST(Dis7MessageDeserializerTest, CreateConstructResponseMessage_RoundTrip) {
  // Build a full, valid create construct response message
  CreateConstructResponseMessage message =
      CreateConstructResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(15)
                         .SetTimestamp(1029384756L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(500)
                           .SetSite(600)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(0)
                             .SetApplication(600)
                             .SetSite(500)
                             .Build())
          .SetRequestId(7)
          .SetConstructId(EntityIdBuilder()
                              .SetEntityNumber(12)
                              .SetApplication(10)
                              .SetSite(20)
                              .Build())
          .SetRequestStatus(RequestStatus::kExecuting)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      64,
      buffer.GetLength());  // Create Construct Response Message PDU takes 56B +
                            // 8B for our FixedDatum containing ActionID

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  // Send MusicMessageType to deserializer (we assume the endpoint will have
  // this info by tracking the requestID)
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto create_construct_response =
      std::dynamic_pointer_cast<CreateConstructResponseMessage>(parsed);
  CHECK_TRUE(create_construct_response != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateConstructResponse),
              static_cast<int>(create_construct_response->GetType()));
  // Check each field
  // unique fields for subclass
  LONGS_EQUAL(12,
              create_construct_response->GetConstructId().GetEntityNumber());
  LONGS_EQUAL(10, create_construct_response->GetConstructId().GetApplication());
  LONGS_EQUAL(20, create_construct_response->GetConstructId().GetSite());

  // header
  LONGS_EQUAL(15, create_construct_response->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      1029384756LL, static_cast<long long>(
                        create_construct_response->GetHeader().GetTimestamp()));

  // common request fields
  LONGS_EQUAL(600, create_construct_response->GetOriginId().GetSite());
  LONGS_EQUAL(500, create_construct_response->GetOriginId().GetApplication());
  LONGS_EQUAL(0, create_construct_response->GetOriginId().GetEntityNumber());

  LONGS_EQUAL(500, create_construct_response->GetReceiverId().GetSite());
  LONGS_EQUAL(600, create_construct_response->GetReceiverId().GetApplication());
  LONGS_EQUAL(0, create_construct_response->GetReceiverId().GetEntityNumber());

  LONGS_EQUAL(7L, create_construct_response->GetRequestId());

  LONGS_EQUAL(static_cast<int>(RequestStatus::kExecuting),
              static_cast<int>(create_construct_response->GetRequestStatus()));
}

TEST(Dis7MessageDeserializerTest, CreateEnvironmentResponseMessage_RoundTrip) {
  // Build a full, valid create environment response message
  CreateEnvironmentResponseMessage message =
      CreateEnvironmentResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(17)
                         .SetTimestamp(1758493021L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(700)
                           .SetSite(800)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(0)
                             .SetApplication(800)
                             .SetSite(700)
                             .Build())
          .SetRequestId(8)
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      48,
      buffer.GetLength());  // Create Environment Response Message PDU takes 40B
                            // + 8B for our FixedDatum containing ActionID
  LONGS_EQUAL(MusicMessageType::kCreateEnvironmentResponse, message.GetType());

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto create_environment_response =
      std::dynamic_pointer_cast<CreateEnvironmentResponseMessage>(parsed);
  CHECK_TRUE(create_environment_response != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kCreateEnvironmentResponse),
              static_cast<int>(create_environment_response->GetType()));

  // Check each field
  // header
  LONGS_EQUAL(17, create_environment_response->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      1758493021LL,
      static_cast<long long>(
          create_environment_response->GetHeader().GetTimestamp()));
  // common request fields
  LONGS_EQUAL(800, create_environment_response->GetOriginId().GetSite());
  LONGS_EQUAL(700, create_environment_response->GetOriginId().GetApplication());
  LONGS_EQUAL(0, create_environment_response->GetOriginId().GetEntityNumber());

  LONGS_EQUAL(700, create_environment_response->GetReceiverId().GetSite());
  LONGS_EQUAL(800,
              create_environment_response->GetReceiverId().GetApplication());
  LONGS_EQUAL(0,
              create_environment_response->GetReceiverId().GetEntityNumber());

  LONGS_EQUAL(8L, create_environment_response->GetRequestId());

  LONGS_EQUAL(
      static_cast<int>(RequestStatus::kComplete),
      static_cast<int>(create_environment_response->GetRequestStatus()));
}

TEST(Dis7MessageDeserializerTest, FinalizeScenarioResponse_RoundTrip) {
  // Build a full, valid finalize scenario response message
  FinalizeScenarioResponseMessage message =
      FinalizeScenarioResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(19)
                         .SetTimestamp(918273645L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(900)
                           .SetSite(1000)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(0)
                             .SetApplication(1000)
                             .SetSite(900)
                             .Build())
          .SetRequestId(9)
          .SetRequestStatus(RequestStatus::kAborted)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  // Finalize Scenario Response Message PDU takes 40B
  // + 8B for our FixedDatum containing ActionID
  LONGS_EQUAL(48, buffer.GetLength());

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto finalize_scenario_response =
      std::dynamic_pointer_cast<FinalizeScenarioResponseMessage>(parsed);
  CHECK_TRUE(finalize_scenario_response != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kFinalizeScenarioResponse),
              static_cast<int>(finalize_scenario_response->GetType()));
  // Check all fields
  // header
  LONGS_EQUAL(19, finalize_scenario_response->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      918273645LL, static_cast<long long>(
                       finalize_scenario_response->GetHeader().GetTimestamp()));
  // common request fields
  LONGS_EQUAL(1000, finalize_scenario_response->GetOriginId().GetSite());
  LONGS_EQUAL(900, finalize_scenario_response->GetOriginId().GetApplication());
  LONGS_EQUAL(0, finalize_scenario_response->GetOriginId().GetEntityNumber());

  LONGS_EQUAL(900, finalize_scenario_response->GetReceiverId().GetSite());
  LONGS_EQUAL(1000,
              finalize_scenario_response->GetReceiverId().GetApplication());
  LONGS_EQUAL(0, finalize_scenario_response->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(9L, finalize_scenario_response->GetRequestId());
  LONGS_EQUAL(static_cast<int>(RequestStatus::kAborted),
              static_cast<int>(finalize_scenario_response->GetRequestStatus()));
}

TEST(Dis7MessageDeserializerTest, ParameterizeConstructResponse_RoundTrip) {
  // Build a full, valid parameterize construct response message
  ParameterizeConstructResponseMessage message =
      ParameterizeConstructResponseMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(21)
                         .SetTimestamp(564738291L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(1100)
                           .SetSite(1200)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(0)
                             .SetApplication(1200)
                             .SetSite(1100)
                             .Build())
          .SetRequestId(10)
          .SetRequestStatus(RequestStatus::kComplete)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(48, buffer.GetLength());  // Parameterize Construct Response
                                        // Message PDU takes 40B + 8B for our
                                        // FixedDatum containing ActionID

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto parameterize_construct_response =
      std::dynamic_pointer_cast<ParameterizeConstructResponseMessage>(parsed);
  CHECK_TRUE(parameterize_construct_response != nullptr);
  LONGS_EQUAL(
      static_cast<int>(MusicMessageType::kParameterizeConstructResponse),
      static_cast<int>(parameterize_construct_response->GetType()));

  // Check all fields
  // header
  LONGS_EQUAL(21, parameterize_construct_response->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      564738291LL,
      static_cast<long long>(
          parameterize_construct_response->GetHeader().GetTimestamp()));

  // common request fields
  LONGS_EQUAL(1200, parameterize_construct_response->GetOriginId().GetSite());
  LONGS_EQUAL(1100,
              parameterize_construct_response->GetOriginId().GetApplication());
  LONGS_EQUAL(0,
              parameterize_construct_response->GetOriginId().GetEntityNumber());

  LONGS_EQUAL(1100, parameterize_construct_response->GetReceiverId().GetSite());
  LONGS_EQUAL(
      1200, parameterize_construct_response->GetReceiverId().GetApplication());
  LONGS_EQUAL(
      0, parameterize_construct_response->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(10L, parameterize_construct_response->GetRequestId());
  LONGS_EQUAL(
      static_cast<int>(RequestStatus::kComplete),
      static_cast<int>(parameterize_construct_response->GetRequestStatus()));
}

TEST(Dis7MessageDeserializerTest, DisplayMessagesMessage_RoundTrip) {
  // Build a full, valid display messages message
  DisplayMessagesMessage message =
      DisplayMessagesMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(23)
                         .SetTimestamp(123123123L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(1300)
                           .SetSite(1400)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(0)
                             .SetApplication(0)
                             .SetSite(0)
                             .Build())
          .SetRequestId(2)
          .SetTimeout(5)
          .SetMessages({"First message", "Second message", "Third message"})
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      120,
      buffer.GetLength());  // Display Messages Message PDU takes 64B + 56B for
                            // 3 messages of data that are over 8B

  // Deserialize the message and compare fields to the original
  Dis7MessageDeserializer deserializer;
  std::shared_ptr<MusicMessage> parsed =
      deserializer.DeserializeMusicMessage(buffer);
  CHECK_TRUE(parsed != nullptr);

  const auto display_messages =
      std::dynamic_pointer_cast<DisplayMessagesMessage>(parsed);
  CHECK_TRUE(display_messages != nullptr);
  LONGS_EQUAL(static_cast<int>(MusicMessageType::kDisplayMessages),
              static_cast<int>(display_messages->GetType()));

  // Check all fields
  // header
  LONGS_EQUAL(23, display_messages->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      123123123LL,
      static_cast<long long>(display_messages->GetHeader().GetTimestamp()));
  LONGS_EQUAL(1400, display_messages->GetOriginId().GetSite());
  LONGS_EQUAL(1300, display_messages->GetOriginId().GetApplication());
  LONGS_EQUAL(0, display_messages->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(0, display_messages->GetReceiverId().GetSite());
  LONGS_EQUAL(0, display_messages->GetReceiverId().GetApplication());
  LONGS_EQUAL(0, display_messages->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(2L, display_messages->GetRequestId());
  LONGS_EQUAL(5, display_messages->GetTimeout());
  CHECK_EQUAL(3u, display_messages->GetMessages().size());
  STRCMP_EQUAL("First message", display_messages->GetMessages()[0].c_str());
  STRCMP_EQUAL("Second message", display_messages->GetMessages()[1].c_str());
  STRCMP_EQUAL("Third message", display_messages->GetMessages()[2].c_str());
}

TEST(Dis7MessageDeserializerTest, RequestSimulationTimeMessage_RoundTrip) {
  // Build a full, valid request simulation time message
  RequestSimulationTimeMessage message =
      RequestSimulationTimeMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(50)
                         .SetTimestamp(123456)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(30)
                           .SetSite(100)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(25)
                             .SetApplication(40)
                             .SetSite(120)
                             .Build())
          .SetRequestId(9876)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify that the stream has data.
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(
      40, buffer.GetLength());  // Request Simulation Time Message PDU takes 40B

  // Deserialize the message
  std::shared_ptr<RequestSimulationTimeMessage> deserialized =
      std::static_pointer_cast<RequestSimulationTimeMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check that the message is valid
  CHECK_TRUE(deserialized != nullptr);

  try {
    deserialized->Validate();
  } catch (std::exception &e) {
    FAIL("Validate should not throw an exception.");
  }

  // Check all the fields
  LONGS_EQUAL(50, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      123456, static_cast<long long>(deserialized->GetHeader().GetTimestamp()));

  LONGS_EQUAL(0, deserialized->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(30, deserialized->GetOriginId().GetApplication());
  LONGS_EQUAL(100, deserialized->GetOriginId().GetSite());
  LONGS_EQUAL(25, deserialized->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(40, deserialized->GetReceiverId().GetApplication());
  LONGS_EQUAL(120, deserialized->GetReceiverId().GetSite());
  LONGS_EQUAL(9876, deserialized->GetRequestId());
}

TEST(Dis7MessageDeserializerTest, SetSimulationTimeMessage_RoundTrip) {
  // Build a full, valid set simulation time message
  SetSimulationTimeMessage message = SetSimulationTimeMessageBuilder()
                                         .SetHeader(MusicMessageHeaderBuilder()
                                                        .SetExerciseId(128)
                                                        .SetTimestamp(1111L)
                                                        .Build())
                                         .SetOriginId(EntityIdBuilder()
                                                          .SetEntityNumber(10)
                                                          .SetApplication(20)
                                                          .SetSite(30)
                                                          .Build())
                                         .SetReceiverId(EntityIdBuilder()
                                                            .SetEntityNumber(0)
                                                            .SetApplication(110)
                                                            .SetSite(120)
                                                            .Build())
                                         .SetRequestId(1001)
                                         .SetNewTime(1767198330)
                                         .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(48,
              buffer.GetLength());  // Set Simulation Time Message PDU takes 48B

  // Deserialize the message
  std::shared_ptr<SetSimulationTimeMessage> deserialized =
      std::static_pointer_cast<SetSimulationTimeMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check the message is valid
  CHECK_TRUE(deserialized != nullptr);

  LONGS_EQUAL(MusicMessageType::kSetSimulationTime, deserialized->GetType());

  try {
    deserialized->Validate();

  } catch (std::exception &e) {
    FAIL((std::string("Validation should not throw an exception: ") + e.what())
             .c_str());
  }

  // Check all fields
  // Header
  LONGS_EQUAL(128, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      1111L, static_cast<long long>(deserialized->GetHeader().GetTimestamp()));
  LONGS_EQUAL(10, deserialized->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(20, deserialized->GetOriginId().GetApplication());
  LONGS_EQUAL(30, deserialized->GetOriginId().GetSite());
  LONGS_EQUAL(0, deserialized->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(110, deserialized->GetReceiverId().GetApplication());
  LONGS_EQUAL(120, deserialized->GetReceiverId().GetSite());
  LONGS_EQUAL(1001, deserialized->GetRequestId());
  LONGS_EQUAL(1767198330, deserialized->GetNewTime());
}

TEST(Dis7MessageDeserializerTest, SimulationTimeMessage_RoundTrip) {
  // Build a full, valid Simulation Time Message (response message).
  SimulationTimeMessage message =
      SimulationTimeMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(123)
                         .SetTimestamp(123456)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(100)
                           .SetSite(200)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(150)
                             .SetApplication(10)
                             .SetSite(20)
                             .Build())
          .SetRequestId(9876)
          .SetRequestStatus(RequestStatus::kComplete)
          .SetSimTime(1767199728)
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify that the steram has some data.
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(56,
              buffer.GetLength());  // Simulation Time Message PDU takes 48B +
                                    // 8B for our FixedDatum containing ActionID

  // Deserialize the message
  std::shared_ptr<SimulationTimeMessage> deserialized =
      std::static_pointer_cast<SimulationTimeMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check that the deserialized message is valid
  CHECK_TRUE(deserialized != nullptr);

  try {
    deserialized->Validate();
  } catch (std::exception &e) {
    FAIL((std::string("Validate should not throw an exception:") + e.what())
             .c_str());
  }

  // Check all fields
  // Header
  LONGS_EQUAL(123, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      123456L,
      static_cast<long long>(deserialized->GetHeader().GetTimestamp()));

  LONGS_EQUAL(0, deserialized->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(100, deserialized->GetOriginId().GetApplication());
  LONGS_EQUAL(200, deserialized->GetOriginId().GetSite());
  LONGS_EQUAL(150, deserialized->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(10, deserialized->GetReceiverId().GetApplication());
  LONGS_EQUAL(20, deserialized->GetReceiverId().GetSite());
  LONGS_EQUAL(9876, deserialized->GetRequestId());
  LONGS_EQUAL(RequestStatus::kComplete, deserialized->GetRequestStatus());
  LONGS_EQUAL(1767199728, deserialized->GetSimTime());
}

TEST(Dis7MessageDeserializerTest, RemoveConstructMessage_RoundTrip) {
  // Build a full, valid remove construct message
  RemoveConstructMessage message =
      RemoveConstructMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(77)
                         .SetTimestamp(88888888L)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(66)
                           .SetSite(77)
                           .Build())
          .SetReceiverId(EntityIdBuilder()
                             .SetEntityNumber(0)
                             .SetApplication(15)
                             .SetSite(25)
                             .Build())
          .SetRequestId(1234)
          .SetRemovedConstruct(EntityIdBuilder()
                                   .SetEntityNumber(33)
                                   .SetApplication(44)
                                   .SetSite(55)
                                   .Build())
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(56,
              buffer.GetLength());  // Remove Construct Message PDU takes 56B

  // Deserialize the message
  std::shared_ptr<RemoveConstructMessage> deserialized =
      std::static_pointer_cast<RemoveConstructMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check the message is valid
  CHECK_TRUE(deserialized != nullptr);

  LONGS_EQUAL(MusicMessageType::kRemoveConstruct, deserialized->GetType());

  try {
    deserialized->Validate();
  } catch (std::exception &e) {
    FAIL("Validate should not throw an exception.");
  }

  // Check all fields
  // Header
  LONGS_EQUAL(77, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      88888888L,
      static_cast<long long>(deserialized->GetHeader().GetTimestamp()));
  LONGS_EQUAL(0, deserialized->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(66, deserialized->GetOriginId().GetApplication());
  LONGS_EQUAL(77, deserialized->GetOriginId().GetSite());
  LONGS_EQUAL(33, deserialized->GetRemovedConstruct().GetEntityNumber());
  LONGS_EQUAL(44, deserialized->GetRemovedConstruct().GetApplication());
  LONGS_EQUAL(55, deserialized->GetRemovedConstruct().GetSite());
}

TEST(Dis7MessageDeserializerTest, StopConstructMessage_RoundTrip) {
  // Build a full, valid stop construct message
  StopConstructMessage message = StopConstructMessageBuilder()
                                     .SetHeader(MusicMessageHeaderBuilder()
                                                    .SetExerciseId(88)
                                                    .SetTimestamp(99999999L)
                                                    .Build())
                                     .SetOriginId(EntityIdBuilder()
                                                      .SetEntityNumber(0)
                                                      .SetApplication(11)
                                                      .SetSite(22)
                                                      .Build())
                                     .SetReceiverId(EntityIdBuilder()
                                                        .SetEntityNumber(77)
                                                        .SetApplication(33)
                                                        .SetSite(44)
                                                        .Build())
                                     .SetRequestId(5678)
                                     .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(40, buffer.GetLength());  // Stop Construct Message PDU takes 40B

  // Deserialize the message
  std::shared_ptr<StopConstructMessage> deserialized =
      std::static_pointer_cast<StopConstructMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check the message is valid
  CHECK_TRUE(deserialized != nullptr);

  LONGS_EQUAL(MusicMessageType::kStopConstruct, deserialized->GetType());

  try {
    deserialized->Validate();
  } catch (std::exception &e) {
    FAIL("Validate should not throw an exception.");
  }

  // Check all fields
  // Header
  LONGS_EQUAL(88, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      99999999L,
      static_cast<long long>(deserialized->GetHeader().GetTimestamp()));
  LONGS_EQUAL(0, deserialized->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(11, deserialized->GetOriginId().GetApplication());
  LONGS_EQUAL(22, deserialized->GetOriginId().GetSite());
  LONGS_EQUAL(77, deserialized->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(33, deserialized->GetReceiverId().GetApplication());
  LONGS_EQUAL(44, deserialized->GetReceiverId().GetSite());
  LONGS_EQUAL(5678, deserialized->GetRequestId());
}

TEST(Dis7MessageDeserializerTest, ControlInitiatedMessage_RoundTrip) {
  // Build a full, valid control initiated message
  ControlInitiatedMessage message = ControlInitiatedMessageBuilder()
                                        .SetHeader(MusicMessageHeaderBuilder()
                                                       .SetExerciseId(99)
                                                       .SetTimestamp(12121212L)
                                                       .Build())
                                        .SetOriginId(EntityIdBuilder()
                                                         .SetEntityNumber(0)
                                                         .SetApplication(88)
                                                         .SetSite(99)
                                                         .Build())
                                        .SetReceiverId(EntityIdBuilder()
                                                           .SetEntityNumber(0)
                                                           .SetApplication(66)
                                                           .SetSite(77)
                                                           .Build())
                                        .SetRequestId(1357)
                                        .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(40,
              buffer.GetLength());  // Control Initiated Message PDU takes 40B

  // Deserialize the message
  std::shared_ptr<ControlInitiatedMessage> deserialized =
      std::static_pointer_cast<ControlInitiatedMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check the message is valid
  CHECK_TRUE(deserialized != nullptr);

  LONGS_EQUAL(MusicMessageType::kControlInitiated, deserialized->GetType());

  try {
    deserialized->Validate();
  } catch (std::exception &e) {
    FAIL("Validate should not throw an exception.");
  }

  // Check all fields
  // Header
  LONGS_EQUAL(99, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      12121212L,
      static_cast<long long>(deserialized->GetHeader().GetTimestamp()));
  LONGS_EQUAL(0, deserialized->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(88, deserialized->GetOriginId().GetApplication());
  LONGS_EQUAL(99, deserialized->GetOriginId().GetSite());
  LONGS_EQUAL(0, deserialized->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(66, deserialized->GetReceiverId().GetApplication());
  LONGS_EQUAL(77, deserialized->GetReceiverId().GetSite());
  LONGS_EQUAL(1357, deserialized->GetRequestId());
}

TEST(Dis7MessageDeserializerTest, ControlReleasedMessage_RoundTrip) {
  // Build a full, valid control released message
  ControlReleasedMessage message = ControlReleasedMessageBuilder()
                                       .SetHeader(MusicMessageHeaderBuilder()
                                                      .SetExerciseId(111)
                                                      .SetTimestamp(13131313L)
                                                      .Build())
                                       .SetOriginId(EntityIdBuilder()
                                                        .SetEntityNumber(0)
                                                        .SetApplication(55)
                                                        .SetSite(66)
                                                        .Build())
                                       .SetReceiverId(EntityIdBuilder()
                                                          .SetEntityNumber(0)
                                                          .SetApplication(44)
                                                          .SetSite(33)
                                                          .Build())
                                       .SetRequestId(2468)
                                       .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(40,
              buffer.GetLength());  // Control Released Message PDU takes 40B

  // Deserialize the message
  std::shared_ptr<ControlReleasedMessage> deserialized =
      std::static_pointer_cast<ControlReleasedMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check the message is valid
  CHECK_TRUE(deserialized != nullptr);

  LONGS_EQUAL(MusicMessageType::kControlReleased, deserialized->GetType());

  try {
    deserialized->Validate();
  } catch (std::exception &e) {
    FAIL("Validate should not throw an exception.");
  }

  // Check all fields
  // Header
  LONGS_EQUAL(111, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      13131313L,
      static_cast<long long>(deserialized->GetHeader().GetTimestamp()));
  LONGS_EQUAL(0, deserialized->GetOriginId().GetEntityNumber());
  LONGS_EQUAL(55, deserialized->GetOriginId().GetApplication());
  LONGS_EQUAL(66, deserialized->GetOriginId().GetSite());
  LONGS_EQUAL(0, deserialized->GetReceiverId().GetEntityNumber());
  LONGS_EQUAL(44, deserialized->GetReceiverId().GetApplication());
  LONGS_EQUAL(33, deserialized->GetReceiverId().GetSite());
  LONGS_EQUAL(2468, deserialized->GetRequestId());
}

TEST(Dis7MessageDeserializerTest, FireMessage_RoundTrip) {
  // Build a full, valid fire message
  FireMessage message =
      FireMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(222)
                         .SetTimestamp(14141414L)
                         .Build())
          .SetShooterId(EntityIdBuilder()
                            .SetEntityNumber(1)
                            .SetApplication(2)
                            .SetSite(3)
                            .Build())
          .SetTargetId(EntityIdBuilder()
                           .SetEntityNumber(4)
                           .SetApplication(5)
                           .SetSite(6)
                           .Build())
          .SetMunitionId(EntityIdBuilder()
                             .SetEntityNumber(200)
                             .SetApplication(10)
                             .SetSite(5)
                             .Build())
          .SetEventId(MusicEventIdBuilder()
                          .SetEventId(5555)
                          .SetSimulationAddress(SimulationAddressBuilder()
                                                    .SetSite(10)
                                                    .SetApplication(20)
                                                    .Build())
                          .Build())
          .SetVelocity(
              VectorBuilder().SetX(10.0f).SetY(20.0f).SetZ(30.0f).Build())
          .SetLocation(
              LocationBuilder().SetX(100.0f).SetY(200.0f).SetZ(300.0f).Build())
          .SetMunition(EntityTypeBuilder()
                           .SetKind(2)
                           .SetDomain(3)
                           .SetCountry(225)  // USA
                           .SetCategory(4)
                           .SetSubcategory(5)
                           .SetSpecific(6)
                           .SetExtra(7)
                           .Build())
          .SetWarhead(Warhead::kHeIncendiary)
          .SetFuse(FuseType::kAcoustic)
          .SetRate(3)
          .SetQuantity(3)
          .SetRangeToTarget(42.0)
          .Build();

  try {
    message.Validate();
  } catch (std::exception &e) {
    FAIL((std::string("Source message is invalid: ") + e.what()).c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(96, buffer.GetLength());  // Fire Message PDU takes 96B

  // Deserialize the message
  std::shared_ptr<FireMessage> deserialized =
      std::static_pointer_cast<FireMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check the message is valid
  CHECK_TRUE(deserialized != nullptr);

  LONGS_EQUAL(MusicMessageType::kFire, deserialized->GetType());

  try {
    deserialized->Validate();
  } catch (std::exception &e) {
    FAIL((std::string("Validate should not throw an exception.") + e.what())
             .c_str());
  }

  // Check all fields
  // Header
  LONGS_EQUAL(222, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      14141414L,
      static_cast<long long>(deserialized->GetHeader().GetTimestamp()));
  // Shooter ID
  LONGS_EQUAL(1, deserialized->GetShooterId().GetEntityNumber());
  LONGS_EQUAL(2, deserialized->GetShooterId().GetApplication());
  LONGS_EQUAL(3, deserialized->GetShooterId().GetSite());
  // Target ID
  LONGS_EQUAL(4, deserialized->GetTargetId().value().GetEntityNumber());
  LONGS_EQUAL(5, deserialized->GetTargetId().value().GetApplication());
  LONGS_EQUAL(6, deserialized->GetTargetId().value().GetSite());
  // Munition ID
  LONGS_EQUAL(200, deserialized->GetMunitionId().GetEntityNumber());
  LONGS_EQUAL(10, deserialized->GetMunitionId().GetApplication());
  LONGS_EQUAL(5, deserialized->GetMunitionId().GetSite());
  // Event ID
  LONGS_EQUAL(5555, deserialized->GetEventId().GetEventId());
  LONGS_EQUAL(10, deserialized->GetEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(
      20, deserialized->GetEventId().GetSimulationAddress().GetApplication());
  // Velocidad
  DOUBLES_EQUAL(10.0f, deserialized->GetVelocity().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(20.0f, deserialized->GetVelocity().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(30.0f, deserialized->GetVelocity().GetZ(), kDoublePrecision);
  // Location
  DOUBLES_EQUAL(100.0f, deserialized->GetLocation().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(200.0f, deserialized->GetLocation().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(300.0f, deserialized->GetLocation().GetZ(), kDoublePrecision);
  // Munition
  LONGS_EQUAL(2, deserialized->GetMunition().GetKind());
  LONGS_EQUAL(3, deserialized->GetMunition().GetDomain());
  LONGS_EQUAL(225, deserialized->GetMunition().GetCountry());
  LONGS_EQUAL(4, deserialized->GetMunition().GetCategory());
  LONGS_EQUAL(5, deserialized->GetMunition().GetSubcategory());
  LONGS_EQUAL(6, deserialized->GetMunition().GetSpecific());
  LONGS_EQUAL(7, deserialized->GetMunition().GetExtra());
  // Warhead
  LONGS_EQUAL(static_cast<int>(Warhead::kHeIncendiary),
              static_cast<int>(deserialized->GetWarhead()));
  // Fuse
  LONGS_EQUAL(static_cast<int>(FuseType::kAcoustic),
              static_cast<int>(deserialized->GetFuse()));
  // Rate
  LONGS_EQUAL(3, deserialized->GetRate());
  // Cantidad
  LONGS_EQUAL(3, deserialized->GetQuantity());
  // Range
  DOUBLES_EQUAL(42.0, deserialized->GetRangeToTarget(), kDoublePrecision);
}

TEST(Dis7MessageDeserializerTest, DetonationMessage_RoundTrip) {
  // Build a full, valid detonation message
  DetonationMessage message =
      DetonationMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(67)
                         .SetTimestamp(15151515L)
                         .Build())
          .SetShooterId(EntityIdBuilder()
                            .SetEntityNumber(7)
                            .SetApplication(8)
                            .SetSite(9)
                            .Build())
          .SetTargetId(EntityIdBuilder()
                           .SetEntityNumber(10)
                           .SetApplication(11)
                           .SetSite(12)
                           .Build())
          .SetMunitionId(EntityIdBuilder()
                             .SetEntityNumber(100)
                             .SetApplication(20)
                             .SetSite(15)
                             .Build())
          .SetEventId(MusicEventIdBuilder()
                          .SetEventId(1234)
                          .SetSimulationAddress(SimulationAddressBuilder()
                                                    .SetSite(30)
                                                    .SetApplication(40)
                                                    .Build())
                          .Build())
          .SetMunition(EntityTypeBuilder()
                           .SetKind(3)
                           .SetDomain(4)
                           .SetCountry(225)
                           .SetCategory(5)
                           .SetSubcategory(6)
                           .SetSpecific(7)
                           .SetExtra(8)
                           .Build())
          .SetWarhead(Warhead::kChemicalGeneral)
          .SetFuse(FuseType::kProximityActiveLaser)
          .SetQuantity(300)
          .SetRate(60)
          .SetLocation(
              LocationBuilder().SetX(400.0f).SetY(500.0f).SetZ(600.0f).Build())
          .SetVelocity(
              VectorBuilder().SetX(40.0f).SetY(50.0f).SetZ(60.0f).Build())
          .SetResult(DetonationResultType::kDetonation)
          .Build();

  try {
    message.Validate();
  } catch (std::exception &e) {
    FAIL((std::string("Validate should not throw an exception.") + e.what())
             .c_str());
  }

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(message);

  // Verify the stream has data
  CHECK_TRUE(buffer.GetLength() > 0);
  LONGS_EQUAL(104, buffer.GetLength());  // Detonation Message PDU takes 104B
                                         // (with empty Variable Parameters)

  // Deserialize the message
  std::shared_ptr<DetonationMessage> deserialized =
      std::static_pointer_cast<DetonationMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  // Check the message is valid
  CHECK_TRUE(deserialized != nullptr);

  LONGS_EQUAL(MusicMessageType::kDetonation, deserialized->GetType());

  try {
    deserialized->Validate();
  } catch (std::exception &e) {
    FAIL((std::string("Validate should not throw an exception.") + e.what())
             .c_str());
  }

  // Check all fields
  // Header
  LONGS_EQUAL(67, deserialized->GetHeader().GetExerciseId());
  CheckSameMillisSinceHour(
      15151515L,
      static_cast<long long>(deserialized->GetHeader().GetTimestamp()));
  // Shooter ID
  LONGS_EQUAL(7, deserialized->GetShooterId().GetEntityNumber());
  LONGS_EQUAL(8, deserialized->GetShooterId().GetApplication());
  LONGS_EQUAL(9, deserialized->GetShooterId().GetSite());
  // Target ID
  LONGS_EQUAL(10, deserialized->GetTargetId().value().GetEntityNumber());
  LONGS_EQUAL(11, deserialized->GetTargetId().value().GetApplication());
  LONGS_EQUAL(12, deserialized->GetTargetId().value().GetSite());
  // Munition ID
  LONGS_EQUAL(100, deserialized->GetMunitionId().GetEntityNumber());
  LONGS_EQUAL(20, deserialized->GetMunitionId().GetApplication());
  LONGS_EQUAL(15, deserialized->GetMunitionId().GetSite());
  // Event ID
  LONGS_EQUAL(1234, deserialized->GetEventId().GetEventId());
  LONGS_EQUAL(30, deserialized->GetEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(
      40, deserialized->GetEventId().GetSimulationAddress().GetApplication());
  // Munition Type
  LONGS_EQUAL(3, deserialized->GetMunition().GetKind());
  LONGS_EQUAL(4, deserialized->GetMunition().GetDomain());
  LONGS_EQUAL(225, deserialized->GetMunition().GetCountry());
  LONGS_EQUAL(5, deserialized->GetMunition().GetCategory());
  LONGS_EQUAL(6, deserialized->GetMunition().GetSubcategory());
  LONGS_EQUAL(7, deserialized->GetMunition().GetSpecific());
  LONGS_EQUAL(8, deserialized->GetMunition().GetExtra());
  // Warhead
  LONGS_EQUAL(static_cast<int>(Warhead::kChemicalGeneral),
              static_cast<int>(deserialized->GetWarhead()));
  // Fuse
  LONGS_EQUAL(static_cast<int>(FuseType::kProximityActiveLaser),
              static_cast<int>(deserialized->GetFuse()));
  // Quantity
  LONGS_EQUAL(300, deserialized->GetQuantity());
  // Rate
  LONGS_EQUAL(60, deserialized->GetRate());
  // Location
  DOUBLES_EQUAL(400.0f, deserialized->GetLocation().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(500.0f, deserialized->GetLocation().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(600.0f, deserialized->GetLocation().GetZ(), kDoublePrecision);
  // Velocity
  DOUBLES_EQUAL(40.0f, deserialized->GetVelocity().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(50.0f, deserialized->GetVelocity().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(60.0f, deserialized->GetVelocity().GetZ(), kDoublePrecision);
  // Result
  LONGS_EQUAL(static_cast<int>(DetonationResultType::kDetonation),
              static_cast<int>(deserialized->GetResult()));
}

TEST(Dis7MessageDeserializerTest,
     Test_SerializeConstructData_DeserializedMatchesSource) {
  // Build a full, valid create construct request message
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
                                      .SetEntityNumber(4)
                                      .SetApplication(5)
                                      .SetSite(6)
                                      .Build())
          .SetCurrentControllerId(EntityIdBuilder()
                                      .SetEntityNumber(7)
                                      .SetApplication(8)
                                      .SetSite(9)
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
          .SetCallsign("FOX55")
          .SetConstructName(
              "com.discoverymachine.Entity.AbstractVehicle.AbstractAircraft."
              "AbstractFixedWingAircraft.F16AEntity")
          .SetInteractionRecord(
              "land, fireWeaponAtTarget, fireWeaponAtLocation, moveToLocation, "
              "moveOnRoute, setEndOfRouteAction, setEquipmentQuantity, "
              "assignFormation")
          .Build();

  // Serialize the message
  Dis7MessageSerializer serializer;
  MusicByteBuffer buffer = serializer.SerializeMusicMessage(construct_message);
  // Verify the stream has data - serialization succeeded
  CHECK_TRUE(buffer.GetLength() > 0);

  // Deserialize the message and compare the JSON to the original
  ConstructDataMessage deserialized =
      *std::static_pointer_cast<ConstructDataMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buffer));

  nlohmann::json expected_json = construct_message.ToJson();
  nlohmann::json actual_json = deserialized.ToJson();
  CheckSameMillisSinceHour(
      expected_json["header"]["timestamp"].get<long long>(),
      actual_json["header"]["timestamp"].get<long long>());
  expected_json["header"]["timestamp"] = 0;
  actual_json["header"]["timestamp"] = 0;
  STRCMP_EQUAL(expected_json.dump().data(), actual_json.dump().data());
}

TEST(Dis7MessageDeserializerTest,
     Test_SerializePerceptionMessage_DeserializedMatchesSource) {
  PerceptionRecord record1 = PerceptionRecordBuilder()
                                 .SetPerceptionId(EntityIdBuilder()
                                                      .SetSite(4)
                                                      .SetApplication(5)
                                                      .SetEntityNumber(6)
                                                      .Build())
                                 .SetForce(Force::kFriendly)
                                 .SetAccuracy(0.98f)
                                 .SetBearingFactor(12)
                                 .SetAltitudeError(3)
                                 .SetRangeError(7)
                                 .SetEntityType(EntityTypeBuilder()
                                                    .SetKind(1)
                                                    .SetDomain(2)
                                                    .SetCountry(225)  // USA
                                                    .SetCategory(3)
                                                    .SetSubcategory(4)
                                                    .SetSpecific(5)
                                                    .SetExtra(6)
                                                    .Build())
                                 .Build();

  PerceptionRecord record2 = PerceptionRecordBuilder()
                                 .SetPerceptionId(EntityIdBuilder()
                                                      .SetSite(7)
                                                      .SetApplication(8)
                                                      .SetEntityNumber(9)
                                                      .Build())
                                 .SetForce(Force::kNeutral)
                                 .SetAccuracy(0.85f)
                                 .SetBearingFactor(15)
                                 .SetAltitudeError(4)
                                 .SetRangeError(10)
                                 .SetEntityType(EntityTypeBuilder()
                                                    .SetKind(2)
                                                    .SetDomain(3)
                                                    .SetCountry(225)  // USA
                                                    .SetCategory(4)
                                                    .SetSubcategory(5)
                                                    .SetSpecific(6)
                                                    .SetExtra(7)
                                                    .Build())
                                 .Build();

  PerceptionDataMessage message = PerceptionDataMessageBuilder()
                                      .SetHeader(MusicMessageHeaderBuilder()
                                                     .SetExerciseId(2)
                                                     .SetTimestamp(15555)
                                                     .Build())
                                      .SetOriginId(EntityIdBuilder()
                                                       .SetSite(1)
                                                       .SetApplication(2)
                                                       .SetEntityNumber(3)
                                                       .Build())
                                      .SetNumPerceptions(2)
                                      .SetPerceptionRecords({record1, record2})
                                      .Build();

  MusicByteBuffer buff = Dis7MessageSerializer().SerializeMusicMessage(message);
  PerceptionDataMessage deserialized =
      *std::static_pointer_cast<PerceptionDataMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buff));

  nlohmann::json expected_json = message.ToJson();
  nlohmann::json actual_json = deserialized.ToJson();
  CheckSameMillisSinceHour(
      expected_json["header"]["timestamp"].get<long long>(),
      actual_json["header"]["timestamp"].get<long long>());
  expected_json["header"]["timestamp"] = 0;
  actual_json["header"]["timestamp"] = 0;
  STRCMP_EQUAL(expected_json.dump().data(), actual_json.dump().data());
}

TEST(Dis7MessageDeserializerTest,
     Test_SerializeWaypointDataMessage_DeserializedMatchesSource) {
  WaypointRecord first_record =
      WaypointRecordBuilder()
          .SetEstimatedArrivalTime(5)
          .SetArrivalTimeError(2)
          .SetWorldCoordinateRecord(
              LocationBuilder().SetX(1).SetY(2).SetZ(3).Build())
          .Build();

  WaypointRecord second_record =
      WaypointRecordBuilder()
          .SetEstimatedArrivalTime(5)
          .SetArrivalTimeError(13)
          .SetWorldCoordinateRecord(
              LocationBuilder().SetX(4).SetY(5).SetZ(7).Build())
          .Build();

  WaypointDataMessage message =
      WaypointDataMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(2)
                         .SetTimestamp(15555)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(0)
                           .SetApplication(1)
                           .SetSite(2)
                           .Build())
          .SetNumWaypoints(2)
          .SetWaypointRecords({first_record, second_record})
          .SetCurrentWaypointIndex(0)
          .SetStartOrAppend(false)
          .SetType(MusicMessageType::kWaypoint)
          .Build();
  MusicByteBuffer serialized_buff =
      Dis7MessageSerializer().SerializeMusicMessage(message);
  WaypointDataMessage deserialized =
      *std::static_pointer_cast<WaypointDataMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(serialized_buff));

  nlohmann::json source_json = message.ToJson();
  nlohmann::json deserialized_json = deserialized.ToJson();

  CheckSameMillisSinceHour(
      source_json["header"]["timestamp"].get<long long>(),
      deserialized_json["header"]["timestamp"].get<long long>());
  source_json["header"]["timestamp"] = 0;
  deserialized_json["header"]["timestamp"] = 0;

  STRCMP_EQUAL(source_json.dump().data(), deserialized_json.dump().data());
}

TEST(Dis7MessageDeserializerTest,
     Test_SerializeConstructInteractionMessage_DeserializedMatchesSource) {
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
          .SetInteractionData(nlohmann::json{{"Test Key", "Test Value"}})
          .Build();

  MusicByteBuffer buff =
      Dis7MessageSerializer().SerializeMusicMessage(test_mesg);
  ConstructInteractionRequestMessage deserialized =
      *std::static_pointer_cast<ConstructInteractionRequestMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buff));

  // Make sure it's valid
  bool failed_validation = false;
  try {
    { deserialized.Validate(); }
  } catch (std::runtime_error err) {
    failed_validation = true;
  }
  CHECK_FALSE(failed_validation);

  nlohmann::json source_json = test_mesg.ToJson();
  nlohmann::json deserialized_json = deserialized.ToJson();

  CheckSameMillisSinceHour(
      source_json["header"]["timestamp"].get<long long>(),
      deserialized_json["header"]["timestamp"].get<long long>());
  source_json["header"]["timestamp"] = 0;
  deserialized_json["header"]["timestamp"] = 0;

  STRCMP_EQUAL(source_json.dump().c_str(), deserialized_json.dump().c_str());
}

TEST(
    Dis7MessageDeserializerTest,
    Test_SerializeConstructInteractionResponseMessage_DeserializedMatchesSource) {
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

  const auto &optional_data =
      nlohmann::json{{"Optional data key", "Optional data value"}}.dump();

  test_mesg = ConstructInteractionResponseMessageBuilder()
                  .SetHeader(header)
                  .SetOriginId(origin_id)
                  .SetReceiverId(receiver_id)
                  .SetRequestId(1)
                  .SetRequestStatus(RequestStatus::kExecuting)
                  .SetOptionalData(optional_data)
                  .Build();

  MusicByteBuffer buff =
      Dis7MessageSerializer().SerializeMusicMessage(test_mesg);
  ConstructInteractionResponseMessage deserialized =
      *std::static_pointer_cast<ConstructInteractionResponseMessage>(
          Dis7MessageDeserializer().DeserializeMusicMessage(buff));

  // Make sure it's valid
  bool failed_validation = false;
  try {
    { deserialized.Validate(); }
  } catch (std::runtime_error err) {
    failed_validation = true;
  }
  CHECK_FALSE(failed_validation);

  nlohmann::json source_json = test_mesg.ToJson();
  nlohmann::json deserialized_json = deserialized.ToJson();

  CheckSameMillisSinceHour(
      source_json["header"]["timestamp"].get<long long>(),
      deserialized_json["header"]["timestamp"].get<long long>());
  source_json["header"]["timestamp"] = 0;
  deserialized_json["header"]["timestamp"] = 0;

  STRCMP_EQUAL(source_json.dump().data(), deserialized_json.dump().data());
}

}  // namespace music::test
