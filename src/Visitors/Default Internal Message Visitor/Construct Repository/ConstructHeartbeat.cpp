/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ConstructHeartbeat.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;

namespace MUSICLibrary::Visitors::Default_Internal_Message_Visitor::Construct_Repository
{

	std::shared_ptr<IMUSICTransmitter> ConstructHeartbeat::getTransmitter() const
	{
		return Transmitter;
	}

	void ConstructHeartbeat::setTransmitter(std::shared_ptr<IMUSICTransmitter> value)
	{
		Transmitter = value;
	}

	ConstructHeartbeat::ConstructHeartbeat(std::shared_ptr<IMUSICTransmitter> transmitter)
	{
		setTransmitter(transmitter);
	}

	void ConstructHeartbeat::ApplyTo(std::unordered_map<std::shared_ptr<EntityIDRecord>, std::shared_ptr<MUSICLibrary::Interfaces::IConstruct>> &batch)
	{
		// Send the construct data and state fields for each construct in the batch
		for (auto construct : batch)
		{
			// If construct is remote, throw exception
			if (construct.second->IsRemote())
			{
				throw InvalidOperationException();
			}

			std::shared_ptr<ConstructDataMessage> constructData = construct.second->GetConstructData();
			getTransmitter()->Transmit(constructData);

			std::shared_ptr<StateFieldMessage> stateFieldData = construct.second->GetStateFieldData();
			getTransmitter()->Transmit(stateFieldData);
		}
	}
}
