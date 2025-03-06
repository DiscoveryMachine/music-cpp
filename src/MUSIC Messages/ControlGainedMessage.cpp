/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlGainedMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	std::shared_ptr<EntityIDRecord> ControlGainedMessage::getReceiver() const
	{
		return receiver;
	}

	void ControlGainedMessage::setReceiver(std::shared_ptr<EntityIDRecord> value)
	{
		receiver = value;
	}

	std::shared_ptr<EntityIDRecord> ControlGainedMessage::getGainedControlOf() const
	{
		return gainedControlOf;
	}

	void ControlGainedMessage::setGainedControlOf(std::shared_ptr<EntityIDRecord> value)
	{
		gainedControlOf = value;
	}

	ControlGainedMessage::ControlGainedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiver, std::shared_ptr<EntityIDRecord> gainedControlOf) : MUSICMessage(exerciseID, originID)
	{
		this->setReceiver(receiver);
		this->setGainedControlOf(gainedControlOf);
	}

	ControlGainedMessage::ControlGainedMessage(std::shared_ptr<object> obj) : MUSICMessage((*obj)["header"].as_object(), (*obj)["originID"].as_object())
	{
		object objVal = *obj;

		setReceiver(extractEntityIDRecordFromJson(objVal, "receiverID"));
		setGainedControlOf(extractEntityIDRecordFromJson(objVal, "gainedControlOf"));
	}

	std::shared_ptr<object> ControlGainedMessage::ToJsonObject()
	{
		object obj;

		obj["header"] = *(getMUSICHeader()->ToJsonObject());
		obj["originID"] = *(getOriginID()->ToJsonObject());
		obj["receiverID"] = *(getReceiver()->ToJsonObject());
		obj["gainedControlOf"] = *(getGainedControlOf()->ToJsonObject());

		return std::make_shared<object>(obj);
	}

	void ControlGainedMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlGained(shared_from_this());
	}

	std::any ControlGainedMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> originClone = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> receiverClone = std::make_shared<EntityIDRecord>(getReceiver()->getSiteID(), getReceiver()->getAppID(), getReceiver()->getEntityID());
		std::shared_ptr<EntityIDRecord> gainedControlOfClone = std::make_shared<EntityIDRecord>(getGainedControlOf()->getSiteID(), getGainedControlOf()->getAppID(), getGainedControlOf()->getEntityID());
		std::shared_ptr<ControlGainedMessage> message = std::make_shared<ControlGainedMessage>(getMUSICHeader()->getExerciseID(), originClone, receiverClone, gainedControlOfClone);

		return message;
	}

	bool ControlGainedMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlGainedMessage>))
		{
			std::shared_ptr<ControlGainedMessage> other = std::any_cast<std::shared_ptr<ControlGainedMessage>>(obj);

			std::shared_ptr<MUSICMessage> parent = std::make_shared<MUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID());

			if (MUSICMessage::Equals(parent) &&
				getReceiver()->Equals(other->getReceiver()) && 
				getGainedControlOf()->Equals(other->getGainedControlOf()))
			{
				return true;
			}
		}

		return false;
	}

	int ControlGainedMessage::GetHashCode() const
	{
		auto hashCode = 272905447;
		hashCode = hashCode * -1521134210 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134210 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134210 + ((getReceiver() != nullptr) ? getReceiver()->GetHashCode() : 0);
		hashCode = hashCode * -1521134210 + ((getGainedControlOf() != nullptr) ? getGainedControlOf()->GetHashCode() : 0);
		return hashCode;
	}

	size_t ControlGainedMessage::HashFunction::operator()(const ControlGainedMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlGainedMessage::operator == (const ControlGainedMessage &message2)
	{
		return this->Equals(std::make_shared<ControlGainedMessage>(message2));
	}

	bool ControlGainedMessage::operator != (const ControlGainedMessage &message2)
	{
		return !(*this == message2);
	}
}
