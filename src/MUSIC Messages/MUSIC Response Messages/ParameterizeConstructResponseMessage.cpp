/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ParameterizeConstructResponseMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	ParameterizeConstructResponseMessage::ParameterizeConstructResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(exerciseID, originID, receiverID, requestID, status)
	{
	}

	ParameterizeConstructResponseMessage::ParameterizeConstructResponseMessage(std::shared_ptr<object> jobj) : MUSICResponseMessage(jobj)
	{
	}

	ParameterizeConstructResponseMessage::ParameterizeConstructResponseMessage(std::shared_ptr<ParameterizeConstructRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(msg, status)
	{
	}

	void ParameterizeConstructResponseMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitParameterizeConstructResponse(shared_from_this());
	}

	std::any ParameterizeConstructResponseMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<ParameterizeConstructResponseMessage> clonedMessage = std::make_shared<ParameterizeConstructResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus());

		clonedMessage->setRequestStatus(getRequestStatus());

		return clonedMessage;
	}

	unsigned int ParameterizeConstructResponseMessage::GetCommandIdentifier()
	{
		return 454013005;
	}

	std::shared_ptr<object> ParameterizeConstructResponseMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["status"] = static_cast<int>(getRequestStatus());

		return std::make_shared<object>(objVal);
	}

	bool ParameterizeConstructResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ParameterizeConstructResponseMessage>))
		{
			std::shared_ptr<ParameterizeConstructResponseMessage> other = std::any_cast<std::shared_ptr<ParameterizeConstructResponseMessage>>(obj);

			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());

			if (MUSICResponseMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ParameterizeConstructResponseMessage::GetHashCode() const
	{
		auto hashCode = 410441230;
		hashCode = hashCode * -1526766591 + MUSICResponseMessage::GetHashCode();
		return hashCode;
	}

	size_t ParameterizeConstructResponseMessage::HashFunction::operator()(const ParameterizeConstructResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ParameterizeConstructResponseMessage::operator == (const ParameterizeConstructResponseMessage &message2)
	{
		return this->Equals(std::make_shared<ParameterizeConstructResponseMessage>(message2));
	}

	bool ParameterizeConstructResponseMessage::operator != (const ParameterizeConstructResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
