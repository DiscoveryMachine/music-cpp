/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "CreateEnvironmentResponseMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	CreateEnvironmentResponseMessage::CreateEnvironmentResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(exerciseID, originID, receiverID, requestID, status)
	{
	}

	CreateEnvironmentResponseMessage::CreateEnvironmentResponseMessage(std::shared_ptr<object> jobj) : MUSICResponseMessage(jobj)
	{
	}

	CreateEnvironmentResponseMessage::CreateEnvironmentResponseMessage(std::shared_ptr<CreateEnvironmentRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(msg, status)
	{
	}

	void CreateEnvironmentResponseMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitCreateEnvironmentResponse(shared_from_this());
	}

	std::any CreateEnvironmentResponseMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());

		std::shared_ptr<CreateEnvironmentResponseMessage> clonedMessage = std::make_shared<CreateEnvironmentResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus());

		clonedMessage->setRequestStatus(getRequestStatus());


		return clonedMessage;
	}

	unsigned int CreateEnvironmentResponseMessage::GetCommandIdentifier()
	{
		return 454013001;
	}

	std::shared_ptr<object> CreateEnvironmentResponseMessage::ToJsonObject()
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

	bool CreateEnvironmentResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<CreateEnvironmentResponseMessage>))
		{
			std::shared_ptr<CreateEnvironmentResponseMessage> other = std::any_cast<std::shared_ptr<CreateEnvironmentResponseMessage>>(obj);
			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());
			if (MUSICResponseMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int CreateEnvironmentResponseMessage::GetHashCode() const
	{
		auto hashCode = 210212260;
		hashCode = hashCode * -1221134295 + MUSICResponseMessage::GetHashCode();
		return hashCode;
	}

	size_t CreateEnvironmentResponseMessage::HashFunction::operator()(const CreateEnvironmentResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool CreateEnvironmentResponseMessage::operator == (const CreateEnvironmentResponseMessage &message2)
	{
		return this->Equals(std::make_shared<CreateEnvironmentResponseMessage>(message2));
	}

	bool CreateEnvironmentResponseMessage::operator != (const CreateEnvironmentResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
