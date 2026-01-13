// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "message/construct/damage.h"
#include <CppUTest/TestHarness.h>
#include <UtestMacros.h>
#include <nlohmann/detail/conversions/to_json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>

const double DOUBLE_TOLERANCE = 0.001;

namespace music::test {
TEST_GROUP(MusicDamageTest){
    // NO-OP
};

TEST(MusicDamageTest, Test_InvalidDefaultData_ThrowsValidationError) {
  MusicDamageBuilder builder = MusicDamageBuilder();
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());
  builder.SetCatastrophicDamage(1);

  CHECK_THROWS(std::runtime_error, builder.Build().Validate());
  builder.SetMobilityDamage(1);

  // Should have all valid data now- fail if the validate finds an issue
  try {
    { builder.Build().Validate(); }
  } catch (std::runtime_error err) {
    FAIL("music::Damage threw a validation error with all valid data!");
  }
}

TEST(MusicDamageTest, Test_SetDataWithBuilder_BuiltObjectValuesMatch) {
  MusicDamageBuilder builder =
      MusicDamageBuilder().SetCatastrophicDamage(1.5).SetMobilityDamage(2.0).SetIsFirepowerDisabled(
          true);

  Damage final = builder.Build();
  DOUBLES_EQUAL(1.5, final.GetCatastrophicDamage(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(2.0, final.GetMobilityDamage(), DOUBLE_TOLERANCE);
  CHECK_EQUAL(true, final.IsFirepowerDisabled());
}

TEST(MusicDamageTest, Test_ConstructFromJson_FinalValuesMatchJson) {

  // Setup: Assumes the to_json works (as validated in other tests)
  MusicDamageBuilder builder =
      MusicDamageBuilder().SetCatastrophicDamage(1.5).SetMobilityDamage(2.0).SetIsFirepowerDisabled(
          true);
  Damage original = builder.Build();
  original.Validate(); // Make sure it's valid for the purpose of this test
  nlohmann::json jsn;
  nlohmann::to_json(jsn, original);

  // Parse and validate the damage
  Damage parsed = Damage();
  from_json(jsn, parsed);

  DOUBLES_EQUAL(parsed.GetCatastrophicDamage(), original.GetCatastrophicDamage(), DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(parsed.GetMobilityDamage(), original.GetMobilityDamage(), DOUBLE_TOLERANCE);
  CHECK_EQUAL(parsed.IsFirepowerDisabled(), original.IsFirepowerDisabled());

  // Fail if the parsed object is invalid
  try {
    { parsed.Validate(); }
  } catch (std::runtime_error err) {
    FAIL("music::Damage parsed from JSON to an invalid object!");
  }
}

TEST(MusicDamageTest, Test_ConvertToJson_JsonValuesMatchObject) {
  MusicDamageBuilder builder =
      MusicDamageBuilder().SetCatastrophicDamage(1.5).SetMobilityDamage(2.0).SetIsFirepowerDisabled(
          true);
  nlohmann::json final_json;
  to_json(final_json, builder.Build());

  DOUBLES_EQUAL(final_json[Damage::kCatastrophicDamageKey].get<float>(), 1.5, DOUBLE_TOLERANCE);
  DOUBLES_EQUAL(final_json[Damage::kMobilityDamageKey].get<float>(), 2.0, DOUBLE_TOLERANCE);
  CHECK_TRUE(final_json[Damage::kFirepowerDisabledKey].get<bool>());
}

} // namespace music::test