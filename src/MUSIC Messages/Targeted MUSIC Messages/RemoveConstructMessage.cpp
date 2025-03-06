/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "RemoveConstructMessage.h"

using namespace boost::json;
using namespace MUSICLibrary::Interfaces;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	std::shared_ptr<EntityIDRecord> RemoveConstructMessage::getRemovedConstruct() const
	{
		return RemovedConstruct;
	}

	void RemoveConstructMessage::setRemovedConstruct(std::shared_ptr<EntityIDRecord> value)
	{
		RemovedConstruct = value;
	}

	RemoveConstructMessage::RemoveConstructMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, std::shared_ptr<EntityIDRecord> removeConstruct) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
		this->setRemovedConstruct(removeConstruct);
	}

	RemoveConstructMessage::RemoveConstructMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
		setRemovedConstruct(extractEntityIDRecordFromJson(*obj, "removedConstruct"));
	}

	void RemoveConstructMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitRemoveConstruct(shared_from_this());
	}

	std::any RemoveConstructMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedRemovedConstruct = std::make_shared<EntityIDRecord>(getRemovedConstruct()->getSiteID(), getRemovedConstruct()->getAppID(), getRemovedConstruct()->getEntityID());
		std::shared_ptr<RemoveConstructMessage> clonedMessage = std::make_shared<RemoveConstructMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, clonedRemovedConstruct);

		return clonedMessage;
	}

	unsigned int RemoveConstructMessage::GetCommandIdentifier()
	{
		return 454007002;
	}

	std::shared_ptr<object> RemoveConstructMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["removedConstruct"] = *(getRemovedConstruct()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool RemoveConstructMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<RemoveConstructMessage>))
		{
			std::shared_ptr<RemoveConstructMessage> other = std::any_cast<std::shared_ptr<RemoveConstructMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent) && (*getRemovedConstruct()) == (*other->getRemovedConstruct()))
			{
				return true;
			}
		}

		return false;
	}

	int RemoveConstructMessage::GetHashCode() const
	{
		auto hashCode = -1819630550;
		hashCode = hashCode * -1521134295 + TargetedMUSICMessage::GetHashCode();
		hashCode = hashCode * -1521134295 + ((getRemovedConstruct() != nullptr) ? getRemovedConstruct()->GetHashCode() : 0);
		return hashCode;
	}

	size_t RemoveConstructMessage::HashFunction::operator()(const RemoveConstructMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool RemoveConstructMessage::operator == (const RemoveConstructMessage &message2)
	{
		return this->Equals(std::make_shared<RemoveConstructMessage>(message2));
	}

	bool RemoveConstructMessage::operator != (const RemoveConstructMessage &message2)
	{
		return !(*this == message2);
	}
}
