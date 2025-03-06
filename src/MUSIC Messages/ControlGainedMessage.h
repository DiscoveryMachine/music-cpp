/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_CONTROL_GAINED_MESSAGE_H
#define DMI_MUSIC_MESSAGES_CONTROL_GAINED_MESSAGE_H

#include "MUSICMessage.h"
#include "../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class ControlGainedMessage : public MUSICMessage
	{
	private:
		std::shared_ptr<EntityIDRecord> receiver;
		std::shared_ptr<EntityIDRecord> gainedControlOf;

	public:
		std::shared_ptr<EntityIDRecord> getReceiver() const;
		void setReceiver(std::shared_ptr<EntityIDRecord> value);
		std::shared_ptr<EntityIDRecord> getGainedControlOf() const;
		void setGainedControlOf(std::shared_ptr<EntityIDRecord> value);

		ControlGainedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiver, std::shared_ptr<EntityIDRecord> gainedControlOf);

		ControlGainedMessage(std::shared_ptr<boost::json::object> obj);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const ControlGainedMessage& obj) const;
		};

		bool operator == (const ControlGainedMessage &message2);

		bool operator != (const ControlGainedMessage &message2);

	protected:
		std::shared_ptr<ControlGainedMessage> shared_from_this()
		{
			return std::static_pointer_cast<ControlGainedMessage>(MUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_MESSAGES_CONTROL_GAINED_MESSAGE_H