/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_STATE_FIELD_MESSAGE_H
#define DMI_MUSIC_MESSAGES_STATE_FIELD_MESSAGE_H

#include <string>
#include "MUSICMessage.h"
#include "../Interfaces/IMUSICMessageVisitor.h"

namespace MUSICLibrary::MUSIC_Messages
{
	class StateFieldMessage : public MUSICMessage
	{
	private:
		std::shared_ptr<boost::json::object> StateDataObject;

	public:
		std::shared_ptr<boost::json::object> getStateDataObject() const;
		void setStateDataObject(std::shared_ptr<boost::json::object> value);

	private:
		static const std::wstring HEADER_KEY;
		static const std::wstring ORIGIN_ID_KEY;
		static const std::wstring STATE_DATA_KEY;

	public:
		StateFieldMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> originID, std::shared_ptr<boost::json::object> jObject);

		StateFieldMessage(std::shared_ptr<boost::json::object> jObject);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const StateFieldMessage& obj) const;
		};

		bool operator == (const StateFieldMessage &message2);

		bool operator != (const StateFieldMessage &message2);

	protected:
		std::shared_ptr<StateFieldMessage> shared_from_this()
		{
			return std::static_pointer_cast<StateFieldMessage>(MUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_MESSAGES_STATE_FIELD_MESSAGE_H