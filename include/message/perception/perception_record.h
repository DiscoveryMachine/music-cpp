// Copyright (c) 2025 Discovery Machine®, Inc.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_PERCEPTION_PERCEPTION_RECORD_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_PERCEPTION_PERCEPTION_RECORD_H_

#include "enum/force.h"
#include "enum/sensor_type.h"
#include "message/entity_id.h"
#include "message/entity_type.h"
namespace music {

class PerceptionRecord {
  template <class T>
  friend class PerceptionRecordBuilderTemplate;

 public:
  // Validate
  void Validate() const;

  // Getters
  EntityId GetPerceptionId() const { return perception_id_; }
  Force GetForce() const { return force_; }
  EntityType GetEntityType() const { return entity_type_; }
  SensorType GetSystem() const { return system_; }
  double GetAccuracy() const { return accuracy_; }
  int GetBearingFactor() const { return bearing_factor_; }
  int GetAltitudeError() const { return altitude_error_; }
  int GetRangeError() const { return range_error_; }

  // Setters
  void SetPerceptionId(const EntityId &perception_id) {
    perception_id_ = perception_id;
  }
  void SetForce(const Force &force) { force_ = force; }
  void SetEntityType(const EntityType &entity_type) {
    entity_type_ = entity_type;
  }
  void SetSystem(const SensorType &system) { system_ = system; }
  void SetAccuracy(double accuracy) { accuracy_ = accuracy; }
  void SetBearingFactor(int bearing_factor) {
    bearing_factor_ = bearing_factor;
  }
  void SetAltitudeError(int altitude_error) {
    altitude_error_ = altitude_error;
  }
  void SetRangeError(int range_error) { range_error_ = range_error; }

  // JSON Keys
  static inline const std::string kPerceptionIdKey = "perceptionID";
  static inline const std::string kForceKey = "force";
  static inline const std::string kEntityTypeKey = "entityType";
  static inline const std::string kSystemKey = "system";
  static inline const std::string kAccuracyKey = "accuracy";
  static inline const std::string kBearingFactorKey = "bearingFactor";
  static inline const std::string kAltitudeErrorKey = "altitudeError";
  static inline const std::string kRangeErrorKey = "rangeError";

  // JSON serialization
  friend void to_json(nlohmann::json &j, const PerceptionRecord &record);
  friend void from_json(const nlohmann::json &j, PerceptionRecord &record);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static PerceptionRecord FromJson(const nlohmann::json &j) {
    PerceptionRecord record;
    from_json(j, record);
    return record;
  }

  // String representation
  friend std::ostream &operator<<(std::ostream &os,
                                  const PerceptionRecord &record) {
    os << record.ToString();
    return os;
  }
  std::string ToString() const {
    return "PerceptionRecord: " + ToJson().dump();
  }

 protected:
  // The ID of this perception.
  EntityId perception_id_{};

  // The force affiliation of the perceived entity.
  Force force_{Force::kUnset};

  // The entity type enumeration (SIS-REF-010-2019) that this perception is
  // perceived to be.
  EntityType entity_type_{};

  // The sensor system which is returning this perception.
  SensorType system_{SensorType::kUnset};

  // The likelihood that the information provided is correct. Noted as a decimal
  // percentage. A value of 1.0 means 100% accurate, while a value of 0.0 means
  // 0% accurate.
  double accuracy_{std::numeric_limits<double>::quiet_NaN()};

  // The error factor by which the bearing may be off, in meters. This value
  // shall be assumed to be “+” or “–“ the reported bearing. Unknown error
  // factors shall be reported as max value.

  int bearing_factor_{std::numeric_limits<int>::min()};

  // The error factor by which the estimated altitude may be off, in meters.
  // This value shall be assumed to be “+” or “–“ the reported altitude .Unknown
  // error factors shall be reported as max value.

  int altitude_error_{std::numeric_limits<int>::min()};
  // The error factor by which the estimated range may be off, in meters. This
  // value shall be assumed to be “+” or “–“ the reported range .Unknown error
  // factors shall be reported as max value.
  int range_error_{std::numeric_limits<int>::min()};
};

// Template Builder Class
template <class T>
class PerceptionRecordBuilderTemplate {
 protected:
  std::shared_ptr<PerceptionRecord> perception_record_;
  void SetBuiltRecord(std::shared_ptr<PerceptionRecord> new_record) {
    perception_record_ = new_record;
  }

 public:
  T &SetPerceptionId(const EntityId &perception_id) {
    perception_record_->perception_id_ = perception_id;
    return static_cast<T &>(*this);
  }
  T &SetForce(const Force &force) {
    perception_record_->force_ = force;
    return static_cast<T &>(*this);
  }
  T &SetEntityType(const EntityType &entity_type) {
    perception_record_->entity_type_ = entity_type;
    return static_cast<T &>(*this);
  }
  T &SetSystem(const SensorType &system) {
    perception_record_->system_ = system;
    return static_cast<T &>(*this);
  }
  T &SetAccuracy(double accuracy) {
    perception_record_->accuracy_ = accuracy;
    return static_cast<T &>(*this);
  }
  T &SetBearingFactor(int bearing_factor) {
    perception_record_->bearing_factor_ = bearing_factor;
    return static_cast<T &>(*this);
  }
  T &SetAltitudeError(int altitude_error) {
    perception_record_->altitude_error_ = altitude_error;
    return static_cast<T &>(*this);
  }
  T &SetRangeError(int range_error) {
    perception_record_->range_error_ = range_error;
    return static_cast<T &>(*this);
  }
};
class PerceptionRecordBuilder
    : public PerceptionRecordBuilderTemplate<PerceptionRecordBuilder> {
 public:
  PerceptionRecordBuilder() {
    perception_record_ = std::make_shared<PerceptionRecord>();
    SetBuiltRecord(perception_record_);
  }
  PerceptionRecord Build() const { return *perception_record_; }
};

}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_PERCEPTION_PERCEPTION_RECORD_H_