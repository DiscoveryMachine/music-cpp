// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "config/config.h"
#include "message/perception/perception_data_message.h"
#include "message/perception/perception_record.h"

namespace music::test {
TEST_GROUP(PerceptionDataMessageTest){
    // SETUP
};

TEST(PerceptionDataMessageTest, DefaultConstructor) {
  PerceptionDataMessage message;
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetOriginId().GetSite());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetApplication());
  LONGS_EQUAL(std::numeric_limits<int>::min(),
              message.GetOriginId().GetEntityNumber());
  LONGS_EQUAL(std::numeric_limits<int>::min(), message.GetNumPerceptions());
  LONGS_EQUAL(0, static_cast<long>(message.GetPerceptionRecords().size()));
}

TEST(PerceptionDataMessageTest, SettersAndGetters) {
  PerceptionDataMessage message;

  EntityId origin_id;
  origin_id.SetSite(1);
  origin_id.SetApplication(2);
  origin_id.SetEntityNumber(3);

  message.SetOriginId(origin_id);

  message.SetNumPerceptions(2);

  // Record 1
  PerceptionRecord record1;
  EntityId record1_perception_id;
  record1_perception_id.SetSite(4);
  record1_perception_id.SetApplication(5);
  record1_perception_id.SetEntityNumber(6);

  EntityType record1_entity_type;
  record1_entity_type.SetKind(1);
  record1_entity_type.SetDomain(2);
  record1_entity_type.SetCountry(225);  // USA
  record1_entity_type.SetCategory(3);
  record1_entity_type.SetSubcategory(4);
  record1_entity_type.SetSpecific(5);
  record1_entity_type.SetExtra(6);

  record1.SetPerceptionId(record1_perception_id);
  record1.SetForce(Force::kFriendly);
  record1.SetAccuracy(.98);
  record1.SetBearingFactor(12);
  record1.SetAltitudeError(3);
  record1.SetRangeError(7);
  record1.SetEntityType(record1_entity_type);

  // Record 2
  PerceptionRecord record2;
  EntityId record2_perception_id;
  record2_perception_id.SetSite(7);
  record2_perception_id.SetApplication(8);
  record2_perception_id.SetEntityNumber(9);
  record2.SetPerceptionId(record2_perception_id);
  record2.SetForce(Force::kNeutral);
  record2.SetAccuracy(.85);
  record2.SetBearingFactor(15);
  record2.SetAltitudeError(4);
  record2.SetRangeError(10);
  EntityType record2_entity_type;
  record2_entity_type.SetKind(2);
  record2_entity_type.SetDomain(3);
  record2_entity_type.SetCountry(225);
  record2_entity_type.SetCategory(4);
  record2_entity_type.SetSubcategory(5);
  record2_entity_type.SetSpecific(6);
  record2_entity_type.SetExtra(7);
  record2.SetEntityType(record2_entity_type);

  message.SetPerceptionRecords({record1, record2});

  LONGS_EQUAL(1, message.GetOriginId().GetSite());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, message.GetOriginId().GetEntityNumber());

  LONGS_EQUAL(2, message.GetNumPerceptions());

  LONGS_EQUAL(2, static_cast<long>(message.GetPerceptionRecords().size()));
  // Record 1 checks
  PerceptionRecord gotten_record1 = message.GetPerceptionRecords().at(0);
  LONGS_EQUAL(4, gotten_record1.GetPerceptionId().GetSite());
  LONGS_EQUAL(5, gotten_record1.GetPerceptionId().GetApplication());
  LONGS_EQUAL(6, gotten_record1.GetPerceptionId().GetEntityNumber());
  LONGS_EQUAL(static_cast<long>(Force::kFriendly),
              static_cast<long>(gotten_record1.GetForce()));
  DOUBLES_EQUAL(.98, gotten_record1.GetAccuracy(), 0.0);
  LONGS_EQUAL(12, gotten_record1.GetBearingFactor());
  LONGS_EQUAL(3, gotten_record1.GetAltitudeError());
  LONGS_EQUAL(7, gotten_record1.GetRangeError());
  LONGS_EQUAL(1, gotten_record1.GetEntityType().GetKind());
  LONGS_EQUAL(2, gotten_record1.GetEntityType().GetDomain());
  LONGS_EQUAL(225, gotten_record1.GetEntityType().GetCountry());
  LONGS_EQUAL(3, gotten_record1.GetEntityType().GetCategory());
  LONGS_EQUAL(4, gotten_record1.GetEntityType().GetSubcategory());
  LONGS_EQUAL(5, gotten_record1.GetEntityType().GetSpecific());
  LONGS_EQUAL(6, gotten_record1.GetEntityType().GetExtra());

  // Record 2 checks
  PerceptionRecord gotten_record2 = message.GetPerceptionRecords().at(1);
  LONGS_EQUAL(7, gotten_record2.GetPerceptionId().GetSite());
  LONGS_EQUAL(8, gotten_record2.GetPerceptionId().GetApplication());
  LONGS_EQUAL(9, gotten_record2.GetPerceptionId().GetEntityNumber());
  LONGS_EQUAL(static_cast<long>(Force::kNeutral),
              static_cast<long>(gotten_record2.GetForce()));
  DOUBLES_EQUAL(.85, gotten_record2.GetAccuracy(), 0.0);
  LONGS_EQUAL(15, gotten_record2.GetBearingFactor());
  LONGS_EQUAL(4, gotten_record2.GetAltitudeError());
  LONGS_EQUAL(10, gotten_record2.GetRangeError());
  LONGS_EQUAL(2, gotten_record2.GetEntityType().GetKind());
  LONGS_EQUAL(3, gotten_record2.GetEntityType().GetDomain());
  LONGS_EQUAL(225, gotten_record2.GetEntityType().GetCountry());
  LONGS_EQUAL(4, gotten_record2.GetEntityType().GetCategory());
  LONGS_EQUAL(5, gotten_record2.GetEntityType().GetSubcategory());
  LONGS_EQUAL(6, gotten_record2.GetEntityType().GetSpecific());
  LONGS_EQUAL(7, gotten_record2.GetEntityType().GetExtra());
}

