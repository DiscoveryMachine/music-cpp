/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlTransferResponseMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	ControlTransferResponseMessage::ControlTransferResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(exerciseID, originID, receiverID, requestID, status)
	{
	}

	ControlTransferResponseMessage::ControlTransferResponseMessage(std::shared_ptr<object> jobj) : MUSICResponseMessage(jobj)
	{
	}

	ControlTransferResponseMessage::ControlTransferResponseMessage(std::shared_ptr<ControlTransferRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(msg, status)
	{
	}

	void ControlTransferResponseMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlTransferResponse(shared_from_this());
	}

	std::any ControlTransferResponseMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());

		std::shared_ptr<ControlTransferResponseMessage> clonedMessage = std::make_shared<ControlTransferResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus());

		clonedMessage->setRequestStatus(getRequestStatus());

		return clonedMessage;
	}

	std::shared_ptr<object> ControlTransferResponseMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["status"] = static_cast<int>(getRequestStatus());

		return std::make_shared<object>(objVal);
	}

	bool ControlTransferResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlTransferResponseMessage>))
		{
			std::shared_ptr<ControlTransferResponseMessage> other = std::any_cast<std::shared_ptr<ControlTransferResponseMessage>>(obj);

			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());

			if (MUSICResponseMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ControlTransferResponseMessage::GetHashCode() const
	{
		auto hashCode = 500213460;
		hashCode = hashCode * -1521134265 + MUSICResponseMessage::GetHashCode();
		return hashCode;
	}

	size_t ControlTransferResponseMessage::HashFunction::operator()(const ControlTransferResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlTransferResponseMessage::operator == (const ControlTransferResponseMessage &message2)
	{
		return this->Equals(std::make_shared<ControlTransferResponseMessage>(message2));
	}

	bool ControlTransferResponseMessage::operator != (const ControlTransferResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
