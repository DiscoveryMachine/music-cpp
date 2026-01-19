// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <CppUTest/TestHarness.h>
#include <UtestMacros.h>

#include <exception>

#include "message/entity_id.h"
#include "message/event/event_message.h"
#include "message/music_message_header.h"
#include "message/music_request_message.h"

namespace music::test {
TEST_GROUP(EventMessageTests) {
  MusicMessageHeader header;
  EntityId origin_id;
  std::string event_type;
  std::string event_data;
  EventMessage test_mesg;

  void setup() {
    header = MusicMessageHeaderBuilder()
                 .SetExerciseId(3)
                 .SetTimestamp(1234567890L)
                 .Build();

    origin_id.SetSite(1);
    origin_id.SetApplication(2);
    origin_id.SetEntityNumber(3);

    event_type = "test.event";
    event_data = nlohmann::json{{"Test data key", "Test data value"}}.dump();

    test_mesg = EventMessageBuilder()
                    .SetHeader(header)
                    .SetOriginId(origin_id)
                    .SetEventType(event_type)
                    .SetEventData(event_data)
                    .Build();
  }

  void teardown() {}
};

TEST(EventMessageTests, ValidationTest) {
  EventMessage default_mesg;

  CHECK_THROWS(std::exception, default_mesg.Validate());

  test_mesg.Validate();

  test_mesg.SetOriginId(EntityId{});
  CHECK_THROWS(std::exception, default_mesg.Validate());

  setup();
  test_mesg.SetEventType("");
  CHECK_THROWS(std::exception, default_mesg.Validate());

  setup();
  test_mesg.SetEventData("");
  CHECK_THROWS(std::exception, default_mesg.Validate());

  setup();
  const auto &bad_json = "{ \"test_key\" : 1, }";
  test_mesg.SetEventData(bad_json);
  CHECK_THROWS(std::exception, default_mesg.Validate());
}

TEST(EventMessageTests, ToJsonTest) {
  // Assumes well formed parent
  auto req_mesg_json = static_cast<MusicMessage>(test_mesg).ToJson();

  req_mesg_json[MusicRequestMessage::kOriginIdKey] = origin_id.ToJson();
  req_mesg_json[EventMessage::kEventTypeJsonKey] = event_type;
  req_mesg_json[EventMessage::kEventDataJsonKey] = event_data;

  test_mesg.SetOriginId(origin_id);
  test_mesg.SetEventType(event_type);
  test_mesg.SetEventData(event_data);

  STRCMP_EQUAL(req_mesg_json.dump().c_str(), test_mesg.ToJson().dump().c_str());
}

TEST(EventMessageTests, FromJsonTest) {
  EventMessage from_json = EventMessage::FromJson(test_mesg.ToJson());

  CHECK_TRUE(test_mesg.GetOriginId() == from_json.GetOriginId());
  CHECK_EQUAL(test_mesg.GetEventType(), from_json.GetEventType());
  STRCMP_EQUAL(test_mesg.GetEventData().dump().c_str(),
               from_json.GetEventData().dump().c_str());
}

}  // namespace music::test
