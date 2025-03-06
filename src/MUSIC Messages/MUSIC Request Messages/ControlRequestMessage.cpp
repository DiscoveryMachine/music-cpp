/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	const std::wstring ControlRequestMessage::TARGET_CONSTRUCT = L"targetConstruct";
	const std::wstring ControlRequestMessage::CONTEXT = L"context";

	std::shared_ptr<EntityIDRecord> ControlRequestMessage::getTargetConstruct() const
	{
		return TargetConstruct;
	}

	void ControlRequestMessage::setTargetConstruct(std::shared_ptr<EntityIDRecord> value)
	{
		TargetConstruct = value;
	}

	std::shared_ptr<object> ControlRequestMessage::getContext() const
	{
		return Context;
	}

	void ControlRequestMessage::setContext(std::shared_ptr<object> value)
	{
		Context = value;
	}

	ControlRequestMessage::ControlRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, std::shared_ptr<EntityIDRecord> targetConstruct, std::shared_ptr<object> context) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
		this->setTargetConstruct(targetConstruct);
		this->setContext(context);
	}

	ControlRequestMessage::ControlRequestMessage(std::shared_ptr<object> jobj) : MUSICRequestMessage(jobj)
	{
		object objVal = *jobj;

		setTargetConstruct(extractEntityIDRecordFromJson(objVal, StringHelper::wstringToString(TARGET_CONSTRUCT)));
		setContext(std::make_shared<object>(objVal[StringHelper::wstringToString(CONTEXT)].as_object()));
	}

	void ControlRequestMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlRequest(shared_from_this());
	}

	std::any ControlRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedTargetConstruct = std::make_shared<EntityIDRecord>(getTargetConstruct()->getSiteID(), getTargetConstruct()->getAppID(), getTargetConstruct()->getEntityID());
		std::shared_ptr<object> clonedContext = std::make_shared<object>(object(*getContext()));

		std::shared_ptr<ControlRequestMessage> clonedMessage = std::make_shared<ControlRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedTargetConstruct, clonedContext);

		return clonedMessage;
	}

	std::shared_ptr<object> ControlRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal[StringHelper::wstringToString(TARGET_CONSTRUCT)] = *(getTargetConstruct()->ToJsonObject());
		objVal[StringHelper::wstringToString(CONTEXT)] = *getContext();

		return std::make_shared<object>(objVal);
	}

	bool ControlRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlRequestMessage>))
		{
			std::shared_ptr<ControlRequestMessage> other = std::any_cast<std::shared_ptr<ControlRequestMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> request = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), 
																								 other->getReceiverID(), other->getRequestID());

			if (MUSICRequestMessage::Equals(request) &&
				getTargetConstruct()->Equals(other->getTargetConstruct()) &&
				serialize(*getContext()).compare(serialize(*other->getContext())) == 0)
			{
				return true;
			}
		}

		return false;
	}

	int ControlRequestMessage::GetHashCode() const
	{
		auto hashCode = -326822633;
		hashCode = hashCode * -1521134295 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		hashCode = hashCode * -1521134295 + ((getTargetConstruct() != nullptr) ? getTargetConstruct()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + stdext::hash_value(serialize(*getContext()));
		return hashCode;
	}

	size_t ControlRequestMessage::HashFunction::operator()(const ControlRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlRequestMessage::operator == (const ControlRequestMessage &message2)
	{
		return this->Equals(std::make_shared<ControlRequestMessage>(message2));
	}

	bool ControlRequestMessage::operator != (const ControlRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
