/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DamageRecord.h"
#include "../../tangible_string_helper.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{
	double DamageRecord::getCatastrophicDamage() const
	{
		return catastrophicDamage;
	}

	void DamageRecord::setCatastrophicDamage(double value)
	{
		catastrophicDamage = value;
	}

	double DamageRecord::getMobilityDamage() const
	{
		return mobilityDamage;
	}

	void DamageRecord::setMobilityDamage(double value)
	{
		mobilityDamage = value;
	}

	bool DamageRecord::getFirepowerDisabled() const
	{
		return firepowerDisabled;
	}

	void DamageRecord::setFirepowerDisabled(bool value)
	{
		firepowerDisabled = value;
	}

	DamageRecord::DamageRecord()
	{
	}

	DamageRecord::DamageRecord(double catastrophicDamage, double mobilityDamage, bool firepowerDisabled)
	{
		this->setCatastrophicDamage(catastrophicDamage);
		this->setMobilityDamage(mobilityDamage);
		this->setFirepowerDisabled(firepowerDisabled);
	}

	DamageRecord::DamageRecord(std::shared_ptr<object> obj)
	{
		// Dereference obj since its pointer was passed in
		object objVal = *obj;
		setCatastrophicDamage(static_cast<double>(objVal["catastrophicDamage"].as_double()));
		setMobilityDamage(static_cast<double>(objVal["mobilityDamage"].as_double()));
		setFirepowerDisabled(static_cast<bool>(objVal["firepowerDisabled"].as_bool()));
	}

	std::shared_ptr<object> DamageRecord::ToJsonObject()
	{
		object objVal = object();

		objVal["catastrophicDamage"] = this->getCatastrophicDamage();
		objVal["mobilityDamage"] = this->getMobilityDamage();
		objVal["firepowerDisabled"] = this->getFirepowerDisabled();

		return std::make_shared<object>(objVal);;
	}

	bool DamageRecord::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DamageRecord>))
		{
			std::shared_ptr<DamageRecord> other = std::any_cast<std::shared_ptr<DamageRecord>>(obj);

			if (other == nullptr)
			{
				return false;
			}

			if (getCatastrophicDamage() == other->getCatastrophicDamage() &&
				getMobilityDamage() == other->getMobilityDamage() &&
				getFirepowerDisabled() == other->getFirepowerDisabled())
			{
				return true;
			}
		}
		else if (obj.type() == typeid(DamageRecord))
		{
			DamageRecord other = std::any_cast<DamageRecord>(obj);
			if (getCatastrophicDamage() == other.getCatastrophicDamage() &&
				getMobilityDamage() == other.getMobilityDamage() &&
				getFirepowerDisabled() == other.getFirepowerDisabled())
			{
				return true;
			}
		}

		return false;
	}

	bool DamageRecord::operator == (const DamageRecord &message2)
	{
		return this->Equals(std::make_shared<DamageRecord>(message2));
	}

	bool DamageRecord::operator != (const DamageRecord &message2)
	{
		return !(*this == message2);
	}

	int DamageRecord::GetHashCode() const
	{
		auto hashCode = -1380379703;
		hashCode = hashCode * -1521134295 + ((catastrophicDamage != NULL) ? std::hash<int>{}(catastrophicDamage) : 0);
		hashCode = hashCode * -1521134295 + ((mobilityDamage != NULL) ? std::hash<int>{}(mobilityDamage) : 0);
		hashCode = hashCode * -1521134295 + ((firepowerDisabled != NULL) ? std::hash<bool>{}(firepowerDisabled) : 0);

		return hashCode;
	}

	size_t DamageRecord::HashFunction::operator()(const DamageRecord& obj) const
	{
		return obj.GetHashCode();
	}
}
