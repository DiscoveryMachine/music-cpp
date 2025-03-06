/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlInitiatedMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	ControlInitiatedMessage::ControlInitiatedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
	}

	ControlInitiatedMessage::ControlInitiatedMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void ControlInitiatedMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlInitiated(shared_from_this());
	}

	std::any ControlInitiatedMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<ControlInitiatedMessage> clonedMessage = std::make_shared<ControlInitiatedMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	unsigned int ControlInitiatedMessage::GetCommandIdentifier()
	{
		return 454999001;
	}

	std::shared_ptr<object> ControlInitiatedMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool ControlInitiatedMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlInitiatedMessage>))
		{
			std::shared_ptr<ControlInitiatedMessage> other = std::any_cast<std::shared_ptr<ControlInitiatedMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ControlInitiatedMessage::GetHashCode() const
	{
		auto hashCode = 122224770;
		hashCode = hashCode * -1137134285 + TargetedMUSICMessage::GetHashCode();
		return hashCode;
	}

	size_t ControlInitiatedMessage::HashFunction::operator()(const ControlInitiatedMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlInitiatedMessage::operator == (const ControlInitiatedMessage &message2)
	{
		return this->Equals(std::make_shared<ControlInitiatedMessage>(message2));
	}

	bool ControlInitiatedMessage::operator != (const ControlInitiatedMessage &message2)
	{
		return !(*this == message2);
	}
}
