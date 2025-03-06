/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "WaypointDataMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{
	unsigned short WaypointDataMessage::getCurrentWaypointIndex() const
	{
		return CurrentWaypointIndex;
	}

	void WaypointDataMessage::setCurrentWaypointIndex(unsigned short value)
	{
		CurrentWaypointIndex = value;
	}

	std::vector<std::shared_ptr<WaypointRecord>> WaypointDataMessage::getWaypoints() const
	{
		return Waypoints;
	}

	void WaypointDataMessage::setWaypoints(const std::vector<std::shared_ptr<WaypointRecord>> &value)
	{
		Waypoints = value;
	}

	const std::wstring WaypointDataMessage::HEADER_KEY = L"header";
	const std::wstring WaypointDataMessage::ORIGIN_ID_KEY = L"originID";
	const std::wstring WaypointDataMessage::CURRENT_WAYPOINT_INDEX_KEY = L"currentWaypointIndex";
	const std::wstring WaypointDataMessage::WAYPOINT_RECORDS_KEY = L"waypointRecords";
	const std::wstring WaypointDataMessage::NUM_WAYPOINTS_KEY = L"numWaypoints";

	WaypointDataMessage::WaypointDataMessage()
	{
	}

	WaypointDataMessage::WaypointDataMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> entityIDRecord, unsigned short currentWaypointIndex, std::vector<std::shared_ptr<WaypointRecord>> &waypoints) : MUSICMessage(exerciseID, entityIDRecord)
	{
		this->setCurrentWaypointIndex(currentWaypointIndex);
		this->setWaypoints(waypoints);
	}

	WaypointDataMessage::WaypointDataMessage(std::shared_ptr<boost::json::object> jObject) : MUSICMessage((*jObject)["header"].as_object(), (*jObject)["originID"].as_object())
	{
		boost::json::object objVal = *jObject;

		setCurrentWaypointIndex(boost::json::value_to<unsigned short>(objVal[StringHelper::wstringToString(CURRENT_WAYPOINT_INDEX_KEY)]));

		boost::json::array waypointArray = objVal[StringHelper::wstringToString(WAYPOINT_RECORDS_KEY)].as_array();
		std::vector<std::shared_ptr<WaypointRecord>> waypoints = std::vector<std::shared_ptr<WaypointRecord>>();

		// Create a shared pointer for each waypoint record and put it in the vector of these pointers
		for (int i = 0; i < waypointArray.size(); i++)
		{
			waypoints.push_back(std::make_shared<WaypointRecord>(std::make_shared<boost::json::object>(waypointArray.at(i).as_object())));
		}

		setWaypoints(waypoints);
	}

	void WaypointDataMessage::AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor)
	{
		visitor->VisitWaypointData(shared_from_this());
	}

	std::any WaypointDataMessage::Clone()
	{
		std::vector<std::shared_ptr<WaypointRecord>> clonedList;

		// need to iterate through waypoints to create a deep copy as it has more than
		// just primitive data types.
		for (auto j : getWaypoints())
		{
			std::shared_ptr<MUSICVector3> clonedMUSICVector3 = std::make_shared<MUSICVector3>(j->getWorldCoordinateRecord()->getX(), j->getWorldCoordinateRecord()->getY(), j->getWorldCoordinateRecord()->getZ());
			std::shared_ptr<WaypointRecord> clonedWR = std::make_shared<WaypointRecord>(clonedMUSICVector3, j->getEstimatedArrivalTime(), j->getArrivalTimeError());
			clonedList.push_back(clonedWR);
		}

		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<WaypointDataMessage> clonedMessage = std::make_shared<WaypointDataMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, getCurrentWaypointIndex(), clonedList);

		return clonedMessage;
	}

	std::shared_ptr<object> WaypointDataMessage::ToJsonObject()
	{
		object objVal = object();

		objVal[StringHelper::wstringToString(HEADER_KEY)] = *(getMUSICHeader()->ToJsonObject());
		objVal[StringHelper::wstringToString(ORIGIN_ID_KEY)] = *(getOriginID()->ToJsonObject());
		objVal[StringHelper::wstringToString(CURRENT_WAYPOINT_INDEX_KEY)] = getCurrentWaypointIndex();

		objVal[StringHelper::wstringToString(NUM_WAYPOINTS_KEY)] = getWaypoints().size();

		// need to create a J array, loop through our objects, convert them to JObjects
		// then add them to J array.
		std::shared_ptr<boost::json::array> jArray = std::make_shared<boost::json::array>();
		std::for_each(Waypoints.begin(), Waypoints.end(), [jArray] (std::shared_ptr<WaypointRecord> record)
		{
			jArray->push_back(*(record->ToJsonObject()));
		});
		objVal[StringHelper::wstringToString(WAYPOINT_RECORDS_KEY)] = *jArray;

		return std::make_shared<object>(objVal);
	}

	int WaypointDataMessage::GetHashCode() const
	{
		auto hashCode = -375801064;
		hashCode = hashCode * -1521134295 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getCurrentWaypointIndex() != NULL) ? std::hash<unsigned short>{}(getCurrentWaypointIndex()) : 0);

		int sum = 0;
		for (auto wp : getWaypoints())
		{
			sum += wp->GetHashCode();
		}

		hashCode = hashCode * -1521134295 + sum;
		return hashCode;
	}

	size_t WaypointDataMessage::HashFunction::operator()(const WaypointDataMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool WaypointDataMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<WaypointDataMessage>))
		{
			std::shared_ptr<WaypointDataMessage> other = std::any_cast<std::shared_ptr<WaypointDataMessage>>(obj);

			std::shared_ptr<MUSICMessage> parent = std::make_shared<MUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID());

			//if any of the message's properties don't equate to the other, then it will return false
			if (MUSICMessage::Equals(parent) &&
				getWaypoints().size() == other->getWaypoints().size() &&
				getCurrentWaypointIndex() == other->getCurrentWaypointIndex())
			{
				for (int i = 0; i < getWaypoints().size(); i++)
				{
					if (!(getWaypoints()[i]->Equals(other->getWaypoints()[i])))
					{
						return false;
					}
				}
				return true;
			}
		}
		return false;
	}

	bool WaypointDataMessage::operator == (const WaypointDataMessage &message2)
	{
		return this->Equals(std::make_shared<WaypointDataMessage>(message2));
	}

	bool WaypointDataMessage::operator != (const WaypointDataMessage &message2)
	{
		return !(*this == message2);
	}
}
