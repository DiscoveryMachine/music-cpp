/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef MUSIC_MESSAGES_ENTITY_ID_RECORD_H
#define MUSIC_MESSAGES_ENTITY_ID_RECORD_H

#include "SiteAndAppID.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class EntityIDRecord : public SiteAndAppID
	{
	private:
		unsigned int EntityID = 0;

	public:
		unsigned int getEntityID() const;
		void setEntityID(unsigned int value);

		EntityIDRecord() {}

		EntityIDRecord(unsigned int siteID, unsigned int appID, unsigned int entityID);

		EntityIDRecord(std::shared_ptr<SiteAndAppID> siteAndApp, unsigned int entityID);

		/**Converts the object into a Json Object.
		* @return object : the converted object's parameters*/
		std::shared_ptr<boost::json::object> ToJsonObject() override;

		std::shared_ptr<SiteAndAppID> GetSiteAndApp() const;

		bool Equals(std::any obj) const;

		int GetHashCode() const;

		bool operator == (const EntityIDRecord &record2) const;

		bool operator != (const EntityIDRecord &record2) const;

		struct HashFunction
		{
			size_t operator()(const EntityIDRecord& id) const;
		};
	};
}

#endif // !MUSIC_LIBRARY_ENTITY_ID_RECORD_H

