/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "TransferConstructIDMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	std::shared_ptr<EntityIDRecord> TransferConstructIDMessage::getOldID() const
	{
		return OldID;
	}

	void TransferConstructIDMessage::setOldID(std::shared_ptr<EntityIDRecord> value)
	{
		OldID = value;
	}

	std::shared_ptr<EntityIDRecord> TransferConstructIDMessage::getNewID() const
	{
		return NewID;
	}

	void TransferConstructIDMessage::setNewID(std::shared_ptr<EntityIDRecord> value)
	{
		NewID = value;
	}

	TransferConstructIDMessage::TransferConstructIDMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, std::shared_ptr<EntityIDRecord> oldID, std::shared_ptr<EntityIDRecord> newID) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
		this->setOldID(oldID);
		this->setNewID(newID);
	}

	TransferConstructIDMessage::TransferConstructIDMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
		setOldID(extractEntityIDRecordFromJson(*obj, "oldID"));
		setNewID(extractEntityIDRecordFromJson(*obj, "newID"));
	}

	void TransferConstructIDMessage::AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor)
	{
		visitor->VisitTransferConstructID(shared_from_this());
	}

	std::any TransferConstructIDMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedOldID = std::make_shared<EntityIDRecord>(getOldID()->getSiteID(), getOldID()->getAppID(), getOldID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedNewID = std::make_shared<EntityIDRecord>(getNewID()->getSiteID(), getNewID()->getAppID(), getNewID()->getEntityID());
		std::shared_ptr<TransferConstructIDMessage> clonedMessage = std::make_shared<TransferConstructIDMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, clonedOldID, clonedNewID);

		return clonedMessage;
	}

	unsigned int TransferConstructIDMessage::GetCommandIdentifier()
	{
		return 454000002;
	}

	std::shared_ptr<object> TransferConstructIDMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["oldID"] = *(getOldID()->ToJsonObject());
		objVal["newID"] = *(getNewID()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool TransferConstructIDMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<TransferConstructIDMessage>))
		{
			std::shared_ptr<TransferConstructIDMessage> other = std::any_cast<std::shared_ptr<TransferConstructIDMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent) &&
				getOldID()->Equals(other->getOldID()) && 
				getNewID()->Equals(other->getNewID()))
			{
				return true;
			}
		}

		return false;
	}

	int TransferConstructIDMessage::GetHashCode() const
	{
		auto hashCode = 122224770;
		hashCode = hashCode * -1127174295 + TargetedMUSICMessage::GetHashCode();
		hashCode = hashCode * -1127174295 + ((getOldID() != nullptr) ? getOldID()->GetHashCode() : 0);
		hashCode = hashCode * -1127174295 + ((getNewID() != nullptr) ? getNewID()->GetHashCode() : 0);
		return hashCode;
	}

	size_t TransferConstructIDMessage::HashFunction::operator()(const TransferConstructIDMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool TransferConstructIDMessage::operator == (const TransferConstructIDMessage &message2)
	{
		return this->Equals(std::make_shared<TransferConstructIDMessage>(message2));
	}

	bool TransferConstructIDMessage::operator != (const TransferConstructIDMessage &message2)
	{
		return !(*this == message2);
	}
}
