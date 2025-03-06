/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PrimaryControlResponseMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	PrimaryControlResponseMessage::PrimaryControlResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(exerciseID, originID, receiverID, requestID, status)
	{
	}

	PrimaryControlResponseMessage::PrimaryControlResponseMessage(std::shared_ptr<object> jobj) : MUSICResponseMessage(jobj)
	{
	}

	PrimaryControlResponseMessage::PrimaryControlResponseMessage(std::shared_ptr<PrimaryControlRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(msg, status)
	{
	}

	void PrimaryControlResponseMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitPrimaryControlResponse(shared_from_this());
	}

	std::any PrimaryControlResponseMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());

		std::shared_ptr<PrimaryControlResponseMessage> clonedMessage = std::make_shared<PrimaryControlResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus());

		return clonedMessage;
	}

	std::shared_ptr<object> PrimaryControlResponseMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["status"] = static_cast<int>(getRequestStatus());

		return std::make_shared<object>(objVal);
	}

	bool PrimaryControlResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<PrimaryControlResponseMessage>))
		{
			std::shared_ptr<PrimaryControlResponseMessage> other = std::any_cast<std::shared_ptr<PrimaryControlResponseMessage>>(obj);

			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());

			if (MUSICResponseMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int PrimaryControlResponseMessage::GetHashCode() const
	{
		auto hashCode = 210445777;
		hashCode = hashCode * -1521137895 + MUSICResponseMessage::GetHashCode();

		return hashCode;
	}

	size_t PrimaryControlResponseMessage::HashFunction::operator()(const PrimaryControlResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool PrimaryControlResponseMessage::operator == (const PrimaryControlResponseMessage &message2)
	{
		return this->Equals(std::make_shared<PrimaryControlResponseMessage>(message2));
	}

	bool PrimaryControlResponseMessage::operator != (const PrimaryControlResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
