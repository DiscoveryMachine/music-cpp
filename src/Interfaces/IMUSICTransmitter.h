﻿/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_IMUSIC_TRANSMITTER_H
#define DMI_INTERFACES_IMUSIC_TRANSMITTER_H

#include "../MUSIC Messages/MUSICMessage.h"


namespace MUSICLibrary::Interfaces
{
	class IMUSICTransmitter
	{
	public:
		virtual void Transmit(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message) = 0;

		virtual unsigned int GetExerciseID() = 0;
		virtual std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> GetSiteAndAppID() = 0;
	};
}

#endif // !DMI_INTERFACES_IMUSIC_TRANSMITTER_H