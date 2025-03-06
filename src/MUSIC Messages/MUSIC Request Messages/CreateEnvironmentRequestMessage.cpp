/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "CreateEnvironmentRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	const std::wstring CreateEnvironmentRequestMessage::ENVIRONMENT_NAME = L"environmentName";
	const std::wstring CreateEnvironmentRequestMessage::ENVIRONMENT_META_DATA = L"environmentMetaData";

	std::wstring CreateEnvironmentRequestMessage::getEnvironmentName() const
	{
		return EnvironmentName;
	}

	void CreateEnvironmentRequestMessage::setEnvironmentName(const std::wstring &value)
	{
		EnvironmentName = value;
	}

	std::shared_ptr<object> CreateEnvironmentRequestMessage::getEnvironmentMetaData() const
	{
		return EnvironmentMetaData;
	}

	void CreateEnvironmentRequestMessage::setEnvironmentMetaData(std::shared_ptr<object> value)
	{
		EnvironmentMetaData = value;
	}

	CreateEnvironmentRequestMessage::CreateEnvironmentRequestMessage(std::shared_ptr<object> jobj) : MUSICRequestMessage(jobj)
	{
		object objVal = *jobj;

		setEnvironmentName(StringHelper::stringToWString(objVal[StringHelper::wstringToString(ENVIRONMENT_NAME)].as_string()));
		setEnvironmentMetaData(std::make_shared<object>(objVal[StringHelper::wstringToString(ENVIRONMENT_META_DATA)].as_object()));
	}

	CreateEnvironmentRequestMessage::CreateEnvironmentRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, const std::wstring &environmentName, std::shared_ptr<object> environmentMetaData) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
		this->setEnvironmentName(environmentName);
		this->setEnvironmentMetaData(environmentMetaData);
	}

	void CreateEnvironmentRequestMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitCreateEnvironmentRequest(shared_from_this());
	}

	std::any CreateEnvironmentRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::wstring clonedEnvironmentName = std::wstring(getEnvironmentName());
		std::shared_ptr<object> clonedEnvironmentMetadata = std::make_shared<object>(object(*getEnvironmentMetaData()));

		std::shared_ptr<CreateEnvironmentRequestMessage> clonedMessage = std::make_shared<CreateEnvironmentRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedEnvironmentName, clonedEnvironmentMetadata);

		return clonedMessage;
	}

	unsigned int CreateEnvironmentRequestMessage::GetCommandIdentifier()
	{
		return 454013000;
	}

	std::shared_ptr<object> CreateEnvironmentRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal[StringHelper::wstringToString(ENVIRONMENT_NAME)] = StringHelper::wstringToString(getEnvironmentName());
		objVal[StringHelper::wstringToString(ENVIRONMENT_META_DATA)] = *getEnvironmentMetaData();

		return std::make_shared<object>(objVal);
	}

	bool CreateEnvironmentRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<CreateEnvironmentRequestMessage>))
		{
			std::shared_ptr<CreateEnvironmentRequestMessage> other = std::any_cast<std::shared_ptr<CreateEnvironmentRequestMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> request = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(),
																								 other->getReceiverID(), other->getRequestID());
			
			if (MUSICRequestMessage::Equals(request) &&
				getEnvironmentName() == other->getEnvironmentName() && 
				serialize(*getEnvironmentMetaData()).compare(serialize(*other->getEnvironmentMetaData())) == 0)
			{
				return true;
			}
		}

		return false;
	}

	int CreateEnvironmentRequestMessage::GetHashCode() const
	{
		auto hashCode = -968179538;
		hashCode = hashCode * -1001134095 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1001134095 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1001134095 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1001134095 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		hashCode = hashCode * -1001134095 + ((!getEnvironmentName().empty()) ? std::hash<std::wstring>{}(getEnvironmentName()) : 0);
		hashCode = hashCode * -1001134095 + stdext::hash_value(serialize(*getEnvironmentMetaData()));
		return hashCode;
	}

	size_t CreateEnvironmentRequestMessage::HashFunction::operator()(const CreateEnvironmentRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool CreateEnvironmentRequestMessage::operator == (const CreateEnvironmentRequestMessage &message2)
	{
		return this->Equals(std::make_shared<CreateEnvironmentRequestMessage>(message2));
	}

	bool CreateEnvironmentRequestMessage::operator != (const CreateEnvironmentRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
