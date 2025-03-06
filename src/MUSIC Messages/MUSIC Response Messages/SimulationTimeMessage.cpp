/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "SimulationTimeMessage.h"


using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	unsigned int SimulationTimeMessage::getSimTime() const
	{
		return SimTime;
	}

	void SimulationTimeMessage::setSimTime(unsigned int value)
	{
		SimTime = value;
	}

	SimulationTimeMessage::SimulationTimeMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, unsigned int requestID,
		MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, unsigned int simTime) : MUSICResponseMessage(exerciseID, originID, recieverID, requestID, status)
	{
		this->setSimTime(simTime);
	}

	SimulationTimeMessage::SimulationTimeMessage(std::shared_ptr<object> obj) : MUSICResponseMessage(obj)
	{
		object objVal = *obj;
		setSimTime(value_to<unsigned int>(objVal["simTime"]));
	}

	SimulationTimeMessage::SimulationTimeMessage(std::shared_ptr<RequestSimulationTimeMessage> msg,
		MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, unsigned int simTime) : MUSICResponseMessage(msg, status)
	{
		this->setSimTime(simTime);
	}

	void SimulationTimeMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitSimulationTime(shared_from_this());
	}

	std::any SimulationTimeMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<SimulationTimeMessage> clonedMessage = std::make_shared<SimulationTimeMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus(), getSimTime());

		return clonedMessage;
	}

	unsigned int SimulationTimeMessage::GetCommandIdentifier()
	{
		return 454009999;
	}

	std::shared_ptr<object> SimulationTimeMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["status"] = static_cast<int>(getRequestStatus());
		objVal["simTime"] = getSimTime();

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool SimulationTimeMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<SimulationTimeMessage>))
		{
			std::shared_ptr<SimulationTimeMessage> other = std::any_cast<std::shared_ptr<SimulationTimeMessage>>(obj);
			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());
			if (MUSICResponseMessage::Equals(parent) &&
				getSimTime() == other->getSimTime())
			{
				return true;
			}
		}

		return false;
	}

	int SimulationTimeMessage::GetHashCode() const
	{
		auto hashCode = 1106421067;
		hashCode = hashCode * -1333334277 + MUSICResponseMessage::GetHashCode();
		hashCode = hashCode * -1333334277 + ((getSimTime() != NULL) ? std::hash<unsigned int>{}(getSimTime()) : 0);
		return hashCode;
	}

	size_t SimulationTimeMessage::HashFunction::operator()(const SimulationTimeMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool SimulationTimeMessage::operator == (const SimulationTimeMessage &message2)
	{
		return this->Equals(std::make_shared<SimulationTimeMessage>(message2));
	}

	bool SimulationTimeMessage::operator != (const SimulationTimeMessage &message2)
	{
		return !(*this == message2);
	}
}
