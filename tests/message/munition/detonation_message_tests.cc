// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/event_id.h"
#include "message/munition/detonation_message.h"
#include "message/simulation_address.h"

namespace music::test {

TEST_GROUP(DetonationMessageTest){
    // SETUP
};

TEST(DetonationMessageTest, DefaultConstructor) {
  DetonationMessage message;
  LONGS_EQUAL(DetonationResultType::kUnset,
              static_cast<long>(message.GetResult()));

  // Shooter ID Entity ID
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetShooterId().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetShooterId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetShooterId().GetEntityNumber());

  // Target ID Entity ID (optional)
  CHECK_FALSE(message.GetTargetId().has_value());

  // Velocity Vector
  CHECK_TRUE(std::isnan(message.GetVelocity().GetX()));
  CHECK_TRUE(std::isnan(message.GetVelocity().GetY()));
  CHECK_TRUE(std::isnan(message.GetVelocity().GetZ()));

  // Location
  CHECK_TRUE(std::isnan(message.GetLocation().GetX()));
  CHECK_TRUE(std::isnan(message.GetLocation().GetY()));
  CHECK_TRUE(std::isnan(message.GetLocation().GetZ()));

  // Munitition EntityType
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetMunition().GetKind());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetDomain());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetCountry());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetCategory());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetSubcategory());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetSpecific());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetMunition().GetExtra());

  // Event ID
  CHECK_TRUE(message.GetEventId() == MusicEventIdBuilder().Build());
}
TEST(DetonationMessageTest, SettersAndGetters) {
  DetonationMessage message;

  message.SetResult(DetonationResultType::kAirBust);

  LONGS_EQUAL(DetonationResultType::kAirBust,
              static_cast<long>(message.GetResult()));
}

TEST(DetonationMessageTest, Validate) {
  // Valid header
  MusicMessageHeader valid_header;
  valid_header.SetExerciseId(100);
  valid_header.SetTimestamp(1765894109000);

  // Valid shooter id
  EntityId valid_shooter_id;
  valid_shooter_id.SetSite(1);
  valid_shooter_id.SetApplication(2);
  valid_shooter_id.SetEntityNumber(3);

  // Valid target id
  EntityId valid_target_id;
  valid_target_id.SetSite(4);
  valid_target_id.SetApplication(5);
  valid_target_id.SetEntityNumber(6);

  // Valid munition id
  EntityId valid_munition_id;
  valid_munition_id.SetSite(7);
  valid_munition_id.SetApplication(8);
  valid_munition_id.SetEntityNumber(9);

  // Valid velocity
  Vector valid_velocity;
  valid_velocity.SetX(10.0);
  valid_velocity.SetY(20.0);
  valid_velocity.SetZ(30.0);

  // Valid location
  Location valid_location;
  valid_location.SetX(40.0);
  valid_location.SetY(50.0);
  valid_location.SetZ(60.0);

  // Valid munition entity type
  EntityType valid_munition;
  valid_munition.SetKind(7);
  valid_munition.SetDomain(8);
  valid_munition.SetCountry(225);
  valid_munition.SetCategory(9);
  valid_munition.SetSubcategory(10);
  valid_munition.SetSpecific(11);
  valid_munition.SetExtra(12);

  // Valid event id
  MusicEventId valid_event_id;
  valid_event_id.SetSimulationAddress(
      SimulationAddressBuilder().SetApplication(14).SetSite(13).Build());
  valid_event_id.SetEventId(15);

  DetonationMessage message =
      DetonationMessageBuilder()
          .SetHeader(valid_header)
          .SetShooterId(valid_shooter_id)
          .SetTargetId(valid_target_id)
          .SetMunitionId(valid_munition_id)
          .SetVelocity(valid_velocity)
          .SetLocation(valid_location)
          .SetMunition(valid_munition)
          .SetEventId(valid_event_id)
          .SetWarhead(Warhead::kHighExplosive)
          .SetFuse(FuseType::kProximity)
          .SetQuantity(5)
          .SetRate(10)
          .SetResult(DetonationResultType::kEntityImpact)
          .Build();

  // Should not throw
  message.Validate();

  // Set invalid result
  message.SetResult(static_cast<DetonationResultType>(-1));
  CHECK_THROWS(std::exception, message.Validate());
  message.SetResult(DetonationResultType::kEntityImpact);

  // Set invalid shooter id
  message.SetShooterId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetShooterId(valid_shooter_id);

  // Final check should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL((std::string("Final validation threw an exception: ") +
          std::string(e.what()))
             .c_str());
  }
}

