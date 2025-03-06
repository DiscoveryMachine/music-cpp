/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DeadReckoningParametersRecord.h"
#include "../../tangible_string_helper.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{
	unsigned char DeadReckoningParametersRecord::getAlgorithmValue() const
	{
		return static_cast<unsigned char>(algorithmValue);
	}

	void DeadReckoningParametersRecord::setAlgorithmValue(unsigned char value)
	{
		algorithmValue = value;
	}

	std::shared_ptr<MUSICVector3> DeadReckoningParametersRecord::getLinearAcceleration() const
	{
		return linearAcceleration;
	}

	void DeadReckoningParametersRecord::setLinearAcceleration(std::shared_ptr<MUSICVector3> value)
	{
		linearAcceleration = value;
	}

	std::shared_ptr<MUSICVector3> DeadReckoningParametersRecord::getAngularVelocity() const
	{
		return angularVelocity;
	}

	void DeadReckoningParametersRecord::setAngularVelocity(std::shared_ptr<MUSICVector3> value)
	{
		angularVelocity = value;
	}

	DeadReckoningParametersRecord::DeadReckoningParametersRecord()
	{
	}

	DeadReckoningParametersRecord::DeadReckoningParametersRecord(unsigned char algorithmVal, std::shared_ptr<MUSICVector3> linearAcceleration, std::shared_ptr<MUSICVector3> angularVelocity)
	{
		this->setLinearAcceleration(linearAcceleration);
		this->setAngularVelocity(angularVelocity);
		algorithmValue = algorithmVal;
	}

	DeadReckoningParametersRecord::DeadReckoningParametersRecord(std::shared_ptr<object> obj)
	{
		object objVal = *obj;

		unsigned char value = value_to<unsigned char>(objVal["deadReckAlgorithm"]);

		algorithmValue = value;
		setAngularVelocity(std::make_shared<MUSICVector3>(std::make_shared<boost::json::object>(objVal["angularVelocity"].as_object())));
		setLinearAcceleration(std::make_shared<MUSICVector3>(std::make_shared<boost::json::object>(objVal["linearAcceleration"].as_object())));
	}

	void DeadReckoningParametersRecord::Update(std::shared_ptr<DeadReckoningParametersRecord> record)
	{
		if (record->getAlgorithmValue())
		{
			setAlgorithmValue(record->getAlgorithmValue());
		}
		if (record->getLinearAcceleration() != nullptr)
		{
			setLinearAcceleration(record->getLinearAcceleration());

		}
		if (record->getAngularVelocity() != nullptr)
		{
			setAngularVelocity(record->getAngularVelocity());
		}
	}

	std::shared_ptr<object> DeadReckoningParametersRecord::ToJsonObject()
	{
		object objVal = object();

		objVal["deadReckAlgorithm"] = getAlgorithmValue();
		objVal["angularVelocity"] = *(getAngularVelocity()->ToJsonObject());
		objVal["linearAcceleration"] = *(getLinearAcceleration()->ToJsonObject());

		return std::make_shared<object>(objVal);;
	}

	bool DeadReckoningParametersRecord::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<DeadReckoningParametersRecord>))
		{
			std::shared_ptr<DeadReckoningParametersRecord> other = std::any_cast<std::shared_ptr<DeadReckoningParametersRecord>>(obj);

			if (other == nullptr)
			{
				return false;
			}

			if ((!algorithmValue && !other->algorithmValue) || 
				(algorithmValue && algorithmValue == other->algorithmValue))
			{
				if ((getLinearAcceleration() == nullptr && other->getLinearAcceleration() == nullptr) || 
					(getLinearAcceleration() != nullptr && getLinearAcceleration()->Equals(other->getLinearAcceleration())))
				{
					if ((getAngularVelocity() == nullptr && other->getAngularVelocity() == nullptr) || 
						(getAngularVelocity() != nullptr && getAngularVelocity()->Equals(other->getAngularVelocity())))
					{
						return true;
					}
				}
			}
		}
		else if (obj.type() == typeid(DeadReckoningParametersRecord))
		{
			DeadReckoningParametersRecord other = std::any_cast<DeadReckoningParametersRecord>(obj);

			if ((!algorithmValue && !other.algorithmValue) ||
				(algorithmValue && algorithmValue == other.algorithmValue))
			{
				if ((getLinearAcceleration() == nullptr && other.getLinearAcceleration() == nullptr) ||
					(getLinearAcceleration() != nullptr && getLinearAcceleration()->Equals(other.getLinearAcceleration())))
				{
					if ((getAngularVelocity() == nullptr && other.getAngularVelocity() == nullptr) ||
						(getAngularVelocity() != nullptr && getAngularVelocity()->Equals(other.getAngularVelocity())))
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool DeadReckoningParametersRecord::operator == (const DeadReckoningParametersRecord &dr2)
	{
		return this->Equals(std::make_shared<DeadReckoningParametersRecord>(dr2));
	}

	bool DeadReckoningParametersRecord::operator != (const DeadReckoningParametersRecord &dr2)
	{
		return !(*this == dr2);
	}

	int DeadReckoningParametersRecord::GetHashCode() const
	{
		auto hashCode = 435009586;
		hashCode = hashCode * -1521134295 + ((linearAcceleration != nullptr) ? linearAcceleration->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((angularVelocity != nullptr) ? angularVelocity->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((algorithmValue != NULL) ? std::hash<unsigned char>{}(algorithmValue) : 0);
		
		return hashCode;
	}

	size_t DeadReckoningParametersRecord::HashFunction::operator()(const DeadReckoningParametersRecord& obj) const
	{
		return obj.GetHashCode();
	}
}
