/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "CreateConstructRequestMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	const std::wstring CreateConstructRequestMessage::CONSTRUCT_TYPE = L"constructType";
	const std::wstring CreateConstructRequestMessage::CONSTRUCT_CALLSIGN = L"constructCallsign";
	const std::wstring CreateConstructRequestMessage::CONSTRUCT_LOCATION = L"constructLocation";
	const std::wstring CreateConstructRequestMessage::CONSTRUCT_ORIENTATION = L"constructOrientation";

	std::wstring CreateConstructRequestMessage::getConstructType() const
	{
		return constructType;
	}

	void CreateConstructRequestMessage::setConstructType(const std::wstring &value)
	{
		constructType = value;
	}

	std::wstring CreateConstructRequestMessage::getConstructCallsign() const
	{
		return constructCallsign;
	}

	void CreateConstructRequestMessage::setConstructCallsign(const std::wstring &value)
	{
		constructCallsign = value;
	}

	std::shared_ptr<MUSICVector3> CreateConstructRequestMessage::getConstructLocation() const
	{
		return constructLocation;
	}

	void CreateConstructRequestMessage::setConstructLocation(std::shared_ptr<MUSICVector3> value)
	{
		constructLocation = value;
	}

	std::shared_ptr<MUSICVector3> CreateConstructRequestMessage::getConstructOrientation() const
	{
		return constructOrientation;
	}

	void CreateConstructRequestMessage::setConstructOrientation(std::shared_ptr<MUSICVector3> value)
	{
		constructOrientation = value;
	}

	CreateConstructRequestMessage::CreateConstructRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, const std::wstring &constructType, const std::wstring &constructCallsign, std::shared_ptr<MUSICVector3> constructLocation, std::shared_ptr<MUSICVector3> constructOrientation) : MUSICRequestMessage(exerciseID, originID, receiverID, requestID)
	{
		this->setConstructType(constructType);
		this->setConstructCallsign(constructCallsign);
		this->setConstructLocation(constructLocation);
		this->setConstructOrientation(constructOrientation);
	}

	CreateConstructRequestMessage::CreateConstructRequestMessage(std::shared_ptr<object> jobj) : MUSICRequestMessage(jobj)
	{
		object objVal = *jobj;

		setConstructType(StringHelper::stringToWString(objVal[StringHelper::wstringToString(CONSTRUCT_TYPE)].as_string()));
		setConstructCallsign(StringHelper::stringToWString(objVal[StringHelper::wstringToString(CONSTRUCT_CALLSIGN)].as_string()));

		if (objVal.if_contains(StringHelper::wstringToString(CONSTRUCT_LOCATION)))
		{
			setConstructLocation(std::make_shared<MUSICVector3>(std::make_shared<object>(objVal[StringHelper::wstringToString(CONSTRUCT_LOCATION)].as_object())));
		}
		else
		{
			setConstructLocation(std::make_shared<MUSICVector3>());
		}

		if (objVal.if_contains(StringHelper::wstringToString(CONSTRUCT_ORIENTATION)))
		{
			setConstructOrientation(std::make_shared<MUSICVector3>(std::make_shared<object>(objVal[StringHelper::wstringToString(CONSTRUCT_ORIENTATION)].as_object())));
		}
		else
		{
			setConstructOrientation(std::make_shared<MUSICVector3>());
		}
	}

	void CreateConstructRequestMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitCreateConstructRequest(shared_from_this());
	}

	std::any CreateConstructRequestMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<EntityIDRecord> clonedReceiverID = std::make_shared<EntityIDRecord>(getReceiverID()->getSiteID(), getReceiverID()->getAppID(), getReceiverID()->getEntityID());
		std::wstring clonedConstructType = std::wstring(getConstructType());
		std::wstring clonedConstructCallsign = std::wstring(getConstructCallsign());
		std::shared_ptr<MUSICVector3> clonedConstructLocation = std::make_shared<MUSICVector3>(getConstructLocation()->getX(), getConstructLocation()->getY(), getConstructLocation()->getZ());
		std::shared_ptr<MUSICVector3> clonedConstructOrientation = std::make_shared<MUSICVector3>(getConstructOrientation()->getX(), getConstructOrientation()->getY(), getConstructOrientation()->getZ());

		std::shared_ptr<CreateConstructRequestMessage> clonedMessage = std::make_shared<CreateConstructRequestMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedReceiverID, getRequestID(), clonedConstructType, clonedConstructCallsign, clonedConstructLocation, clonedConstructOrientation);

		return clonedMessage;
	}

	std::shared_ptr<object> CreateConstructRequestMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(getMUSICHeader()->ToJsonObject());
		objVal["commandIdentifier"] = GetCommandIdentifier();
		objVal["originID"] = *(getOriginID()->ToJsonObject());
		objVal["receiverID"] = *(getReceiverID()->ToJsonObject());
		objVal["requestID"] = getRequestID();
		objVal[StringHelper::wstringToString(CONSTRUCT_TYPE)] = StringHelper::wstringToString(getConstructType());
		objVal[StringHelper::wstringToString(CONSTRUCT_CALLSIGN)] = StringHelper::wstringToString(getConstructCallsign());
		objVal[StringHelper::wstringToString(CONSTRUCT_LOCATION)] = *(getConstructLocation()->ToJsonObject());
		objVal[StringHelper::wstringToString(CONSTRUCT_ORIENTATION)] = *(getConstructOrientation()->ToJsonObject());

		return std::make_shared<object>(objVal);
	}

	unsigned int CreateConstructRequestMessage::GetCommandIdentifier()
	{
		return 454013002;
	}

	bool CreateConstructRequestMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<CreateConstructRequestMessage>))
		{
			std::shared_ptr<CreateConstructRequestMessage> other = std::any_cast<std::shared_ptr<CreateConstructRequestMessage>>(obj);
			std::shared_ptr<MUSICRequestMessage> request = std::make_shared<MUSICRequestMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID(),
																								 other->getReceiverID(), other->getRequestID());

			if (MUSICRequestMessage::Equals(request) && 
				getConstructType() == other->getConstructType() && 
				getConstructCallsign() == other->getConstructCallsign() && 
				getConstructLocation()->Equals(other->getConstructLocation()) && 
				getConstructOrientation()->Equals(other->getConstructOrientation()))
			{
				return true;
			}
		}

		return false;
	}

	int CreateConstructRequestMessage::GetHashCode() const
	{
		auto hashCode = 510219160;
		hashCode = hashCode * -1521134295 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getReceiverID() != nullptr) ? getReceiverID()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getRequestID() != NULL) ? std::hash<unsigned int>{}(getRequestID()) : 0);
		hashCode = hashCode * -1521134295 + ((!getConstructType().empty()) ? std::hash<std::wstring>{}(getConstructType()) : 0);
		hashCode = hashCode * -1521134295 + ((!getConstructCallsign().empty()) ? std::hash<std::wstring>{}(getConstructCallsign()) : 0);
		hashCode = hashCode * -1521134295 + ((getConstructLocation() != nullptr) ? getConstructLocation()->GetHashCode() : 0);
		hashCode = hashCode * -1521134295 + ((getConstructOrientation() != nullptr) ? getConstructOrientation()->GetHashCode() : 0);
		return hashCode;
	}

	size_t CreateConstructRequestMessage::HashFunction::operator()(const CreateConstructRequestMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool CreateConstructRequestMessage::operator == (const CreateConstructRequestMessage &message2)
	{
		return this->Equals(std::make_shared<CreateConstructRequestMessage>(message2));
	}

	bool CreateConstructRequestMessage::operator != (const CreateConstructRequestMessage &message2)
	{
		return !(*this == message2);
	}
}
