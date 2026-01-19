// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>

#include <nlohmann/detail/conversions/to_json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <stdexcept>

#include "enum/construct_render_type.h"
#include "message/construct/construct_information_record.h"

const double DOUBLE_TOLERANCE = 0.001;

namespace music::test {
TEST_GROUP(MusicConstructInformationRecordTests){
    // NO-OP
};
TEST(MusicConstructInformationRecordTests,
     Test_InvalidDefaultData_ThrowsValidationError) {
  ConstructInformationRecordBuilder builder =
      ConstructInformationRecordBuilder();
  CHECK_THROWS(std::runtime_error, builder.Build().Validate());
  builder.SetConstructCategoryType(ConstructCategoryType::kAggregate);

  CHECK_THROWS(std::runtime_error, builder.Build().Validate());
  builder.SetConstructRenderType(ConstructRenderType::kRenderedPhysical);

  // Should have all valid data now- fail if the validate finds an issue
  try {
    { builder.Build().Validate(); }
  } catch (std::runtime_error err) {
    FAIL(
        "music::ConstructInformationRecord threw a validation error with all "
        "valid data!");
  }
}

TEST(MusicConstructInformationRecordTests,
     Test_SetDataWithBuilder_BuiltObjectValuesMatch) {
  ConstructInformationRecordBuilder builder =
      ConstructInformationRecordBuilder()
          .SetConstructCategoryType(ConstructCategoryType::kAggregate)
          .SetConstructRenderType(ConstructRenderType::kRenderedPhysical);

  ConstructInformationRecord final = builder.Build();
  CHECK_EQUAL(static_cast<int>(ConstructCategoryType::kAggregate),
              static_cast<int>(final.GetConstructCategory()));
  CHECK_EQUAL(static_cast<int>(ConstructRenderType::kRenderedPhysical),
              static_cast<int>(final.GetConstructRender()));
}

TEST(MusicConstructInformationRecordTests,
     Test_ConstructFromJson_FinalValuesMatchJson) {
  // Setup: Assumes the to_json works (as validated in other tests)
  ConstructInformationRecordBuilder builder =
      ConstructInformationRecordBuilder()
          .SetConstructCategoryType(ConstructCategoryType::kAggregate)
          .SetConstructRenderType(ConstructRenderType::kRenderedPhysical);

  ConstructInformationRecord original = builder.Build();
  original.Validate();  // Make sure it's valid for the purpose of this test
  nlohmann::json jsn;
  nlohmann::to_json(jsn, original);

  // Parse and validate the damage
  ConstructInformationRecord parsed = ConstructInformationRecord();
  from_json(jsn, parsed);

  CHECK_EQUAL(static_cast<int>(original.GetConstructCategory()),
              static_cast<int>(parsed.GetConstructCategory()));
  CHECK_EQUAL(static_cast<int>(original.GetConstructRender()),
              static_cast<int>(parsed.GetConstructRender()));

  // Fail if the parsed object is invalid
  try {
    { parsed.Validate(); }
  } catch (std::runtime_error err) {
    FAIL(
        "music::ConstructInformationRecord parsed from JSON to an invalid "
        "object!");
  }
}

TEST(MusicConstructInformationRecordTests,
     Test_ConvertToJson_JsonValuesMatchObject) {
  ConstructInformationRecordBuilder builder =
      ConstructInformationRecordBuilder()
          .SetConstructCategoryType(ConstructCategoryType::kAggregate)
          .SetConstructRenderType(ConstructRenderType::kRenderedPhysical);
  nlohmann::json final_json;
  to_json(final_json, builder.Build());

  CHECK_EQUAL(static_cast<int>(
                  final_json[ConstructInformationRecord::kConstructCategoryKey]
                      .get<ConstructCategoryType>()),
              static_cast<int>(ConstructCategoryType::kAggregate));
  CHECK_EQUAL(
      static_cast<int>(
          final_json[ConstructInformationRecord::kConstructRenderTypeKey]
              .get<ConstructRenderType>()),
      static_cast<int>(ConstructRenderType::kRenderedPhysical));
}

}  // namespace music::test