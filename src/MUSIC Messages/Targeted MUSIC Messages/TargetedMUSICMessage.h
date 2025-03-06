/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_TARGETED_MUSIC_MESSAGE_TARGETED_MUSIC_MESSAGE_H
#define DMI_TARGETED_MUSIC_MESSAGE_TARGETED_MUSIC_MESSAGE_H

#include "../MUSICMessage.h"

namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{
	class TargetedMUSICMessage : public MUSICMessage
	{
	private:
		std::shared_ptr<EntityIDRecord> ReceiverID;

	public:
		std::shared_ptr<EntityIDRecord> getReceiverID() const;
		void setReceiverID(std::shared_ptr<EntityIDRecord> value);

		TargetedMUSICMessage();

		TargetedMUSICMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID);

		TargetedMUSICMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICHeader> header, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID);

		TargetedMUSICMessage(std::shared_ptr<boost::json::object> obj);

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const TargetedMUSICMessage& obj) const;
		};

		bool operator == (const TargetedMUSICMessage &message2);

		bool operator != (const TargetedMUSICMessage &message2);

	protected:
		std::shared_ptr<TargetedMUSICMessage> shared_from_this()
		{
			return std::static_pointer_cast<TargetedMUSICMessage>(MUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_TARGETED_MUSIC_MESSAGE_TARGETED_MUSIC_MESSAGE_H