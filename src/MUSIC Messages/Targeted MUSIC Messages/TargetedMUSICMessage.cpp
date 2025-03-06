/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "TargetedMUSICMessage.h"

using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	std::shared_ptr<EntityIDRecord> TargetedMUSICMessage::getReceiverID() const
	{
		return ReceiverID;
	}

	void TargetedMUSICMessage::setReceiverID(std::shared_ptr<EntityIDRecord> value)
	{
		ReceiverID = value;
	}

	TargetedMUSICMessage::TargetedMUSICMessage()
	{
	}

	TargetedMUSICMessage::TargetedMUSICMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID) : MUSICMessage(exerciseID, originID)
	{
		this->setReceiverID(recieverID);
	}

	TargetedMUSICMessage::TargetedMUSICMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> header, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID) : MUSICMessage(header, originID)
	{
		this->setReceiverID(receiverID);
	}

	TargetedMUSICMessage::TargetedMUSICMessage(std::shared_ptr<boost::json::object> obj) : MUSICMessage((*obj)["header"].as_object(), (*obj)["originID"].as_object())
	{
		setReceiverID(extractEntityIDRecordFromJson(*obj, "receiverID"));
	}

	bool TargetedMUSICMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<TargetedMUSICMessage>))
		{
			std::shared_ptr<TargetedMUSICMessage> other = std::any_cast<std::shared_ptr<TargetedMUSICMessage>>(obj);
			std::shared_ptr<MUSICMessage> base = std::make_shared<MUSICMessage>(other->getMUSICHeader(), other->getOriginID());

			return MUSICMessage::Equals(base) && getReceiverID()->Equals(other->getReceiverID());
		}
		else
		{
			return false;
		}
	}

	int TargetedMUSICMessage::GetHashCode() const
	{
		auto hashCode = 1839857571;
		hashCode = hashCode * -1521134000 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134000 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134000 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		return hashCode;
	}

	size_t TargetedMUSICMessage::HashFunction::operator()(const TargetedMUSICMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool TargetedMUSICMessage::operator == (const TargetedMUSICMessage &message2)
	{
		return this->Equals(std::make_shared<TargetedMUSICMessage>(message2));
	}

	bool TargetedMUSICMessage::operator != (const TargetedMUSICMessage &message2)
	{
		return !(*this == message2);
	}
}
