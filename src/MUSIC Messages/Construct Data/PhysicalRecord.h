/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATA_PHYSICAL_RECORD_H
#define DMI_CONSTRUCT_DATA_PHYSICAL_RECORD_H

#include <optional>
#include "../Force.h"
#include "../EntityTypeRecord.h"
#include "DeadReckoningParametersRecord.h"
#include "DamageRecord.h"
#include "../MUSICVector3.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class PhysicalRecord
	{
	private:
		std::optional<Force> ForceIDField;
		std::shared_ptr<EntityTypeRecord> EntityType;
		std::shared_ptr<DeadReckoningParametersRecord> DeadReckoningParameters;
		std::shared_ptr<DamageRecord> Damage;
		std::shared_ptr<MUSICVector3> Location;
		std::shared_ptr<MUSICVector3> Orientation;
		std::shared_ptr<MUSICVector3> LinearVelocity;

	public:
		std::optional<Force> getForceIDField() const;
		void setForceIDField(const std::optional<Force> &value);
		std::shared_ptr<EntityTypeRecord> getEntityType() const;
		void setEntityType(std::shared_ptr<EntityTypeRecord> value);
		std::shared_ptr<DeadReckoningParametersRecord> getDeadReckoningParameters() const;
		void setDeadReckoningParameters(std::shared_ptr<DeadReckoningParametersRecord> value);
		std::shared_ptr<DamageRecord> getDamage() const;
		void setDamage(std::shared_ptr<DamageRecord> value);
		std::shared_ptr<MUSICVector3> getLocation() const;
		void setLocation(std::shared_ptr<MUSICVector3> value);
		std::shared_ptr<MUSICVector3> getOrientation() const;
		void setOrientation(std::shared_ptr<MUSICVector3> value);
		std::shared_ptr<MUSICVector3> getLinearVelocity() const;
		void setLinearVelocity(std::shared_ptr<MUSICVector3> value);

		PhysicalRecord();

		PhysicalRecord(Force force, std::shared_ptr<EntityTypeRecord> eRecord, std::shared_ptr<MUSICVector3> location, std::shared_ptr<MUSICVector3> orientation, std::shared_ptr<MUSICVector3> linearVelocity, std::shared_ptr<DamageRecord> dRecord, std::shared_ptr<DeadReckoningParametersRecord> deadRecord = nullptr);

		PhysicalRecord(std::shared_ptr<boost::json::object> obj);

		void Update(std::shared_ptr<PhysicalRecord> other);

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const PhysicalRecord& obj) const;
		};

		bool operator == (const PhysicalRecord &record2);

		bool operator != (const PhysicalRecord &record2);
	};
}

#endif // !DMI_CONSTRUCT_DATA_PHYSICAL_RECORD_H