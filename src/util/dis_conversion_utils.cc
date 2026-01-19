// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <chrono>
#include <exception>

#include "enum/music_message_type.h"
#include "util/dis_conversion_utils.h"
#include "util/validate.h"

namespace music {

constexpr uint64_t kMillisPerHour = 3600000ULL;
// DIS timestamps use (2^31 - 1) time units per hour. The timestamp field is a
// 32-bit unsigned integer: bits 31..1 hold the time units, bit 0 is the mode
// (1=absolute, 0=relative).
constexpr uint32_t kDisTimeUnitsPerHour = 0x7FFFFFFFU;

uint32_t EpochMillisToDisTimestamp(uint64_t epoch_millis,
                                   const bool absolute_timestamp) {
  const uint64_t millis_since_hour = epoch_millis % kMillisPerHour;
  const double ratio = static_cast<double>(millis_since_hour) /
                       static_cast<double>(kMillisPerHour);
  uint32_t units =
      static_cast<uint32_t>(ratio * static_cast<double>(kDisTimeUnitsPerHour));
  if (units > kDisTimeUnitsPerHour) {
    units = kDisTimeUnitsPerHour;
  }
  return static_cast<uint32_t>((units << 1) | (absolute_timestamp ? 1U : 0U));
}

MusicByteBuffer DataStreamToMusicByteBuffer(const DIS::DataStream &stream) {
  return MusicByteBuffer(&stream[0], stream.size(), Endian::kBig);
}

void SetPDUBaseData(DIS::Pdu &pdu, const music::MusicMessage &message) {
  pdu.setPduType(
      MusicMessageTypeHelper::GetInfoForType(message.GetType()).pdu_type);

  pdu.setProtocolFamily(5);  // Simulation Management
  pdu.setProtocolVersion(7);

  pdu.setExerciseID(message.GetHeader().GetExerciseId());
  // MUSIC stores epoch millis (uint64). DIS stores an unsigned 32-bit timestamp
  // with 2^31-1 units/hour and an LSB mode bit.
  pdu.setTimestamp(EpochMillisToDisTimestamp(
      static_cast<uint64_t>(message.GetHeader().GetTimestamp()), true));
}

void PadToEightBytes(std::vector<char> &bytes) {
  const size_t remainder = bytes.size() % 8;
  if (remainder != 0) {
    bytes.resize(bytes.size() + (8 - remainder), 0);
  }
}

std::vector<char> Get8BytePaddedString(std::string source_string) {
  std::vector<char> string_bytes =
      std::vector<char>(source_string.begin(), source_string.end());
  PadToEightBytes(string_bytes);
  return string_bytes;
}

DIS::DataStream MarshalPdu(DIS::PduSuperclass &pdu) {
  // OpenDIS does not always populate the PDU header length automatically.
  // Set it explicitly so the marshalled bytes are spec-compliant.
  pdu.setLength(static_cast<unsigned short>(pdu.getMarshalledSize()));

  // !! This needs to be initialized as an empty buffer with a length of 0 for
  // the interal read/write pointers to be set correctly.
  DIS::DataStream stream("", 0, DIS::Endian::BIG);

  pdu.marshal(stream);
  return stream;
}

MusicByteBuffer MarshalPduToBuffer(DIS::PduSuperclass &pdu) {
  return DataStreamToMusicByteBuffer(MarshalPdu(pdu));
}

DIS::SimulationAddress ToDisSimAddress(
    const music::SimulationAddress music_sim_address) {
  DIS::SimulationAddress addr = DIS::SimulationAddress();
  addr.setApplication(music_sim_address.GetApplication());
  addr.setSite(music_sim_address.GetSite());
  return addr;
}

const DIS::EventIdentifier ToDisEventId(const music::MusicEventId id) {
  DIS::EventIdentifier new_id = DIS::EventIdentifier();
  new_id.setEventNumber(id.GetEventId());
  new_id.setSimulationAddress(ToDisSimAddress(id.GetSimulationAddress()));

  return new_id;
}

DIS::EntityID ToDisEntityId(const music::EntityId &music_entity_id) {
  DIS::EntityID dis_entity_id = DIS::EntityID();
  DIS::SimulationAddress sim_addr = DIS::SimulationAddress();
  sim_addr.setSite(music_entity_id.GetSite());
  sim_addr.setApplication(music_entity_id.GetApplication());
  dis_entity_id.setSimulationAddress(sim_addr);
  dis_entity_id.setEntityNumber(music_entity_id.GetEntityNumber());
  return dis_entity_id;
}

DIS::EntityID AllOnesDisEntityId() {
  DIS::EntityID all_ones_entity = DIS::EntityID();
  DIS::SimulationAddress all_ones_sim_address = DIS::SimulationAddress();
  all_ones_sim_address.setApplication(0xFFFF);
  all_ones_sim_address.setSite(0xFFFF);
  all_ones_entity.setEntityNumber(0xFFFF);
  all_ones_entity.setSimulationAddress(all_ones_sim_address);
  return all_ones_entity;
}

// Location uses double precision, but the collision pdu's Location uses a
// vector with float precision
DIS::Vector3Float ToDisVectorFloatFromLocation(
    const music::Location &location) {
  DIS::Vector3Float dis_vector = DIS::Vector3Float();
  dis_vector.setX(static_cast<float>(location.GetX()));
  dis_vector.setY(static_cast<float>(location.GetY()));
  dis_vector.setZ(static_cast<float>(location.GetZ()));
  return dis_vector;
}

DIS::Vector3Float ToDisVectorFloatFromVector(const music::Vector &vector) {
  DIS::Vector3Float dis_vector = DIS::Vector3Float();
  dis_vector.setX(vector.GetX());
  dis_vector.setY(vector.GetY());
  dis_vector.setZ(vector.GetZ());
  return dis_vector;
}
// Likely unnecessary, but included for in case any other PDU like collision pdu
// oddly requires a different vector with double precision instead of float
DIS::Vector3Double ToDisVectorDoubleFromVector(const music::Vector &vector) {
  DIS::Vector3Double dis_vector = DIS::Vector3Double();
  dis_vector.setX(static_cast<double>(vector.GetX()));
  dis_vector.setY(static_cast<double>(vector.GetY()));
  dis_vector.setZ(static_cast<double>(vector.GetZ()));
  return dis_vector;
}

DIS::Vector3Double ToDisVectorDoubleFromLocation(
    const music::Location &location) {
  DIS::Vector3Double dis_vector = DIS::Vector3Double();
  dis_vector.setX(location.GetX());
  dis_vector.setY(location.GetY());
  dis_vector.setZ(location.GetZ());
  return dis_vector;
}

// Orientation is represented in DIS7 using Euler Angles (Psi, Theta, Phi)
DIS::EulerAngles ToDisEulerAngles(const music::Orientation &orientation) {
  DIS::EulerAngles dis_euler_angles = DIS::EulerAngles();
  dis_euler_angles.setPsi(orientation.GetPsi());
  dis_euler_angles.setTheta(orientation.GetTheta());
  dis_euler_angles.setPhi(orientation.GetPhi());
  return dis_euler_angles;
}

DIS::EntityType ToDisEntityType(const music::EntityType &music_entity_type) {
  DIS::EntityType dis_entity_type = DIS::EntityType();
  dis_entity_type.setEntityKind(music_entity_type.GetKind());
  dis_entity_type.setDomain(music_entity_type.GetDomain());
  dis_entity_type.setCountry(music_entity_type.GetCountry());
  dis_entity_type.setCategory(music_entity_type.GetCategory());
  dis_entity_type.setSubcategory(music_entity_type.GetSubcategory());
  dis_entity_type.setSpecific(music_entity_type.GetSpecific());
  dis_entity_type.setExtra(music_entity_type.GetExtra());
  return dis_entity_type;
}

void AddEntityIdToVariableDatum(DIS::VariableDatum &datum,
                                const music::EntityId &entity_id,
                                const uint32_t variable_datum_id) {
  datum.setVariableDatumID(variable_datum_id);

  DIS::EntityID new_controller_entity_id = ToDisEntityId(entity_id);
  std::vector<char> entity_bytes = MarshalToBytes(new_controller_entity_id);
  PadToEightBytes(entity_bytes);
  datum.setVariableDatumLength(
      static_cast<unsigned int>(entity_bytes.size() * 8));
  datum.setVariableDatums(entity_bytes.data(), entity_bytes.size());
}

void AddStringToVariableDatum(DIS::VariableDatum &datum,
                              const std::string &value,
                              const uint32_t variable_datum_id) {
  datum.setVariableDatumID(variable_datum_id);

  std::vector<char> string_bytes = Get8BytePaddedString(value);
  datum.setVariableDatumLength(
      static_cast<unsigned int>(string_bytes.size() * 8));
  datum.setVariableDatums(string_bytes.data(), string_bytes.size());
}

void AddJsonToVariableDatum(DIS::VariableDatum &datum,
                            const nlohmann::json &json,
                            const uint32_t variable_datum_id) {
  datum.setVariableDatumID(variable_datum_id);

  std::string json_string = json.dump();
  AddStringToVariableDatum(datum, json_string, variable_datum_id);
}

void AddOrientationToVariableDatum(DIS::VariableDatum &datum,
                                   const music::Orientation &orientation,
                                   const uint32_t variable_datum_id) {
  datum.setVariableDatumID(variable_datum_id);

  DIS::EulerAngles dis_euler_angles = ToDisEulerAngles(orientation);
  std::vector<char> orientation_bytes = MarshalToBytes(dis_euler_angles);
  PadToEightBytes(orientation_bytes);
  datum.setVariableDatumLength(
      static_cast<unsigned int>(orientation_bytes.size() * 8));
  datum.setVariableDatums(orientation_bytes.data(), orientation_bytes.size());
}

void AddLocationToVariableDatum(DIS::VariableDatum &datum,
                                const music::Location &location,
                                const uint32_t variable_datum_id) {
  datum.setVariableDatumID(variable_datum_id);

  DIS::Vector3Float dis_location = ToDisVectorFloatFromLocation(location);
  std::vector<char> location_bytes = MarshalToBytes(dis_location);
  PadToEightBytes(location_bytes);
  datum.setVariableDatumLength(
      static_cast<unsigned int>(location_bytes.size() * 8));
  datum.setVariableDatums(location_bytes.data(), location_bytes.size());
}

// Deserialization

uint64_t NowEpochMillis() {
  return static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count());
}

