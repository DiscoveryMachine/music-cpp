/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MESSAGE_FILTERS_DEFAULT_MESSAGE_FILTER_H
#define DMI_MESSAGE_FILTERS_DEFAULT_MESSAGE_FILTER_H

#include "../Interfaces/IMessageFilter.h"
#include "../MUSIC Messages/Targeted MUSIC Messages/TargetedMUSICMessage.h"


namespace MUSICLibrary::MessageFilters
{
	class DefaultMessageFilter : public MUSICLibrary::Interfaces::IMessageFilter
	{
	private:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndApp;
		unsigned int exerciseID = 0;

	public:
		DefaultMessageFilter(std::shared_ptr<MUSICLibrary::MUSIC_Messages::SiteAndAppID> siteAndApp, unsigned int exerciseID);

		void OnHandledMessage(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message) override;

		bool ShouldDiscard(std::shared_ptr<MUSICLibrary::MUSIC_Messages::MUSICMessage> message) override;
	};
}

#endif // !DMI_MESSAGE_FILTERS_DEFAULT_MESSAGE_FILTER_H