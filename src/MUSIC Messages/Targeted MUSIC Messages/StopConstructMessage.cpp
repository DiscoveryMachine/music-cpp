/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "StopConstructMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	StopConstructMessage::StopConstructMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
	}

	StopConstructMessage::StopConstructMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void StopConstructMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitStopConstruct(shared_from_this());
	}

	std::any StopConstructMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<StopConstructMessage> clonedMessage = std::make_shared<StopConstructMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	unsigned int StopConstructMessage::GetCommandIdentifier()
	{
		return 454007001;
	}

	std::shared_ptr<object> StopConstructMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());

		return std::make_shared<object>(objVal);

	}

	bool StopConstructMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<StopConstructMessage>))
		{
			std::shared_ptr<StopConstructMessage> other = std::any_cast<std::shared_ptr<StopConstructMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());

			if (TargetedMUSICMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int StopConstructMessage::GetHashCode() const
	{
		auto hashCode = 12222687;
		hashCode = hashCode * -1127555295 + TargetedMUSICMessage::GetHashCode();
		return hashCode;
	}

	size_t StopConstructMessage::HashFunction::operator()(const StopConstructMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool StopConstructMessage::operator == (const StopConstructMessage &message2)
	{
		return this->Equals(std::make_shared<StopConstructMessage>(message2));
	}

	bool StopConstructMessage::operator != (const StopConstructMessage &message2)
	{
		return !(*this == message2);
	}
}
