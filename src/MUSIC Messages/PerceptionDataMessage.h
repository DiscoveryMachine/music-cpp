/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_PERCEPTION_DATA_MESSAGE_H
#define DMI_MUSIC_MESSAGES_PERCEPTION_DATA_MESSAGE_H

#include <string>
#include <vector>
#include <algorithm>
#include "MUSICMessage.h"
#include "PerceptionData/PerceptionRecord.h"
#include "../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages
{
	class PerceptionDataMessage : public MUSICMessage
	{
	private:
		std::vector<std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionData::PerceptionRecord>> PerceptionRecords;

	public:
		std::vector<std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionData::PerceptionRecord>> getPerceptionRecords() const;
		void setPerceptionRecords(const std::vector<std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionData::PerceptionRecord>> &value);
	private:
		const std::wstring PERCEPTION_RECORDS = L"perceptionRecords";

	public:
		PerceptionDataMessage();

		PerceptionDataMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::vector<std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionData::PerceptionRecord>> &perceptionRecords);

		PerceptionDataMessage(std::shared_ptr<boost::json::object> jobj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const PerceptionDataMessage& obj) const;
		};

		bool operator == (const PerceptionDataMessage &message2);

		bool operator != (const PerceptionDataMessage &message2);

	protected:
		std::shared_ptr<PerceptionDataMessage> shared_from_this()
		{
			return std::static_pointer_cast<PerceptionDataMessage>(MUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_MESSAGES_PERCEPTION_DATA_MESSAGE_H