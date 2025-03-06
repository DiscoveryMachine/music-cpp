/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_ICONSTRUCT_MESSAGE_VISITOR_H
#define DMI_INTERFACES_ICONSTRUCT_MESSAGE_VISITOR_H

#include <memory>

namespace MUSICLibrary::MUSIC_Messages 
{
	class StateFieldMessage;
	class ControlGainedMessage;
	class ConstructDataMessage;
	class WaypointDataMessage;
	class PerceptionDataMessage;

	namespace MUSIC_Request_Messages 
	{
		class ControlRequestMessage;
		class PrimaryControlRequestMessage;
		class ControlTransferRequestMessage;
		class InteractionRequestMessage;
		class ControlGrantedMessage;
		class PrimaryControlGrantedMessage;
		class ControlTransferGrantedMessage;
		class SetCurrentControllerMessage;
	}

	namespace MUSIC_Response_Messages 
	{
		class ControlResponseMessage;
		class PrimaryControlResponseMessage;
		class ControlTransferResponseMessage;
		class InteractionResponseMessage;
	}

	namespace Targeted_MUSIC_Messages
	{
		class ControlLostMessage;
		class ControlRegainedMessage;
		class ControlReclamationMessage;
		class ControlRelinquishedMessage;
		class PrimaryControlRelinquishedMessage;
	}
}

namespace MUSICLibrary::Interfaces
{
	class IConstructMessageVisitor
	{
		//MUSICMessage
	public:
		virtual void VisitStateField(std::shared_ptr<MUSICLibrary::MUSIC_Messages::StateFieldMessage> message) = 0;
		virtual void VisitControlGained(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ControlGainedMessage> message) = 0;
		virtual void VisitConstructData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ConstructDataMessage> message) = 0;
		virtual void VisitWaypointData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::WaypointDataMessage> message) = 0;
		virtual void VisitPerceptionData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionDataMessage> message) = 0;

		//MUSICRequestMessage
		virtual void VisitControlRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlRequestMessage> message) = 0;
		virtual void VisitPrimaryControlRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlRequestMessage> message) = 0;
		virtual void VisitControlTransferRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlTransferRequestMessage> message) = 0;
		virtual void VisitInteractionRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionRequestMessage> message) = 0;
		virtual void VisitControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlGrantedMessage> message) = 0;
		virtual void VisitPrimaryControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlGrantedMessage> message) = 0;
		virtual void VisitControlTransferGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlTransferGrantedMessage> message) = 0;
		virtual void VisitSetCurrentController(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::SetCurrentControllerMessage> message) = 0;

		//MUSICResponseMessage
		virtual void VisitControlResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ControlResponseMessage> message) = 0;
		virtual void VisitPrimaryControlResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::PrimaryControlResponseMessage> message) = 0;
		virtual void VisitControlTransferResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ControlTransferResponseMessage> message) = 0;
		virtual void VisitInteractionResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::InteractionResponseMessage> message) = 0;

		//TargetedMUSICMessage
		virtual void VisitControlLost(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlLostMessage> message) = 0;
		virtual void VisitControlRegained(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlRegainedMessage> message) = 0;
		virtual void VisitControlRelinquished(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlRelinquishedMessage> message) = 0;
		virtual void VisitPrimaryControlRelinquished(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::PrimaryControlRelinquishedMessage> message) = 0;
		virtual void VisitControlReclamation(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlReclamationMessage> message) = 0;
	};
}

#endif // !DMI_INTERFACES_ICONSTRUCT_MESSAGE_VISITOR_H