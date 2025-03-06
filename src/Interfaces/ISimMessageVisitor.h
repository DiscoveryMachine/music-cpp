/**
 * Copyright (c)  Discovery Machine®, Inc.

 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_ISIM_MESSAGE_VISITOR_H
#define DMI_INTERFACES_ISIM_MESSAGE_VISITOR_H

#include <memory>

namespace MUSICLibrary::MUSIC_Messages 
{
	class MUSICEventMessage;

	namespace Targeted_MUSIC_Messages 
	{
		class ControlInitiatedMessage;
		class ControlReleasedMessage;
		class TransferConstructIDMessage;
		class DisplayMessagesMessage;
		class StopConstructMessage;
		class RemoveConstructMessage;
		class SetSimulationTimeMessage;
		class ScenarioStartMessage;
	}

	namespace MUSIC_Request_Messages 
	{
		class CreateEnvironmentRequestMessage;
		class FinalizeScenarioRequestMessage;
		class CreateConstructRequestMessage;
		class ParameterizeConstructRequestMessage;
		class RequestSimulationTimeMessage;
	}

	namespace MUSIC_Response_Messages 
	{
		class CreateEnvironmentResponseMessage;
		class FinalizeScenarioResponseMessage;
		class CreateConstructResponseMessage;
		class ParameterizeConstructResponseMessage;
		class SimulationTimeMessage;
	}
}

namespace MUSICLibrary::Interfaces
{
	class ISimMessageVisitor
	{
	public:
		virtual void VisitMUSICEvent(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICEventMessage> message) = 0;
		virtual void VisitControlInitiated(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlInitiatedMessage> message) = 0;
		virtual void VisitControlReleased(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlReleasedMessage> message) = 0;
		virtual void VisitTransferConstructID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::TransferConstructIDMessage> message) = 0;
		virtual void VisitDisplayMessages(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::DisplayMessagesMessage> message) = 0;
		virtual void VisitStopConstruct(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::StopConstructMessage> message) = 0;
		virtual void VisitRemoveConstruct(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::RemoveConstructMessage> message) = 0;
		virtual void VisitSetSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::SetSimulationTimeMessage> message) = 0;
		virtual void VisitRequestSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::RequestSimulationTimeMessage> message) = 0;
		virtual void VisitCreateEnvironmentRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateEnvironmentRequestMessage> message) = 0;
		virtual void VisitCreateEnvironmentResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::CreateEnvironmentResponseMessage> message) = 0;
		virtual void VisitFinalizeScenarioRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::FinalizeScenarioRequestMessage> message) = 0;
		virtual void VisitFinalizeScenarioResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::FinalizeScenarioResponseMessage> message) = 0;
		virtual void VisitCreateConstructRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateConstructRequestMessage> message) = 0;
		virtual void VisitCreateConstructResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::CreateConstructResponseMessage> message) = 0;
		virtual void VisitParameterizeConstructRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ParameterizeConstructRequestMessage> message) = 0;
		virtual void VisitParameterizeConstructResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ParameterizeConstructResponseMessage> message) = 0;
		virtual void VisitScenarioStart(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ScenarioStartMessage> message) = 0;
		virtual void VisitSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::SimulationTimeMessage> message) = 0;
	};
}

#endif // !DMI_INTERFACES_ISIM_MESSAGE_VISITOR_H