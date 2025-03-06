/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATA_INTERACTION_RECORD_H
#define DMI_CONSTRUCT_DATA_INTERACTION_RECORD_H

#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <any>
#include <memory>
#include "../../tangible_string_helper.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class InteractionRecord
	{
	private:
		std::unordered_set<std::wstring> availableInteractions;

	public:
		std::unordered_set<std::wstring> getAvailableInteractions() const;
		void setAvailableInteractions(const std::unordered_set<std::wstring> &value);

		InteractionRecord();

		InteractionRecord(std::unordered_set<std::wstring> interactions);

		std::wstring ToCsvString();

		void AddAllInteraction(const std::wstring &csvString);

		void AddInteraction(const std::wstring &actionString);

		bool ContainsInteraction(const std::wstring &action);

		void RemoveInteraction(const std::wstring &action);

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const InteractionRecord& obj) const;
		};

		bool operator == (const InteractionRecord &message2);

		bool operator != (const InteractionRecord &message2);
	};
}

#endif // !DMI_CONSTRUCT_DATA_INTERACTION_RECORD_H