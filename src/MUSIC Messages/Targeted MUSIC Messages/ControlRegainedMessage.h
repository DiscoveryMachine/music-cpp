/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_TARGETED_MUSIC_MESSAGES_CONTOL_REGAINED_MESSAGE_H
#define DMI_TARGETED_MUSIC_MESSAGES_CONTOL_REGAINED_MESSAGE_H

#include "TargetedMUSICMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{
	class ControlRegainedMessage : public TargetedMUSICMessage
	{
	public:
		ControlRegainedMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID);

		ControlRegainedMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);
		
		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const ControlRegainedMessage& obj) const;
		};

		bool operator == (const ControlRegainedMessage &message2);

		bool operator != (const ControlRegainedMessage &message2);

	protected:
		std::shared_ptr<ControlRegainedMessage> shared_from_this()
		{
			return std::static_pointer_cast<ControlRegainedMessage>(TargetedMUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_TARGETED_MUSIC_MESSAGES_CONTOL_REGAINED_MESSAGE_H