TEST(DetonationMessageTest, Builder) {
  DetonationMessage message =
      DetonationMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(1)
                         .SetTimestamp(123456789)
                         .Build())
          .SetShooterId(EntityIdBuilder()
                            .SetSite(10)
                            .SetApplication(20)
                            .SetEntityNumber(30)
                            .Build())
          .SetResult(DetonationResultType::kEntityImpact)
          .Build();

  // Check header
  LONGS_EQUAL(1, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(123456789, message.GetHeader().GetTimestamp());

  // Check shooter ID
  LONGS_EQUAL(10, message.GetShooterId().GetSite());
  LONGS_EQUAL(20, message.GetShooterId().GetApplication());
  LONGS_EQUAL(30, message.GetShooterId().GetEntityNumber());

  // Check result
  LONGS_EQUAL(static_cast<long>(DetonationResultType::kEntityImpact),
              static_cast<long>(message.GetResult()));
}

TEST(DetonationMessageTest, ToJson) {
  DetonationMessage message =
      DetonationMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(1)
                         .SetTimestamp(123456789)
                         .Build())
          .SetShooterId(EntityIdBuilder()
                            .SetSite(10)
                            .SetApplication(20)
                            .SetEntityNumber(30)
                            .Build())
          .SetTargetId(EntityIdBuilder()
                           .SetSite(40)
                           .SetApplication(50)
                           .SetEntityNumber(60)
                           .Build())
          .SetMunitionId(EntityIdBuilder()
                             .SetSite(70)
                             .SetApplication(80)
                             .SetEntityNumber(90)
                             .Build())
          .SetVelocity(VectorBuilder().SetX(0.1).SetY(0.2).SetZ(0.3).Build())
          .SetLocation(LocationBuilder().SetX(0.4).SetY(0.5).SetZ(0.6).Build())
          .SetEventId(MusicEventIdBuilder()
                          .SetSimulationAddress(SimulationAddressBuilder()
                                                    .SetApplication(8)
                                                    .SetSite(7)
                                                    .Build())
                          .SetEventId(9)
                          .Build())
          .SetFuse(FuseType::kAltitude)
          .SetWarhead(Warhead::kBiologicalGeneticallyModifiedMicroOrganism)
          .SetQuantity(2)
          .SetRate(200)
          .SetResult(DetonationResultType::kEntityImpact)
          .Build();

  nlohmann::json json = message.ToJson();

  // Check header
  LONGS_EQUAL(1, json["header"]["exerciseID"].get<long>());
  LONGS_EQUAL(123456789, json["header"]["timestamp"].get<long>());

  // Check shooter ID
  LONGS_EQUAL(10, json["shooterID"]["siteID"].get<long>());
  LONGS_EQUAL(20, json["shooterID"]["appID"].get<long>());
  LONGS_EQUAL(30, json["shooterID"]["entityID"].get<long>());

  // Check result
  LONGS_EQUAL(static_cast<long>(DetonationResultType::kEntityImpact),
              json["result"].get<long>());
}

TEST(DetonationMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "eventID": {
        "appID": 8,
        "eventID": 9,
        "siteID": 7
    },
    "fuse": 5000,
    "header": {
        "exerciseID": 1,
        "timestamp": 123456789
    },
    "location": {
        "x": 0.4,
        "y": 0.5,
        "z": 0.6
    },
    "munition": {
        "category": -1,
        "country": -1,
        "domain": -1,
        "extra": -1,
        "kind": -1,
        "specific": -1,
        "subcategory": -1
    },
    "munitionID": {
        "appID": 80,
        "entityID": 90,
        "siteID": 70
    },
    "quantity": 2,
    "rate": 200,
    "result": 1,
    "shooterID": {
        "appID": 20,
        "entityID": 30,
        "siteID": 10
    },
    "targetID": {
        "appID": 50,
        "entityID": 60,
        "siteID": 40
    },
    "velocity": {
        "x": 0.10000000149011612,
        "y": 0.20000000298023224,
        "z": 0.30000001192092896
    },
    "warhead": 9400
}
)");

  DetonationMessage message = DetonationMessage::FromJson(json);

  // Check new field and a couple others

  // Check result
  LONGS_EQUAL(static_cast<long>(DetonationResultType::kEntityImpact),
              static_cast<long>(message.GetResult()));

  // Check header
  LONGS_EQUAL(1, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(123456789, message.GetHeader().GetTimestamp());

  // Check shooter ID
  LONGS_EQUAL(10, message.GetShooterId().GetSite());
  LONGS_EQUAL(20, message.GetShooterId().GetApplication());
  LONGS_EQUAL(30, message.GetShooterId().GetEntityNumber());
}
}  // namespace music::test