/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PrimaryControlRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	PrimaryControlRequestMessage::PrimaryControlRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{

	}

	PrimaryControlRequestMessage::PrimaryControlRequestMessage(std::shared_ptr<object> jobj) : MUSICRequestMessage(jobj)
	{
	}

	void PrimaryControlRequestMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitPrimaryControlRequest(shared_from_this());
	}

	std::any PrimaryControlRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<PrimaryControlRequestMessage> clonedMessage = std::make_shared<PrimaryControlRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID());

		return clonedMessage;
	}

	std::shared_ptr<object> PrimaryControlRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();

		return std::make_shared<object>(objVal);
	}

	bool PrimaryControlRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<PrimaryControlRequestMessage>))
		{
			std::shared_ptr<PrimaryControlRequestMessage> other = std::any_cast<std::shared_ptr<PrimaryControlRequestMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> request = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(),
																								 other->getReceiverID(), other->getRequestID());
			
			if (MUSICRequestMessage::Equals(request))
			{
				return true;
			}
		}

		return false;
	}

	int PrimaryControlRequestMessage::GetHashCode() const
	{
		auto hashCode = 122224500;
		hashCode = hashCode * -1020134295 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1020134295 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1020134295 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1020134295 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		return hashCode;
	}

	size_t PrimaryControlRequestMessage::HashFunction::operator()(const PrimaryControlRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool PrimaryControlRequestMessage::operator == (const PrimaryControlRequestMessage &message2)
	{
		return this->Equals(std::make_shared<PrimaryControlRequestMessage>(message2));
	}

	bool PrimaryControlRequestMessage::operator != (const PrimaryControlRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
