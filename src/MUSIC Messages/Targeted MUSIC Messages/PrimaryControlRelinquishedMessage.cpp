/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PrimaryControlRelinquishedMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	PrimaryControlRelinquishedMessage::PrimaryControlRelinquishedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
	}

	PrimaryControlRelinquishedMessage::PrimaryControlRelinquishedMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void PrimaryControlRelinquishedMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitPrimaryControlRelinquished(shared_from_this());
	}

	std::any PrimaryControlRelinquishedMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<PrimaryControlRelinquishedMessage> clonedMessage = std::make_shared<PrimaryControlRelinquishedMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	std::shared_ptr<object> PrimaryControlRelinquishedMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool PrimaryControlRelinquishedMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<PrimaryControlRelinquishedMessage>))
		{
			std::shared_ptr<PrimaryControlRelinquishedMessage> other = std::any_cast<std::shared_ptr<PrimaryControlRelinquishedMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int PrimaryControlRelinquishedMessage::GetHashCode() const
	{
		auto hashCode = 122224550;
		hashCode = hashCode * -1127134295 + TargetedMUSICMessage::GetHashCode();
		return hashCode;
	}

	size_t PrimaryControlRelinquishedMessage::HashFunction::operator()(const PrimaryControlRelinquishedMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool PrimaryControlRelinquishedMessage::operator == (const PrimaryControlRelinquishedMessage &message2)
	{
		return this->Equals(std::make_shared<PrimaryControlRelinquishedMessage>(message2));
	}

	bool PrimaryControlRelinquishedMessage::operator != (const PrimaryControlRelinquishedMessage &message2)
	{
		return !(*this == message2);
	}
}
