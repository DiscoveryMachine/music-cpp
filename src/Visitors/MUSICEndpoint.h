/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_VISITORS_MUSIC_ENDPOINT_H
#define DMI_VISITORS_MUSIC_ENDPOINT_H

#include "../Interfaces/IInternalMessageVisitor.h"
#include "../Interfaces/IMessageFilter.h"
#include "../MUSIC Messages/MUSICEventMessage.h"
#include "../MUSIC Messages/StateFieldMessage.h"
#include "../MUSIC Messages/ConstructDataMessage.h"
#include "../MUSIC Messages/WaypointDataMessage.h"
#include "../MUSIC Messages/PerceptionDataMessage.h"
#include "../MUSIC Messages/ControlGainedMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/ControlRequestMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/ControlTransferRequestMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/CreateConstructRequestMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/CreateEnvironmentRequestMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/FinalizeScenarioRequestMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/ParameterizeConstructRequestMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/PrimaryControlRequestMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/InteractionRequestMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/PrimaryControlGrantedMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/ControlGrantedMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/ControlTransferGrantedMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/SetCurrentControllerMessage.h"
#include "../MUSIC Messages/MUSIC Request Messages/RequestSimulationTimeMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/ControlResponseMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/ControlTransferResponseMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/CreateConstructResponseMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/CreateEnvironmentResponseMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/FinalizeScenarioResponseMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/ParameterizeConstructResponseMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/PrimaryControlResponseMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/InteractionResponseMessage.h"
#include "../MUSIC Messages/MUSIC Response Messages/SimulationTimeMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/ControlInitiatedMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/ControlLostMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/ControlReclamationMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/ControlRegainedMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/ControlReleasedMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/ControlRelinquishedMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/DisplayMessagesMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/PrimaryControlRelinquishedMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/RemoveConstructMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/ScenarioStartMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/SetSimulationTimeMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/StopConstructMessage.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/TransferConstructIDMessage.h"
#include <spdlog/spdlog.h>


namespace MUSICLibrary::Visitors
{
	class MUSICEndpoint : public MUSICLibrary::Interfaces::IMUSICMessageVisitor, public std::enable_shared_from_this<MUSICEndpoint>
	{
	private:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> SiteAndAppID;
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> OriginID;
		unsigned int ExerciseID = 0;
		std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> InternalVisitor;
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> ExternalVisitor;
		std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> MessageFilter;

		/// <summary> The DIS site and app ID for this endpoint. </summary>
	public:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> getSiteAndAppID() const;

		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> getOriginID() const;
		void setOriginID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> value);

		/// <summary> The DIS exercise ID for this endpoint. </summary>
		unsigned int getExerciseID() const;
		/// <summary> The internal visitor object for this endpoint. </summary>
		std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> getInternalVisitor() const;
		/// <summary> The external visitor object for this endpoint. </summary>
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> getExternalVisitor() const;
		/// <summary> The message filter object for this endpoint. </summary>
		std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> getMessageFilter() const;
		/// <summary> Whether the endpoint is subscribed to MUSIC. </summary>
	protected:
		bool isSubscribed = false;
		
		std::shared_ptr<MUSICEndpoint> shared_from_this() {
			return std::enable_shared_from_this<MUSICEndpoint>::shared_from_this();
		}

	public:
		// Default constructor (only use if you just need the object to toss into the MUSICLibrary InitializeEndpoint method, which will call
		//
		MUSICEndpoint() {}
		// Full Constructor
		MUSICEndpoint(std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndAppID, unsigned int exerciseID, std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> messageFilter, std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> internalVisitor, std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> externalVisitor = nullptr);

		// Works as a late constructor. We made this to get around not being able to just dynamically construct an object based on type in C++.
		void ConstructThisEndpoint(std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndAppID, unsigned int exerciseID, std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> messageFilter, std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> internalVisitor, std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> externalVisitor = nullptr);
		
		virtual void SubscribeToMUSIC();

		virtual void UnsubscribeFromMUSIC() { throw NotImplementedException(); };

		bool IsSubscribed();

		virtual void Transmit(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message);

		void Receive(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message);


