// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <nlohmann/json_fwd.hpp>
#include <stdexcept>

#include "enum/construct_render_type.h"
#include "enum/dead_reckoning_algorithm_type.h"
#include "enum/music_message_type.h"
#include "message/Location.h"
#include "message/Orientation.h"
#include "message/Vector.h"
#include "message/construct/construct_data_message.h"
#include "message/construct/construct_information_record.h"
#include "message/construct/damage.h"
#include "message/construct/dead_reckoning.h"
#include "message/entity_id.h"
#include "message/entity_type.h"
#include "message/music_message_header.h"
const double DOUBLE_TOLERANCE = 0.001;

namespace music::test {
TEST_GROUP(MusicConstructDataMessageTests){
    // NO-OP
};

ConstructDataMessageBuilder CreateBaseMessageBuilder() {
  ConstructDataMessageBuilder builder =
      ConstructDataMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(2)
                         .SetTimestamp(1000L)
                         .Build())
          .SetType(MusicMessageType::kConstructData)
          .SetOriginId(EntityIdBuilder()
                           .SetEntityNumber(1)
                           .SetApplication(1)
                           .SetSite(1)
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
                      ConstructRenderType::kRenderedPhysical)
                  .Build())
          .SetCallsign("FOX55")
          .SetConstructName(
              "com.discoverymachine.Entity.AbstractVehicle.AbstractAircraft."
              "AbstractFixedWingAircraft.F16AEntity")
          .SetInteractionRecord(
              "land, fireWeaponAtTarget, fireWeaponAtLocation, moveToLocation, "
              "moveOnRoute, setEndOfRouteAction, setEquipmentQuantity, "
              "assignFormation");

  return builder;
}

TEST(MusicConstructDataMessageTests,
     Test_InvalidRequiredValues_ValidationFails) {
  ConstructDataMessageBuilder builder = ConstructDataMessageBuilder();
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  // Superclass fields
  builder.SetHeader(
      MusicMessageHeaderBuilder().SetExerciseId(2).SetTimestamp(1000L).Build());
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  builder.SetType(MusicMessageType::kConstructData);
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  // Add ONLY required fields
  builder.SetOriginId(EntityIdBuilder()
                          .SetEntityNumber(1)
                          .SetApplication(1)
                          .SetSite(1)
                          .Build());
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  builder.SetPrimaryControllerId(EntityIdBuilder()
                                     .SetEntityNumber(1)
                                     .SetApplication(1)
                                     .SetSite(1)
                                     .Build());
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  builder.SetCurrentControllerId(EntityIdBuilder()
                                     .SetEntityNumber(1)
                                     .SetApplication(1)
                                     .SetSite(1)
                                     .Build());
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  builder.SetConstructInformationRecord(
      ConstructInformationRecordBuilder()
          .SetConstructCategoryType(ConstructCategoryType::kAggregate)
          .SetConstructRenderType(ConstructRenderType::kRenderedPhysical)
          .Build());
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  builder.SetCallsign("FOX55");
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  builder.SetConstructName(
      "com.discoverymachine.Entity.AbstractVehicle.AbstractAircraft."
      "AbstractFixedWingAircraft.F16AEntity");
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());

  builder.SetInteractionRecord(
      "land, fireWeaponAtTarget, fireWeaponAtLocation, moveToLocation, "
      "moveOnRoute, setEndOfRouteAction, setEquipmentQuantity, "
      "assignFormation");

  // At this point the object should be 'valid', although all optional values
  // are unset
  try {
    { builder.Build().Validate(); }
  } catch (std::runtime_error err) {
    FAIL(
        "music::ConstructDataMessage threw a validation error with all valid "
        "data!");
  }
}

TEST(MusicConstructDataMessageTests,
     Test_BaseConstructDataMarshalled_OptionalFieldsOmitted) {
  ConstructDataMessage base_message = CreateBaseMessageBuilder().Build();
  nlohmann::json jsn;
  to_json(jsn, base_message);

  // Make sure none of the optional values are populated
  CHECK_FALSE(jsn.contains(ConstructDataMessage::kGhostedIdKey));
  CHECK_FALSE(jsn.contains(ConstructDataMessage::kForceKey));
  CHECK_FALSE(jsn.contains(ConstructDataMessage::kEntityTypeKey));
  CHECK_FALSE(jsn.contains(ConstructDataMessage::kLocationKey));
  CHECK_FALSE(jsn.contains(ConstructDataMessage::kOrientationKey));
  CHECK_FALSE(jsn.contains(ConstructDataMessage::kVelocityKey));
  CHECK_FALSE(jsn.contains(ConstructDataMessage::kDeadReckoningKey));
  CHECK_FALSE(jsn.contains(ConstructDataMessage::kDamageKey));
}

