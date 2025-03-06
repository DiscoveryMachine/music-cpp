/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "StateFieldMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	std::shared_ptr<object> StateFieldMessage::getStateDataObject() const
	{
		return StateDataObject;
	}

	void StateFieldMessage::setStateDataObject(std::shared_ptr<object> value)
	{
		StateDataObject = value;
	}

	const std::wstring StateFieldMessage::HEADER_KEY = L"header";
	const std::wstring StateFieldMessage::ORIGIN_ID_KEY = L"originID";
	const std::wstring StateFieldMessage::STATE_DATA_KEY = L"stateData";

	StateFieldMessage::StateFieldMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<object> jObject) : MUSICMessage(exerciseID, originID)
	{
		setStateDataObject(jObject);
	}

	StateFieldMessage::StateFieldMessage(std::shared_ptr<object> jObject) : MUSICMessage((*jObject)["header"].as_object(), (*jObject)["originID"].as_object())
	{
		object objVal = *jObject;

		object stateFields = objVal[StringHelper::wstringToString(STATE_DATA_KEY)].as_object();
		setStateDataObject(std::make_shared<object>(stateFields));
	}

	std::shared_ptr<object> StateFieldMessage::ToJsonObject()
	{
		object obj;

		obj[StringHelper::wstringToString(HEADER_KEY)] = *(getMUSICHeader()->ToJsonObject());
		obj[StringHelper::wstringToString(ORIGIN_ID_KEY)] = *(getOriginID()->ToJsonObject());
		obj[StringHelper::wstringToString(STATE_DATA_KEY)] = *(getStateDataObject());

		return std::make_shared<object>(obj);
	}

	void StateFieldMessage::AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor)
	{
		visitor->VisitStateField(shared_from_this());
	}

	std::any StateFieldMessage::Clone()
	{
		std::shared_ptr<EntityIDRecord> originClone = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<object> stateDataObjectClone = std::make_shared<object>(object(*getStateDataObject()));
		std::shared_ptr<StateFieldMessage> message = std::make_shared<StateFieldMessage>(getMUSICHeader()->getExerciseID(), originClone, stateDataObjectClone);

		return message;
	}

	bool StateFieldMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<StateFieldMessage>))
		{
			std::shared_ptr<StateFieldMessage> other = std::any_cast<std::shared_ptr<StateFieldMessage>>(obj);

			std::shared_ptr<MUSICMessage> parent = std::make_shared<MUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID());

			if (MUSICMessage::Equals(parent) &&
				*getStateDataObject() == *other->getStateDataObject())
			{
				return true;
			}
		}

		return false;
	}

	int StateFieldMessage::GetHashCode() const
	{
		return 1566961789 + ((getMUSICHeader()->getExerciseID() != NULL) ? getMUSICHeader()->GetHashCode() : 0) +
			((getOriginID() != nullptr) ? getOriginID()->GetHashCode() : 0) +
			stdext::hash_value(serialize(*getStateDataObject()));
	}

	size_t StateFieldMessage::HashFunction::operator()(const StateFieldMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool StateFieldMessage::operator == (const StateFieldMessage &message2)
	{
		return this->Equals(std::make_shared<StateFieldMessage>(message2));
	}

	bool StateFieldMessage::operator != (const StateFieldMessage &message2)
	{
		return !(*this == message2);
	}
}