// Interpret the 31 MSBs (after shifting right 1) as units in [0, 2^31-1], then
// scale to millis since the top of the hour. The LSB (absolute vs relative) is
// ignored.
uint64_t DisTimestampToMillisSinceHour(const uint32_t dis_time) {
  const uint32_t units = (dis_time >> 1) & kDisTimeUnitsPerHour;
  const double ratio =
      static_cast<double>(units) / static_cast<double>(kDisTimeUnitsPerHour);
  return static_cast<uint64_t>(ratio * static_cast<double>(kMillisPerHour));
}

// Does its best to convert DIS time to epoch millis.
// We don't try to infer which hour the sender meant; we anchor to the local
// machine's current hour.
uint64_t DisTimestampToEpochMillis(const uint32_t dis_time) {
  const uint64_t epoch_millis = NowEpochMillis();
  const uint64_t hour_base = epoch_millis - (epoch_millis % kMillisPerHour);
  return hour_base + DisTimestampToMillisSinceHour(dis_time);
}

MusicMessageHeader HeaderFromPdu(const DIS::PduSuperclass &pdu) {
  return MusicMessageHeaderBuilder()
      .SetExerciseId(static_cast<int>(pdu.getExerciseID()))
      .SetTimestamp(static_cast<long>(
          DisTimestampToEpochMillis(static_cast<uint32_t>(pdu.getTimestamp()))))
      .Build();
}