TEST(PerceptionDataMessageTest, Builder) {
  PerceptionRecord record1 = PerceptionRecordBuilder()
                                 .SetPerceptionId(EntityIdBuilder()
                                                      .SetSite(4)
                                                      .SetApplication(5)
                                                      .SetEntityNumber(6)
                                                      .Build())
                                 .SetForce(Force::kFriendly)
                                 .SetAccuracy(.98)
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
                                 .SetAccuracy(.85)
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
                                      .SetOriginId(EntityIdBuilder()
                                                       .SetSite(1)
                                                       .SetApplication(2)
                                                       .SetEntityNumber(3)
                                                       .Build())
                                      .SetNumPerceptions(2)
                                      .SetPerceptionRecords({record1, record2})
                                      .Build();

  LONGS_EQUAL(1, message.GetOriginId().GetSite());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, message.GetOriginId().GetEntityNumber());

  LONGS_EQUAL(2, message.GetNumPerceptions());

  LONGS_EQUAL(2, static_cast<long>(message.GetPerceptionRecords().size()));
}

TEST(PerceptionDataMessageTest, Validate) {
  PerceptionRecord record1 = PerceptionRecordBuilder()
                                 .SetPerceptionId(EntityIdBuilder()
                                                      .SetSite(4)
                                                      .SetApplication(5)
                                                      .SetEntityNumber(6)
                                                      .Build())
                                 .SetForce(Force::kFriendly)
                                 .SetAccuracy(.98)
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
                                 .SetSystem(SensorType::kRadar)
                                 .Build();

  PerceptionDataMessage message =
      PerceptionDataMessageBuilder()
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(128)
                         .SetTimestamp(1765820313617)
                         .Build())
          .SetOriginId(EntityIdBuilder()
                           .SetSite(1)
                           .SetApplication(2)
                           .SetEntityNumber(3)
                           .Build())
          .SetNumPerceptions(1)
          .SetPerceptionRecords({record1})
          .Build();

  // Should not throw any exceptions
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }

  // Set negative num perceptions
  message.SetNumPerceptions(-5);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetNumPerceptions(1);

  // Set num perceptions not matching size of records
  message.SetNumPerceptions(2);
  CHECK_THROWS(std::exception, message.Validate());
  message.SetNumPerceptions(1);

  // Set invalid origin id
  message.SetOriginId(EntityId());
  CHECK_THROWS(std::exception, message.Validate());
  message.SetOriginId(EntityIdBuilder()
                          .SetSite(1)
                          .SetApplication(2)
                          .SetEntityNumber(3)
                          .Build());
  // Final check should pass
  try {
    message.Validate();
  } catch (const std::exception &e) {
    FAIL(
        (std::string("Validation threw an exception: ") + std::string(e.what()))
            .c_str());
  }
}