		virtual void VisitMUSICEvent(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICEventMessage> msg) { throw NotImplementedException(); };
		virtual void VisitStateField(std::shared_ptr<MUSICLibrary::MUSIC_Messages::StateFieldMessage> msg) { throw NotImplementedException(); };
		virtual void VisitConstructData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ConstructDataMessage> msg) { throw NotImplementedException(); };
		virtual void VisitWaypointData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::WaypointDataMessage> msg) { throw NotImplementedException(); };
		virtual void VisitPerceptionData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionDataMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlGained(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ControlGainedMessage> msg) { throw NotImplementedException(); };

		//request MUSIC messages
		virtual void VisitControlRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlRequestMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlTransferRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlTransferRequestMessage> msg) { throw NotImplementedException(); };
		virtual void VisitCreateConstructRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateConstructRequestMessage> msg) { throw NotImplementedException(); };
		virtual void VisitCreateEnvironmentRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateEnvironmentRequestMessage> msg) { throw NotImplementedException(); };
		virtual void VisitFinalizeScenarioRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::FinalizeScenarioRequestMessage> msg) { throw NotImplementedException(); };
		virtual void VisitParameterizeConstructRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ParameterizeConstructRequestMessage> msg) { throw NotImplementedException(); };
		virtual void VisitPrimaryControlRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlRequestMessage> msg) { throw NotImplementedException(); };
		virtual void VisitInteractionRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionRequestMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlGrantedMessage> msg) { throw NotImplementedException(); };
		virtual void VisitPrimaryControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlGrantedMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlTransferGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlTransferGrantedMessage> msg) { throw NotImplementedException(); };
		virtual void VisitSetCurrentController(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::SetCurrentControllerMessage> msg) { throw NotImplementedException(); };
		virtual void VisitRequestSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::RequestSimulationTimeMessage> msg) { throw NotImplementedException(); };

		//response MUSIC messages
		virtual void VisitControlResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ControlResponseMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlTransferResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ControlTransferResponseMessage> msg) { throw NotImplementedException(); };
		virtual void VisitCreateConstructResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::CreateConstructResponseMessage> msg) { throw NotImplementedException(); };
		virtual void VisitCreateEnvironmentResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::CreateEnvironmentResponseMessage> msg) { throw NotImplementedException(); };
		virtual void VisitFinalizeScenarioResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::FinalizeScenarioResponseMessage> msg) { throw NotImplementedException(); };
		virtual void VisitParameterizeConstructResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ParameterizeConstructResponseMessage> msg) { throw NotImplementedException(); };
		virtual void VisitPrimaryControlResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::PrimaryControlResponseMessage> msg) { throw NotImplementedException(); };
		virtual void VisitInteractionResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::InteractionResponseMessage> msg) { throw NotImplementedException(); };
		virtual void VisitSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::SimulationTimeMessage> msg) { throw NotImplementedException(); };

		//targeted MUSIC messages
		virtual void VisitControlInitiated(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlInitiatedMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlLost(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlLostMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlReclamation(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlReclamationMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlRegained(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlRegainedMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlReleased(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlReleasedMessage> msg) { throw NotImplementedException(); };
		virtual void VisitControlRelinquished(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlRelinquishedMessage> msg) { throw NotImplementedException(); };
		virtual void VisitDisplayMessages(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::DisplayMessagesMessage> msg) { throw NotImplementedException(); };
		virtual void VisitPrimaryControlRelinquished(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::PrimaryControlRelinquishedMessage> msg) { throw NotImplementedException(); };
		virtual void VisitRemoveConstruct(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::RemoveConstructMessage> msg) { throw NotImplementedException(); };
		virtual void VisitScenarioStart(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ScenarioStartMessage> msg) { throw NotImplementedException(); };
		virtual void VisitSetSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::SetSimulationTimeMessage> msg) { throw NotImplementedException(); };
		virtual void VisitStopConstruct(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::StopConstructMessage> msg) { throw NotImplementedException(); };
		virtual void VisitTransferConstructID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::TransferConstructIDMessage> msg) { throw NotImplementedException(); };
	};
}

#endif // !DMI_VISITORS_MUSIC_ENDPOINT_H