// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include "message/entity_type.h"

namespace music::test {
TEST_GROUP(EntityTypeTest){void setup(){
    // No setup needed
}

                           void teardown(){
                               // No teardown needed
                           }};

TEST(EntityTypeTest, DefaultConstructor) {
  // Test each of the default values (expect all to be -1)
  EntityType entity_type;
  LONGS_EQUAL(std::numeric_limits<int>::min(), entity_type.GetDomain());
  LONGS_EQUAL(std::numeric_limits<int>::min(), entity_type.GetCountry());
  LONGS_EQUAL(std::numeric_limits<int>::min(), entity_type.GetKind());
  LONGS_EQUAL(std::numeric_limits<int>::min(), entity_type.GetCategory());
  LONGS_EQUAL(std::numeric_limits<int>::min(), entity_type.GetSubcategory());
  LONGS_EQUAL(std::numeric_limits<int>::min(), entity_type.GetSpecific());
  LONGS_EQUAL(std::numeric_limits<int>::min(), entity_type.GetExtra());
}

TEST(EntityTypeTest, SettersAndGetters) {
  EntityType entity_type;
  entity_type.SetKind(1);
  entity_type.SetDomain(2);
  entity_type.SetCountry(3);
  entity_type.SetCategory(4);
  entity_type.SetSubcategory(5);
  entity_type.SetSpecific(6);
  entity_type.SetExtra(7);

  LONGS_EQUAL(1, entity_type.GetKind());
  LONGS_EQUAL(2, entity_type.GetDomain());
  LONGS_EQUAL(3, entity_type.GetCountry());
  LONGS_EQUAL(4, entity_type.GetCategory());
  LONGS_EQUAL(5, entity_type.GetSubcategory());
  LONGS_EQUAL(6, entity_type.GetSpecific());
  LONGS_EQUAL(7, entity_type.GetExtra());
}

TEST(EntityTypeTest, Validate) {
  EntityType entity_type;
  // Set invalid values and validate that exceptions are thrown
  entity_type.SetKind(256);
  CHECK_THROWS(std::exception, entity_type.Validate());
  entity_type.SetKind(1);  // Reset to valid
  entity_type.SetDomain(300);
  CHECK_THROWS(std::exception, entity_type.Validate());
  entity_type.SetDomain(2);  // Reset to valid
  entity_type.SetCountry(67000);
  CHECK_THROWS(std::exception, entity_type.Validate());
  entity_type.SetCountry(3);  // Reset to valid
  entity_type.SetCategory(-1024);
  CHECK_THROWS(std::exception, entity_type.Validate());
  entity_type.SetCategory(4);  // Reset to valid
  entity_type.SetSubcategory(-512);
  CHECK_THROWS(std::exception, entity_type.Validate());
  entity_type.SetSubcategory(5);  // Reset to valid
  entity_type.SetSpecific(1024);
  CHECK_THROWS(std::exception, entity_type.Validate());
  entity_type.SetSpecific(6);  // Reset to valid
  entity_type.SetExtra(-1024);
  CHECK_THROWS(std::exception, entity_type.Validate());
  entity_type.SetExtra(7);  // Reset to valid

  // Final validation should pass
  entity_type.Validate();
}

TEST(EntityTypeTest, Builder) {
  EntityTypeBuilder builder;
  EntityType entity_type = builder.SetKind(10)
                               .SetDomain(20)
                               .SetCountry(30)
                               .SetCategory(40)
                               .SetSubcategory(50)
                               .SetSpecific(60)
                               .SetExtra(70)
                               .Build();

  LONGS_EQUAL(10, entity_type.GetKind());
  LONGS_EQUAL(20, entity_type.GetDomain());
  LONGS_EQUAL(30, entity_type.GetCountry());
  LONGS_EQUAL(40, entity_type.GetCategory());
  LONGS_EQUAL(50, entity_type.GetSubcategory());
  LONGS_EQUAL(60, entity_type.GetSpecific());
  LONGS_EQUAL(70, entity_type.GetExtra());
}

TEST(EntityTypeTest, ToJson) {
  EntityType entity_type;
  entity_type.SetKind(1);
  entity_type.SetDomain(2);
  entity_type.SetCountry(3);
  entity_type.SetCategory(4);
  entity_type.SetSubcategory(5);
  entity_type.SetSpecific(6);
  entity_type.SetExtra(7);

  nlohmann::json j;
  to_json(j, entity_type);

  LONGS_EQUAL(1, j[EntityType::kKindKey].get<int>());
  LONGS_EQUAL(2, j[EntityType::kDomainKey].get<int>());
  LONGS_EQUAL(3, j[EntityType::kCountryKey].get<int>());
  LONGS_EQUAL(4, j[EntityType::kCategoryKey].get<int>());
  LONGS_EQUAL(5, j[EntityType::kSubcategoryKey].get<int>());
  LONGS_EQUAL(6, j[EntityType::kSpecificKey].get<int>());
  LONGS_EQUAL(7, j[EntityType::kExtraKey].get<int>());
}

TEST(EntityTypeTest, FromJson) {
  nlohmann::json j = {
      {EntityType::kKindKey, 10},        {EntityType::kDomainKey, 20},
      {EntityType::kCountryKey, 30},     {EntityType::kCategoryKey, 40},
      {EntityType::kSubcategoryKey, 50}, {EntityType::kSpecificKey, 60},
      {EntityType::kExtraKey, 70}};

  EntityType entity_type;
  from_json(j, entity_type);

  LONGS_EQUAL(10, entity_type.GetKind());
  LONGS_EQUAL(20, entity_type.GetDomain());
  LONGS_EQUAL(30, entity_type.GetCountry());
  LONGS_EQUAL(40, entity_type.GetCategory());
  LONGS_EQUAL(50, entity_type.GetSubcategory());
  LONGS_EQUAL(60, entity_type.GetSpecific());
  LONGS_EQUAL(70, entity_type.GetExtra());
}

TEST(EntityTypeTest, ToString) {
  EntityType entity_type;
  entity_type.SetKind(1);
  entity_type.SetDomain(2);
  entity_type.SetCountry(3);
  entity_type.SetCategory(4);
  entity_type.SetSubcategory(5);
  entity_type.SetSpecific(6);
  entity_type.SetExtra(7);

  std::ostringstream os;
  os << entity_type;
  std::string output = os.str();

  std::string expected_json = entity_type.ToJson().dump();
  std::string expected_output = "EntityType: " + expected_json;

  CHECK_EQUAL(expected_output, output);
}
}  // namespace music::test