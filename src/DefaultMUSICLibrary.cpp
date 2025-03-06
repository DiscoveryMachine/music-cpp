/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DefaultMUSICLibrary.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::Visitors;

namespace MUSICLibrary
{

	std::shared_ptr<EntityIDRecord> DefaultMUSICLibrary::getSimulationID() const
	{
		return SimulationID;
	}

	unsigned int DefaultMUSICLibrary::getExerciseID() const
	{
		return ExerciseID;
	}

	std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> DefaultMUSICLibrary::getMessageFilter() const
	{
		return MessageFilter;
	}

	std::shared_ptr<IInternalMessageVisitor> DefaultMUSICLibrary::getInternalVisitor() const
	{
		return InternalVisitor;
	}

	std::shared_ptr<IMUSICMessageVisitor> DefaultMUSICLibrary::getExternalVisitor() const
	{
		return ExternalVisitor;
	}

	std::unordered_map<std::type_index, std::shared_ptr<MUSICEndpoint>> DefaultMUSICLibrary::getEndpoints() const
	{
		return Endpoints;
	}

	DefaultMUSICLibrary::DefaultMUSICLibrary(std::shared_ptr<SiteAndAppID> siteAndAppID, unsigned int exerciseID, std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> messageFilter, std::shared_ptr<IInternalMessageVisitor> internalVisitor, std::shared_ptr<IMUSICMessageVisitor> externalVisitor)
	{
		logger->set_level(spdlog::level::debug);
		logger->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] [thread %t] %v");
		logger->info("DefaultMUSICLibrary Constructed");
		SimulationID = std::make_shared<EntityIDRecord>(siteAndAppID, 0);
		ExerciseID = exerciseID;
		MessageFilter = messageFilter;
		InternalVisitor = internalVisitor;
		ExternalVisitor = externalVisitor;
		Endpoints = std::unordered_map<std::type_index, std::shared_ptr<MUSICEndpoint>>();
		subscribed = false;
	}

	std::shared_ptr<IMUSICLibrary> DefaultMUSICLibrary::InitializeEndpoint(std::shared_ptr<MUSICEndpoint> endpoint)
	{
		if (!subscribed)
		{
			endpoint->ConstructThisEndpoint(getSimulationID()->GetSiteAndApp(), getExerciseID(), getMessageFilter(), getInternalVisitor(), getExternalVisitor());
			Endpoints.emplace(typeid(*endpoint), endpoint);
		}
		else
		{
			throw EndpointInitializationException("Cannot initialize endpoints while subscribed to MUSIC traffic.");
		}

		return shared_from_this();
	}

	std::shared_ptr<IMUSICLibrary> DefaultMUSICLibrary::RemoveMUSICEndpoint(std::type_index type)
	{
		if (getEndpoints().count(type))
		{
			std::shared_ptr<MUSICEndpoint> endpoint = getEndpoints().at(type);

			if (endpoint->IsSubscribed())
			{
				endpoint->UnsubscribeFromMUSIC();
			}
		}

		Endpoints.erase(type);
		return shared_from_this();
	}

	void DefaultMUSICLibrary::SubscribeToMUSIC()
	{
		if (!subscribed)
		{

			for (const auto& [type, endpoint] : getEndpoints())
			{
				endpoint->SubscribeToMUSIC();
			}
		}

		subscribed = true;
	}

	void DefaultMUSICLibrary::UnsubscribeToMUSIC()
	{
		if (subscribed)
		{
			for (const auto& [type, endpoint] : getEndpoints())
			{
				endpoint->UnsubscribeFromMUSIC();
			}
		}

		subscribed = false;
	}

	void DefaultMUSICLibrary::Transmit(std::shared_ptr<MUSICMessage> message)
	{
		for (const auto& [type, endpoint] : getEndpoints())
		{
			endpoint->Transmit(message);
		}
	}

	bool DefaultMUSICLibrary::IsSubscribed()
	{
		return subscribed;
	}

	unsigned int DefaultMUSICLibrary::GetExerciseID()
	{
		return getExerciseID();
	}

	std::shared_ptr<MUSICEndpoint> DefaultMUSICLibrary::GetEndpoint(std::type_index type)
	{
		try 
		{
			return getEndpoints().at(type);
		}
		catch (std::exception e)
		{
			throw e;
		}
	}

	void DefaultMUSICLibrary::AddEndpoint(std::shared_ptr<MUSICEndpoint> endpoint)
	{
		std::type_index key = typeid(*endpoint);
		Endpoints.emplace(key, endpoint);
	}

	std::shared_ptr<SiteAndAppID> DefaultMUSICLibrary::GetSiteAndAppID()
	{
		return getSimulationID()->GetSiteAndApp();
	}


	std::shared_ptr<MUSICLibrary::Interfaces::IConstruct> DefaultMUSICLibrary::InitializeLocalConstruct(std::shared_ptr<AbstractConstructCreateInfo> createInfo)
	{
		return getInternalVisitor()->InitializeLocalConstruct(createInfo);
	}
}
