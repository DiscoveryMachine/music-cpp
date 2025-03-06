/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICRequestMessage.h"

using namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	const std::wstring MUSICRequestMessage::REQUEST_ID_KEY = L"requestID";

	unsigned int MUSICRequestMessage::getRequestID() const
	{
		return RequestID;
	}

	void MUSICRequestMessage::setRequestID(unsigned int value)
	{
		RequestID = value;
	}

	MUSICRequestMessage::MUSICRequestMessage()
	{
	}

	MUSICRequestMessage::MUSICRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID) : TargetedMUSICMessage(exerciseID, originID, receiverID)
	{
		setRequestID(requestID);
	}

	MUSICRequestMessage::MUSICRequestMessage(std::shared_ptr<object> jobj) : TargetedMUSICMessage(jobj)
	{
		object objVal = *jobj;

		setRequestID(value_to<unsigned int>(objVal[StringHelper::wstringToString(REQUEST_ID_KEY)]));
	}

	bool MUSICRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<MUSICRequestMessage>))
		{
			std::shared_ptr<MUSICRequestMessage> other = std::any_cast<std::shared_ptr<MUSICRequestMessage>>(obj);
			std::shared_ptr<TargetedMUSICMessage> targeted = std::make_shared<TargetedMUSICMessage>(other->getMUSICHeader(), other->getOriginID(), other->getReceiverID());

			if (TargetedMUSICMessage::Equals(targeted) && 
				getRequestID() == other->getRequestID())
			{
				return true;
			}
		}

		return false;
	}

	int MUSICRequestMessage::GetHashCode() const
	{
		auto hashCode = 1500581749;
		hashCode = hashCode * -1331134295 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1331134295 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1331134295 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1331134295 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		return hashCode;
	}

	std::any MUSICRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<MUSICRequestMessage> clonedMessage = std::make_shared<MUSICRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, this->getRequestID());

		return clonedMessage;
	}

	std::shared_ptr<object> MUSICRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = this->getRequestID();
		std::shared_ptr<object> obj = std::make_shared<object>(objVal);
		return obj;
	}

	size_t MUSICRequestMessage::HashFunction::operator()(const MUSICRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool MUSICRequestMessage::operator == (const MUSICRequestMessage &message2)
	{
		return this->Equals(std::make_shared<MUSICRequestMessage>(message2));
	}

	bool MUSICRequestMessage::operator != (const MUSICRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
