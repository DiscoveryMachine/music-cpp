/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PerceptionRecord.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::PerceptionData
{

	std::shared_ptr<EntityIDRecord> PerceptionRecord::getPerceptionID() const
	{
		return perceptionID;
	}

	void PerceptionRecord::setPerceptionID(std::shared_ptr<EntityIDRecord> value)
	{
		perceptionID = value;
	}

	MUSICLibrary::MUSIC_Messages::Force PerceptionRecord::getForceIDField() const
	{
		return forceIDField;
	}

	void PerceptionRecord::setForceIDField(Force value)
	{
		forceIDField = value;
	}

	std::shared_ptr<EntityTypeRecord> PerceptionRecord::getEntityTypeRecord() const
	{
		return entityTypeRecord;
	}

	void PerceptionRecord::setEntityTypeRecord(std::shared_ptr<EntityTypeRecord> value)
	{
		entityTypeRecord = value;
	}

	MUSICLibrary::MUSIC_Messages::SensorType PerceptionRecord::getPerceptionSystem() const
	{
		return perceptionSystem;
	}

	void PerceptionRecord::setPerceptionSystem(SensorType value)
	{
		perceptionSystem = value;
	}

	float PerceptionRecord::getAccuracy() const
	{
		return accuracy;
	}

	void PerceptionRecord::setAccuracy(float value)
	{
		accuracy = value;
	}

	std::shared_ptr<PerceptionErrors> PerceptionRecord::getPerceptionErrors() const
	{
		return perceptionErrors;
	}

	void PerceptionRecord::setPerceptionErrors(std::shared_ptr<PerceptionErrors> value)
	{
		perceptionErrors = value;
	}

	PerceptionRecord::PerceptionRecord()
	{
	}

	PerceptionRecord::PerceptionRecord(std::shared_ptr<EntityIDRecord> perceptionID, Force forceIDField, std::shared_ptr<EntityTypeRecord> entityTypeRecord, SensorType perceptionSystem, float accuracy, std::shared_ptr<PerceptionErrors> perceptionErrors)
	{
		this->setPerceptionID(perceptionID);
		this->setForceIDField(forceIDField);
		this->setEntityTypeRecord(entityTypeRecord);
		this->setPerceptionSystem(perceptionSystem);
		this->setAccuracy(accuracy);
		this->setPerceptionErrors(perceptionErrors);
	}

	std::shared_ptr<EntityIDRecord> PerceptionRecord::extractEntityIDRecordFromJson(object json, std::string entityIDKey)
	{
		object entityIDJson = json[entityIDKey].as_object();
		EntityIDRecord record = EntityIDRecord(value_to<unsigned int>(entityIDJson["siteID"]),
											   value_to<unsigned int>(entityIDJson["appID"]),
											   entityIDJson["entityID"].is_null() ? 0 : value_to<unsigned int>(entityIDJson["entityID"]));
		return std::make_shared<EntityIDRecord>(record);
	}

	PerceptionRecord::PerceptionRecord(std::shared_ptr<object> jobj)
	{
		object objVal = *jobj;

		this->setPerceptionID(extractEntityIDRecordFromJson(objVal, StringHelper::wstringToString(PERCEPTION_ID)));
		this->setForceIDField(static_cast<Force>(value_to<int>(objVal[StringHelper::wstringToString(FORCE_ID_FIELD)])));

		object entityTypeJson = objVal[StringHelper::wstringToString(ENTITY_TYPE_RECORD)].as_object();
		EntityTypeRecord record = EntityTypeRecord(std::make_shared<object>(entityTypeJson));
		this->setEntityTypeRecord(std::make_shared<EntityTypeRecord>(record));

		this->setPerceptionSystem(static_cast<SensorType>(value_to<int>(objVal[StringHelper::wstringToString(PERCEPTION_SYSTEM)])));
		this->setAccuracy(value_to<float>(objVal[StringHelper::wstringToString(ACCURACY)]));

		//object perceptionErrorJson = objVal[StringHelper::wstringToString(PERCEPTION_ERRORS)].as_object();
		unsigned int bearing = value_to<unsigned int>(objVal[StringHelper::wstringToString(PerceptionErrors::BEARING_ERROR)]);
		unsigned int altitude = value_to<unsigned int>(objVal[StringHelper::wstringToString(PerceptionErrors::ALTITUDE_ERROR)]);
		unsigned int range = value_to<unsigned int>(objVal[StringHelper::wstringToString(PerceptionErrors::RANGE_ERROR)]);
		PerceptionErrors errors = PerceptionErrors(bearing, altitude, range);
		this->setPerceptionErrors(std::make_shared<PerceptionErrors>(errors));
	}

	std::shared_ptr<object> PerceptionRecord::ToJsonObject()
	{
		object jsonObj;

		jsonObj[StringHelper::wstringToString(PERCEPTION_ID)] = *(getPerceptionID()->ToJsonObject());
		jsonObj[StringHelper::wstringToString(FORCE_ID_FIELD)] = static_cast<int>(getForceIDField());
		jsonObj[StringHelper::wstringToString(ENTITY_TYPE_RECORD)] = *(getEntityTypeRecord()->ToJsonObject());
		jsonObj[StringHelper::wstringToString(PERCEPTION_SYSTEM)] = static_cast<int>(getPerceptionSystem());
		jsonObj[StringHelper::wstringToString(ACCURACY)] = getAccuracy();
		jsonObj[StringHelper::wstringToString(getPerceptionErrors()->BEARING_ERROR)] = (getPerceptionErrors()->getBearingError());
		jsonObj[StringHelper::wstringToString(getPerceptionErrors()->ALTITUDE_ERROR)] = (getPerceptionErrors()->getAltitudeError());
		jsonObj[StringHelper::wstringToString(getPerceptionErrors()->RANGE_ERROR)] = (getPerceptionErrors()->getRangeError());
		

		std::shared_ptr<object> jobj = std::make_shared<object>(jsonObj);
		return jobj;
	}

	bool PerceptionRecord::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<PerceptionRecord>))
		{
			std::shared_ptr<PerceptionRecord> other = std::any_cast<std::shared_ptr<PerceptionRecord>>(obj);
			return getPerceptionErrors()->Equals(other->getPerceptionErrors()) && 
				getPerceptionID()->Equals(other->getPerceptionID()) && 
				getPerceptionSystem() == other->getPerceptionSystem() && 
				getEntityTypeRecord()->Equals(other->getEntityTypeRecord()) && 
				getForceIDField() == other->getForceIDField() && 
				getAccuracy() == other->getAccuracy();
		}
			
		return false;
	}

	int PerceptionRecord::GetHashCode() const
	{
		auto hashCode = 852925;
		hashCode = hashCode * -1521134295 + (PerceptionRecord::getPerceptionID() != nullptr) ? getPerceptionID()->GetHashCode() : 0;
		hashCode = hashCode * -1521134295 + std::hash<MUSICLibrary::MUSIC_Messages::Force>{}(getForceIDField());
		hashCode = hashCode * -1521134295 + (PerceptionRecord::getEntityTypeRecord() != nullptr) ? getEntityTypeRecord()->GetHashCode() : 0;
		hashCode = hashCode * -1521134295 + std::hash<MUSICLibrary::MUSIC_Messages::SensorType>{}(getPerceptionSystem());
		hashCode = hashCode * -1521134295 + std::hash<float>{}(getAccuracy());
		hashCode = hashCode * -1521134295 + (PerceptionRecord::getPerceptionErrors() != nullptr) ? getPerceptionErrors()->GetHashCode() : 0;
		return hashCode;
	}

	size_t PerceptionRecord::HashFunction::operator()(const PerceptionRecord& obj) const
	{
		return obj.GetHashCode();
	}

	bool PerceptionRecord::operator == (const PerceptionRecord &pr2)
	{
		return this->Equals(std::make_shared<PerceptionRecord>(pr2));
	}

	bool PerceptionRecord::operator != (const PerceptionRecord &pr2)
	{
		return !(*this == pr2);
	}
}
