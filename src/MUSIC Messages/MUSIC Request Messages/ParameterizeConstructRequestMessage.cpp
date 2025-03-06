/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ParameterizeConstructRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	const std::wstring ParameterizeConstructRequestMessage::GHOSTED_ID = L"ghostedID";
	const std::wstring ParameterizeConstructRequestMessage::CONSTRUCT_PARAMETERS = L"constructParameters";

	std::shared_ptr<EntityIDRecord> ParameterizeConstructRequestMessage::getGhostedID() const
	{
		return GhostedID;
	}

	void ParameterizeConstructRequestMessage::setGhostedID(std::shared_ptr<EntityIDRecord> value)
	{
		GhostedID = value;
	}

	std::shared_ptr<object> ParameterizeConstructRequestMessage::getConstructParameters() const
	{
		return ConstructParameters;
	}

	void ParameterizeConstructRequestMessage::setConstructParameters(std::shared_ptr<object> value)
	{
		ConstructParameters = value;
	}

	ParameterizeConstructRequestMessage::ParameterizeConstructRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, std::shared_ptr<object> constructParameters, std::shared_ptr<EntityIDRecord> ghostedID) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
		this->setGhostedID(ghostedID);
		this->setConstructParameters(constructParameters);
	}

	ParameterizeConstructRequestMessage::ParameterizeConstructRequestMessage(std::shared_ptr<object> jobj) : MUSICRequestMessage(jobj)
	{
		object objVal = *jobj;

		if (objVal.if_contains(StringHelper::wstringToString(GHOSTED_ID)) != nullptr)
		{
			setGhostedID(extractEntityIDRecordFromJson(objVal, StringHelper::wstringToString(GHOSTED_ID)));
		}

		this->setConstructParameters(std::make_shared<object>(objVal[StringHelper::wstringToString(CONSTRUCT_PARAMETERS)].as_object()));
	}

	void ParameterizeConstructRequestMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitParameterizeConstructRequest(shared_from_this());
	}

	std::any ParameterizeConstructRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::shared_ptr<object> clonedConstructParameters = std::make_shared<object>(object(*getConstructParameters()));

		if (getGhostedID() != nullptr)
		{
			std::shared_ptr<EntityIDRecord> clonedGhostedID = std::make_shared<EntityIDRecord>(getGhostedID()->getSiteID(), getGhostedID()->getAppID(), getGhostedID()->getEntityID());
			std::shared_ptr<ParameterizeConstructRequestMessage> clonedMessage = std::make_shared<ParameterizeConstructRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedConstructParameters, clonedGhostedID);

			return clonedMessage;
		}

		std::shared_ptr<ParameterizeConstructRequestMessage> clonedMessages = std::make_shared<ParameterizeConstructRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedConstructParameters);

		return clonedMessages;
	}

	unsigned int ParameterizeConstructRequestMessage::GetCommandIdentifier()
	{
		return 454013004;
	}

	std::shared_ptr<object> ParameterizeConstructRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(this->getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(this->getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(this->getReceiverID()->ToJsonObject());
		objVal["requestID"] = this->getRequestID();

		if (getGhostedID() != nullptr)
		{
			objVal[StringHelper::wstringToString(GHOSTED_ID)] = *(this->getGhostedID()->ToJsonObject());
		}

		objVal[StringHelper::wstringToString(CONSTRUCT_PARAMETERS)] = *this->getConstructParameters();

		return std::make_shared<object>(objVal);
	}

	bool ParameterizeConstructRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<ParameterizeConstructRequestMessage>))
		{
			std::shared_ptr<ParameterizeConstructRequestMessage> other = std::any_cast<std::shared_ptr<ParameterizeConstructRequestMessage>>(obj);

			// If this has a ghosted ID but the other doesn't, return false
			if (other->getGhostedID() == nullptr && getGhostedID() != nullptr)
			{
				return false;
			}

			std::shared_ptr<MUSICRequestMessage> request = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(),
																								 other->getReceiverID(), other->getRequestID());\

			if (MUSICRequestMessage::Equals(request) &&
				getGhostedID()->Equals(other->getGhostedID()) &&
				serialize(*getConstructParameters()).compare(serialize(*other->getConstructParameters())) == 0)
			{
				return true;
			}
		}

		return false;
	}

	int ParameterizeConstructRequestMessage::GetHashCode() const
	{
		auto hashCode = -158961684;
		hashCode = hashCode * -1521130295 + MUSICRequestMessage::GetHashCode();
		hashCode = hashCode * -1521130295 + ((getGhostedID() != nullptr) ? getGhostedID()->GetHashCode() : 0);
		hashCode = hashCode * -1521130295 + stdext::hash_value(serialize(*getConstructParameters()));
		return hashCode;
	}

	size_t ParameterizeConstructRequestMessage::HashFunction::operator()(const ParameterizeConstructRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool ParameterizeConstructRequestMessage::operator == (const ParameterizeConstructRequestMessage &message2)
	{
		return this->Equals(std::make_shared<ParameterizeConstructRequestMessage>(message2));
	}

	bool ParameterizeConstructRequestMessage::operator != (const ParameterizeConstructRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
