/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATADAMAGE_RECORD_H
#define DMI_CONSTRUCT_DATADAMAGE_RECORD_H

#include <codecvt>
#include "../../Interfaces/IToJSON.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class DamageRecord : public MUSICLibrary::Interfaces::IToJSON
	{
	private:
		double catastrophicDamage = 0.0;
		double mobilityDamage = 0.0;
		bool firepowerDisabled = false;
	public:
		double getCatastrophicDamage() const;
		void setCatastrophicDamage(double value);
		double getMobilityDamage() const;
		void setMobilityDamage(double value);
		bool getFirepowerDisabled() const;
		void setFirepowerDisabled(bool value);

		//default constructor
		DamageRecord();

		//Constructor
		DamageRecord(double catastrophicDamage, double mobilityDamage, bool firepowerDisabled);

		//JSON Constructor
		DamageRecord(std::shared_ptr<boost::json::object> obj);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		bool operator == (const DamageRecord &message2);

		bool operator != (const DamageRecord &message2);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const DamageRecord& obj) const;
		};

	};
}

#endif // !DMI_CONSTRUCT_DATADAMAGE_RECORD_H