/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "InteractionResponseMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	std::shared_ptr<object> InteractionResponseMessage::getOptionalData() const
	{
		return OptionalData;
	}

	void InteractionResponseMessage::setOptionalData(std::shared_ptr<object> value)
	{
		OptionalData = value;
	}

	const std::wstring InteractionResponseMessage::OPTIONAL_DATA_KEY = L"optionalData";

	InteractionResponseMessage::InteractionResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status, std::shared_ptr<object> optionalData) : MUSICResponseMessage(exerciseID, originID, receiverID, requestID, status)
	{
		this->setOptionalData(optionalData);
	}

	InteractionResponseMessage::InteractionResponseMessage(std::shared_ptr<object> jobj) : MUSICResponseMessage(jobj)
	{
		object objVal = *jobj;

		if (objVal.if_contains(StringHelper::wstringToString(OPTIONAL_DATA_KEY)) != nullptr)
		{
			OptionalData = std::make_shared<object>(objVal[StringHelper::wstringToString(OPTIONAL_DATA_KEY)].as_object());
		}

		setRequestStatus(static_cast<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus>(value_to<int>(objVal["status"])));
	}

	InteractionResponseMessage::InteractionResponseMessage(std::shared_ptr<InteractionRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus requestStatus, std::shared_ptr<object> optionalData) : MUSICResponseMessage(msg, requestStatus)
	{
		setOptionalData(optionalData);
		setRequestStatus(requestStatus);
	}

	void InteractionResponseMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitInteractionResponse(shared_from_this());
	}

	std::any InteractionResponseMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());

		if (getOptionalData()!= nullptr)
		{
			std::shared_ptr<object> clonedOptionalData = std::make_shared<object>(*getOptionalData());
			std::shared_ptr<InteractionResponseMessage> clonedMessage = std::make_shared<InteractionResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus(), clonedOptionalData);

			return clonedMessage;
		}

		std::shared_ptr<InteractionResponseMessage> clonedMessages = std::make_shared<InteractionResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus());

		return clonedMessages;
	}

	std::shared_ptr<object> InteractionResponseMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["status"] = static_cast<int>(static_cast<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus>(getRequestStatus()));

		if (getOptionalData() != nullptr)
		{
			objVal["optionalData"] = *getOptionalData();
		}
		return std::make_shared<object>(objVal);
	}

	bool InteractionResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<InteractionResponseMessage>))
		{
			std::shared_ptr<InteractionResponseMessage> other = std::any_cast<std::shared_ptr<InteractionResponseMessage>>(obj);
			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());

			if (MUSICResponseMessage::Equals(parent))
			{
				// If both have no optional data, return true
				if (getOptionalData() == nullptr && other->getOptionalData() == nullptr)
				{
					return true;
				}
				// If at most one has no optional data, return false
				else if ((getOptionalData() != nullptr && other->getOptionalData() == nullptr) ||
						 (getOptionalData() == nullptr && other->getOptionalData() != nullptr))
				{
					return false;
				}
				// If both have optional data and contents are the same, return true
				else if (serialize(*getOptionalData()).compare(serialize(*other->getOptionalData())) == 0)
				{
					return true;
				}
			}
		}
		return false;
	}

	int InteractionResponseMessage::GetHashCode() const
	{
		auto hashCode = 627368575;
		hashCode = hashCode * -1521137895 + MUSICResponseMessage::GetHashCode();

		hashCode = hashCode * -1521137895 + std::hash<std::string>{}(serialize(*getOptionalData()));
		return hashCode;
	}

	size_t InteractionResponseMessage::HashFunction::operator()(const InteractionResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool InteractionResponseMessage::operator == (const InteractionResponseMessage &message2)
	{
		return this->Equals(std::make_shared<InteractionResponseMessage>(message2));
	}

	bool InteractionResponseMessage::operator != (const InteractionResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
