/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "RequestSimulationTimeMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{

	RequestSimulationTimeMessage::RequestSimulationTimeMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, 
		unsigned int requestID) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
	}

	RequestSimulationTimeMessage::RequestSimulationTimeMessage(std::shared_ptr<object> obj) : MUSICRequestMessage(obj)
	{
	}

	void RequestSimulationTimeMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitRequestSimulationTime(shared_from_this());
	}

	std::any RequestSimulationTimeMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<RequestSimulationTimeMessage> clonedMessage = std::make_shared<RequestSimulationTimeMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID());

		return clonedMessage;
	}

	unsigned int RequestSimulationTimeMessage::GetCommandIdentifier()
	{
		return 454009000;
	}

	std::shared_ptr<object> RequestSimulationTimeMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool RequestSimulationTimeMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<RequestSimulationTimeMessage>))
		{
			std::shared_ptr<RequestSimulationTimeMessage> other = std::any_cast<std::shared_ptr<RequestSimulationTimeMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> parent = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), 
																								other->getReceiverID(), other->getRequestID());
			if (MUSICRequestMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int RequestSimulationTimeMessage::GetHashCode() const
	{
		auto hashCode = 128224585;
		hashCode = hashCode * -1127134277 + MUSICRequestMessage::GetHashCode();
		return hashCode;
	}

	size_t RequestSimulationTimeMessage::HashFunction::operator()(const RequestSimulationTimeMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool RequestSimulationTimeMessage::operator == (const RequestSimulationTimeMessage &message2)
	{
		return this->Equals(std::make_shared<RequestSimulationTimeMessage>(message2));
	}

	bool RequestSimulationTimeMessage::operator != (const RequestSimulationTimeMessage &message2)
	{
		return !(*this == message2);
	}
}
