/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "EntityTypeRecord.h"
#include "../tangible_string_helper.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	EntityKind EntityTypeRecord::getKind() const
	{
		return Kind;
	}

	void EntityTypeRecord::setKind(EntityKind value)
	{
		Kind = value;
	}

	EntityDomain EntityTypeRecord::getDomain() const
	{
		return Domain;
	}

	void EntityTypeRecord::setDomain(EntityDomain value)
	{
		Domain = value;
	}

	EntityCountry EntityTypeRecord::getCountry() const
	{
		return Country;
	}

	void EntityTypeRecord::setCountry(EntityCountry value)
	{
		Country = value;
	}

	unsigned char EntityTypeRecord::getCategory() const
	{
		return Category;
	}

	void EntityTypeRecord::setCategory(unsigned char value)
	{
		Category = value;
	}

	unsigned char EntityTypeRecord::getSubcategory() const
	{
		return Subcategory;
	}

	void EntityTypeRecord::setSubcategory(unsigned char value)
	{
		Subcategory = value;
	}

	unsigned char EntityTypeRecord::getSpecificInfo() const
	{
		return SpecificInfo;
	}

	void EntityTypeRecord::setSpecificInfo(unsigned char value)
	{
		SpecificInfo = value;
	}

	unsigned char EntityTypeRecord::getExtraValue() const
	{
		return ExtraValue;
	}

	void EntityTypeRecord::setExtraValue(unsigned char value)
	{
		ExtraValue = value;
	}

	EntityTypeRecord::EntityTypeRecord()
	{
	}

	EntityTypeRecord::EntityTypeRecord(EntityKind kind, EntityDomain domain, EntityCountry country, unsigned char category, unsigned char subcategory, unsigned char specificInfo, unsigned char extraValue)
	{
		setKind(kind);
		setDomain(domain);
		setCountry(country);
		setCategory(category);
		setSubcategory(subcategory);
		setSpecificInfo(specificInfo);
		setExtraValue(extraValue);
	}

	EntityTypeRecord::EntityTypeRecord(std::shared_ptr<object> obj)
	{
		object objVal = *obj;

		setKind(static_cast<EntityKind>(objVal["kind"].as_int64()));
		setDomain(static_cast<EntityDomain>(objVal["domain"].as_int64()));
		setCountry(static_cast<EntityCountry>(objVal["country"].as_int64()));
		setCategory(value_to<unsigned char>(objVal["category"]));
		setSubcategory(value_to<unsigned char>(objVal["subcategory"]));
		setSpecificInfo(value_to<unsigned char>(objVal["specific"]));
		setExtraValue(value_to<unsigned char>(objVal["extra"]));
	}

	std::shared_ptr<object> EntityTypeRecord::ToJsonObject()
	{
		object objVal;

		objVal["kind"] = static_cast<int>(getKind());
		objVal["domain"] = static_cast<int>(getDomain());
		objVal["country"] = static_cast<int>(getCountry());
		objVal["category"] = getCategory();
		objVal["subcategory"] = getSubcategory();
		objVal["specific"] = getSpecificInfo();
		objVal["extra"] = getExtraValue();

		return std::make_shared<object>(objVal);
	}


	bool EntityTypeRecord::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<EntityTypeRecord>))
		{
			std::shared_ptr<EntityTypeRecord> other = std::any_cast<std::shared_ptr<EntityTypeRecord>>(obj);

			if (other == nullptr)
			{
				return false;
			}

			if (getKind() == other->getKind() && 
				getDomain() == other->getDomain() && 
				getCountry() == other->getCountry() && 
				getCategory() == other->getCategory() && 
				getSubcategory() == other->getSubcategory() && 
				getSpecificInfo() == other->getSpecificInfo() && 
				getExtraValue() == other->getExtraValue())
			{
				return true;
			}
		}
		else if (obj.type() == typeid(EntityTypeRecord))
		{
			EntityTypeRecord other = std::any_cast<EntityTypeRecord>(obj);
			if (getKind() == other.getKind() &&
				getDomain() == other.getDomain() &&
				getCountry() == other.getCountry() &&
				getCategory() == other.getCategory() &&
				getSubcategory() == other.getSubcategory() &&
				getSpecificInfo() == other.getSpecificInfo() &&
				getExtraValue() == other.getExtraValue())
			{
				return true;
			}
		}

		return false;
	}

	int EntityTypeRecord::GetHashCode() const
	{
		auto hashCode = 965029686;
		hashCode = hashCode * -1521134295 + std::hash<EntityKind>{}(getKind());
		hashCode = hashCode * -1521134295 + std::hash<EntityDomain>{}(getDomain());
		hashCode = hashCode * -1521134295 + std::hash<EntityCountry>{}(getCountry());
		hashCode = hashCode * -1521134295 + std::hash<unsigned char>{}(getCategory());
		hashCode = hashCode * -1521134295 + std::hash<unsigned char>{}(getSubcategory());
		hashCode = hashCode * -1521134295 + std::hash<unsigned char>{}(getSpecificInfo());
		hashCode = hashCode * -1521134295 + std::hash<unsigned char>{}(getExtraValue());
		return hashCode;
	}

	size_t EntityTypeRecord::HashFunction::operator()(const EntityTypeRecord& obj) const
	{
		return obj.GetHashCode();
	}

	bool EntityTypeRecord::operator == (const EntityTypeRecord &message2)
	{
		return this->Equals(std::make_shared<EntityTypeRecord>(message2));
	}

	bool EntityTypeRecord::operator != (const EntityTypeRecord &message2)
	{
		return !(*this == message2);
	}
}
