// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/CommandLineTestRunner.h>
#include <spdlog/spdlog.h>

#include "config/config.h"

int main(int ac, char **av) {
  spdlog::info("-- Running MUSIC library unit tests --");
  // Validate tests depend on exceptions being thrown on error.
  music::throw_exceptions_on_error = true;
  return CommandLineTestRunner::RunAllTests(ac, av);
}