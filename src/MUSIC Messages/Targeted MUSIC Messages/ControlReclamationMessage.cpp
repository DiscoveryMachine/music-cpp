/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlReclamationMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	ControlReclamationMessage::ControlReclamationMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
	}

	ControlReclamationMessage::ControlReclamationMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void ControlReclamationMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlReclamation(shared_from_this());
	}

	std::any ControlReclamationMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<ControlReclamationMessage> clonedMessage = std::make_shared<ControlReclamationMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	std::shared_ptr<object> ControlReclamationMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool ControlReclamationMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlReclamationMessage>))
		{
			std::shared_ptr<ControlReclamationMessage> other = std::any_cast<std::shared_ptr<ControlReclamationMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ControlReclamationMessage::GetHashCode() const
	{
		/* NOTE that this hashcode seems to be computed differently than the other messages. Those have an additional int
		* that gets multiplied with the TargetMUSICMessage hash code. This one seems to have gotten funky during the C# conversion.
		* For an example of what I'm talking about see the getHasCode methods of ControlLostMessage.cpp and ControlRegainedMessage.cpp.
		* We may want to look into making this one consistent at some point.
		*/
		return 362745333 + TargetedMUSICMessage::GetHashCode();
	}

	size_t ControlReclamationMessage::HashFunction::operator()(const ControlReclamationMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlReclamationMessage::operator == (const ControlReclamationMessage &message2)
	{
		return this->Equals(std::make_shared<ControlReclamationMessage>(message2));
	}

	bool ControlReclamationMessage::operator != (const ControlReclamationMessage &message2)
	{
		return !(*this == message2);
	}
}
