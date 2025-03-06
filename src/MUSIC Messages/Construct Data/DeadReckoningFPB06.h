﻿/*
 * Copyright (c) 2006-2011, Naval Postgraduate School, MOVES Institute All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Naval Postgraduate School, MOVES Institute, nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY NPS AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DMI_CONSTRUCT_DATA_DEAD_RECKONING_FPB06_H
#define DMI_CONSTRUCT_DATA_DEAD_RECKONING_FPB06_H

#include <vector>
#include <cmath>
#include "IDeadReckoningAlgorithm.h"
#include "PhysicalRecord.h"
#include "../../OpenDis/CoordinateConversion.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class DeadReckoningFPB06 : public MUSICLibrary::MUSIC_Messages::Construct_Data::IDeadReckoningAlgorithm
	{
	public:
		void ApplyDeadReckoning(std::shared_ptr<PhysicalRecord> record, float secondsToProject);

		unsigned char GetAlgorithm() override;

		bool Equals(std::any obj);

		bool operator == (const DeadReckoningFPB06 &dr2);

		bool operator != (const DeadReckoningFPB06 &dr2);
	};
}

#endif // !DMI_CONSTRUCT_DATA_DEAD_RECKONING_FPB06_H
