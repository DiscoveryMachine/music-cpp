/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_TARGETED_MUSIC_MESSAGES_CONTROL_GRANTED_TRANSFER_MESSAGE
#define DMI_TARGETED_MUSIC_MESSAGES_CONTROL_GRANTED_TRANSFER_MESSAGE

#include "MUSICRequestMessage.h"

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{

	class ControlTransferGrantedMessage : public MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::MUSICRequestMessage
	{
		public:
			ControlTransferGrantedMessage(unsigned int exerciseId, unsigned int requestId, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverId);

			ControlTransferGrantedMessage(std::shared_ptr<boost::json::object> jobj);

			void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

			bool Equals(std::any obj);

			int GetHashCode() const;

			struct HashFunction
			{
				size_t operator()(const ControlTransferGrantedMessage& obj) const;
			};

			bool operator == (const ControlTransferGrantedMessage &message2);

			bool operator != (const ControlTransferGrantedMessage &message2);

		protected:
			std::shared_ptr<ControlTransferGrantedMessage> shared_from_this()
			{
				return std::static_pointer_cast<ControlTransferGrantedMessage>(MUSICRequestMessage::shared_from_this());
			}
	};
}
#endif