/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "InteractionRecord.h"

namespace MUSICLibrary::MUSIC_Messages
{

	std::unordered_set<std::wstring> InteractionRecord::getAvailableInteractions() const
	{
		return availableInteractions;
	}

	void InteractionRecord::setAvailableInteractions(const std::unordered_set<std::wstring> &value)
	{
		availableInteractions = value;
	}

	InteractionRecord::InteractionRecord()
	{
		setAvailableInteractions(std::unordered_set<std::wstring>());
	}

	InteractionRecord::InteractionRecord(std::unordered_set<std::wstring> interactions)
	{
		setAvailableInteractions(interactions);
	}

	std::wstring InteractionRecord::ToCsvString()
	{
		int count = 0;
		std::wstring allActions = L"";
		for (auto action : getAvailableInteractions())
		{
			// If this is not the last interaction, add action and comma to string
			if (count < getAvailableInteractions().size() - 1)
			{
				allActions += action + L", ";
			}
			// Otherwise, add just the action
			else
			{
				allActions += action;
			}

			count++;
		}

		return allActions;
	}

	void InteractionRecord::AddAllInteraction(const std::wstring &csvString)
	{
		if (csvString != L"")
		{
			std::vector<std::wstring> actionlist = StringHelper::split(csvString, L',');

			for (std::wstring action : actionlist)
			{
				//removes all all whitespace
				action.erase(std::remove_if(action.begin(), action.end(), ::isspace), action.end());

				AddInteraction(action);
			}
		}
	}

	void InteractionRecord::AddInteraction(const std::wstring &actionString)
	{
		availableInteractions.insert(actionString);
	}

	bool InteractionRecord::ContainsInteraction(const std::wstring &action)
	{
		if (std::find(availableInteractions.begin(), availableInteractions.end(), action) != availableInteractions.end())
		{
			return true;
		}

		return false;
	}

	void InteractionRecord::RemoveInteraction(const std::wstring &action)
	{
		availableInteractions.erase(action);
	}

	int InteractionRecord::GetHashCode() const
	{
		auto hashCode = -1384359703;
		hashCode = hashCode * -1521134295 + 0;
   	    std::unordered_set<std::wstring> availableInteractions = getAvailableInteractions();
		for (const std::wstring elem : availableInteractions)
		{
			hashCode = hashCode * -1521134295 + std::hash<std::wstring>{}(elem);
		}
		return hashCode;
	}

	size_t InteractionRecord::HashFunction::operator()(const InteractionRecord& obj) const
	{
		return obj.GetHashCode();
	}

	bool InteractionRecord::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<InteractionRecord>))
		{
			std::shared_ptr<InteractionRecord> other = std::any_cast<std::shared_ptr<InteractionRecord>>(obj);

			if (this->getAvailableInteractions().size() == other->getAvailableInteractions().size())
			{
				for (auto i : getAvailableInteractions())
				{
					if (!other->ContainsInteraction(i))
					{
						return false;
					}
				}
				return true;
			}
		}
		else if (obj.type() == typeid(InteractionRecord))
		{
			InteractionRecord other = std::any_cast<InteractionRecord>(obj);

			if (this->getAvailableInteractions().size() == other.getAvailableInteractions().size())
			{
				for (auto i : getAvailableInteractions())
				{
					if (!other.ContainsInteraction(i))
					{
						return false;
					}
				}
				return true;
			}
		}

		return false;
	}

	bool InteractionRecord::operator == (const InteractionRecord &message2)
	{
		return this->Equals(std::make_shared<InteractionRecord>(message2));
	}

	bool InteractionRecord::operator != (const InteractionRecord &message2)
	{
		return !(*this == message2);
	}
}
