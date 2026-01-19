// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "config/config.h"
#include "message/entity_type.h"
#include "message/perception/perception_record.h"

namespace music::test {

TEST_GROUP(PerceptionRecordTest){
    // SETUP
};

TEST(PerceptionRecordTest, DefaultConstructor) {
  PerceptionRecord record;
  LONGS_EQUAL(static_cast<long>(Force::kUnset),
              static_cast<long>(record.GetForce()));
  LONGS_EQUAL(static_cast<long>(SensorType::kUnset),
              static_cast<long>(record.GetSystem()));
  CHECK_TRUE(std::isnan(record.GetAccuracy()));
  LONGS_EQUAL(std::numeric_limits<int>::min(), record.GetBearingFactor());
  LONGS_EQUAL(std::numeric_limits<int>::min(), record.GetAltitudeError());
  LONGS_EQUAL(std::numeric_limits<int>::min(), record.GetRangeError());
}

TEST(PerceptionRecordTest, SettersAndGetters) {
  PerceptionRecord record;
  EntityId perception_id;
  perception_id.SetSite(1);
  perception_id.SetApplication(2);
  perception_id.SetEntityNumber(3);
  EntityType entity_type;
  entity_type.SetCategory(1);
  entity_type.SetSubcategory(1);
  entity_type.SetCountry(225);  // USA
  entity_type.SetDomain(1);
  entity_type.SetExtra(1);
  entity_type.SetKind(3);  // life form
  entity_type.SetSpecific(1);

  record.SetEntityType(entity_type);
  record.SetPerceptionId(perception_id);
  record.SetForce(Force::kFriendly);
  record.SetSystem(SensorType::kRadar);
  record.SetAccuracy(0.85);
  record.SetBearingFactor(10);
  record.SetAltitudeError(5);
  record.SetRangeError(15);

  LONGS_EQUAL(static_cast<long>(Force::kFriendly),
              static_cast<long>(record.GetForce()));
  LONGS_EQUAL(static_cast<long>(SensorType::kRadar),
              static_cast<long>(record.GetSystem()));
  DOUBLES_EQUAL(0.85, record.GetAccuracy(), 0.0);
  LONGS_EQUAL(10, record.GetBearingFactor());
  LONGS_EQUAL(5, record.GetAltitudeError());
  LONGS_EQUAL(15, record.GetRangeError());
  // Check Entity ID fields
  LONGS_EQUAL(1, record.GetPerceptionId().GetSite());
  LONGS_EQUAL(2, record.GetPerceptionId().GetApplication());
  LONGS_EQUAL(3, record.GetPerceptionId().GetEntityNumber());

  // Check Entity Type fields
  LONGS_EQUAL(1, record.GetEntityType().GetCategory());
  LONGS_EQUAL(1, record.GetEntityType().GetSubcategory());
  LONGS_EQUAL(225, record.GetEntityType().GetCountry());
  LONGS_EQUAL(1, record.GetEntityType().GetDomain());
  LONGS_EQUAL(1, record.GetEntityType().GetExtra());
  LONGS_EQUAL(3, record.GetEntityType().GetKind());
  LONGS_EQUAL(1, record.GetEntityType().GetSpecific());
}

TEST(PerceptionRecordTest, Builder) {
  PerceptionRecord record = PerceptionRecordBuilder()
                                .SetForce(Force::kOpposing)
                                .SetSystem(SensorType::kEsm)
                                .SetAccuracy(0.95)
                                .SetBearingFactor(20)
                                .SetAltitudeError(8)
                                .SetRangeError(12)
                                .SetEntityType(EntityTypeBuilder()
                                                   .SetCategory(1)
                                                   .SetSubcategory(1)
                                                   .SetCountry(225)  // USA
                                                   .SetDomain(1)
                                                   .SetExtra(1)
                                                   .SetKind(3)  // life form
                                                   .SetSpecific(1)
                                                   .Build())
                                .SetPerceptionId(EntityIdBuilder()
                                                     .SetEntityNumber(0)
                                                     .SetApplication(300)
                                                     .SetSite(50)
                                                     .Build())
                                .Build();

  LONGS_EQUAL(static_cast<long>(Force::kOpposing),
              static_cast<long>(record.GetForce()));
  LONGS_EQUAL(static_cast<long>(SensorType::kEsm),
              static_cast<long>(record.GetSystem()));
  DOUBLES_EQUAL(0.95, record.GetAccuracy(), 0.0);
  LONGS_EQUAL(20, record.GetBearingFactor());
  LONGS_EQUAL(8, record.GetAltitudeError());
  LONGS_EQUAL(12, record.GetRangeError());

  // Check Entity ID fields
  LONGS_EQUAL(50, record.GetPerceptionId().GetSite());
  LONGS_EQUAL(300, record.GetPerceptionId().GetApplication());
  LONGS_EQUAL(0, record.GetPerceptionId().GetEntityNumber());

  // Check Entity Type fields
  LONGS_EQUAL(1, record.GetEntityType().GetCategory());
  LONGS_EQUAL(1, record.GetEntityType().GetSubcategory());
  LONGS_EQUAL(225, record.GetEntityType().GetCountry());
  LONGS_EQUAL(1, record.GetEntityType().GetDomain());
  LONGS_EQUAL(1, record.GetEntityType().GetExtra());
  LONGS_EQUAL(3, record.GetEntityType().GetKind());
  LONGS_EQUAL(1, record.GetEntityType().GetSpecific());
}

TEST(PerceptionRecordTest, Validate) {
  EntityType valid_entity_type = EntityTypeBuilder()
                                     .SetCategory(1)
                                     .SetSubcategory(1)
                                     .SetCountry(225)  // USA
                                     .SetDomain(1)
                                     .SetExtra(1)
                                     .SetKind(3)  // life form
                                     .SetSpecific(1)
                                     .Build();

  EntityId valid_perception_id = EntityIdBuilder()
                                     .SetEntityNumber(0)
                                     .SetApplication(300)
                                     .SetSite(50)
                                     .Build();

  PerceptionRecord record = PerceptionRecordBuilder()
                                .SetForce(Force::kOpposing)
                                .SetSystem(SensorType::kEsm)
                                .SetAccuracy(0.95)
                                .SetBearingFactor(20)
                                .SetAltitudeError(8)
                                .SetRangeError(12)
                                .SetEntityType(valid_entity_type)
                                .SetPerceptionId(valid_perception_id)
                                .Build();

  // Should not throw any exceptions
  try {
    record.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set invalid force
  record.SetForce(static_cast<Force>(-1));
  CHECK_THROWS(std::exception, record.Validate());
  record.SetForce(Force::kNeutral);

  // Set invalid system
  record.SetSystem(static_cast<SensorType>(-1));
  CHECK_THROWS(std::exception, record.Validate());

  record.SetSystem(SensorType::kAuditory);

  // Set invalid accuracy
  record.SetAccuracy(1.5);
  CHECK_THROWS(std::exception, record.Validate());
  record.SetAccuracy(0.75);

  // Set negative bearing factor
  record.SetBearingFactor(-10);
  CHECK_THROWS(std::exception, record.Validate());
  record.SetBearingFactor(0);

  // Set negative altitude error
  record.SetAltitudeError(-5);
  CHECK_THROWS(std::exception, record.Validate());
  record.SetAltitudeError(0);

  // Set negative range error
  record.SetRangeError(-20);
  CHECK_THROWS(std::exception, record.Validate());
  record.SetRangeError(0);

  // Set invalid entity type
  record.SetEntityType(EntityType());
  CHECK_THROWS(std::exception, record.Validate());
  record.SetEntityType(valid_entity_type);

  // Set invalid perception id
  record.SetPerceptionId(EntityId());
  CHECK_THROWS(std::exception, record.Validate());
  record.SetPerceptionId(valid_perception_id);

  // Final check should pass
  try {
    record.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }
}

TEST(PerceptionRecordTest, ToJson) {
  PerceptionRecord record = PerceptionRecordBuilder()
                                .SetForce(Force::kOpposing)
                                .SetSystem(SensorType::kEsm)
                                .SetAccuracy(0.95)
                                .SetBearingFactor(20)
                                .SetAltitudeError(8)
                                .SetRangeError(12)
                                .SetEntityType(EntityTypeBuilder()
                                                   .SetCategory(1)
                                                   .SetSubcategory(1)
                                                   .SetCountry(225)  // USA
                                                   .SetDomain(1)
                                                   .SetExtra(1)
                                                   .SetKind(3)  // life form
                                                   .SetSpecific(1)
                                                   .Build())
                                .SetPerceptionId(EntityIdBuilder()
                                                     .SetEntityNumber(0)
                                                     .SetApplication(300)
                                                     .SetSite(50)
                                                     .Build())
                                .Build();

  nlohmann::json j = record.ToJson();

  LONGS_EQUAL(static_cast<long>(Force::kOpposing),
              j[PerceptionRecord::kForceKey].get<long>());
  LONGS_EQUAL(static_cast<long>(SensorType::kEsm),
              j[PerceptionRecord::kSystemKey].get<long>());
  DOUBLES_EQUAL(0.95, j[PerceptionRecord::kAccuracyKey].get<double>(), 0.0);
  LONGS_EQUAL(20, j[PerceptionRecord::kBearingFactorKey].get<int>());
  LONGS_EQUAL(8, j[PerceptionRecord::kAltitudeErrorKey].get<int>());
  LONGS_EQUAL(12, j[PerceptionRecord::kRangeErrorKey].get<int>());

  // Check Entity ID fields
  LONGS_EQUAL(50, j[PerceptionRecord::kPerceptionIdKey]["siteID"].get<int>());
  LONGS_EQUAL(300, j[PerceptionRecord::kPerceptionIdKey]["appID"].get<int>());
  LONGS_EQUAL(0, j[PerceptionRecord::kPerceptionIdKey]["entityID"].get<int>());
  // Check Entity Type fields
  LONGS_EQUAL(1, j[PerceptionRecord::kEntityTypeKey]["category"].get<int>());
  LONGS_EQUAL(1, j[PerceptionRecord::kEntityTypeKey]["subcategory"].get<int>());
  LONGS_EQUAL(225, j[PerceptionRecord::kEntityTypeKey]["country"].get<int>());
  LONGS_EQUAL(1, j[PerceptionRecord::kEntityTypeKey]["domain"].get<int>());
  LONGS_EQUAL(1, j[PerceptionRecord::kEntityTypeKey]["extra"].get<int>());
  LONGS_EQUAL(3, j[PerceptionRecord::kEntityTypeKey]["kind"].get<int>());
  LONGS_EQUAL(1, j[PerceptionRecord::kEntityTypeKey]["specific"].get<int>());
}

TEST(PerceptionRecordTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "accuracy": 0.95,
    "altitudeError": 8,
    "bearingFactor": 20,
    "entityType": {
        "category": 1,
        "country": 225,
        "domain": 1,
        "extra": 1,
        "kind": 3,
        "specific": 1,
        "subcategory": 1
    },
    "force": 2,
    "perceptionID": {
        "appID": 300,
        "entityID": 0,
        "siteID": 50
    },
    "rangeError": 12,
    "system": 1
}
)");

