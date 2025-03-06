/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_LIBRARY_DEFAULT_MUSIC_LIBRARY_H
#define DMI_MUSIC_LIBRARY_DEFAULT_MUSIC_LIBRARY_H

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <any>
#include "Interfaces/IMUSICLibrary.h"
#include "Interfaces/IMUSICTransmitter.h"
#include "tangible_exception_helper.h"
#include <spdlog/spdlog.h>


namespace MUSICLibrary
{
	class DefaultMUSICLibrary : public  MUSICLibrary::Interfaces::IMUSICLibrary, public  MUSICLibrary::Interfaces::IMUSICTransmitter, public std::enable_shared_from_this<DefaultMUSICLibrary>
	{
	private:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> SimulationID;
		unsigned int ExerciseID = 0;
		std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> MessageFilter;
		std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> InternalVisitor;
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> ExternalVisitor;
		std::unordered_map<std::type_index, std::shared_ptr<MUSICLibrary::Visitors::MUSICEndpoint>> Endpoints;

		/// <summary> The DIS site, app ID, and (entity ID = 0) for this instance of the library. </summary>
	public:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> getSimulationID() const override;
		/// <summary> Used to separate different exercises being simulated on the same site and application. </summary>
		unsigned int getExerciseID() const override;
		/// <summary> Used to filter unwanted MUSIC messages. Typically used to discard loop-back traffic. </summary>
		std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> getMessageFilter() const override;
		std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> getInternalVisitor() const override;
		std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> getExternalVisitor() const override;

	private:
		std::unordered_map<std::type_index, std::shared_ptr<MUSICLibrary::Visitors::MUSICEndpoint>> getEndpoints() const;
		bool subscribed = false;
		std::shared_ptr<spdlog::logger> logger = spdlog::get("MUSICLibrary Logger");

	public:
		DefaultMUSICLibrary(std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndAppID, unsigned int exerciseID, std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> messageFilter, std::shared_ptr<MUSICLibrary::Interfaces::IInternalMessageVisitor> internalVisitor, std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> externalVisitor = nullptr);

		std::shared_ptr<IMUSICLibrary> InitializeEndpoint(std::shared_ptr<MUSICLibrary::Visitors::MUSICEndpoint> endpoint) override;

		std::shared_ptr<IMUSICLibrary> RemoveMUSICEndpoint(std::type_index type) override;

		void SubscribeToMUSIC() override;

		void UnsubscribeToMUSIC() override;

		void Transmit(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message) override;

		bool IsSubscribed() override;

		unsigned int GetExerciseID() override;

		std::shared_ptr<MUSICLibrary::Visitors::MUSICEndpoint> GetEndpoint(std::type_index type) override;

		void AddEndpoint(std::shared_ptr<MUSICLibrary::Visitors::MUSICEndpoint> endpoint);

		std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> GetSiteAndAppID() override;

	public:
		std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> InitializeLocalConstruct(std::shared_ptr<MUSICLibrary::Interfaces::AbstractConstructCreateInfo> createInfo) override;
	};
}

#endif // !DMI_MUSIC_LIBRARY_DEFAULT_MUSIC_LIBRARY_H