SimulationAddress FromDisSimAddress(const DIS::SimulationAddress &dis_address) {
  return SimulationAddressBuilder()
      .SetSite(static_cast<int>(dis_address.getSite()))
      .SetApplication(static_cast<int>(dis_address.getApplication()))
      .Build();
}

EntityId FromDisEntityId(const DIS::EntityID &dis_entity_id) {
  return EntityIdBuilder()
      .SetSite(static_cast<int>(dis_entity_id.getSimulationAddress().getSite()))
      .SetApplication(static_cast<int>(
          dis_entity_id.getSimulationAddress().getApplication()))
      .SetEntityNumber(static_cast<int>(dis_entity_id.getEntityNumber()))
      .Build();
}

EntityType FromDisEntityType(const DIS::EntityType &dis_entity_type) {
  return EntityTypeBuilder()
      .SetKind(static_cast<int>(dis_entity_type.getEntityKind()))
      .SetDomain(static_cast<int>(dis_entity_type.getDomain()))
      .SetCountry(static_cast<int>(dis_entity_type.getCountry()))
      .SetCategory(static_cast<int>(dis_entity_type.getCategory()))
      .SetSubcategory(static_cast<int>(dis_entity_type.getSubcategory()))
      .SetSpecific(static_cast<int>(dis_entity_type.getSpecific()))
      .SetExtra(static_cast<int>(dis_entity_type.getExtra()))
      .Build();
}

