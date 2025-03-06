/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlResponseMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	ControlResponseMessage::ControlResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(exerciseID, originID, receiverID, requestID, status)
	{
	}

	ControlResponseMessage::ControlResponseMessage(std::shared_ptr<object> jobj) : MUSICResponseMessage(jobj)
	{
	}

	ControlResponseMessage::ControlResponseMessage(std::shared_ptr<ControlRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(msg, status)
	{
	}

	void ControlResponseMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlResponse(shared_from_this());
	}

	std::any ControlResponseMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());

		//need to revert switch to switch again properly
		std::shared_ptr<ControlResponseMessage> clonedMessage = std::make_shared<ControlResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus());
		clonedMessage->setRequestStatus(getRequestStatus());

		return clonedMessage;
	}

	std::shared_ptr<object> ControlResponseMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["status"] = static_cast<int>(getRequestStatus());

		return std::make_shared<object>(objVal);
	}

	bool ControlResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlResponseMessage>))
		{
			std::shared_ptr<ControlResponseMessage> other = std::any_cast<std::shared_ptr<ControlResponseMessage>>(obj);
			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());
			if (MUSICResponseMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ControlResponseMessage::GetHashCode() const
	{
		auto hashCode = 410205199;
		hashCode = hashCode * -1527804331 + MUSICResponseMessage::GetHashCode();
		return hashCode;
	}

	size_t ControlResponseMessage::HashFunction::operator()(const ControlResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlResponseMessage::operator == (const ControlResponseMessage &message2)
	{
		return this->Equals(std::make_shared<ControlResponseMessage>(message2));
	}

	bool ControlResponseMessage::operator != (const ControlResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
