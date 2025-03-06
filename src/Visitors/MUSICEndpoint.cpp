/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICEndpoint.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Request_Messages;
using namespace MUSICLibrary::MUSIC_Messages::MUSIC_Response_Messages;
using namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages;

namespace MUSICLibrary::Visitors
{

	std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> MUSICEndpoint::getSiteAndAppID() const
	{
		return SiteAndAppID;
	}

	std::shared_ptr<EntityIDRecord> MUSICEndpoint::getOriginID() const
	{
		return OriginID;
	}

	void MUSICEndpoint::setOriginID(std::shared_ptr<EntityIDRecord> value)
	{
		OriginID = value;
	}

	unsigned int MUSICEndpoint::getExerciseID() const
	{
		return ExerciseID;
	}

	std::shared_ptr<IInternalMessageVisitor> MUSICEndpoint::getInternalVisitor() const
	{
		return InternalVisitor;
	}

	std::shared_ptr<IMUSICMessageVisitor> MUSICEndpoint::getExternalVisitor() const
	{
		return ExternalVisitor;
	}

	std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> MUSICEndpoint::getMessageFilter() const
	{
		return MessageFilter;
	}

	MUSICEndpoint::MUSICEndpoint(std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndAppID, unsigned int exerciseID, std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> messageFilter, std::shared_ptr<IInternalMessageVisitor> internalVisitor, std::shared_ptr<IMUSICMessageVisitor> externalVisitor)
	{
		SiteAndAppID = siteAndAppID;
		ExerciseID = exerciseID;
		MessageFilter = messageFilter;
		InternalVisitor = internalVisitor;
		ExternalVisitor = externalVisitor;
		setOriginID(std::make_shared<EntityIDRecord>(getSiteAndAppID(), 0));
	}

	void MUSICEndpoint::ConstructThisEndpoint(std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndAppID, unsigned int exerciseID, std::shared_ptr<MUSICLibrary::Interfaces::IMessageFilter> messageFilter, std::shared_ptr<IInternalMessageVisitor> internalVisitor, std::shared_ptr<IMUSICMessageVisitor> externalVisitor)
	{
		SiteAndAppID = siteAndAppID;
		ExerciseID = exerciseID;
		MessageFilter = messageFilter;
		InternalVisitor = internalVisitor;
		ExternalVisitor = externalVisitor;
		setOriginID(std::make_shared<EntityIDRecord>(getSiteAndAppID(), 0));
	}

	bool MUSICEndpoint::IsSubscribed()
	{
		return isSubscribed;
	}

	void MUSICEndpoint::SubscribeToMUSIC()
	{
		isSubscribed = true;
	}

	void MUSICEndpoint::Transmit(std::shared_ptr<MUSICMessage> message)
	{
		message->AcceptVisitor(shared_from_this());
	}

	void MUSICEndpoint::Receive(std::shared_ptr<MUSICMessage> message)
	{
		bool shouldDiscard = getMessageFilter()->ShouldDiscard(message);
		if (!shouldDiscard)
		{
			try {
				message->AcceptVisitor(getInternalVisitor());

				if (getExternalVisitor() != nullptr)
				{
					message->AcceptVisitor(getExternalVisitor());
				}
			}
			catch (NotImplementedException e)
			{
				spdlog::get("MUSICLibrary Logger")->error("Hit a NotImplementedException in Receive\t\n{}", e.what());
			}
			catch (InvalidOperationException e)
			{
				spdlog::get("MUSICLibrary Logger")->error("Hit a InvalidOperationException in Receive\t\n{}", e.what());
			}
		}

		getMessageFilter()->OnHandledMessage(message);
	}
}
