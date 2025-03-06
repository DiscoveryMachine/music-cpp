/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlReleasedMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	ControlReleasedMessage::ControlReleasedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
	}

	ControlReleasedMessage::ControlReleasedMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void ControlReleasedMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlReleased(shared_from_this());
	}

	std::any ControlReleasedMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<ControlReleasedMessage> clonedMessage = std::make_shared<ControlReleasedMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	unsigned int ControlReleasedMessage::GetCommandIdentifier()
	{
		return 454999002;
	}

	std::shared_ptr<object> ControlReleasedMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool ControlReleasedMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlReleasedMessage>))
		{
			std::shared_ptr<ControlReleasedMessage> other = std::any_cast<std::shared_ptr<ControlReleasedMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ControlReleasedMessage::GetHashCode() const
	{
		auto hashCode = 122724544;
		hashCode = hashCode * -1127194295 + TargetedMUSICMessage::GetHashCode();
		return hashCode;
	}

	size_t ControlReleasedMessage::HashFunction::operator()(const ControlReleasedMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlReleasedMessage::operator == (const ControlReleasedMessage &message2)
	{
		return this->Equals(std::make_shared<ControlReleasedMessage>(message2));
	}

	bool ControlReleasedMessage::operator != (const ControlReleasedMessage &message2)
	{
		return !(*this == message2);
	}
}
