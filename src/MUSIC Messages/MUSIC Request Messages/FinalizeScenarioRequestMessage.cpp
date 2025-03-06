/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FinalizeScenarioRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	FinalizeScenarioRequestMessage::FinalizeScenarioRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{

	}

	FinalizeScenarioRequestMessage::FinalizeScenarioRequestMessage(std::shared_ptr<object> jobj) : MUSICRequestMessage(jobj)
	{

	}

	void FinalizeScenarioRequestMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitFinalizeScenarioRequest(shared_from_this());
	}

	std::any FinalizeScenarioRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());

		std::shared_ptr<FinalizeScenarioRequestMessage> clonedMessage = std::make_shared<FinalizeScenarioRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID());

		return clonedMessage;
	}

	unsigned int FinalizeScenarioRequestMessage::GetCommandIdentifier()
	{
		return 454013006;
	}

	std::shared_ptr<object> FinalizeScenarioRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(this->getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(this->getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(this->getReceiverID()->ToJsonObject());
		objVal["requestID"] = this->getRequestID();
		
		return std::make_shared<object>(objVal);
	}

	bool FinalizeScenarioRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<FinalizeScenarioRequestMessage>))
		{
			std::shared_ptr<FinalizeScenarioRequestMessage> other = std::any_cast<std::shared_ptr<FinalizeScenarioRequestMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> request = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(),
																								 other->getReceiverID(), other->getRequestID());

			if (MUSICRequestMessage::Equals(request))
			{
				return true;
			}
		}

		return false;
	}

	int FinalizeScenarioRequestMessage::GetHashCode() const
	{
		auto hashCode = 440219460;
		hashCode = hashCode * -1001134695 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1001134695 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1001134695 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1001134695 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		return hashCode;
	}

	size_t FinalizeScenarioRequestMessage::HashFunction::operator()(const FinalizeScenarioRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool FinalizeScenarioRequestMessage::operator == (const FinalizeScenarioRequestMessage &message2)
	{
		return this->Equals(std::make_shared<FinalizeScenarioRequestMessage>(message2));
	}

	bool FinalizeScenarioRequestMessage::operator != (const FinalizeScenarioRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
