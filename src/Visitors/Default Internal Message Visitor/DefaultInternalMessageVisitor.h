/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_DEFAULT_INTERNAL_MESSAGE_VISITOR_DEFAULT_INTERNALMESSAGE_VISITOR_H
#define DMI_DEFAULT_INTERNAL_MESSAGE_VISITOR_DEFAULT_INTERNALMESSAGE_VISITOR_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <functional>
#include "../../Interfaces/IInternalMessageVisitor.h"
#include "../../Interfaces/IConstructFactory.h"
#include "../../MUSIC Messages/MUSIC Request Messages/InteractionRequestMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/InteractionResponseMessage.h"
#include "../../MUSIC Messages/MUSICEventMessage.h"
#include "../../MUSIC Messages/ControlGainedMessage.h"
#include "../../MUSIC Messages/WaypointDataMessage.h"
#include "../../MUSIC Messages/PerceptionDataMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/ControlRequestMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/PrimaryControlRequestMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/ControlTransferRequestMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/PrimaryControlGrantedMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/ControlGrantedMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/ControlTransferGrantedMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/ControlResponseMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/PrimaryControlResponseMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/ControlTransferResponseMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/SetCurrentControllerMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/SimulationTimeMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/ControlLostMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/ControlRegainedMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/ControlRelinquishedMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/PrimaryControlRelinquishedMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/ControlReclamationMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/ControlInitiatedMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/ControlReleasedMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/TransferConstructIDMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/DisplayMessagesMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/StopConstructMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/RemoveConstructMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/SetSimulationTimeMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/RequestSimulationTimeMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/CreateEnvironmentRequestMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/CreateEnvironmentResponseMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/FinalizeScenarioRequestMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/FinalizeScenarioResponseMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/CreateConstructRequestMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/CreateConstructResponseMessage.h"
#include "../../MUSIC Messages/MUSIC Request Messages/ParameterizeConstructRequestMessage.h"
#include "../../MUSIC Messages/MUSIC Response Messages/ParameterizeConstructResponseMessage.h"
#include "../../MUSIC Messages/Targeted MUSIC Messages/ScenarioStartMessage.h"
#include "../../tangible_exception_helper.h"
#include "../../tangible_string_helper.h"


namespace MUSICLibrary::Visitors
{
	class DefaultInternalMessageVisitor : public  MUSICLibrary::Interfaces::IInternalMessageVisitor
	{
	private:
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructFactory> ConstructFactory;
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> ConstructRepository;
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> Transmitter;
		std::unordered_map<std::wstring, std::vector<std::function<void(std::shared_ptr<boost::json::object>)>>> EventHandlers;

	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructFactory> getConstructFactory() const;
		void setConstructFactory(std::shared_ptr<MUSICLibrary::Interfaces::IConstructFactory> value);
		std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> getConstructRepository() const;
		void setConstructRepository(std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> value);
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> getTransmitter() const;
		void setTransmitter(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICTransmitter> value);
	private:
		std::unordered_map<std::wstring, std::vector<std::function<void(std::shared_ptr<boost::json::object>)>>> getEventHandlers() const;
		void setEventHandlers(const std::unordered_map<std::wstring, std::vector<std::function<void(std::shared_ptr<boost::json::object>)>>> &value);

		unsigned int exerciseID = 0;
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndApp;


		static const std::wstring LOCAL_EVENT_MESSAGE_ERROR_MSG;
		static const std::wstring WRONG_EXERCISE_ID_ERROR_MSG_FORMAT;
		static const std::wstring EVENT_NOT_FOUND_ERROR_MSG_FORMAT;
		static const std::wstring HANDLER_NOT_FOUND_ERROR_MSG_FORMAT;
		static const std::wstring CONSTRUCT_NOT_FOUND_ERROR_MSG;

	public:
		DefaultInternalMessageVisitor(unsigned int exerciseID, std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndApp);

