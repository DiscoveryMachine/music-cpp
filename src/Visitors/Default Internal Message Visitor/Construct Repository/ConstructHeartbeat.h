/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_REPOSITORY_CONSTRUCT_HEARTBEAT_H
#define DMI_CONSTRUCT_REPOSITORY_CONSTRUCT_HEARTBEAT_H

#include "../../../Interfaces/IBatchOperation.h"
#include "../../../Interfaces/IMUSICTransmitter.h"
#include "../../../tangible_exception_helper.h"


namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository
{
	class ConstructHeartbeat : public MUSICLibrary::Interfaces::IBatchOperation
	{
	private:
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> Transmitter;

	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> getTransmitter() const;
		void setTransmitter(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> value);

		ConstructHeartbeat(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> transmitter);

		void ApplyTo(std::unordered_map<std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> &batch) override;
	};
}

#endif // !DMI_CONSTRUCT_REPOSITORY_CONSTRUCT_HEARTBEAT_H