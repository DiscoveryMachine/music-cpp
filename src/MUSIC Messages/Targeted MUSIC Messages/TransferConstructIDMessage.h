/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_TARGETED_MUSIC_MESSAGE_TRANSFER_CONSTRUCT_ID_MESSAGE_H
#define DMI_TARGETED_MUSIC_MESSAGE_TRANSFER_CONSTRUCT_ID_MESSAGE_H

#include "../../Interfaces/IMUSICCommandMessage.h"
#include "TargetedMUSICMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages
{
	class TransferConstructIDMessage : public TargetedMUSICMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	private:
		std::shared_ptr<EntityIDRecord> OldID;
		std::shared_ptr<EntityIDRecord> NewID;

	public:
		std::shared_ptr<EntityIDRecord> getOldID() const;
		void setOldID(std::shared_ptr<EntityIDRecord> value);
		std::shared_ptr<EntityIDRecord> getNewID() const;
		void setNewID(std::shared_ptr<EntityIDRecord> value);

		TransferConstructIDMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> recieverID, std::shared_ptr<EntityIDRecord> oldID, std::shared_ptr<EntityIDRecord> newID);

		TransferConstructIDMessage(std::shared_ptr<boost::json::object> obj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const TransferConstructIDMessage& obj) const;
		};

		bool operator == (const TransferConstructIDMessage &message2);

		bool operator != (const TransferConstructIDMessage &message2);

	protected:
		std::shared_ptr<TransferConstructIDMessage> shared_from_this()
		{
			return std::static_pointer_cast<TransferConstructIDMessage>(TargetedMUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_TARGETED_MUSIC_MESSAGE_TRANSFER_CONSTRUCT_ID_MESSAGE_H