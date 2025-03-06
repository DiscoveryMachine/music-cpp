/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "SetSimulationTimeMessage.h"

using namespace MUSICLibrary::Interfaces;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	unsigned int SetSimulationTimeMessage::getNewTime() const
	{
		return NewTime;
	}

	void SetSimulationTimeMessage::setNewTime(unsigned int value)
	{
		NewTime = value;
	}

	SetSimulationTimeMessage::SetSimulationTimeMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, unsigned int newTime) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
		this->setNewTime(newTime);
	}

	SetSimulationTimeMessage::SetSimulationTimeMessage(std::shared_ptr<boost::json::object> obj) : TargetedMUSICMessage(obj)
	{
		boost::json::object objVal = *obj;
		setNewTime(boost::json::value_to<unsigned int>(objVal["newTime"]));
	}

	void SetSimulationTimeMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitSetSimulationTime(shared_from_this());
	}

	std::any SetSimulationTimeMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<SetSimulationTimeMessage> clonedMessage = std::make_shared<SetSimulationTimeMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getNewTime());

		return clonedMessage;
	}

	unsigned int SetSimulationTimeMessage::GetCommandIdentifier()
	{
		return 454009001;
	}

	std::shared_ptr<boost::json::object> SetSimulationTimeMessage::ToJsonObject()
	{
		boost::json::object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["newTime"] = getNewTime();

		std::shared_ptr<boost::json::object> obj = std::make_shared<boost::json::object>(objVal);
		return obj;
	}

	bool SetSimulationTimeMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<SetSimulationTimeMessage>))
		{
			std::shared_ptr<SetSimulationTimeMessage> other = std::any_cast<std::shared_ptr<SetSimulationTimeMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent) &&
				getNewTime() == other->getNewTime())
			{
				return true;
			}
		}

		return false;
	}

	int SetSimulationTimeMessage::GetHashCode() const
	{
		auto hashCode = 2047867454;
		hashCode = hashCode * -1521134295 + TargetedMUSICMessage::GetHashCode();
		hashCode = hashCode * -1521134295 + ((getNewTime() != NULL) ? std::hash<unsigned int>{}(getNewTime()) : 0);
		return hashCode;
	}

	size_t SetSimulationTimeMessage::HashFunction::operator()(const SetSimulationTimeMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool SetSimulationTimeMessage::operator == (const SetSimulationTimeMessage &message2)
	{
		return this->Equals(std::make_shared<SetSimulationTimeMessage>(message2));
	}

	bool SetSimulationTimeMessage::operator != (const SetSimulationTimeMessage &message2)
	{
		return !(*this == message2);
	}
}
