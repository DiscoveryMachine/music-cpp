/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlRegainedMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	ControlRegainedMessage::ControlRegainedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID) : TargetedMUSICMessage(exerciseID, originID, receiverID)
	{

	}

	ControlRegainedMessage::ControlRegainedMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void ControlRegainedMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlRegained(shared_from_this());
	}

	std::any ControlRegainedMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<ControlRegainedMessage> clonedMessage = std::make_shared<ControlRegainedMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	std::shared_ptr<object> ControlRegainedMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool ControlRegainedMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlRegainedMessage>))
		{
			std::shared_ptr<ControlRegainedMessage> other = std::any_cast<std::shared_ptr<ControlRegainedMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ControlRegainedMessage::GetHashCode() const
	{
		auto hashCode = 142424450;
		hashCode = hashCode * -1127144299 + TargetedMUSICMessage::GetHashCode();
		return hashCode;
	}

	size_t ControlRegainedMessage::HashFunction::operator()(const ControlRegainedMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlRegainedMessage::operator == (const ControlRegainedMessage &message2)
	{
		return this->Equals(std::make_shared<ControlRegainedMessage>(message2));
	}

	bool ControlRegainedMessage::operator != (const ControlRegainedMessage &message2)
	{
		return !(*this == message2);
	}
}
