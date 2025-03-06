/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlLostMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	ControlLostMessage::ControlLostMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID) : TargetedMUSICMessage(exerciseID, originID, receiverID)
	{
	}

	ControlLostMessage::ControlLostMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void ControlLostMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlLost(shared_from_this());
	}

	std::any ControlLostMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<ControlLostMessage> clonedMessage = std::make_shared<ControlLostMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	std::shared_ptr<object> ControlLostMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool ControlLostMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlLostMessage>))
		{
			std::shared_ptr<ControlLostMessage> other = std::any_cast<std::shared_ptr<ControlLostMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ControlLostMessage::GetHashCode() const
	{
		auto hashCode = 129994559;
		hashCode = hashCode * -1417194105 + TargetedMUSICMessage::GetHashCode();
		return hashCode;
	}

	size_t ControlLostMessage::HashFunction::operator()(const ControlLostMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlLostMessage::operator == (const ControlLostMessage &message2)
	{
		return this->Equals(std::make_shared<ControlLostMessage>(message2));
	}

	bool ControlLostMessage::operator != (const ControlLostMessage &message2)
	{
		return !(*this == message2);
	}
}