TEST(PerceptionDataMessageTest, ToJson) {
  PerceptionRecord record1 = PerceptionRecordBuilder()
                                 .SetPerceptionId(EntityIdBuilder()
                                                      .SetSite(4)
                                                      .SetApplication(5)
                                                      .SetEntityNumber(6)
                                                      .Build())
                                 .SetForce(Force::kFriendly)
                                 .SetAccuracy(.98)
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
                                 .SetSystem(SensorType::kSonar)
                                 .Build();

  PerceptionDataMessage message =
      PerceptionDataMessageBuilder()
          .SetOriginId(EntityIdBuilder()
                           .SetSite(1)
                           .SetApplication(2)
                           .SetEntityNumber(3)
                           .Build())
          .SetNumPerceptions(1)
          .SetPerceptionRecords({record1})
          .SetHeader(MusicMessageHeaderBuilder()
                         .SetExerciseId(200)
                         .SetTimestamp(1765819655931)
                         .Build())
          .Build();

  nlohmann::json json = message.ToJson();

  // Check fields
  LONGS_EQUAL(1, json["originID"]["siteID"].get<int>());
  LONGS_EQUAL(2, json["originID"]["appID"].get<int>());
  LONGS_EQUAL(3, json["originID"]["entityID"].get<int>());
  LONGS_EQUAL(1, json["numPerceptions"].get<int>());
  LONGS_EQUAL(1, static_cast<long>(json["perceptionRecords"].size()));
  // Check first record fields
  nlohmann::json record_json = json["perceptionRecords"][0];
  LONGS_EQUAL(4, record_json["perceptionID"]["siteID"].get<int>());
  LONGS_EQUAL(5, record_json["perceptionID"]["appID"].get<int>());
  LONGS_EQUAL(6, record_json["perceptionID"]["entityID"].get<int>());
  LONGS_EQUAL(static_cast<long>(Force::kFriendly),
              record_json["force"].get<long>());
  DOUBLES_EQUAL(.98, record_json["accuracy"].get<double>(), 0.0);
  LONGS_EQUAL(12, record_json["bearingFactor"].get<int>());
  LONGS_EQUAL(3, record_json["altitudeError"].get<int>());
  LONGS_EQUAL(7, record_json["rangeError"].get<int>());
  LONGS_EQUAL(static_cast<long>(SensorType::kSonar),
              record_json["system"].get<long>());
  // Check entity type fields
  LONGS_EQUAL(1, record_json["entityType"]["kind"].get<int>());
  LONGS_EQUAL(2, record_json["entityType"]["domain"].get<int>());
  LONGS_EQUAL(225, record_json["entityType"]["country"].get<int>());
  LONGS_EQUAL(3, record_json["entityType"]["category"].get<int>());
  LONGS_EQUAL(4, record_json["entityType"]["subcategory"].get<int>());
  LONGS_EQUAL(5, record_json["entityType"]["specific"].get<int>());
  LONGS_EQUAL(6, record_json["entityType"]["extra"].get<int>());
}

TEST(PerceptionDataMessageTest, FromJson) {
  nlohmann::json json = nlohmann::json::parse(R"(
{
    "header": {
        "exerciseID": 200,
        "timestamp": 1765819655931
    },
    "numPerceptions": 1,
    "originID": {
        "appID": 2,
        "entityID": 3,
        "siteID": 1
    },
    "perceptionRecords": [
        {
            "accuracy": 0.98,
            "altitudeError": 3,
            "bearingFactor": 12,
            "entityType": {
                "category": 3,
                "country": 225,
                "domain": 2,
                "extra": 6,
                "kind": 1,
                "specific": 5,
                "subcategory": 4
            },
            "force": 1,
            "perceptionID": {
                "appID": 5,
                "entityID": 6,
                "siteID": 4
            },
            "rangeError": 7,
            "system": 2
        }
    ]
}
)");

  PerceptionDataMessage message = PerceptionDataMessage::FromJson(json);

  // Header checks
  LONGS_EQUAL(200, message.GetHeader().GetExerciseId());
  LONGS_EQUAL(1765819655931, message.GetHeader().GetTimestamp());

  // Origin ID checks
  LONGS_EQUAL(1, message.GetOriginId().GetSite());
  LONGS_EQUAL(2, message.GetOriginId().GetApplication());
  LONGS_EQUAL(3, message.GetOriginId().GetEntityNumber());

  // Perception data message field checks
  LONGS_EQUAL(1, message.GetNumPerceptions());
  LONGS_EQUAL(1, static_cast<long>(message.GetPerceptionRecords().size()));

  // Perception record checks
  PerceptionRecord record = message.GetPerceptionRecords().at(0);
  LONGS_EQUAL(4, record.GetPerceptionId().GetSite());
  LONGS_EQUAL(5, record.GetPerceptionId().GetApplication());
  LONGS_EQUAL(6, record.GetPerceptionId().GetEntityNumber());
  LONGS_EQUAL(static_cast<long>(Force::kFriendly),
              static_cast<long>(record.GetForce()));
  DOUBLES_EQUAL(.98, record.GetAccuracy(), kDoublePrecision);
  LONGS_EQUAL(12, record.GetBearingFactor());
  LONGS_EQUAL(3, record.GetAltitudeError());
  LONGS_EQUAL(7, record.GetRangeError());
  LONGS_EQUAL(static_cast<long>(SensorType::kRadar),
              static_cast<long>(record.GetSystem()));

  // Record - Entity Type checks
  LONGS_EQUAL(1, record.GetEntityType().GetKind());
  LONGS_EQUAL(2, record.GetEntityType().GetDomain());
  LONGS_EQUAL(225, record.GetEntityType().GetCountry());
  LONGS_EQUAL(3, record.GetEntityType().GetCategory());
  LONGS_EQUAL(4, record.GetEntityType().GetSubcategory());
  LONGS_EQUAL(5, record.GetEntityType().GetSpecific());
  LONGS_EQUAL(6, record.GetEntityType().GetExtra());
}

}  // namespace music::test