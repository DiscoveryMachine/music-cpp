// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <limits>

#include "config/config.h"
#include "message/munition/fire_message.h"
#include "message/simulation_address.h"

namespace music::test {

TEST_GROUP(FireMessageTest){
    // SETUP
};

TEST(FireMessageTest, DefaultConstructor) {
  FireMessage message;
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetRate());
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetQuantity());
  CHECK_TRUE(std::isnan(message.GetRangeToTarget()));

  LONGS_EQUAL(Warhead::kUnset, static_cast<long>(message.GetWarhead()));
  LONGS_EQUAL(FuseType::kUnset, static_cast<long>(message.GetFuse()));

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
  CHECK_TRUE(SimulationAddressBuilder().Build() ==
             message.GetEventId().GetSimulationAddress());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetEventId().GetEventId());
}

TEST(FireMessageTest, SettersAndGetters) {
  FireMessage message;

  // Shooter ID
  EntityId shooter_id;
  shooter_id.SetSite(10);
  shooter_id.SetApplication(20);
  shooter_id.SetEntityNumber(30);
  message.SetShooterId(shooter_id);

  // Target ID
  EntityId target_id;
  target_id.SetSite(40);
  target_id.SetApplication(50);
  target_id.SetEntityNumber(60);
  message.SetTargetId(target_id);

  // Velocity Vector
  Vector velocity;
  velocity.SetX(100.0);
  velocity.SetY(200.0);
  velocity.SetZ(300.0);
  message.SetVelocity(velocity);

  // Location
  Location location;
  location.SetX(400.0);
  location.SetY(500.0);
  location.SetZ(600.0);
  message.SetLocation(location);

  // Munition EntityType
  EntityType munition;
  munition.SetKind(1);
  munition.SetDomain(2);
  munition.SetCountry(225);  // USA
  munition.SetCategory(3);
  munition.SetSubcategory(4);
  munition.SetSpecific(5);
  munition.SetExtra(6);
  message.SetMunition(munition);

  // Event ID
  MusicEventId event_id;
  event_id.SetSimulationAddress(
      SimulationAddressBuilder().SetSite(7).SetApplication(8).Build());
  event_id.SetEventId(9);
  message.SetEventId(event_id);

  // Other fields
  message.SetWarhead(Warhead::kHighExplosive);
  message.SetFuse(FuseType::kProximity);
  message.SetRate(15);
  message.SetQuantity(25);
  message.SetRangeToTarget(3500.5);

  // Validate getters
  LONGS_EQUAL(10, message.GetShooterId().GetSite());
  LONGS_EQUAL(20, message.GetShooterId().GetApplication());
  LONGS_EQUAL(30, message.GetShooterId().GetEntityNumber());

  LONGS_EQUAL(40, message.GetTargetId()->GetSite());
  LONGS_EQUAL(50, message.GetTargetId()->GetApplication());
  LONGS_EQUAL(60, message.GetTargetId()->GetEntityNumber());

  DOUBLES_EQUAL(100.0, message.GetVelocity().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(200.0, message.GetVelocity().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(300.0, message.GetVelocity().GetZ(), kDoublePrecision);
  DOUBLES_EQUAL(400.0, message.GetLocation().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(500.0, message.GetLocation().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(600.0, message.GetLocation().GetZ(), kDoublePrecision);
  LONGS_EQUAL(1, message.GetMunition().GetKind());
  LONGS_EQUAL(2, message.GetMunition().GetDomain());
  LONGS_EQUAL(225, message.GetMunition().GetCountry());
  LONGS_EQUAL(3, message.GetMunition().GetCategory());
  LONGS_EQUAL(4, message.GetMunition().GetSubcategory());
  LONGS_EQUAL(5, message.GetMunition().GetSpecific());
  LONGS_EQUAL(6, message.GetMunition().GetExtra());
  LONGS_EQUAL(7, message.GetEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(8, message.GetEventId().GetSimulationAddress().GetApplication());
  LONGS_EQUAL(9, message.GetEventId().GetEventId());
  LONGS_EQUAL(static_cast<long>(Warhead::kHighExplosive),
              static_cast<long>(message.GetWarhead()));
  LONGS_EQUAL(static_cast<long>(FuseType::kProximity),
              static_cast<long>(message.GetFuse()));
  LONGS_EQUAL(15, message.GetRate());
  LONGS_EQUAL(25, message.GetQuantity());
  DOUBLES_EQUAL(3500.5, message.GetRangeToTarget(), kDoublePrecision);
}

TEST(FireMessageTest, Builder) {
  FireMessage message =
      FireMessageBuilder()
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
          .SetVelocity(
              VectorBuilder().SetX(100.0).SetY(200.0).SetZ(300.0).Build())
          .SetLocation(
              LocationBuilder().SetX(400.00).SetY(500.0).SetZ(600.0).Build())
          .SetMunition(EntityTypeBuilder()
                           .SetKind(1)
                           .SetDomain(2)
                           .SetCountry(225)
                           .SetCategory(3)
                           .SetSubcategory(4)
                           .SetSpecific(5)
                           .SetExtra(6)
                           .Build())
          .SetEventId(MusicEventIdBuilder()
                          .SetSimulationAddress(SimulationAddressBuilder()
                                                    .SetSite(7)
                                                    .SetApplication(8)
                                                    .Build())
                          .SetEventId(9)
                          .Build())
          .SetWarhead(Warhead::kHighExplosive)
          .SetFuse(FuseType::kProximity)
          .SetRate(15)
          .SetQuantity(25)
          .SetRangeToTarget(3500.5)
          .Build();

  LONGS_EQUAL(10, message.GetShooterId().GetSite());
  LONGS_EQUAL(20, message.GetShooterId().GetApplication());
  LONGS_EQUAL(30, message.GetShooterId().GetEntityNumber());

  LONGS_EQUAL(40, message.GetTargetId()->GetSite());
  LONGS_EQUAL(50, message.GetTargetId()->GetApplication());
  LONGS_EQUAL(60, message.GetTargetId()->GetEntityNumber());

  DOUBLES_EQUAL(100.0, message.GetVelocity().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(200.0, message.GetVelocity().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(300.0, message.GetVelocity().GetZ(), kDoublePrecision);
  DOUBLES_EQUAL(400.0, message.GetLocation().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(500.0, message.GetLocation().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(600.0, message.GetLocation().GetZ(), kDoublePrecision);
  LONGS_EQUAL(1, message.GetMunition().GetKind());
  LONGS_EQUAL(2, message.GetMunition().GetDomain());
  LONGS_EQUAL(225, message.GetMunition().GetCountry());
  LONGS_EQUAL(3, message.GetMunition().GetCategory());
  LONGS_EQUAL(4, message.GetMunition().GetSubcategory());
  LONGS_EQUAL(5, message.GetMunition().GetSpecific());
  LONGS_EQUAL(6, message.GetMunition().GetExtra());
  LONGS_EQUAL(7, message.GetEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(8, message.GetEventId().GetSimulationAddress().GetApplication());
  LONGS_EQUAL(9, message.GetEventId().GetEventId());
  LONGS_EQUAL(static_cast<long>(Warhead::kHighExplosive),
              static_cast<long>(message.GetWarhead()));
  LONGS_EQUAL(static_cast<long>(FuseType::kProximity),
              static_cast<long>(message.GetFuse()));
  LONGS_EQUAL(15, message.GetRate());
  LONGS_EQUAL(25, message.GetQuantity());
  DOUBLES_EQUAL(3500.5, message.GetRangeToTarget(), kDoublePrecision);
}

TEST(FireMessageTest, Validate) {
  MusicMessageHeader header;
  header.SetExerciseId(1);
  header.SetTimestamp(123456789);

  EntityId valid_shooter_id;
  valid_shooter_id.SetSite(10);
  valid_shooter_id.SetApplication(20);
  valid_shooter_id.SetEntityNumber(30);

  EntityId valid_target_id;
  valid_target_id.SetSite(40);
  valid_target_id.SetApplication(50);
  valid_target_id.SetEntityNumber(60);

  EntityId valid_munition_id;
  valid_munition_id.SetSite(70);
  valid_munition_id.SetApplication(80);
  valid_munition_id.SetEntityNumber(90);

  Vector valid_velocity;
  valid_velocity.SetX(100.0);
  valid_velocity.SetY(200.0);
  valid_velocity.SetZ(300.0);

  Location valid_location;
  valid_location.SetX(400.0);
  valid_location.SetY(500.0);
  valid_location.SetZ(600.0);

  EntityType valid_munition;
  valid_munition.SetKind(1);
  valid_munition.SetDomain(2);
  valid_munition.SetCountry(225);
  valid_munition.SetCategory(3);
  valid_munition.SetSubcategory(4);
  valid_munition.SetSpecific(5);
  valid_munition.SetExtra(6);

  MusicEventId valid_event_id;
  valid_event_id.SetSimulationAddress(
      SimulationAddressBuilder().SetSite(7).SetApplication(8).Build());
  valid_event_id.SetEventId(9);

  FireMessage message = FireMessageBuilder()
                            .SetHeader(header)
                            .SetShooterId(valid_shooter_id)
                            .SetTargetId(valid_target_id)
                            .SetMunitionId(valid_munition_id)
                            .SetVelocity(valid_velocity)
                            .SetLocation(valid_location)
                            .SetMunition(valid_munition)
                            .SetEventId(valid_event_id)
                            .SetWarhead(Warhead::kHighExplosive)
                            .SetFuse(FuseType::kProximity)
                            .SetRate(15)
                            .SetQuantity(25)
                            .SetRangeToTarget(3500.5)
                            .Build();

  // Should not throw error, passing validation
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set invalid shooter id
  message.SetShooterId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetShooterId(valid_shooter_id);

  // Set null target id (should not throw, as target id is optional)
  message.SetTargetId(std::nullopt);
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }
  message.SetTargetId(valid_target_id);

  // Set invalid munition id entity id
  message.SetMunitionId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetMunitionId(valid_munition_id);

  // Set invalid velocity
  message.SetVelocity(Vector());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetVelocity(valid_velocity);

  // Set invalid location
  message.SetLocation(Location());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetLocation(valid_location);

  // Set invalid munition entity type
  message.SetMunition(EntityType());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetMunition(valid_munition);

  // Set invalid event id
  message.SetEventId(MusicEventId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetEventId(valid_event_id);

  // Final check should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }
}

TEST(FireMessageTest, ToJson) {
  FireMessage message =
      FireMessageBuilder()
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
          .SetVelocity(
              VectorBuilder().SetX(100.0).SetY(200.0).SetZ(300.0).Build())
          .SetLocation(
              LocationBuilder().SetX(400.00).SetY(500.0).SetZ(600.0).Build())
          .SetMunition(EntityTypeBuilder()
                           .SetKind(1)
                           .SetDomain(2)
                           .SetCountry(225)
                           .SetCategory(3)
                           .SetSubcategory(4)
                           .SetSpecific(5)
                           .SetExtra(6)
                           .Build())
          .SetEventId(MusicEventIdBuilder()
                          .SetSimulationAddress(SimulationAddressBuilder()
                                                    .SetSite(7)
                                                    .SetApplication(8)
                                                    .Build())
                          .SetEventId(9)
                          .Build())
          .SetWarhead(Warhead::kHighExplosive)
          .SetFuse(FuseType::kProximity)
          .SetRate(15)
          .SetQuantity(25)
          .SetRangeToTarget(3500.5)
          .Build();

  nlohmann::json json = message.ToJson();

  // check header
  LONGS_EQUAL(1, json["header"]["exerciseID"].get<long>());
  LONGS_EQUAL(123456789, json["header"]["timestamp"].get<long>());

  // check shooter ID
  LONGS_EQUAL(10, json["shooterID"]["siteID"].get<long>());
  LONGS_EQUAL(20, json["shooterID"]["appID"].get<long>());
  LONGS_EQUAL(30, json["shooterID"]["entityID"].get<long>());

  // check target ID
  LONGS_EQUAL(40, json["targetID"]["siteID"].get<long>());
  LONGS_EQUAL(50, json["targetID"]["appID"].get<long>());
  LONGS_EQUAL(60, json["targetID"]["entityID"].get<long>());

  // check munition ID
  LONGS_EQUAL(70, json["munitionID"]["siteID"].get<long>());
  LONGS_EQUAL(80, json["munitionID"]["appID"].get<long>());
  LONGS_EQUAL(90, json["munitionID"]["entityID"].get<long>());

  // check velocity
  DOUBLES_EQUAL(100.0, json["velocity"]["x"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(200.0, json["velocity"]["y"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(300.0, json["velocity"]["z"].get<double>(), kDoublePrecision);

  // check location
  DOUBLES_EQUAL(400.0, json["location"]["x"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(500.0, json["location"]["y"].get<double>(), kDoublePrecision);
  DOUBLES_EQUAL(600.0, json["location"]["z"].get<double>(), kDoublePrecision);

  // check munition
  LONGS_EQUAL(1, json["munition"]["kind"].get<long>());
  LONGS_EQUAL(2, json["munition"]["domain"].get<long>());
  LONGS_EQUAL(225, json["munition"]["country"].get<long>());
  LONGS_EQUAL(3, json["munition"]["category"].get<long>());
  LONGS_EQUAL(4, json["munition"]["subcategory"].get<long>());
  LONGS_EQUAL(5, json["munition"]["specific"].get<long>());
  LONGS_EQUAL(6, json["munition"]["extra"].get<long>());

  // check event ID
  LONGS_EQUAL(7, json["eventID"]["siteID"].get<long>());
  LONGS_EQUAL(8, json["eventID"]["appID"].get<long>());
  LONGS_EQUAL(9, json["eventID"]["eventID"].get<long>());

  // check fire message fields
  LONGS_EQUAL(static_cast<long>(Warhead::kHighExplosive),
              json["warhead"].get<long>());
  LONGS_EQUAL(static_cast<long>(FuseType::kProximity),
              json["fuse"].get<long>());
  LONGS_EQUAL(15, json["rate"].get<long>());
  LONGS_EQUAL(25, json["quantity"].get<long>());
  DOUBLES_EQUAL(3500.5, json["rangeToTarget"].get<double>(), kDoublePrecision);
}

TEST(FireMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "eventID": {
        "appID": 8,
        "eventID": 9,
        "siteID": 7
    },
    "fuse": 3000,
    "header": {
        "exerciseID": 1,
        "timestamp": 123456789
    },
    "location": {
        "x": 400.0,
        "y": 500.0,
        "z": 600.0
    },
    "munition": {
        "category": 3,
        "country": 225,
        "domain": 2,
        "extra": 6,
        "kind": 1,
        "specific": 5,
        "subcategory": 4
    },
    "munitionID": {
        "appID": 80,
        "entityID": 90,
        "siteID": 70
    },
    "quantity": 25,
    "rangeToTarget": 3500.5,
    "rate": 15,
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
        "x": 100.0,
        "y": 200.0,
        "z": 300.0
    },
    "warhead": 1000
}
)");

  FireMessage message = FireMessage::FromJson(json);

  // Check header
  LONGS_EQUAL(1, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(123456789, message.GetHeader().GetTimestamp());

  // Check shooter ID
  LONGS_EQUAL(10, message.GetShooterId().GetSite());
  LONGS_EQUAL(20, message.GetShooterId().GetApplication());
  LONGS_EQUAL(30, message.GetShooterId().GetEntityNumber());

  // Check target ID
  LONGS_EQUAL(40, message.GetTargetId()->GetSite());
  LONGS_EQUAL(50, message.GetTargetId()->GetApplication());
  LONGS_EQUAL(60, message.GetTargetId()->GetEntityNumber());

  // Check munition ID
  LONGS_EQUAL(70, message.GetMunitionId().GetSite());
  LONGS_EQUAL(80, message.GetMunitionId().GetApplication());
  LONGS_EQUAL(90, message.GetMunitionId().GetEntityNumber());

  // Check velocity
  DOUBLES_EQUAL(100.0, message.GetVelocity().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(200.0, message.GetVelocity().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(300.0, message.GetVelocity().GetZ(), kDoublePrecision);

  // Check location
  DOUBLES_EQUAL(400.0, message.GetLocation().GetX(), kDoublePrecision);
  DOUBLES_EQUAL(500.0, message.GetLocation().GetY(), kDoublePrecision);
  DOUBLES_EQUAL(600.0, message.GetLocation().GetZ(), kDoublePrecision);

  // Check munition
  LONGS_EQUAL(1, message.GetMunition().GetKind());
  LONGS_EQUAL(2, message.GetMunition().GetDomain());
  LONGS_EQUAL(225, message.GetMunition().GetCountry());
  LONGS_EQUAL(3, message.GetMunition().GetCategory());
  LONGS_EQUAL(4, message.GetMunition().GetSubcategory());
  LONGS_EQUAL(5, message.GetMunition().GetSpecific());
  LONGS_EQUAL(6, message.GetMunition().GetExtra());

  // Check event ID
  LONGS_EQUAL(7, message.GetEventId().GetSimulationAddress().GetSite());
  LONGS_EQUAL(8, message.GetEventId().GetSimulationAddress().GetApplication());
  LONGS_EQUAL(9, message.GetEventId().GetEventId());

  // Check fire message fields
  LONGS_EQUAL(static_cast<long>(Warhead::kHighExplosive),
              static_cast<long>(message.GetWarhead()));
  LONGS_EQUAL(static_cast<long>(FuseType::kProximity),
              static_cast<long>(message.GetFuse()));
  LONGS_EQUAL(15, message.GetRate());
  LONGS_EQUAL(25, message.GetQuantity());
  DOUBLES_EQUAL(3500.5, message.GetRangeToTarget(), kDoublePrecision);
}
}  // namespace music::test