/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_MUSIC_EVENT_MESSAGE_H
#define DMI_MUSIC_MESSAGES_MUSIC_EVENT_MESSAGE_H

#include <string>
#include "MUSICMessage.h"
#include "../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages
{
	class MUSICEventMessage : public MUSICMessage
	{
	private:
		std::wstring EventType;
		std::shared_ptr<boost::json::object> EventData;

		static const std::wstring HEADER;
		static const std::wstring ORIGIN_ID; //equvialent of constructID
		static const std::wstring EVENT_TYPE;
		static const std::wstring EVENT_DATA;

	public:
		std::wstring getEventType() const;
		void setEventType(const std::wstring &value);
		std::shared_ptr<boost::json::object> getEventData() const;
		void setEventData(std::shared_ptr<boost::json::object> value);

		MUSICEventMessage(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> originID, const std::wstring &eventType, std::shared_ptr<boost::json::object> eventData);

		MUSICEventMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const MUSICEventMessage& obj) const;
		};

		bool operator == (const MUSICEventMessage &message2);

		bool operator != (const MUSICEventMessage &message2);

	protected:
		std::shared_ptr<MUSICEventMessage> shared_from_this()
		{
			return std::static_pointer_cast<MUSICEventMessage>(MUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_MESSAGES_MUSIC_EVENT_MESSAGE_H