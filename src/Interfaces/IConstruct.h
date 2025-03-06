/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_ICONSTRUCT_H
#define DMI_INTERFACES_ICONSTRUCT_H

#include "../MUSIC Messages/EntityIDRecord.h"
#include "../MUSIC Messages/ConstructDataMessage.h"
#include "../MUSIC Messages/StateFieldMessage.h"
#include "../MUSICRequestMonitor.h"


namespace MUSICLibrary::Interfaces
{
	class IConstruct : public IMUSICMessageVisitor
	{
	public:
		IConstruct() {}
		virtual std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> GetID() = 0;
		virtual bool IsRemote() = 0;
		virtual std::shared_ptr<MUSICLibrary::MUSIC_Messages::ConstructDataMessage> GetConstructData() = 0;
		virtual std::shared_ptr<MUSICLibrary::MUSIC_Messages::StateFieldMessage> GetStateFieldData() = 0;
		virtual bool Parameterize(std::shared_ptr<boost::json::object> json) = 0;
		virtual std::shared_ptr<MUSICRequestStatus> RequestPrimaryControl(std::shared_ptr<IConstruct> sender) = 0;
		virtual void RelinquishPrimaryControl(std::shared_ptr<IConstruct> sender) = 0;
		virtual void RelinquishControl(std::shared_ptr<IConstruct> sender) = 0;
		virtual void SetCurrentController(std::shared_ptr<IConstruct> controller) = 0;
		virtual void OnPrimaryControlRelinquished(std::shared_ptr<IConstruct> construct) = 0;
		virtual void OnControlRelinquished(std::shared_ptr<IConstruct> construct) = 0;
		virtual void OnControlLost(std::shared_ptr<IConstruct> construct) = 0;
		virtual void OnControlRegained(std::shared_ptr<IConstruct> construct) = 0;
		virtual void OnControlRelamation(std::shared_ptr<IConstruct> construct) = 0;
		virtual void OnControlGained(std::shared_ptr<IConstruct> construct) = 0;
		virtual void OnStopConstruct() = 0;
		virtual std::shared_ptr<MUSICRequestMonitor> GetRequestMonitor() = 0;
	};
}

#endif // !DMI_INTERFACES_ICONSTRUCT_H