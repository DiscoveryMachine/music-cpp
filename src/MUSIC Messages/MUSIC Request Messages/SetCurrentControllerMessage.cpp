/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "SetCurrentControllerMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{

	std::shared_ptr<EntityIDRecord> SetCurrentControllerMessage::getCurrentController() const
	{
		return CurrentController;
	}

	void SetCurrentControllerMessage::setCurrentController(std::shared_ptr<EntityIDRecord> value)
	{
		CurrentController = value;
	}

	SetCurrentControllerMessage::SetCurrentControllerMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, unsigned int requestID, std::shared_ptr<EntityIDRecord> currentController) : MUSICRequestMessage(exerciseID, originID, recieverID, requestID)
	{
		this->setCurrentController(currentController);
	}

	SetCurrentControllerMessage::SetCurrentControllerMessage(std::shared_ptr<object> obj) : MUSICRequestMessage(obj)
	{
		setCurrentController(extractEntityIDRecordFromJson(*obj, "currentControllerID"));
	}

	void SetCurrentControllerMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitSetCurrentController(shared_from_this());
	}

	std::any SetCurrentControllerMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedCurrentController = std::make_shared<EntityIDRecord>(getCurrentController()->getSiteID(), getCurrentController()->getAppID(), getCurrentController()->getEntityID());
		std::shared_ptr<SetCurrentControllerMessage> clonedMessage = std::make_shared<SetCurrentControllerMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedCurrentController);

		return clonedMessage;
	}

	std::shared_ptr<object> SetCurrentControllerMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["currentControllerID"] = *(getCurrentController()->ToJsonObject());

		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	bool SetCurrentControllerMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<SetCurrentControllerMessage>))
		{
			std::shared_ptr<SetCurrentControllerMessage> other = std::any_cast<std::shared_ptr<SetCurrentControllerMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> parent = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID(), other->getRequestID());
			if (MUSICRequestMessage::Equals(parent) &&
				getCurrentController()->Equals(other->getCurrentController()))
			{
				return true;
			}
		}

		return false;
	}

	int SetCurrentControllerMessage::GetHashCode() const
	{
		return 2123520178 + MUSICRequestMessage::GetHashCode() +
			((getCurrentController() != nullptr) ? getCurrentController()->GetHashCode() : 0);
	}

	size_t SetCurrentControllerMessage::HashFunction::operator()(const SetCurrentControllerMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool SetCurrentControllerMessage::operator == (const SetCurrentControllerMessage &message2)
	{
		return this->Equals(std::make_shared<SetCurrentControllerMessage>(message2));
	}

	bool SetCurrentControllerMessage::operator != (const SetCurrentControllerMessage &message2)
	{
		return !(*this == message2);
	}
}
