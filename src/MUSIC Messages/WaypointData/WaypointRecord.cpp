/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "WaypointRecord.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{
	std::shared_ptr<MUSICVector3> WaypointRecord::getWorldCoordinateRecord() const
	{
		return worldCoordinateRecord;
	}

	void WaypointRecord::setWorldCoordinateRecord(std::shared_ptr<MUSICVector3> value)
	{
		worldCoordinateRecord = value;
	}

	unsigned int WaypointRecord::getEstimatedArrivalTime() const
	{
		return estimatedArrivalTime;
	}

	void WaypointRecord::setEstimatedArrivalTime(unsigned int value)
	{
		estimatedArrivalTime = value;
	}

	unsigned int WaypointRecord::getArrivalTimeError() const
	{
		return arrivalTimeError;
	}

	void WaypointRecord::setArrivalTimeError(unsigned int value)
	{
		arrivalTimeError = value;
	}

	const std::wstring WaypointRecord::WORLD_COORDINATE_RECORD = L"worldCoordinateRecord";
	const std::wstring WaypointRecord::ESTIMATED_ARRIVAL_TIME = L"estimatedArrivalTime";
	const std::wstring WaypointRecord::ARRIVAL_TIME_ERROR = L"arrivalTimeError";

	WaypointRecord::WaypointRecord()
	{
	}

	WaypointRecord::WaypointRecord(std::shared_ptr<MUSICVector3> worldCoordinateRecord, unsigned int estimatedArrivalTime, unsigned int arrivalTimeError)
	{
		this->setWorldCoordinateRecord(worldCoordinateRecord);
		this->setEstimatedArrivalTime(estimatedArrivalTime);
		this->setArrivalTimeError(arrivalTimeError);
	}

	WaypointRecord::WaypointRecord(std::shared_ptr<object> jobj)
	{
		object objVal = *jobj;
		object coordJson = objVal[StringHelper::wstringToString(WORLD_COORDINATE_RECORD)].as_object();

		this->setWorldCoordinateRecord(std::make_shared<MUSICVector3>(std::make_shared<object>(coordJson)));
		this->setEstimatedArrivalTime(value_to<unsigned int>(objVal[StringHelper::wstringToString(ESTIMATED_ARRIVAL_TIME)]));
		this->setArrivalTimeError(value_to<unsigned int>(objVal[StringHelper::wstringToString(ARRIVAL_TIME_ERROR)]));
	}

	std::shared_ptr<object> WaypointRecord::ToJsonObject()
	{
		object objVal = object();

		objVal[StringHelper::wstringToString(WORLD_COORDINATE_RECORD)] = *(getWorldCoordinateRecord()->ToJsonObject());
		objVal[StringHelper::wstringToString(ESTIMATED_ARRIVAL_TIME)] = getEstimatedArrivalTime();
		objVal[StringHelper::wstringToString(ARRIVAL_TIME_ERROR)] = getArrivalTimeError();

		return std::make_shared<object>(objVal);
	}

	bool WaypointRecord::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<WaypointRecord>))
		{
			std::shared_ptr<WaypointRecord> other = std::any_cast<std::shared_ptr<WaypointRecord>>(obj);

			if (getWorldCoordinateRecord()->Equals(other->getWorldCoordinateRecord()) &&
				getEstimatedArrivalTime() == other->getEstimatedArrivalTime() &&
				getArrivalTimeError() == other->getArrivalTimeError())
			{
				return true;
			}
		}

		return false;
	}

	int WaypointRecord::GetHashCode() const
	{
		auto hashCode = 400749366;
		hashCode = hashCode * -1521134295 + (getWorldCoordinateRecord() != nullptr) ? getWorldCoordinateRecord()->GetHashCode() : 0;
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getEstimatedArrivalTime());
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getArrivalTimeError());
		return hashCode;
	}

	size_t WaypointRecord::HashFunction::operator()(const WaypointRecord& obj) const
	{
		return obj.GetHashCode();
	}

	bool WaypointRecord::operator == (const WaypointRecord &record2)
	{
		return this->Equals(std::make_shared<WaypointRecord>(record2));
	}

	bool WaypointRecord::operator != (const WaypointRecord &record2)
	{
		return !(*this == record2);
	}
}
