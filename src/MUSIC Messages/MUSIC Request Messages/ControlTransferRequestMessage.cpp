/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ControlTransferRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	std::shared_ptr<EntityIDRecord> ControlTransferRequestMessage::getProposedController() const
	{
		return ProposedController;
	}

	void ControlTransferRequestMessage::setProposedController(std::shared_ptr<EntityIDRecord> value)
	{
		ProposedController = value;
	}

const std::wstring ControlTransferRequestMessage::PROPOSED_CONTROLLER = L"proposedController";

	ControlTransferRequestMessage::ControlTransferRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, std::shared_ptr<EntityIDRecord> targetConstruct, std::shared_ptr<object> context, std::shared_ptr<EntityIDRecord> proposedController) : ControlRequestMessage(exerciseID, originID, receiverID, requestID, targetConstruct, context)
	{
		this->setProposedController(proposedController);
	}

	ControlTransferRequestMessage::ControlTransferRequestMessage(std::shared_ptr<object> jobj) : ControlRequestMessage(jobj)
	{
		setProposedController(extractEntityIDRecordFromJson(*jobj, StringHelper::wstringToString(PROPOSED_CONTROLLER)));
	}

	void ControlTransferRequestMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitControlTransferRequest(shared_from_this());
	}

	std::any ControlTransferRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedTargetConstruct = std::make_shared<EntityIDRecord>(getTargetConstruct()->getSiteID(), getTargetConstruct()->getAppID(), getTargetConstruct()->getEntityID());
		std::shared_ptr<object> clonedContext = std::make_shared<object>(object(*getContext()));
		std::shared_ptr<EntityIDRecord> clonedProposedController = std::make_shared<EntityIDRecord>(getProposedController()->getSiteID(), getProposedController()->getAppID(), getProposedController()->getEntityID());
		std::shared_ptr<ControlTransferRequestMessage> clonedMessage = std::make_shared<ControlTransferRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedTargetConstruct, clonedContext, clonedProposedController);

		return clonedMessage;
	}

	std::shared_ptr<object> ControlTransferRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal["targetConstruct"] = *(getTargetConstruct()->ToJsonObject());
		objVal[StringHelper::wstringToString(PROPOSED_CONTROLLER)] = *(getProposedController()->ToJsonObject());
		objVal["context"] = *getContext();

		return std::make_shared<object>(objVal);
	}
	//unsigned int requestID, std::shared_ptr<EntityIDRecord> targetConstruct, std::shared_ptr<boost::json::object> context
	bool ControlTransferRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ControlTransferRequestMessage>))
		{
			std::shared_ptr<ControlTransferRequestMessage> other = std::any_cast<std::shared_ptr<ControlTransferRequestMessage>>(obj);
			std::shared_ptr<ControlRequestMessage> request = std::make_shared<ControlRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(), other->getReceiverID(),
																									 other->getRequestID(), other->getTargetConstruct(), other->getContext());

			if (ControlRequestMessage::Equals(request) && 
				getProposedController()->Equals( other->getProposedController()))
			{
				return true;
			}
		}

		return false;
	}

	int ControlTransferRequestMessage::GetHashCode() const
	{
		auto hashCode = -500387931;
		hashCode = hashCode * -1521134211 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134211 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134211 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134211 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		hashCode = hashCode * -1521134211 + ((getTargetConstruct() != nullptr) ? getTargetConstruct()->GetHashCode() : 0);
		hashCode = hashCode * -1521134211 + ((getProposedController() != nullptr) ? getProposedController()->GetHashCode() : 0);
		hashCode = hashCode * -1521134211 + stdext::hash_value(serialize(*getContext()));
		return hashCode;
	}

	size_t ControlTransferRequestMessage::HashFunction::operator()(const ControlTransferRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ControlTransferRequestMessage::operator == (const ControlTransferRequestMessage &message2)
	{
		return this->Equals(std::make_shared<ControlTransferRequestMessage>(message2));
	}

	bool ControlTransferRequestMessage::operator != (const ControlTransferRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
