/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_SITE_AND_APP_ID_H
#define DMI_MUSIC_MESSAGES_SITE_AND_APP_ID_H

#include <any>
#include <memory>
#include"../Interfaces/IToJSON.h"



namespace MUSICLibrary::MUSIC_Messages
{
	class SiteAndAppID
	{
	private:
		unsigned int SiteID = 0;
		unsigned int AppID = 0;

	public:
		SiteAndAppID();

		SiteAndAppID(unsigned int siteID, unsigned int appID);

		unsigned int getSiteID() const;
		void setSiteID(unsigned int value);
		unsigned int getAppID() const;
		void setAppID(unsigned int value);

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const SiteAndAppID& obj) const;
		};

		/**Converts the object into a Json Object.
		* @return JObject : the converted object's parameters*/
		virtual std::shared_ptr<boost::json::object> ToJsonObject();

		bool operator == (const SiteAndAppID &iD2);

		bool operator != (const SiteAndAppID &iD2);
	};
}

#endif // !DMI_MUSIC_MESSAGES_SITE_AND_APP_ID_H