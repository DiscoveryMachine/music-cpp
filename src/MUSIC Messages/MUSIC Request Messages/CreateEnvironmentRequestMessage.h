/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_REQUEST_MESSAGES_CREATE_ENVIRONMENT_REQUEST_MESSAGE_H
#define DMI_MUSIC_REQUEST_MESSAGES_CREATE_ENVIRONMENT_REQUEST_MESSAGE_H

#include "MUSICRequestMessage.h"
#include "../../Interfaces/IMUSICCommandMessage.h"
#include "../../Interfaces/IMUSICMessageVisitor.h"


namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages
{
	class CreateEnvironmentRequestMessage : public MUSICRequestMessage, public MUSICLibrary::Interfaces::IMUSICCommandMessage
	{
	private:
		std::wstring EnvironmentName;
		std::shared_ptr<boost::json::object> EnvironmentMetaData;

		static const std::wstring ENVIRONMENT_NAME;
		static const std::wstring ENVIRONMENT_META_DATA;

	public:
		std::wstring getEnvironmentName() const;
		void setEnvironmentName(const std::wstring &value);
		std::shared_ptr<boost::json::object> getEnvironmentMetaData() const;
		void setEnvironmentMetaData(std::shared_ptr<boost::json::object> value);

		CreateEnvironmentRequestMessage(std::shared_ptr<boost::json::object> jobj);

		CreateEnvironmentRequestMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<EntityIDRecord> receiverID, unsigned int requestID, const std::wstring &environmentName, std::shared_ptr<boost::json::object> environmentMetaData);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		unsigned int GetCommandIdentifier() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const CreateEnvironmentRequestMessage& obj) const;
		};

		bool operator == (const CreateEnvironmentRequestMessage &message2);

		bool operator != (const CreateEnvironmentRequestMessage &message2);

	protected:
		std::shared_ptr<CreateEnvironmentRequestMessage> shared_from_this()
		{
			return std::static_pointer_cast<CreateEnvironmentRequestMessage>(MUSICRequestMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_REQUEST_MESSAGES_CREATE_ENVIRONMENT_REQUEST_MESSAGE_H