MusicEventId FromDisEventId(const DIS::EventIdentifier &dis_event_id) {
  return MusicEventIdBuilder()
      .SetSimulationAddress(
          FromDisSimAddress(dis_event_id.getSimulationAddress()))
      .SetEventId(static_cast<int>(dis_event_id.getEventNumber()))
      .Build();
}

Location FromDisVector3Double(const DIS::Vector3Double &v) {
  return LocationBuilder().SetX(v.getX()).SetY(v.getY()).SetZ(v.getZ()).Build();
}

Location FromDisVector3FloatToLocation(const DIS::Vector3Float &v) {
  return LocationBuilder()
      .SetX(static_cast<double>(v.getX()))
      .SetY(static_cast<double>(v.getY()))
      .SetZ(static_cast<double>(v.getZ()))
      .Build();
}

Vector FromDisVector3Float(const DIS::Vector3Float &v) {
  return VectorBuilder().SetX(v.getX()).SetY(v.getY()).SetZ(v.getZ()).Build();
}

Orientation FromDisEulerAngles(const DIS::EulerAngles &angles) {
  return OrientationBuilder()
      .SetPsi(angles.getPsi())
      .SetTheta(angles.getTheta())
      .SetPhi(angles.getPhi())
      .Build();
}

std::vector<char> DatumBytes(const DIS::VariableDatum &datum) {
  const int header_size = 8;  // variableDatumID (4) + variableDatumLength (4)
  const int marshalled_size = datum.getMarshalledSize();
  const int payload_size =
      marshalled_size > header_size ? marshalled_size - header_size : 0;
  const char *payload = datum.getVariableDatums();
  return std::vector<char>(payload, payload + payload_size);
}

std::string TrimNullTerminatorPadding(std::vector<char> bytes) {
  while (!bytes.empty() && bytes.back() == '\0') {
    bytes.pop_back();
  }
  return std::string(bytes.begin(), bytes.end());
}

Location ReadDisVector3FloatLocation(MusicByteBuffer &buffer) {
  DIS::Vector3Float vec = DIS::Vector3Float();
  std::vector<char> data = buffer.GetNextBytes(vec.getMarshalledSize());
  DIS::DataStream stream =
      DIS::DataStream(data.data(), data.size(), DIS::Endian::BIG);
  vec.unmarshal(stream);
  return FromDisVector3FloatToLocation(vec);
}

Location ReadDisVector3DoubleLocation(MusicByteBuffer &buffer) {
  DIS::Vector3Double vec = DIS::Vector3Double();
  std::vector<char> data = buffer.GetNextBytes(vec.getMarshalledSize());
  DIS::DataStream stream =
      DIS::DataStream(data.data(), data.size(), DIS::Endian::BIG);
  vec.unmarshal(stream);
  return FromDisVector3Double(vec);
}

