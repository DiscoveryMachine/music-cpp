/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PhysicalRecord.h"

using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	std::optional<Force> PhysicalRecord::getForceIDField() const
	{
		return ForceIDField;
	}

	void PhysicalRecord::setForceIDField(const std::optional<Force> &value)
	{
		ForceIDField = value;
	}

	std::shared_ptr<EntityTypeRecord> PhysicalRecord::getEntityType() const
	{
		return EntityType;
	}

	void PhysicalRecord::setEntityType(std::shared_ptr<EntityTypeRecord> value)
	{
		EntityType = value;
	}

	std::shared_ptr<DeadReckoningParametersRecord> PhysicalRecord::getDeadReckoningParameters() const
	{
		return DeadReckoningParameters;
	}

	void PhysicalRecord::setDeadReckoningParameters(std::shared_ptr<DeadReckoningParametersRecord> value)
	{
		DeadReckoningParameters = value;
	}

	std::shared_ptr<DamageRecord> PhysicalRecord::getDamage() const
	{
		return Damage;
	}

	void PhysicalRecord::setDamage(std::shared_ptr<DamageRecord> value)
	{
		Damage = value;
	}

	std::shared_ptr<MUSICVector3> PhysicalRecord::getLocation() const
	{
		return Location;
	}

	void PhysicalRecord::setLocation(std::shared_ptr<MUSICVector3> value)
	{
		Location = value;
	}

	std::shared_ptr<MUSICVector3> PhysicalRecord::getOrientation() const
	{
		return Orientation;
	}

	void PhysicalRecord::setOrientation(std::shared_ptr<MUSICVector3> value)
	{
		Orientation = value;
	}

	std::shared_ptr<MUSICVector3> PhysicalRecord::getLinearVelocity() const
	{
		return LinearVelocity;
	}

	void PhysicalRecord::setLinearVelocity(std::shared_ptr<MUSICVector3> value)
	{
		LinearVelocity = value;
	}

	PhysicalRecord::PhysicalRecord()
	{
	}

	PhysicalRecord::PhysicalRecord(Force force, std::shared_ptr<EntityTypeRecord> eRecord, std::shared_ptr<MUSICVector3> location, std::shared_ptr<MUSICVector3> orientation, std::shared_ptr<MUSICVector3> linearVelocity, std::shared_ptr<DamageRecord> dRecord, std::shared_ptr<DeadReckoningParametersRecord> deadRecord)
	{
		setForceIDField(force);
		setEntityType(eRecord);
		setDeadReckoningParameters(deadRecord);
		setDamage(dRecord);
		setLocation(location);
		setOrientation(orientation);
		setLinearVelocity(linearVelocity);
	}

	PhysicalRecord::PhysicalRecord(std::shared_ptr<object> obj)
	{
		object objVal = *obj;

		setForceIDField(static_cast<Force>(value_to<int>(objVal["force"])));
		setEntityType(std::make_shared<EntityTypeRecord>(EntityTypeRecord(std::make_shared<object>(objVal["entityType"].as_object()))));
		setLocation(std::make_shared<MUSICVector3>(MUSICVector3(std::make_shared<object>(objVal["location"].as_object()))));
		setOrientation(std::make_shared<MUSICVector3>(MUSICVector3(std::make_shared<object>(objVal["orientation"].as_object()))));
		setLinearVelocity(std::make_shared<MUSICVector3>(MUSICVector3(std::make_shared<object>(objVal["linearVelocity"].as_object()))));
		if (!objVal["deadReck"].as_object().empty())
		{
			setDeadReckoningParameters(std::make_shared<DeadReckoningParametersRecord>(DeadReckoningParametersRecord(std::make_shared<object>(objVal["deadReck"].as_object()))));
		}
		setDamage(std::make_shared<DamageRecord>(DamageRecord(std::make_shared<object>(objVal["damageRecord"].as_object()))));
	}

	void PhysicalRecord::Update(std::shared_ptr<PhysicalRecord> other)
	{
		setLocation(other->getLocation() == nullptr ? getLocation() : other->getLocation());
		setForceIDField(!other->getForceIDField() ? getForceIDField() : other->getForceIDField());
		setOrientation(other->getOrientation() == nullptr ? getOrientation() : other->getOrientation());
		setEntityType(other->getEntityType() == nullptr ? getEntityType() : other->getEntityType());
		setDamage(other->getDamage() == nullptr ? getDamage() : other->getDamage());
		setDeadReckoningParameters(other->getDeadReckoningParameters() == nullptr ? getDeadReckoningParameters() : other->getDeadReckoningParameters());
		setLinearVelocity(other->getLinearVelocity() == nullptr ? getLinearVelocity() : other->getLinearVelocity());
	}

	bool PhysicalRecord::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<PhysicalRecord>))
		{
			std::shared_ptr<PhysicalRecord> other = std::any_cast<std::shared_ptr<PhysicalRecord>>(obj);

			if (other == nullptr)
			{
				return false;
			}

			// Test empty case
			if (!getForceIDField().has_value() && !other->getForceIDField().has_value() &&
				getEntityType() == nullptr && other->getEntityType() == nullptr &&
				getDeadReckoningParameters() == nullptr && other->getDeadReckoningParameters() == nullptr &&
				getDamage() == nullptr && other->getDamage() == nullptr &&
				getLocation() == nullptr && other->getLocation() == nullptr &&
				getOrientation() == nullptr && other->getOrientation() == nullptr &&
				getLinearVelocity() == nullptr && other->getLinearVelocity() == nullptr)
			{
				return true;
			}

			if (getForceIDField() == other->getForceIDField() && 
				(*getEntityType()) == (*other->getEntityType()) && 
				getDeadReckoningParameters()->Equals(other->getDeadReckoningParameters()) && 
				getDamage()->Equals(other->getDamage()) && 
				getLocation()->Equals(other->getLocation())&& 
				getOrientation()->Equals(other->getOrientation()) && 
				getLinearVelocity()->Equals(other->getLinearVelocity()))
			{
				return true;
			}
		}
		else if (obj.type() == typeid(PhysicalRecord))
		{
			PhysicalRecord other = std::any_cast<PhysicalRecord>(obj);

			if (getForceIDField() == other.getForceIDField() &&
				getEntityType()->Equals(other.getEntityType()) &&
				getDeadReckoningParameters()->Equals(other.getDeadReckoningParameters()) &&
				getDamage()->Equals(other.getDamage()) &&
				getLocation()->Equals(other.getLocation()) &&
				getOrientation()->Equals(other.getOrientation()) &&
				getLinearVelocity()->Equals(other.getLinearVelocity()))
			{
				return true;
			}
		}

		return false;
	}

	int PhysicalRecord::GetHashCode() const
	{
		auto hashCode = -1980379703;
		hashCode = hashCode * -1521134295 + ((ForceIDField.has_value() ? std::hash<std::optional<Force>>{}(ForceIDField) : 0));
		hashCode = hashCode * -1521134295 + ((EntityType != nullptr) ? EntityType->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((DeadReckoningParameters != nullptr) ? DeadReckoningParameters->GetHashCode() : 0); 
		hashCode = hashCode * -1521134295 + ((Damage != nullptr) ? Damage->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((Location != nullptr) ? Location->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((Orientation != nullptr) ? Orientation->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((LinearVelocity != nullptr) ? LinearVelocity->GetHashCode() : 0);

		return hashCode;
	}


	size_t PhysicalRecord::HashFunction::operator()(const PhysicalRecord& obj) const
	{
		return obj.GetHashCode();
	}


	bool PhysicalRecord::operator == (const PhysicalRecord &record2)
	{
		return this->Equals(std::make_shared<PhysicalRecord>(record2));
	}

	bool PhysicalRecord::operator != (const PhysicalRecord &record2)
	{
		return !(*this == record2);
	}
}