TEST(MusicConstructDataMessageTests,
     Test_FullConstructDataMarshalled_OptionalFieldsIncluded) {
  ConstructDataMessageBuilder base_message_builder = CreateBaseMessageBuilder();
  // Add all the optional fields
  base_message_builder
      .SetGhostedId(EntityIdBuilder()
                        .SetEntityNumber(1)
                        .SetApplication(1)
                        .SetSite(1)
                        .Build())
      .SetForce(Force::kNeutral)
      .SetEntityType(EntityTypeBuilder()
                         .SetCategory(1)
                         .SetSubcategory(1)
                         .SetCountry(3)
                         .SetDomain(1)
                         .SetExtra(1)
                         .SetKind(1)
                         .SetSpecific(2)
                         .Build())
      .SetLocation(LocationBuilder().SetX(1).SetY(1).SetZ(1).Build())
      .SetOrientation(
          OrientationBuilder().SetPhi(1).SetPsi(1).SetTheta(1).Build())
      .SetVelocity(VectorBuilder().SetX(1).SetY(1).SetZ(1).Build())
      .SetDeadReckoning(
          DeadReckoningBuilder()
              .SetAngularVelocity(
                  VectorBuilder().SetX(1).SetY(1).SetZ(1).Build())
              .SetAlgorithmType(DeadReckoningAlgorithmType::kDrm_F_P_W)
              .SetLinearAcceleration(
                  VectorBuilder().SetX(1).SetY(1).SetZ(1).Build())
              .Build())
      .SetDamage(MusicDamageBuilder()
                     .SetCatastrophicDamage(1.0f)
                     .SetMobilityDamage(1.0f)
                     .SetIsFirepowerDisabled(false)
                     .Build());

  nlohmann::json jsn;
  to_json(jsn, base_message_builder.Build());

  CHECK_TRUE(jsn.contains(ConstructDataMessage::kGhostedIdKey));
  CHECK_TRUE(jsn.contains(ConstructDataMessage::kForceKey));
  CHECK_TRUE(jsn.contains(ConstructDataMessage::kEntityTypeKey));
  CHECK_TRUE(jsn.contains(ConstructDataMessage::kLocationKey));
  CHECK_TRUE(jsn.contains(ConstructDataMessage::kOrientationKey));
  CHECK_TRUE(jsn.contains(ConstructDataMessage::kVelocityKey));
  CHECK_TRUE(jsn.contains(ConstructDataMessage::kDeadReckoningKey));
  CHECK_TRUE(jsn.contains(ConstructDataMessage::kDamageKey));
}

