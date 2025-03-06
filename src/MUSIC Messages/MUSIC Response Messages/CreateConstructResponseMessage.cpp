/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "CreateConstructResponseMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	std::shared_ptr<EntityIDRecord> CreateConstructResponseMessage::getConstructID() const
	{
		return ConstructID;
	}

	void CreateConstructResponseMessage::setConstructID(std::shared_ptr<EntityIDRecord> value)
	{
		ConstructID = value;
	}

	CreateConstructResponseMessage::CreateConstructResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, std::shared_ptr<EntityIDRecord> constructID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(exerciseID, originID, receiverID, requestID, status)
	{
		this->setConstructID(constructID);
	}

	CreateConstructResponseMessage::CreateConstructResponseMessage(std::shared_ptr<object> jobj) : MUSICResponseMessage(jobj)
	{
		setConstructID(extractEntityIDRecordFromJson(*jobj, "originID"));
	}

	CreateConstructResponseMessage::CreateConstructResponseMessage(std::shared_ptr<CreateConstructRequestMessage> msg, std::shared_ptr<EntityIDRecord> constructID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(msg, status)
	{
		this->setConstructID(constructID);
	}

	void CreateConstructResponseMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitCreateConstructResponse(shared_from_this());
	}

	std::any CreateConstructResponseMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedConstructID = std::make_shared<EntityIDRecord>(getConstructID()->getSiteID(), getConstructID()->getAppID(), getConstructID()->getEntityID());

		std::shared_ptr<CreateConstructResponseMessage> clonedMessage = std::make_shared<CreateConstructResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedConstructID, getRequestStatus());

		clonedMessage->setRequestStatus(getRequestStatus());

		return clonedMessage;
	}

	unsigned int CreateConstructResponseMessage::GetCommandIdentifier()
	{
		return 454013003;
	}

	std::shared_ptr<object> CreateConstructResponseMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["status"] = static_cast<int>(getRequestStatus());
		objVal["originID"] = *(getConstructID()->ToJsonObject());

		return std::make_shared<object>(objVal);
	}

	bool CreateConstructResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<CreateConstructResponseMessage>))
		{
			std::shared_ptr<CreateConstructResponseMessage> other = std::any_cast<std::shared_ptr<CreateConstructResponseMessage>>(obj);
			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());

			if (MUSICResponseMessage::Equals(parent) &&
				getConstructID()->Equals(other->getConstructID()))
			{
				return true;
			}
		}

		return false;
	}

	int CreateConstructResponseMessage::GetHashCode() const
	{
		auto hashCode = -99675531;
		hashCode = hashCode * -1521134295 + MUSICResponseMessage::GetHashCode();
		hashCode = hashCode * -1521134295 + ((getConstructID() != nullptr) ? getConstructID()->GetHashCode() : 0);
		return hashCode;
	}

	size_t CreateConstructResponseMessage::HashFunction::operator()(const CreateConstructResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool CreateConstructResponseMessage::operator == (const CreateConstructResponseMessage &message2)
	{
		return this->Equals(std::make_shared<CreateConstructResponseMessage>(message2));
	}

	bool CreateConstructResponseMessage::operator != (const CreateConstructResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
