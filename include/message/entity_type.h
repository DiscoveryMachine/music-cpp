// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ENTITY_TYPE_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ENTITY_TYPE_H_

#include <nlohmann/json.hpp>
#include <string>

namespace music {
// The type of entity in a DIS exercise shall be specified by an Entity Type
// record. This record shall specify the kind of entity, the country of design,
// the domain, the specific identification of the entity, and any extra
// information necessary for describing the entity. Fields not used shall
// contain the value zero.
// See: SISO-REF-010-2019
// https://cdn.ymaws.com/www.sisostandards.org/resource/resmgr/reference_documents_/siso-ref-010.1-2019_operatio.pdf
class EntityType {
  friend class EntityTypeBuilder;

 public:
  // Validate
  void Validate() const;

  // JSON Keys
  inline static const std::string kKindKey = "kind";
  inline static const std::string kDomainKey = "domain";
  inline static const std::string kCountryKey = "country";
  inline static const std::string kCategoryKey = "category";
  inline static const std::string kSubcategoryKey = "subcategory";
  inline static const std::string kSpecificKey = "specific";
  inline static const std::string kExtraKey = "extra";

  // Getters
  const int GetKind() const { return kind_; }
  const int GetDomain() const { return domain_; }
  const int GetCountry() const { return country_; }
  const int GetCategory() const { return category_; }
  const int GetSubcategory() const { return subcategory_; }
  const int GetSpecific() const { return specific_; }
  const int GetExtra() const { return extra_; }

  // Setters
  void SetKind(int kind) { kind_ = kind; }
  void SetDomain(int domain) { domain_ = domain; }
  void SetCountry(int country) { country_ = country; }
  void SetCategory(int category) { category_ = category; }
  void SetSubcategory(int subcategory) { subcategory_ = subcategory; }
  void SetSpecific(int specific) { specific_ = specific; }
  void SetExtra(int extra) { extra_ = extra; }

  // JSON serialization
  friend void to_json(nlohmann::json &j, const EntityType &entity_type);
  friend void from_json(const nlohmann::json &j, EntityType &entity_type);

  nlohmann::json ToJson() const {
    nlohmann::json j;
    to_json(j, *this);
    return j;
  }

  static EntityType FromJson(const nlohmann::json &j) {
    EntityType entity_type;
    from_json(j, entity_type);
    return entity_type;
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const EntityType &entity_type) {
    os << entity_type.ToString();
    return os;
  }

  inline std::string ToString() const {
    return "EntityType: " + ToJson().dump();
  }

  bool operator==(const music::EntityType &other) const {
    // clang-format off
    return other.category_ == category_
        && other.subcategory_ == subcategory_
        && other.country_ == country_
        && other.domain_ == domain_
        && other.extra_ == extra_
        && other.kind_ == kind_
        && other.specific_ == specific_; 
    // clang-format on 
  };

  bool operator!=(const music::EntityType &other) const { return !(other == *this);}
   
protected:
  // The kind the entity belongs to. Unsigned 8-bit integer.
  // Enumerations :
  // 0 Other
  // 1 Platform
  // 2 Munition
  // 3 Life form
  // 4 Environmental
  // 5 Cultural feature
  // 6 Supply
  // 7 Radio
  // 8 Expendable
  // 9 Sensor/Emitter
  int kind_{std::numeric_limits<int>::min()};

  // The domain the entity belongs to. Unsigned 8-bit integer.
  int domain_{std::numeric_limits<int>::min()};

  // The country the entity belongs to. Unsigned 16-bit integer.
  // See:
  // https://faculty.nps.edu/brutzman/vrtp/mil/navy/nps/disEnumerations/JdbeHtmlFiles/pdu/d.htm
  int country_{std::numeric_limits<int>::min()};

  // The category the entity belongs to. Unsigned 8-bit integer.
  int category_{std::numeric_limits<int>::min()};

  // The subcategory the entity belongs to. Unsigned 8-bit integer.
  int subcategory_{std::numeric_limits<int>::min()};

  // The specific the entity belongs to. Unsigned 8-bit integer.
  int specific_{std::numeric_limits<int>::min()};

  // The extra the entity belongs to. Unsigned 8-bit integer.
  int extra_{std::numeric_limits<int>::min()};

};

// --- Builder Implementation ---
class EntityTypeBuilder {
 private:
  EntityType entity_type_;

 public:
  // Setters that return 'this' (the builder itself)
  EntityTypeBuilder &SetKind(int kind) {
    entity_type_.kind_ = kind;
    return *this;
  }
  EntityTypeBuilder &SetDomain(int domain) {
    entity_type_.domain_ = domain;
    return *this;
  }
  EntityTypeBuilder &SetCountry(int country) {
    entity_type_.country_ = country;
    return *this;
  }
  EntityTypeBuilder &SetCategory(int category) {
    entity_type_.category_ = category;
    return *this;
  }
  EntityTypeBuilder &SetSubcategory(int subcategory) {
    entity_type_.subcategory_ = subcategory;
    return *this;
  }
  EntityTypeBuilder &SetSpecific(int specific) {
    entity_type_.specific_ = specific;
    return *this;
  }
  EntityTypeBuilder &SetExtra(int extra) {
    entity_type_.extra_ = extra;
    return *this;
  }

  // The final 'build' method that creates the EntityType object
  EntityType Build() const {
    return entity_type_;
  }
};
}  // namespace music
#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_MESSAGE_ENTITY_TYPE_H_
