/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlRelinquishedMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	ControlRelinquishedMessage::ControlRelinquishedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
	}

	ControlRelinquishedMessage::ControlRelinquishedMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void ControlRelinquishedMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlRelinquished(shared_from_this());
	}

	std::any ControlRelinquishedMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<ControlRelinquishedMessage> clonedMessage = std::make_shared<ControlRelinquishedMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	std::shared_ptr<object> ControlRelinquishedMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool ControlRelinquishedMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlRelinquishedMessage>))
		{
			std::shared_ptr<ControlRelinquishedMessage> other = std::any_cast<std::shared_ptr<ControlRelinquishedMessage>>(obj);

			if (TargetedMUSICMessage::Equals(other))
			{
				return true;
			}
		}

		return false;
	}

	int ControlRelinquishedMessage::GetHashCode() const
	{
		auto hashCode = 335924550;
		hashCode = hashCode * -1521124292 + TargetedMUSICMessage::GetHashCode();

		return hashCode;
	}

	size_t ControlRelinquishedMessage::HashFunction::operator()(const ControlRelinquishedMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlRelinquishedMessage::operator == (const ControlRelinquishedMessage &message2)
	{
		return this->Equals(std::make_shared<ControlRelinquishedMessage>(message2));
	}

	bool ControlRelinquishedMessage::operator != (const ControlRelinquishedMessage &message2)
	{
		return !(*this == message2);
	}
}
