// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>
#include <math.h>

#include <cmath>
#include <stdexcept>

#include "message/Orientation.h"

namespace music::test {
TEST_GROUP(MUSIC_ORIENTATION){
    // DEFINE ANY SETUP HERE
};

// Tests that validation FAILS when an Orientation has un-initialized values
TEST(MUSIC_ORIENTATION, Test_SetNullParameters_ValidationFails) {
  bool failed_test = true;

  // PHI is null to begin with (should be the first thrown error)
  music::OrientationBuilder orientation = music::OrientationBuilder();
  try {
    orientation.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false;
  }

  if (failed_test) {
    FAIL("No Validation error was thrown when PHI was null.");
  }

  // Make PHI valid and non-null
  orientation.SetPhi(0);

  try {
    orientation.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false;
  }
  if (failed_test) {
    FAIL("No Validation error was thrown when PSI was null");
  }

  // Make PHI valid and non-null
  orientation.SetPsi(0);

  try {
    orientation.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false;
  }
  if (failed_test) {
    FAIL("No Validation error was thrown when THETA was null");
  }
};

// Tests that validation passes when an Orientation is provided with good data
TEST(MUSIC_ORIENTATION, Test_SetValidParameters_ValidationPasses) {
  music::OrientationBuilder orientation =
      music::OrientationBuilder().SetPhi(0).SetPsi(0).SetTheta(0);
  try {
    orientation.Build().Validate();
  } catch (std::runtime_error err) {
    FAIL("A validation error was thrown for a valid orientation definition");
  }
};

// Tests that validation FAILS when an Orientation is provided with BAD values
// for PHI
TEST(MUSIC_ORIENTATION, Test_SetInValidPhi_ValidationFails) {
  bool failed_test = true;
  music::OrientationBuilder orientation =
      music::OrientationBuilder().SetPhi(M_PI * 2.0).SetPsi(0).SetTheta(0);
  try {
    orientation.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false; // If it flags an error, we've passed
  }

  if (failed_test) {
    FAIL("No exception was thrown when providing an orientation with an "
         "invalid PHI value.");
  }
};

// Tests that validation FAILS when an Orientation is provided with BAD values
// for PSI
TEST(MUSIC_ORIENTATION, Test_SetInValidPsi_ValidationFails) {
  bool failed_test = true;
  music::OrientationBuilder orientation =
      music::OrientationBuilder().SetPhi(0).SetPsi(M_PI * 2.0).SetTheta(0);
  try {
    orientation.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false; // If it flags an error, we've passed
  }

  if (failed_test) {
    FAIL("No exception was thrown when providing an orientation with an "
         "invalid PSI value.");
  }
};

// Tests that validation FAILS when an Orientation is provided with BAD values
// for THETA
TEST(MUSIC_ORIENTATION, Test_SetInValidTheta_ValidationFails) {
  bool failed_test = true;
  music::OrientationBuilder orientation =
      music::OrientationBuilder().SetPhi(0).SetPsi(0).SetTheta(M_PI * 2.0);
  try {
    orientation.Build().Validate();
  } catch (std::runtime_error err) {
    failed_test = false; // If it flags an error, we've passed
  }

  if (failed_test) {
    FAIL("No exception was thrown when providing an orientation with an "
         "invalid THETA value.");
  }
}
} // namespace music::test