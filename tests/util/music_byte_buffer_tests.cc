// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "util/music_byte_buffer.h"
#include <CppUTest/TestHarness.h>
#include <UtestMacros.h>
#include <string>
#include <vector>

namespace music::test {
TEST_GROUP(MusicByteBuffer){
    // DEFINE ANY SETUP HERE
};

TEST(MusicByteBuffer, Test_SetInitialData_DataMatches) {

  std::string src = "this";
  // Copy length +1 to include the null terminator (/0) in the string
  MusicByteBuffer buff = MusicByteBuffer(src.c_str(), src.length() + 1);
  std::vector<char> buff_dump = buff.GetDataAt(buff.GetLength());
  STRCMP_EQUAL(src.c_str(), buff_dump.data());

  int my_int = 11;
  MusicByteBuffer int_buffer = MusicByteBuffer(reinterpret_cast<char *>(&my_int), sizeof(int));
  int val = int_buffer.ReadValueSequential<int>();
  CHECK_EQUAL(my_int, val);
};

TEST(MusicByteBuffer, Test_AppendData_DataMatches) {

  std::string src = "this is a super duper long string which is probably longer than a byte";
  MusicByteBuffer buff = MusicByteBuffer();
  buff.AppendValue<std::string>(&src, sizeof(src));

  std::string buff_dump = buff.ReadValueSequential<std::string>();
  STRCMP_EQUAL(src.c_str(), buff_dump.data());

  MusicByteBuffer int_buffer = MusicByteBuffer();
  int my_int = 1525215;
  int_buffer.AppendValue<int>(&my_int, sizeof(my_int));

  int val = int_buffer.ReadValueSequential<int>();
  CHECK_EQUAL(1525215, val);
}

TEST(MusicByteBuffer, Test_InsertData_DataMatchesExpectedOrder) {
  std::string src = "a string";
  MusicByteBuffer buff = MusicByteBuffer();
  buff.AppendValue<std::string>(&src, sizeof(src));

  std::string buff_dump = buff.ReadValueSequential<std::string>();
  STRCMP_EQUAL(src.c_str(), buff_dump.data());

  MusicByteBuffer int_buffer = MusicByteBuffer();
  int my_int = 1525215;
  int_buffer.AppendValue<int>(&my_int, sizeof(my_int));

  int second_int = 12;
  int_buffer.AppendValue<int>(&second_int, sizeof(second_int));

  int inserted_int = 67;
  int_buffer.InsertValue(sizeof(int), &inserted_int);

  int val_one = int_buffer.ReadValueSequential<int>();
  int inserted_val = int_buffer.ReadValueSequential<int>();
  int val_three = int_buffer.ReadValueSequential<int>();
  CHECK_EQUAL(inserted_int, inserted_val);
  CHECK_EQUAL(my_int, val_one);
  CHECK_EQUAL(second_int, val_three);
}

TEST(MusicByteBuffer, Test_InsertMixedData_DataMatchesExpectedOrder) {
  MusicByteBuffer int_buffer = MusicByteBuffer();
  int my_int = 1525215;
  int_buffer.AppendValue<int>(&my_int, sizeof(int));

  int second_int = 12;
  int_buffer.AppendValue<int>(&second_int, sizeof(int));

  std::string inserted_string = "this is in the middle of an int array";
  int_buffer.InsertValue(sizeof(int), &inserted_string);

  int val_one = int_buffer.ReadValueSequential<int>();
  std::string read_string = int_buffer.ReadValueSequential<std::string>();
  int val_three = int_buffer.ReadValueSequential<int>();
  STRCMP_EQUAL(inserted_string.c_str(), read_string.c_str());
  CHECK_EQUAL(my_int, val_one);
  CHECK_EQUAL(second_int, val_three);
}
} // namespace music::test
