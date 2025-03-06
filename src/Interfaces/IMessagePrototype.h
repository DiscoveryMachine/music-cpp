/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IMESSAGE_PROTOTYPE_H
#define DMI_INTERFACES_IMESSAGE_PROTOTYPE_H

#include "IToJSON.h"
#include "IMUSICMessageVisitor.h"
#include "../MUSIC Messages/EntityIDRecord.h"


namespace MUSICLibrary::Interfaces
{
	class IMessagePrototype : public IToJSON
	{
	public:
		virtual void AcceptVisitor(std::shared_ptr<IMUSICMessageVisitor> visitor) = 0;

		virtual std::any Clone() = 0;

		virtual std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> extractEntityIDRecordFromJson(boost::json::object json, std::string entityIDKey) = 0;
	};
}

#endif // !DMI_INTERFACES_IMESSAGE_PROTOTYPE_H