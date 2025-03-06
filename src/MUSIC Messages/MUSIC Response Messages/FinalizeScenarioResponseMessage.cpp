/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FinalizeScenarioResponseMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages
{

	FinalizeScenarioResponseMessage::FinalizeScenarioResponseMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(exerciseID, originID, receiverID, requestID, status)
	{
	}

	FinalizeScenarioResponseMessage::FinalizeScenarioResponseMessage(std::shared_ptr<object> obj) : MUSICResponseMessage(obj)
	{
	}

	FinalizeScenarioResponseMessage::FinalizeScenarioResponseMessage(std::shared_ptr<FinalizeScenarioRequestMessage> msg, MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::RequestStatus status) : MUSICResponseMessage(msg, status)
	{
	}

	void FinalizeScenarioResponseMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitFinalizeScenarioResponse(shared_from_this());
	}

	std::any FinalizeScenarioResponseMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());

		std::shared_ptr<FinalizeScenarioResponseMessage> clonedMessage = std::make_shared<FinalizeScenarioResponseMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), getRequestStatus());

		return clonedMessage;
	}

	unsigned int FinalizeScenarioResponseMessage::GetCommandIdentifier()
	{
		return 454013007;
	}

	std::shared_ptr<object> FinalizeScenarioResponseMessage::ToJsonObject()
	{
		object objVal;

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["status"] = static_cast<int>(getRequestStatus());

		return std::make_shared<object>(objVal);
	}

	bool FinalizeScenarioResponseMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<FinalizeScenarioResponseMessage>))
		{
			std::shared_ptr<FinalizeScenarioResponseMessage> other = std::any_cast<std::shared_ptr<FinalizeScenarioResponseMessage>>(obj);
			std::shared_ptr<MUSICResponseMessage> parent = std::make_shared<MUSICResponseMessage>(other->getMUSICHeader()->getExerciseID(),
				other->getOriginID(), other->getReceiverID(), other->getRequestID(), other->getRequestStatus());

			if (MUSICResponseMessage::Equals(parent))
			{
				return true;
			}
		}

		return false;
	}

	int FinalizeScenarioResponseMessage::GetHashCode() const
	{
		auto hashCode = 410445160;
		hashCode = hashCode * -1521004291 + MUSICResponseMessage::GetHashCode();
		return hashCode;
	}


	size_t FinalizeScenarioResponseMessage::HashFunction::operator()(const FinalizeScenarioResponseMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool FinalizeScenarioResponseMessage::operator == (const FinalizeScenarioResponseMessage &message2)
	{
		return this->Equals(std::make_shared<FinalizeScenarioResponseMessage>(message2));
	}

	bool FinalizeScenarioResponseMessage::operator != (const FinalizeScenarioResponseMessage &message2)
	{
		return !(*this == message2);
	}
}
