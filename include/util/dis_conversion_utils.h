// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_DIS_CONVERSION_UTILS_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_DIS_CONVERSION_UTILS_H_

#include <dis7/ActionRequestPdu.h>
#include <dis7/ActionResponsePdu.h>
#include <dis7/EntityID.h>
#include <dis7/EntityType.h>
#include <dis7/EulerAngles.h>
#include <dis7/EventIdentifier.h>
#include <dis7/Pdu.h>
#include <dis7/SimulationAddress.h>
#include <dis7/VariableDatum.h>
#include <dis7/Vector3Double.h>
#include <dis7/Vector3Float.h>
#include <dis7/utils/DataStream.h>
#include <dis7/utils/Endian.h>

#include <cstdint>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "enum/music_message_type.h"
#include "enum/request_status.h"
#include "message/Location.h"
#include "message/Orientation.h"
#include "message/Vector.h"
#include "message/entity_id.h"
#include "message/entity_type.h"
#include "message/event_id.h"
#include "message/music_message.h"
#include "message/music_request_message.h"
#include "message/music_response_message.h"
#include "util/music_byte_buffer.h"

namespace music {

// ---- Serialization helpers ----

uint32_t EpochMillisToDisTimestamp(uint64_t epoch_millis,
                                   bool absolute_timestamp);

MusicByteBuffer DataStreamToMusicByteBuffer(const DIS::DataStream &stream);

void SetPDUBaseData(DIS::Pdu &pdu, const music::MusicMessage &message);

void PadToEightBytes(std::vector<char> &bytes);

std::vector<char> Get8BytePaddedString(std::string source_string);

DIS::DataStream MarshalPdu(DIS::PduSuperclass &pdu);

MusicByteBuffer MarshalPduToBuffer(DIS::PduSuperclass &pdu);

template <typename T>
std::vector<char> MarshalToBytes(const T &value) {
  DIS::DataStream stream("", 0, DIS::Endian::BIG);
  value.marshal(stream);

  std::vector<char> bytes;
  bytes.resize(stream.size());
  for (size_t i = 0; i < bytes.size(); ++i) {
    bytes[i] = stream[i];
  }
  return bytes;
}

DIS::SimulationAddress ToDisSimAddress(
    music::SimulationAddress music_sim_address);

const DIS::EventIdentifier ToDisEventId(const music::MusicEventId id);

DIS::EntityID ToDisEntityId(const music::EntityId &music_entity_id);

DIS::Vector3Float ToDisVectorFloatFromLocation(const music::Location &location);
DIS::Vector3Float ToDisVectorFloatFromVector(const music::Vector &vector);

DIS::Vector3Double ToDisVectorDoubleFromVector(const music::Vector &vector);
DIS::Vector3Double ToDisVectorDoubleFromLocation(
    const music::Location &location);

DIS::EulerAngles ToDisEulerAngles(const music::Orientation &orientation);

DIS::EntityType ToDisEntityType(const music::EntityType &music_entity_type);

void AddEntityIdToVariableDatum(DIS::VariableDatum &datum,
                                const music::EntityId &entity_id,
                                uint32_t variable_datum_id);

void AddStringToVariableDatum(DIS::VariableDatum &datum,
                              const std::string &value,
                              uint32_t variable_datum_id);

void AddJsonToVariableDatum(DIS::VariableDatum &datum,
                            const nlohmann::json &json,
                            uint32_t variable_datum_id);

void AddOrientationToVariableDatum(DIS::VariableDatum &datum,
                                   const music::Orientation &orientation,
                                   uint32_t variable_datum_id);

void AddLocationToVariableDatum(DIS::VariableDatum &datum,
                                const music::Location &location,
                                uint32_t variable_datum_id);

// ---- Deserialization helpers ----

MusicMessageHeader HeaderFromPdu(const DIS::PduSuperclass &pdu);

std::string TrimNullTerminatorPadding(std::vector<char> bytes);

music::SimulationAddress FromDisSimAddress(
    const DIS::SimulationAddress &dis_address);

music::EntityId FromDisEntityId(const DIS::EntityID &dis_entity_id);

music::EntityType FromDisEntityType(const DIS::EntityType &dis_entity_type);

music::MusicEventId FromDisEventId(const DIS::EventIdentifier &dis_event_id);

music::Location FromDisVector3Double(const DIS::Vector3Double &v);

music::Location FromDisVector3FloatToLocation(const DIS::Vector3Float &v);

music::Vector FromDisVector3Float(const DIS::Vector3Float &v);

music::Orientation FromDisEulerAngles(const DIS::EulerAngles &angles);

music::Location ReadDisVector3FloatLocation(MusicByteBuffer &buffer);

music::Location ReadDisVector3DoubleLocation(MusicByteBuffer &buffer);

music::EntityId EntityIdFromDatumPayload(const DIS::VariableDatum &datum);

bool IsZeroEntity(const music::EntityId &id);

std::string DatumPayloadString(const DIS::VariableDatum &datum);

std::vector<char> DatumBytes(const DIS::VariableDatum &datum);

nlohmann::json ParseJsonOrEmptyObject(const std::string &value,
                                      const std::string &context);

template <typename TBuilder>
std::shared_ptr<MusicRequestMessage> BuildRequestMessageFromBuilder(
    const DIS::ActionRequestPdu &pdu, const MusicMessageType type,
    TBuilder builder) {
  builder.SetHeader(HeaderFromPdu(pdu))
      .SetType(type)
      .SetOriginId(FromDisEntityId(pdu.getOriginatingEntityID()))
      .SetReceiverId(FromDisEntityId(pdu.getReceivingEntityID()))
      .SetRequestId(static_cast<long>(pdu.getRequestID()));

  using BuiltType =
      std::remove_cv_t<std::remove_reference_t<decltype(builder.Build())>>;
  static_assert(std::is_base_of_v<MusicRequestMessage, BuiltType>,
                "Builder must build a MusicRequestMessage subtype");
  return std::make_shared<BuiltType>(builder.Build());
}

template <typename TBuilder>
std::shared_ptr<MusicResponseMessage> BuildResponseMessageFromBuilder(
    const DIS::ActionResponsePdu &pdu, const MusicMessageType type,
    TBuilder builder) {
  builder.SetHeader(HeaderFromPdu(pdu))
      .SetType(type)
      .SetOriginId(FromDisEntityId(pdu.getOriginatingEntityID()))
      .SetReceiverId(FromDisEntityId(pdu.getReceivingEntityID()))
      .SetRequestId(static_cast<long>(pdu.getRequestID()))
      .SetRequestStatus(static_cast<RequestStatus>(pdu.getRequestStatus()));

  using BuiltType =
      std::remove_cv_t<std::remove_reference_t<decltype(builder.Build())>>;
  static_assert(std::is_base_of_v<MusicResponseMessage, BuiltType>,
                "Builder must build a MusicResponseMessage subtype");
  return std::make_shared<BuiltType>(builder.Build());
}

template <typename TBuilder>
std::shared_ptr<MusicRequestMessage> DeserializeSimpleActionRequest(
    const DIS::ActionRequestPdu &pdu, const MusicMessageType type) {
  return BuildRequestMessageFromBuilder(pdu, type, TBuilder{});
}

template <typename TBuilder>
std::shared_ptr<MusicResponseMessage> DeserializeSimpleActionResponse(
    const DIS::ActionResponsePdu &pdu, const MusicMessageType type) {
  return BuildResponseMessageFromBuilder(pdu, type, TBuilder{});
}

DIS::EntityID AllOnesDisEntityId();

music::MusicMessageHeader HeaderFromPDU(DIS::PduSuperclass basePDU);

music::EntityId ReadEntityID(MusicByteBuffer &buff);

music::Orientation ReadOrientation(MusicByteBuffer &buff);

music::Vector ReadVector3FloatVector(MusicByteBuffer &buff);

music::EntityType ReadEntityType(MusicByteBuffer &buff);

music::MusicMessageType GetTypeFromResponsePdu(
    const DIS::ActionResponsePdu &pdu);

}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_UTIL_DIS_CONVERSION_UTILS_H_
