/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ScenarioStartMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{

	ScenarioStartMessage::ScenarioStartMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID) : TargetedMUSICMessage(exerciseID, originID, recieverID)
	{
	}

	ScenarioStartMessage::ScenarioStartMessage(std::shared_ptr<object> obj) : TargetedMUSICMessage(obj)
	{
	}

	void ScenarioStartMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitScenarioStart(shared_from_this());
	}

	std::any ScenarioStartMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<ScenarioStartMessage> clonedMessage = std::make_shared<ScenarioStartMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID);

		return clonedMessage;
	}

	unsigned int ScenarioStartMessage::GetCommandIdentifier()
	{
		return 454013008;
	}

	std::shared_ptr<object> ScenarioStartMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool ScenarioStartMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ScenarioStartMessage>))
		{
			std::shared_ptr<ScenarioStartMessage> other = std::any_cast<std::shared_ptr<ScenarioStartMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> parent = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID());
			if (TargetedMUSICMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int ScenarioStartMessage::GetHashCode() const
	{
		auto hashCode = -122224544;
		hashCode = hashCode * -1127444295 + TargetedMUSICMessage::GetHashCode();
		return hashCode;
	}

	size_t ScenarioStartMessage::HashFunction::operator()(const ScenarioStartMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ScenarioStartMessage::operator == (const ScenarioStartMessage &message2)
	{
		return this->Equals(std::make_shared<ScenarioStartMessage>(message2));
	}

	bool ScenarioStartMessage::operator != (const ScenarioStartMessage &message2)
	{
		return !(*this == message2);
	}
}
