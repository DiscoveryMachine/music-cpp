/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_CONTROL_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_CONTROL_REQUEST_MESSAGE_H

#include <string>
#include "MUSICRequestMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class ControlRequestMessage : public MUSICRequestMessage
	{
	private:
		std::shared_ptr<EntityIDRecord> TargetConstruct;
		std::shared_ptr<boost::json::object> Context;

		//constants
		static const std::wstring TARGET_CONSTRUCT;
		static const std::wstring CONTEXT;

	public:
		std::shared_ptr<EntityIDRecord> getTargetConstruct() const;
		void setTargetConstruct(std::shared_ptr<EntityIDRecord> value);
		std::shared_ptr<boost::json::object> getContext() const;
		void setContext(std::shared_ptr<boost::json::object> value);

		ControlRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, std::shared_ptr<EntityIDRecord> targetConstruct, std::shared_ptr<boost::json::object> context);

		ControlRequestMessage(std::shared_ptr<boost::json::object> jobj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;
		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const ControlRequestMessage& obj) const;
		};

		bool operator == (const ControlRequestMessage &message2);

		bool operator != (const ControlRequestMessage &message2);

	protected:
		std::shared_ptr<ControlRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<ControlRequestMessage>(TargetedMUSICMessage::shared_from_this());;
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_CONTROL_REQUEST_MESSAGE_H