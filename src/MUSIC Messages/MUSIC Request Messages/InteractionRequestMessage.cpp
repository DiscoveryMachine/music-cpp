/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "InteractionRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	std::wstring InteractionRequestMessage::getInteractionName() const
	{
		return InteractionName;
	}

	void InteractionRequestMessage::setInteractionName(const std::wstring &value)
	{
		InteractionName = value;
	}

	MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType InteractionRequestMessage::getInteractionType() const
	{
		return InteractionType;
	}

	void InteractionRequestMessage::setInteractionType(MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType value)
	{
		InteractionType = value;
	}

	std::shared_ptr<object> InteractionRequestMessage::getInteractionData() const
	{
		return InteractionData;
	}

	void InteractionRequestMessage::setInteractionData(std::shared_ptr<object> value)
	{
		InteractionData = value;
	}

	InteractionRequestMessage::InteractionRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, const std::wstring &interactionName, MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType interactionType, std::shared_ptr<object> interactionData) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
		setInteractionName(interactionName);
		setInteractionType(interactionType);
		setInteractionData(interactionData);
	}

	InteractionRequestMessage::InteractionRequestMessage(std::shared_ptr<object> jobj) : MUSICRequestMessage(jobj)
	{
		object objVal = *jobj;

		setInteractionName(StringHelper::stringToWString(objVal["interactionName"].as_string()));
		setInteractionType(static_cast<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType>(value_to<int>(objVal["interactionType"])));
		setInteractionData(std::make_shared<object>(objVal["interactionData"].as_object()));
	}

	void InteractionRequestMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitInteractionRequest(shared_from_this());
	}

	std::any InteractionRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::wstring clonedInteractionName = std::wstring(getInteractionName());
		std::shared_ptr<object> clonedInteractionData = std::make_shared<object>(object(*getInteractionData()));
		std::shared_ptr<InteractionRequestMessage> clonedMessage = std::make_shared<InteractionRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedInteractionName, getInteractionType(), clonedInteractionData);

		return clonedMessage;
	}

	std::shared_ptr<object> InteractionRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["interactionName"] = StringHelper::wstringToString(getInteractionName());
		objVal["interactionType"] = static_cast<int>(getInteractionType());
		objVal["interactionData"] = *getInteractionData();

		return std::make_shared<object>(objVal);
	}

	bool InteractionRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<InteractionRequestMessage>))
		{
			std::shared_ptr<InteractionRequestMessage> other = std::any_cast<std::shared_ptr<InteractionRequestMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> request = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(),
																								 other->getReceiverID(), other->getRequestID());

			if (MUSICRequestMessage::Equals(request) &&
				getInteractionName() == other->getInteractionName() &&
				getInteractionType() == other->getInteractionType())
			{
				// If both have no interaction data, return true
				if (getInteractionData() == nullptr && other->getInteractionData() == nullptr)
				{
					return true;
				}
				// If at most one has no interaction data, return false
				else if ((getInteractionData() != nullptr && other->getInteractionData() == nullptr) ||
						 (getInteractionData() == nullptr && other->getInteractionData() != nullptr))
				{
					return false;
				}
				// If both have interaction data and contents are the same, return true
				else if (serialize(*getInteractionData()).compare(serialize(*other->getInteractionData())) == 0)
				{
					return true;
				}
			}
		}

		return false;
	}

	int InteractionRequestMessage::GetHashCode() const
	{
		auto hashCode = 848132228;
		hashCode = hashCode * -1521134295 + MUSICRequestMessage::GetHashCode();
		hashCode = hashCode * -1521134295 + ((!getInteractionName().empty()) ? std::hash<std::wstring>{}(getInteractionName()) : 0);
		hashCode = hashCode * -1521134295 + std::hash<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionType>{}(getInteractionType());
		hashCode = hashCode * -1521134295 + stdext::hash_value(serialize(*getInteractionData()));
		return hashCode;
	}

	size_t InteractionRequestMessage::HashFunction::operator()(const InteractionRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool InteractionRequestMessage::operator == (const InteractionRequestMessage &message2)
	{
		return this->Equals(std::make_shared<InteractionRequestMessage>(message2));
	}

	bool InteractionRequestMessage::operator != (const InteractionRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