		/// <summary>
		/// Initializes a local construct by creating a local construct, adding it to the repository, 
		/// then transmitting the construct data of the newly created construct.
		/// </summary>
		/// <param name="transmitter"></param>
		/// <param name="constructType"></param>
		/// <returns></returns>
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> InitializeLocalConstruct(std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo> createInfo) override;

		void RegisterEventHandler(const std::wstring &eventName, std::function<void(std::shared_ptr<boost::json::object>)> eventHandler);

		void DeregisterEventHandler(const std::wstring &eventName);

		void Initialize(std::shared_ptr<MUSICLibrary::Interfaces::IConstructFactory> constructFactory, std::shared_ptr<MUSICLibrary::Interfaces::IConstructRepository> constructRepository);

		void VisitConstructData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ConstructDataMessage> message) override;

		void VisitInteractionRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionRequestMessage> message) override;

		void VisitPrimaryControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlGrantedMessage> message) override;

		void VisitControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlGrantedMessage> message) override;

		void VisitControlTransferGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlTransferGrantedMessage> message) override;

		void VisitInteractionResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::InteractionResponseMessage> message) override;

		void VisitMUSICEvent(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICEventMessage> message) override;

		void TriggerEvent(const std::wstring &eventType, std::shared_ptr<boost::json::object> eventData);

		void VisitStateField(std::shared_ptr<MUSICLibrary::MUSIC_Messages::StateFieldMessage> message) override;

		void VisitControlGained(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ControlGainedMessage> message) override;

		void VisitWaypointData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::WaypointDataMessage> message) override;

		void VisitPerceptionData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionDataMessage> message) override;

		void VisitControlRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlRequestMessage> message) override;

		void VisitPrimaryControlRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlRequestMessage> message) override;

		void VisitControlTransferRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlTransferRequestMessage> message) override;

		void VisitControlResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ControlResponseMessage> message) override;

		void VisitPrimaryControlResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::PrimaryControlResponseMessage> message) override;

		void VisitControlTransferResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ControlTransferResponseMessage> message) override;

		void VisitSetCurrentController(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::SetCurrentControllerMessage> message) override;

		void VisitControlLost(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlLostMessage> message) override;

		void VisitControlRegained(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlRegainedMessage> message) override;

		void VisitControlRelinquished(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlRelinquishedMessage> message) override;

		void VisitPrimaryControlRelinquished(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::PrimaryControlRelinquishedMessage> message) override;

		void VisitControlReclamation(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlReclamationMessage> message) override;

		void VisitSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::SimulationTimeMessage> message) override;

		void VisitControlInitiated(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlInitiatedMessage> message) override;

		void VisitControlReleased(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlReleasedMessage> message) override;

		void VisitTransferConstructID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::TransferConstructIDMessage> message) override;

		void VisitDisplayMessages(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::DisplayMessagesMessage> message) override;

		void VisitStopConstruct(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::StopConstructMessage> message) override;

		void VisitRemoveConstruct(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::RemoveConstructMessage> message) override;

		void VisitSetSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::SetSimulationTimeMessage> message) override;

		void VisitRequestSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::RequestSimulationTimeMessage> message) override;

		void VisitCreateEnvironmentRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateEnvironmentRequestMessage> message) override;

		void VisitCreateEnvironmentResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::CreateEnvironmentResponseMessage> message) override;

		void VisitFinalizeScenarioRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::FinalizeScenarioRequestMessage> message) override;

		void VisitFinalizeScenarioResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::FinalizeScenarioResponseMessage> message) override;

		void VisitCreateConstructRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateConstructRequestMessage> message) override;

		void VisitCreateConstructResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::CreateConstructResponseMessage> message) override;

		void VisitParameterizeConstructRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ParameterizeConstructRequestMessage> message) override;

		void VisitParameterizeConstructResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ParameterizeConstructResponseMessage> message) override;

		void VisitScenarioStart(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ScenarioStartMessage> message) override;
	};
}

#endif // !DMI_DEFAULT_INTERNAL_MESSAGE_VISITOR_DEFAULT_INTERNALMESSAGE_VISITOR_H