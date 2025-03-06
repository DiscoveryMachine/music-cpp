/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_TARGETED_MUSIC_MESSAGES_DISPLAY_MESSGES_MESSAGE_H
#define DMI_TARGETED_MUSIC_MESSAGES_DISPLAY_MESSGES_MESSAGE_H

#include <string>
#include <vector>
#include "../../Interfaces/IMUSICCommandMessage.h"
#include "TargetedMUSICMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{
	class DisplayMessagesMessage : public TargetedMUSICMessage, public  MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	private:
		std::vector<std::wstring> Messages;
		unsigned int Timeout = 0;

	public:
		std::vector<std::wstring> getMessages() const;
		void setMessages(const std::vector<std::wstring> &value);
		unsigned int getTimeout() const;
		void setTimeout(unsigned int value);

		DisplayMessagesMessage(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> originID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> recieverID, std::vector<std::wstring> &messages, unsigned int timeout);

		DisplayMessagesMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const DisplayMessagesMessage& obj) const;
		};

		bool operator == (const DisplayMessagesMessage &message2);

		bool operator != (const DisplayMessagesMessage &message2);

	protected:
		std::shared_ptr<DisplayMessagesMessage> shared_from_this()
		{
			return std::static_pointer_cast<DisplayMessagesMessage>(TargetedMUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_TARGETED_MUSIC_MESSAGES_DISPLAY_MESSGES_MESSAGE_H