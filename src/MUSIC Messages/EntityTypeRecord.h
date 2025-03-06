/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_ENTITY_TYPE_RECORD_H
#define DMI_MUSIC_MESSAGES_ENTITY_TYPE_RECORD_H

#include <codecvt>
#include "../Interfaces/IToJSON.h"
#include "EntityKind.h"
#include "EntityDomain.h"
#include "EntityCountry.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class EntityTypeRecord : public MUSICLibrary::Interfaces::IToJSON
	{
	private:
		EntityKind Kind = static_cast<EntityKind>(0);
		EntityDomain Domain = static_cast<EntityDomain>(0);
		EntityCountry Country = static_cast<EntityCountry>(0);
		unsigned char Category = 0;
		unsigned char Subcategory = 0;
		unsigned char SpecificInfo = 0;
		unsigned char ExtraValue = 0;

	public:
		EntityKind getKind() const;
		void setKind(EntityKind value);

		EntityDomain getDomain() const;
		void setDomain(EntityDomain value);

		EntityCountry getCountry() const;
		void setCountry(EntityCountry value);

		unsigned char getCategory() const;
		void setCategory(unsigned char value);

		unsigned char getSubcategory() const;
		void setSubcategory(unsigned char value);

		unsigned char getSpecificInfo() const;
		void setSpecificInfo(unsigned char value);

		unsigned char getExtraValue() const;
		void setExtraValue(unsigned char value);

		//Default Constructor
		EntityTypeRecord();

		//Constructor
		EntityTypeRecord(EntityKind kind, EntityDomain domain, EntityCountry country, unsigned char category, unsigned char subcategory, unsigned char specificInfo, unsigned char extraValue);

		//JSON Constructor
		EntityTypeRecord(std::shared_ptr<boost::json::object> obj);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const EntityTypeRecord& obj) const;
		};

		bool operator == (const EntityTypeRecord &message2);

		bool operator != (const EntityTypeRecord &message2);
	};
}

#endif // !DMI_MUSIC_MESSAGES_ENTITY_TYPE_RECORD_H