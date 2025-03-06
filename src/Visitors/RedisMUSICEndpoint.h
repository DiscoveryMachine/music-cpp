/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_VISITORS_REDIS_MUSIC_ENDPOINT_H
#define DMI_VISITORS_REDIS_MUSIC_ENDPOINT_H

#include <functional>
#include <cpp_redis/cpp_redis>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "MUSICEndpoint.h"
#include "../tangible_string_helper.h"
#include "../tangible_filesystem.h"
#include "../tangible_exception_helper.h"
#include "shlobj_core.h"
#include <spdlog/spdlog.h>


namespace MUSICLibrary::Visitors
{
	class RedisMUSICEndpoint : public MUSICEndpoint
	{
	private:
		static std::wstring ConfigFilePath;
		std::shared_ptr<cpp_redis::client> Publisher;
		std::shared_ptr<cpp_redis::subscriber> Subscriber;
		std::unordered_map<std::wstring, std::wstring> Channels;

	public:
		std::unordered_map<std::wstring, std::wstring> getChannels() const;
		void setChannels(const std::unordered_map<std::wstring, std::wstring> &value);

	private:
		std::wstring subscribedChannelID;

	public:
		static std::wstring getConfigFilePath();
		static void setConfigFilePath(const std::wstring &value);

		RedisMUSICEndpoint(std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndAppID, unsigned int exerciseID, std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> messageFilter, std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> internalVisitor, std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> externalVisitor = nullptr);

		/// <summary>
		/// Subscribes to MUSIC network traffic by using the Redis subscriber to subscribe to MUSIC channels.
		/// Throws an exception if called while already subscribed.
		/// </summary>
		void SubscribeToMUSIC() override;

	private:
		void setSubscriber(std::shared_ptr<cpp_redis::subscriber>);
		void setPublisher(std::shared_ptr<cpp_redis::client>);
		void writeDefaultValuesToConfigFile();

	private:
		std::string defaultIP = "127.0.0.1";
		unsigned int defaultPort = 6379u;
		std::string defaultPassword = "";
		std::shared_ptr<spdlog::logger> _logger = spdlog::get("MUSICLibrary Logger");

	public:
		void UnsubscribeFromMUSIC() override;

		std::shared_ptr<cpp_redis::subscriber> getSubscriber();

		std::shared_ptr<cpp_redis::client> getPublisher();

		void VisitStateField(std::shared_ptr<MUSICLibrary::MUSIC_Messages::StateFieldMessage> message) override;

		void VisitConstructData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ConstructDataMessage> message) override;

		void VisitMUSICEvent(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICEventMessage> message) override;

		void VisitInteractionRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::InteractionRequestMessage> message) override;

		void VisitInteractionResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::InteractionResponseMessage> message) override;

		void VisitControlRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlRequestMessage> message) override;

		void VisitControlTransferRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlTransferRequestMessage> message) override;

		void VisitCreateConstructRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateConstructRequestMessage> message) override;

		void VisitCreateEnvironmentRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::CreateEnvironmentRequestMessage> message) override;

		void VisitFinalizeScenarioRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::FinalizeScenarioRequestMessage> message) override;

		void VisitParameterizeConstructRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ParameterizeConstructRequestMessage> message) override;

		void VisitPrimaryControlRequest(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlRequestMessage> message) override;

		void VisitControlResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ControlResponseMessage> message) override;

		void VisitControlTransferResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ControlTransferResponseMessage> message) override;

		void VisitCreateConstructResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::CreateConstructResponseMessage> message) override;

		void VisitCreateEnvironmentResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::CreateEnvironmentResponseMessage> message) override;

		void VisitFinalizeScenarioResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::FinalizeScenarioResponseMessage> message) override;

		void VisitParameterizeConstructResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::ParameterizeConstructResponseMessage> message) override;

		void VisitPrimaryControlResponse(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::PrimaryControlResponseMessage> message) override;

		void VisitControlInitiated(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlInitiatedMessage> message) override;

		void VisitControlLost(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlLostMessage> message) override;

		void VisitControlReclamation(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlReclamationMessage> message) override;

		void VisitControlRegained(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlRegainedMessage> message) override;

		void VisitControlReleased(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlReleasedMessage> message) override;

		void VisitControlRelinquished(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ControlRelinquishedMessage> message) override;

		void VisitDisplayMessages(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::DisplayMessagesMessage> message) override;

		void VisitPrimaryControlRelinquished(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::PrimaryControlRelinquishedMessage> message) override;

		void VisitControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlGrantedMessage> message) override;

		void VisitPrimaryControlGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::PrimaryControlGrantedMessage> message) override;

		void VisitControlTransferGranted(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::ControlTransferGrantedMessage> message) override;

		void VisitRemoveConstruct(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::RemoveConstructMessage> message) override;

		void VisitRequestSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::RequestSimulationTimeMessage> message) override;

		void VisitScenarioStart(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::ScenarioStartMessage> message) override;

		void VisitSetCurrentController(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages::SetCurrentControllerMessage> message) override;

		void VisitSetSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::SetSimulationTimeMessage> message) override;

		void VisitSimulationTime(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages::SimulationTimeMessage> message) override;

		void VisitStopConstruct(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::StopConstructMessage> message) override;

		void VisitTransferConstructID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages::TransferConstructIDMessage> message) override;

		void VisitControlGained(std::shared_ptr<MUSICLibrary::MUSIC_Messages::ControlGainedMessage> message) override;

		void VisitWaypointData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::WaypointDataMessage> message) override;

		void VisitPerceptionData(std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionDataMessage> message) override;

	protected:
		/*
		std::shared_ptr<RedisMUSICEndpoint> shared_from_this()
		{
			return std::static_pointer_cast<RedisMUSICEndpoint>(MUSICEndpoint::shared_from_this());
		}
		*/
	};
}

#endif // !DMI_VISITORS_REDIS_MUSIC_ENDPOINT_H