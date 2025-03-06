/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_PARAMETERIZE_CONSTRUCT_DATA_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_PARAMETERIZE_CONSTRUCT_DATA_REQUEST_MESSAGE_H

#include "MUSICRequestMessage.h"
#include "../../Interfaces/IMUSICCommandMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class ParameterizeConstructRequestMessage : public MUSICRequestMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	private:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> GhostedID;
		std::shared_ptr<boost::json::object> ConstructParameters;

		// Constants for key lookup.
		static const std::wstring GHOSTED_ID;
		static const std::wstring CONSTRUCT_PARAMETERS;

	public:
		std::shared_ptr<EntityIDRecord> getGhostedID() const;
		void setGhostedID(std::shared_ptr<EntityIDRecord> value);
		std::shared_ptr<boost::json::object> getConstructParameters() const;
		void setConstructParameters(std::shared_ptr<boost::json::object> value);

		ParameterizeConstructRequestMessage(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> originID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> receiverID, unsigned int requestID, std::shared_ptr<boost::json::object> constructParameters, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> ghostedID = nullptr);

		ParameterizeConstructRequestMessage(std::shared_ptr<boost::json::object> jobj);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const ParameterizeConstructRequestMessage& obj) const;
		};

		bool operator == (const ParameterizeConstructRequestMessage &message2);

		bool operator != (const ParameterizeConstructRequestMessage &message2);

	protected:
		std::shared_ptr<ParameterizeConstructRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<ParameterizeConstructRequestMessage>(MUSICRequestMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_PARAMETERIZE_CONSTRUCT_DATA_REQUEST_MESSAGE_H