/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICEventMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{
	const std::wstring MUSICEventMessage::HEADER = L"header";
	const std::wstring MUSICEventMessage::ORIGIN_ID = L"originID";
	const std::wstring MUSICEventMessage::EVENT_TYPE = L"eventType";
	const std::wstring MUSICEventMessage::EVENT_DATA = L"eventData";

	std::wstring MUSICEventMessage::getEventType() const
	{
		return EventType;
	}

	void MUSICEventMessage::setEventType(const std::wstring &value)
	{
		EventType = value;
	}

	std::shared_ptr<object> MUSICEventMessage::getEventData() const
	{
		return EventData;
	}

	void MUSICEventMessage::setEventData(std::shared_ptr<object> value)
	{
		EventData = value;
	}

	MUSICEventMessage::MUSICEventMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, const std::wstring &eventType, std::shared_ptr<object> eventData) : MUSICMessage(exerciseID, originID)
	{
		setEventType(eventType);
		setEventData(eventData);
	}

	MUSICEventMessage::MUSICEventMessage(std::shared_ptr<object> obj) : MUSICMessage((*obj)["header"].as_object(),(*obj)["originID"].as_object())
	{
		object objVal = *obj;

		setEventType(StringHelper::stringToWString(objVal[StringHelper::wstringToString(EVENT_TYPE)].as_string()));
		setEventData(std::make_shared<object>(objVal[StringHelper::wstringToString(EVENT_DATA)].as_object()));
	}

	void MUSICEventMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitMUSICEvent(shared_from_this());
	}

	std::any MUSICEventMessage::Clone()
	{
		//Primitive uints from OriginID are passed by value.
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());

		std::wstring clonedType = std::wstring(getEventType());
		std::shared_ptr<object> clonedEventData = std::make_shared<object>(object(*getEventData()));

		//ExerciseID is ok because it is a primitive (non-string) so it is passed by value.
		std::shared_ptr<MUSICEventMessage> message = std::make_shared<MUSICEventMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedType, clonedEventData);

		return message;
	}

	std::shared_ptr<object> MUSICEventMessage::ToJsonObject()
	{
		object obj = object();

		obj[StringHelper::wstringToString(HEADER)] = *(getMUSICHeader()->ToJsonObject());
		obj[StringHelper::wstringToString(ORIGIN_ID)] = *(getOriginID()->ToJsonObject());
		obj[StringHelper::wstringToString(EVENT_TYPE)] = StringHelper::wstringToString(getEventType());
		obj[StringHelper::wstringToString(EVENT_DATA)] = *(getEventData());

		return std::make_shared<object>(obj);
	}

	bool MUSICEventMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<MUSICEventMessage>))
		{
			auto other = std::any_cast<std::shared_ptr<MUSICEventMessage>>(obj);

			std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> parent = 
				std::make_shared<MUSICLibrary::MUSIC_Messages::MUSICMessage>(
					MUSICMessage(other->getMUSICHeader()->getExerciseID(), other->getOriginID()));

			return MUSICMessage::Equals(parent) &&
				getEventType() == other->getEventType() &&
				serialize(*getEventData()).compare(serialize(*other->getEventData())) == 0;
		}
		return false;
	}

	int MUSICEventMessage::GetHashCode() const
	{
		auto hashCode = 610186252;
		hashCode = hashCode * -1521134295 + MUSICMessage::GetHashCode();
		hashCode = hashCode * -1521134295 + std::hash<std::wstring>{}(getEventType());
		hashCode = hashCode * -1521134295 + std::hash<std::string>{}(serialize(*getEventData()));
		return hashCode;
	}

	size_t MUSICEventMessage::HashFunction::operator()(const MUSICEventMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool MUSICEventMessage::operator == (const MUSICEventMessage &message2)
	{
		return this->Equals(std::make_shared<MUSICEventMessage>(message2));
	}

	bool MUSICEventMessage::operator != (const MUSICEventMessage &message2)
	{
		return !(*this == message2);
	}
}
