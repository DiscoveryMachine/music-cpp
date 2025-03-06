/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PerceptionDataMessage.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages::PerceptionData;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{
	std::vector<std::shared_ptr<PerceptionRecord>> PerceptionDataMessage::getPerceptionRecords() const
	{
		return PerceptionRecords;
	}

	void PerceptionDataMessage::setPerceptionRecords(const std::vector<std::shared_ptr<PerceptionRecord>> &value)
	{
		PerceptionRecords = value;
	}

	PerceptionDataMessage::PerceptionDataMessage()
	{
	}

	PerceptionDataMessage::PerceptionDataMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::vector<std::shared_ptr<PerceptionRecord>> &perceptionRecords) : MUSICMessage(exerciseID, originID)
	{
		this->setPerceptionRecords(perceptionRecords);
	}

	PerceptionDataMessage::PerceptionDataMessage(std::shared_ptr<object> jobj) : MUSICMessage((*jobj)["header"].as_object(), (*jobj)["originID"].as_object())
	{
		object objVal = *jobj;

		array perceptionArray = objVal[StringHelper::wstringToString(PERCEPTION_RECORDS)].as_array();
		std::vector<std::shared_ptr<PerceptionRecord>> records = std::vector<std::shared_ptr<PerceptionRecord>>();

		// Create a shared pointer for each perception record and put it in the vector of these pointers
		for (int i = 0; i < perceptionArray.size(); i++)
		{
			value val = perceptionArray[i];
			//records.push_back(std::make_shared<PerceptionRecord>(std::make_shared<boost::json::object>(perceptionArray[i].get_object())));
			records.push_back(std::make_shared<PerceptionRecord>(std::make_shared<boost::json::object>(perceptionArray[i].as_object())));
		}

		this->setPerceptionRecords(records);
	}

	void PerceptionDataMessage::AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor)
	{
		visitor->VisitPerceptionData(shared_from_this());
	}

	std::any PerceptionDataMessage::Clone()
	{
		std::vector<std::shared_ptr<PerceptionRecord>> clonedList;

		// need to iterate through perceptionRecords
		// to perform a deep copy as it has more than just primitive data types.
		for (auto pr : getPerceptionRecords())
		{
			std::shared_ptr<EntityIDRecord> clonedPerceptionID = std::make_shared<EntityIDRecord>(pr->getPerceptionID()->getSiteID(), pr->getPerceptionID()->getAppID(), pr->getPerceptionID()->getEntityID());
			std::shared_ptr<EntityTypeRecord> clonedEntityTypeRecord = std::make_shared<EntityTypeRecord>(pr->getEntityTypeRecord()->getKind(), pr->getEntityTypeRecord()->getDomain(), pr->getEntityTypeRecord()->getCountry(), pr->getEntityTypeRecord()->getCategory(), pr->getEntityTypeRecord()->getSubcategory(), pr->getEntityTypeRecord()->getSpecificInfo(), pr->getEntityTypeRecord()->getExtraValue());

			std::shared_ptr<PerceptionRecord> clonedPerceptionRecord = std::make_shared<PerceptionRecord>(clonedPerceptionID, pr->getForceIDField(), clonedEntityTypeRecord, pr->getPerceptionSystem(), pr->getAccuracy(), pr->getPerceptionErrors());
			clonedList.push_back(clonedPerceptionRecord);
		}
		std::shared_ptr<EntityIDRecord> clonedOriginID = std::make_shared<EntityIDRecord>(getOriginID()->getSiteID(), getOriginID()->getAppID(), getOriginID()->getEntityID());
		std::shared_ptr<PerceptionDataMessage> clonedMessage = std::make_shared<PerceptionDataMessage>(getMUSICHeader()->getExerciseID(), clonedOriginID, clonedList);

		return clonedMessage;
	}

	std::shared_ptr<object> PerceptionDataMessage::ToJsonObject()
	{
		object objVal = object();

		objVal["header"] = *(this->getMUSICHeader()->ToJsonObject());
		objVal["originID"] = *(this->getOriginID()->ToJsonObject());
		objVal["numPerceptions"] = this->getPerceptionRecords().size();

		std::shared_ptr<array> jArray = std::make_shared<array>();
		std::for_each(PerceptionRecords.begin(), PerceptionRecords.end(), [jArray](std::shared_ptr<PerceptionRecord> record)
		{
			jArray->push_back(*(record->ToJsonObject()));
		});
		objVal[StringHelper::wstringToString(PERCEPTION_RECORDS)] = *jArray;

		return std::make_shared<object>(objVal);
	}

	bool PerceptionDataMessage::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<PerceptionDataMessage>))
		{
			std::shared_ptr<PerceptionDataMessage> other = std::any_cast<std::shared_ptr<PerceptionDataMessage>>(obj);
			std::shared_ptr<MUSICMessage> parent = std::make_shared<MUSICMessage>(other->getMUSICHeader()->getExerciseID(), other->getOriginID());
			if (MUSICMessage::Equals(parent) &&
				PerceptionRecords.size() == other->getPerceptionRecords().size()) 
			{
				for (int i = 0; i < PerceptionRecords.size(); i++)
				{
					if (!(PerceptionRecords[i]->Equals(other->getPerceptionRecords()[i])))
					{
						return false;
					}
				}

				return true;
			}
		}

		return false;
	}

	int PerceptionDataMessage::GetHashCode() const
	{
		auto hashCode = -1573912470;
		hashCode = hashCode * -1521134295 + MUSICMessage::GetHashCode();

		for (auto record : getPerceptionRecords())
		{
			hashCode = hashCode * -1521134295 + record->GetHashCode();
		}

		return hashCode;
	}

	size_t PerceptionDataMessage::HashFunction::operator()(const PerceptionDataMessage& obj) const
	{
		return obj.GetHashCode();
	}

	bool PerceptionDataMessage::operator == (const PerceptionDataMessage &message2)
	{
		return this->Equals(std::make_shared<PerceptionDataMessage>(message2));
	}

	bool PerceptionDataMessage::operator != (const PerceptionDataMessage &message2)
	{
		return !(*this == message2);
	}
}