  PerceptionRecord record = PerceptionRecord::FromJson(json);
  LONGS_EQUAL(static_cast<long>(Force::kOpposing),
              static_cast<long>(record.GetForce()));
  LONGS_EQUAL(static_cast<long>(SensorType::kEsm),
              static_cast<long>(record.GetSystem()));
  DOUBLES_EQUAL(0.95, record.GetAccuracy(), kDoublePrecision);
  LONGS_EQUAL(20, record.GetBearingFactor());
  LONGS_EQUAL(8, record.GetAltitudeError());
  LONGS_EQUAL(12, record.GetRangeError());
  // Check Entity Type fields
  LONGS_EQUAL(1, record.GetEntityType().GetCategory());
  LONGS_EQUAL(1, record.GetEntityType().GetSubcategory());
  LONGS_EQUAL(225, record.GetEntityType().GetCountry());
  LONGS_EQUAL(1, record.GetEntityType().GetDomain());
  LONGS_EQUAL(1, record.GetEntityType().GetExtra());
  LONGS_EQUAL(3, record.GetEntityType().GetKind());
  LONGS_EQUAL(1, record.GetEntityType().GetSpecific());

  // Check Perception ID fields
  LONGS_EQUAL(50, record.GetPerceptionId().GetSite());
  LONGS_EQUAL(300, record.GetPerceptionId().GetApplication());
  LONGS_EQUAL(0, record.GetPerceptionId().GetEntityNumber());
}
}  // namespace music::test