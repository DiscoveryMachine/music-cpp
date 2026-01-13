cmake_minimum_required(VERSION 3.22)

project(CPPMusicDTO)
# Define directories 
set(SRC_DIR "src")
set(INCLUDE_DIR "include")
set(LIBRARY_DIR "lib")

set(BUILD_INTEGRATION_TEST false)
set(BUILD_UNIT_TEST false)

# Build sources
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cc)

# Create a library target (or change to add_executable if you need an exe)
add_library(CPPMusicDTO STATIC ${SOURCES})

# Expose include directory (both root and message subdirectory)
target_include_directories(CPPMusicDTO PUBLIC ${PROJECT_SOURCE_DIR}/${INCLUDE_DIR})

# Use open-dis-cpp if available
find_package(OpenDIS REQUIRED)
find_package(CppUTest REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(CPPMusicDTO PUBLIC OpenDIS::OpenDIS7 spdlog::spdlog nlohmann_json::nlohmann_json)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(BUILD_UNIT_TEST)
# Set test sources
file(GLOB_RECURSE TEST_SOURCES
  tests/message/*.cc
  tests/serialize/*.cc
  tests/util/*.cc
  tests/test_suite.cc
)

# Test executable
add_executable(test_suite 
  ${TEST_SOURCES}
)
target_include_directories(test_suite PRIVATE ${PROJECT_SOURCE_DIR}/${INCLUDE_DIR})
target_link_libraries(test_suite PRIVATE CPPMusicDTO CppUTest CppUTestExt )
target_compile_definitions(test_suite PRIVATE CPPUTEST_MEM_LEAK_DETECTION_DISABLED)
endif()


if(BUILD_INTEGRATION_TEST)
  add_executable(integration_test tests/integration/integration_test.cc)
  target_link_libraries(integration_test PRIVATE CPPMusicDTO)
  target_include_directories(integration_test PRIVATE ${PROJECT_SOURCE_DIR}/lib)
  
endif()
