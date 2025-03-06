/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "DefaultMessageFilter.h"

using namespace MUSICLibrary::Interfaces;
using namespace MUSICLibrary::MUSIC_Messages;
using namespace MUSICLibrary::MUSIC_Messages::Targeted_MUSIC_Messages;

namespace MUSICLibrary::MessageFilters
{

	DefaultMessageFilter::DefaultMessageFilter(std::shared_ptr<SiteAndAppID> siteAndApp, unsigned int exerciseID)
	{
		this->siteAndApp = siteAndApp;
		this->exerciseID = exerciseID;
	}

	void DefaultMessageFilter::OnHandledMessage(std::shared_ptr<MUSICMessage> message)
	{
	}

	bool DefaultMessageFilter::ShouldDiscard(std::shared_ptr<MUSICMessage> message)
	{
		if (std::dynamic_pointer_cast<TargetedMUSICMessage>(message) != nullptr)
		{
			if ((std::static_pointer_cast<TargetedMUSICMessage>(message))->getOriginID()->Equals((std::static_pointer_cast<TargetedMUSICMessage>(message))->getReceiverID()))
			{
				return true;
			}

			return !(std::static_pointer_cast<TargetedMUSICMessage>(message))->getReceiverID()->GetSiteAndApp()->Equals(siteAndApp) || message->getMUSICHeader()->getExerciseID() != exerciseID || message->getOriginID()->GetSiteAndApp()->Equals(siteAndApp);
		}

		return message->getMUSICHeader()->getExerciseID() != exerciseID || message->getOriginID()->GetSiteAndApp()->Equals(siteAndApp);
	}
}
