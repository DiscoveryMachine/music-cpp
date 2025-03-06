/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICResponseMessage.h"

using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{
	const std::wstring MUSICResponseMessage::STATUS_KEY = L"status";

	MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus MUSICResponseMessage::getRequestStatus() const
	{
		return RequestStatus;
	}

	void MUSICResponseMessage::setRequestStatus(MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus value)
	{
		RequestStatus = value;
	}

	MUSICResponseMessage::MUSICResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
		setRequestStatus(status);
	}

	MUSICResponseMessage::MUSICResponseMessage()
	{
	}

	MUSICResponseMessage::MUSICResponseMessage(std::shared_ptr<object> jobj) : MUSICRequestMessage(jobj)
	{
		object objVal = *jobj;

		setRequestStatus(static_cast<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus>(value_to<int>(objVal[StringHelper::wstringToString(STATUS_KEY)])));
	}

	MUSICResponseMessage::MUSICResponseMessage(std::shared_ptr<MUSICRequestMessage> msg) : MUSICRequestMessage(msg->getMUSICHeader()->getExerciseID(), msg->getReceiverID(), msg->getOriginID(), msg->getRequestID())
	{
	}

	MUSICResponseMessage::MUSICResponseMessage(std::shared_ptr<MUSICRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus requestStatus) : MUSICResponseMessage(msg)
	{
		setRequestStatus(requestStatus);
	}

	bool MUSICResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<MUSICResponseMessage>))
		{
			std::shared_ptr<MUSICResponseMessage> other = std::any_cast<std::shared_ptr<MUSICResponseMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> parent = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID());

			if(MUSICRequestMessage::Equals(parent) &&
				getRequestStatus() == other->getRequestStatus())
			{
				return true;
			}
		}
		return false;
	}

	int MUSICResponseMessage::GetHashCode() const
	{
		auto hashCode = -1250670558;
		hashCode = hashCode * -1521134295 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		hashCode = hashCode * -1521134295 + ((getRequestStatus() != MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus::Other) ? 
			std::hash<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus>{}(getRequestStatus()) : 0);
		return hashCode;
	}

	size_t MUSICResponseMessage::HashFunction::operator()(const MUSICResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool MUSICResponseMessage::operator == (const MUSICResponseMessage &message2)
	{
		return this->Equals(std::make_shared<MUSICResponseMessage>(message2));
	}

	bool MUSICResponseMessage::operator != (const MUSICResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