EntityId EntityIdFromDatumPayload(const DIS::VariableDatum &datum) {
  std::vector<char> bytes = DatumBytes(datum);
  DIS::DataStream stream(bytes.data(), bytes.size(), DIS::Endian::BIG);
  DIS::EntityID dis_id;
  dis_id.unmarshal(stream);
  return FromDisEntityId(dis_id);
}

bool IsZeroEntity(const EntityId &id) {
  return id.GetSite() == 0 && id.GetApplication() == 0 &&
         id.GetEntityNumber() == 0;
}

std::string DatumPayloadString(const DIS::VariableDatum &datum) {
  return TrimNullTerminatorPadding(DatumBytes(datum));
}

nlohmann::json ParseJsonOrEmptyObject(const std::string &value,
                                      const std::string &context) {
  try {
    if (value.empty()) {
      return nlohmann::json::object();
    }
    return nlohmann::json::parse(value);
  } catch (const std::exception &e) {
    LogError(context + " - invalid JSON payload: " + std::string(e.what()));
    return nlohmann::json::object();
  }
}

MusicMessageHeader HeaderFromPDU(DIS::PduSuperclass basePDU) {
  MusicMessageHeaderBuilder builder = MusicMessageHeaderBuilder();
  builder.SetExerciseId(basePDU.getExerciseID())
      .SetTimestamp(static_cast<long>(DisTimestampToEpochMillis(
          static_cast<uint32_t>(basePDU.getTimestamp()))));
  return builder.Build();
}

EntityId ReadEntityID(MusicByteBuffer &buff) {
  DIS::EntityID id = DIS::EntityID();
  std::vector<char> data = buff.GetNextBytes(id.getMarshalledSize());
  DIS::DataStream stream =
      DIS::DataStream(data.data(), data.size(), DIS::Endian::BIG);
  id.unmarshal(stream);

  return EntityIdBuilder()
      .SetEntityNumber(id.getEntityNumber())
      .SetApplication(id.getSimulationAddress().getApplication())
      .SetSite(id.getSimulationAddress().getSite())
      .Build();
}

Orientation ReadOrientation(MusicByteBuffer &buff) {
  DIS::EulerAngles orientation = DIS::EulerAngles();
  std::vector<char> data = buff.GetNextBytes(orientation.getMarshalledSize());
  DIS::DataStream stream =
      DIS::DataStream(data.data(), data.size(), DIS::Endian::BIG);
  orientation.unmarshal(stream);

  return FromDisEulerAngles(orientation);
}

Vector ReadVector3FloatVector(MusicByteBuffer &buff) {
  DIS::Vector3Float vec = DIS::Vector3Float();
  std::vector<char> data = buff.GetNextBytes(vec.getMarshalledSize());
  DIS::DataStream stream =
      DIS::DataStream(data.data(), data.size(), DIS::Endian::BIG);
  vec.unmarshal(stream);

  return FromDisVector3Float(vec);
}

EntityType ReadEntityType(MusicByteBuffer &buff) {
  DIS::EntityType type = DIS::EntityType();
  std::vector<char> data = buff.GetNextBytes(type.getMarshalledSize());
  DIS::DataStream stream =
      DIS::DataStream(data.data(), data.size(), DIS::Endian::BIG);
  type.unmarshal(stream);

  return EntityTypeBuilder()
      .SetCategory(type.getCategory())
      .SetSubcategory(type.getSubcategory())
      .SetCountry(type.getCountry())
      .SetDomain(type.getDomain())
      .SetExtra(type.getExtra())
      .SetKind(type.getEntityKind())
      .SetSpecific(type.getSpecific())
      .Build();
}

MusicMessageType GetTypeFromResponsePdu(const DIS::ActionResponsePdu &pdu) {
  for (const auto &datum : pdu.getFixedDatums()) {
    if (datum.getFixedDatumID() == MusicResponseMessage::kActionTypeId) {
      return MusicMessageTypeHelper::FromValues(pdu.getPduType(),
                                                datum.getFixedDatumValue());
    }
  }
  return MusicMessageType::kUnknownResponse;
}

}  // namespace music