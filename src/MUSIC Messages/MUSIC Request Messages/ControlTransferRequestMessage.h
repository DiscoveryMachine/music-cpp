/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_CONTROL_TRANSFER_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_CONTROL_TRANSFER_REQUEST_MESSAGE_H

#include <string>
#include "ControlRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class ControlTransferRequestMessage : public ControlRequestMessage
	{
	private:
		std::shared_ptr<EntityIDRecord> ProposedController;

	public:
		std::shared_ptr<EntityIDRecord> getProposedController() const;
		void setProposedController(std::shared_ptr<EntityIDRecord> value);
	private:
		static const std::wstring PROPOSED_CONTROLLER;

	public:
		ControlTransferRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, std::shared_ptr<EntityIDRecord> targetConstruct, std::shared_ptr<boost::json::object> context, std::shared_ptr<EntityIDRecord> proposedController);

		ControlTransferRequestMessage(std::shared_ptr<boost::json::object> jobj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const ControlTransferRequestMessage& obj) const;
		};

		bool operator == (const ControlTransferRequestMessage &message2);

		bool operator != (const ControlTransferRequestMessage &message2);

	protected:
		std::shared_ptr<ControlTransferRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<ControlTransferRequestMessage>(ControlRequestMessage::shared_from_this());
		}
	};
}
#endif // !DMI_MUSIC_REQUEST_MESSAGES_CONTROL_TRANSFER_REQUEST_MESSAGE_H