TEST(MusicConstructDataMessageTests, Test_ConstructJSONRoundTrip_ValuesMatch) {
  ConstructDataMessageBuilder base_message_builder = CreateBaseMessageBuilder();
  // Add all the optional fields
  base_message_builder
      .SetGhostedId(EntityIdBuilder()
                        .SetEntityNumber(1)
                        .SetApplication(1)
                        .SetSite(1)
                        .Build())
      .SetForce(Force::kNeutral)
      .SetEntityType(EntityTypeBuilder()
                         .SetCategory(1)
                         .SetSubcategory(1)
                         .SetCountry(3)
                         .SetDomain(1)
                         .SetExtra(1)
                         .SetKind(1)
                         .SetSpecific(2)
                         .Build())
      .SetLocation(LocationBuilder().SetX(1).SetY(1).SetZ(1).Build())
      .SetOrientation(
          OrientationBuilder().SetPhi(1).SetPsi(1).SetTheta(1).Build())
      .SetVelocity(VectorBuilder().SetX(1).SetY(1).SetZ(1).Build())
      .SetDeadReckoning(
          DeadReckoningBuilder()
              .SetAngularVelocity(
                  VectorBuilder().SetX(1).SetY(1).SetZ(1).Build())
              .SetAlgorithmType(DeadReckoningAlgorithmType::kDrm_F_P_W)
              .SetLinearAcceleration(
                  VectorBuilder().SetX(1).SetY(1).SetZ(1).Build())
              .Build())
      .SetDamage(MusicDamageBuilder()
                     .SetCatastrophicDamage(1.0f)
                     .SetMobilityDamage(1.0f)
                     .SetIsFirepowerDisabled(false)
                     .Build());

  ConstructDataMessage message = base_message_builder.Build();
  nlohmann::json jsn;
  to_json(jsn, message);

  // Unmarshall
  ConstructDataMessage unmarshalled;
  from_json(jsn, unmarshalled);
  // Checks
  // Check Ghosted ID
  LONGS_EQUAL(message.GetGhostedId()->GetEntityNumber(),
              unmarshalled.GetGhostedId()->GetEntityNumber());
  LONGS_EQUAL(message.GetGhostedId()->GetApplication(),
              unmarshalled.GetGhostedId()->GetApplication());
  LONGS_EQUAL(message.GetGhostedId()->GetSite(),
              unmarshalled.GetGhostedId()->GetSite());
  // Check Force
  LONGS_EQUAL(static_cast<uint8_t>(message.GetForce().value()),
              static_cast<uint8_t>(unmarshalled.GetForce().value()));
  // Check Entity Type
  LONGS_EQUAL(message.GetEntityType()->GetCategory(),
              unmarshalled.GetEntityType()->GetCategory());
  LONGS_EQUAL(message.GetEntityType()->GetSubcategory(),
              unmarshalled.GetEntityType()->GetSubcategory());
  LONGS_EQUAL(message.GetEntityType()->GetCountry(),
              unmarshalled.GetEntityType()->GetCountry());
  LONGS_EQUAL(message.GetEntityType()->GetDomain(),
              unmarshalled.GetEntityType()->GetDomain());
  LONGS_EQUAL(message.GetEntityType()->GetExtra(),
              unmarshalled.GetEntityType()->GetExtra());
  LONGS_EQUAL(message.GetEntityType()->GetKind(),
              unmarshalled.GetEntityType()->GetKind());
  LONGS_EQUAL(message.GetEntityType()->GetSpecific(),
              unmarshalled.GetEntityType()->GetSpecific());
  // Check Location
  DOUBLES_EQUAL(message.GetLocation()->GetX(),
                unmarshalled.GetLocation()->GetX(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetLocation()->GetY(),
                unmarshalled.GetLocation()->GetY(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetLocation()->GetZ(),
                unmarshalled.GetLocation()->GetZ(), DOUBLE_TOLERANCE);
  // Check Orientation
  DOUBLES_EQUAL(message.GetOrientation()->GetPsi(),
                unmarshalled.GetOrientation()->GetPsi(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetOrientation()->GetTheta(),
                unmarshalled.GetOrientation()->GetTheta(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetOrientation()->GetPhi(),
                unmarshalled.GetOrientation()->GetPhi(), DOUBLE_TOLERANCE);
  // Check Velocity
  DOUBLES_EQUAL(message.GetVelocity()->GetX(),
                unmarshalled.GetVelocity()->GetX(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetVelocity()->GetY(),
                unmarshalled.GetVelocity()->GetY(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetVelocity()->GetZ(),
                unmarshalled.GetVelocity()->GetZ(), DOUBLE_TOLERANCE);
  // Check Dead Reckoning
  LONGS_EQUAL(
      static_cast<uint8_t>(message.GetDeadReckoning()->GetDeadReckAlgorithm()),
      static_cast<uint8_t>(
          unmarshalled.GetDeadReckoning()->GetDeadReckAlgorithm()));
  DOUBLES_EQUAL(message.GetDeadReckoning()->GetAngularVelocity().GetX(),
                unmarshalled.GetDeadReckoning()->GetAngularVelocity().GetX(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetDeadReckoning()->GetAngularVelocity().GetY(),
                unmarshalled.GetDeadReckoning()->GetAngularVelocity().GetY(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetDeadReckoning()->GetAngularVelocity().GetZ(),
                unmarshalled.GetDeadReckoning()->GetAngularVelocity().GetZ(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetDeadReckoning()->GetLinearAcceleration().GetX(),
                unmarshalled.GetDeadReckoning()->GetLinearAcceleration().GetX(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetDeadReckoning()->GetLinearAcceleration().GetY(),
                unmarshalled.GetDeadReckoning()->GetLinearAcceleration().GetY(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetDeadReckoning()->GetLinearAcceleration().GetZ(),
                unmarshalled.GetDeadReckoning()->GetLinearAcceleration().GetZ(),
                DOUBLE_TOLERANCE);
  // Check Damage
  DOUBLES_EQUAL(message.GetDamage()->GetCatastrophicDamage(),
                unmarshalled.GetDamage()->GetCatastrophicDamage(),
                DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(message.GetDamage()->GetMobilityDamage(),
                unmarshalled.GetDamage()->GetMobilityDamage(),
                DOUBLE_TOLERANCE);
  LONGS_EQUAL(
      static_cast<uint8_t>(message.GetDamage()->IsFirepowerDisabled()),
      static_cast<uint8_t>(unmarshalled.GetDamage()->IsFirepowerDisabled()));
  // Check Callsign, Construct Name, Interaction Record
  STRCMP_EQUAL(message.GetCallsign().c_str(),
               unmarshalled.GetCallsign().c_str());
  STRCMP_EQUAL(message.GetConstructName().c_str(),
               unmarshalled.GetConstructName().c_str());
  STRCMP_EQUAL(message.GetInteractionRecord().c_str(),
               unmarshalled.GetInteractionRecord().c_str());
}
}  // namespace music::test