/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "EntityIDRecord.h"

using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	unsigned int EntityIDRecord::getEntityID() const
	{
		return EntityID;
	}

	void EntityIDRecord::setEntityID(unsigned int value)
	{
		EntityID = value;
	}

	EntityIDRecord::EntityIDRecord(unsigned int siteID, unsigned int appID, unsigned int entityID) : SiteAndAppID(siteID, appID)
	{
		setEntityID(entityID);
	}

	EntityIDRecord::EntityIDRecord(std::shared_ptr<SiteAndAppID> siteAndApp, unsigned int entityID) : SiteAndAppID(siteAndApp->getSiteID(), siteAndApp->getAppID())
	{
		setEntityID(entityID);
	}

	std::shared_ptr<object> EntityIDRecord::ToJsonObject()
	{
		object objVal;

		objVal["siteID"] = getSiteID();
		objVal["appID"] = getAppID();
		objVal["entityID"] = getEntityID();

		return std::make_shared<object>(objVal);
	}

	std::shared_ptr<SiteAndAppID> EntityIDRecord::GetSiteAndApp() const
	{
		return std::make_shared<SiteAndAppID>(getSiteID(), getAppID());
	}

	bool EntityIDRecord::Equals(std::any obj) const
	{
		if (obj.type() == typeid(std::shared_ptr<EntityIDRecord>))
		{
			std::shared_ptr<EntityIDRecord> other = std::any_cast<std::shared_ptr<EntityIDRecord>>(obj);

			if (this->GetSiteAndApp()->Equals(other->GetSiteAndApp()) && 
				getEntityID() == other->getEntityID())
			{
				return true;
			}
		}
		else if (obj.type() == typeid(EntityIDRecord))
		{
			EntityIDRecord other = std::any_cast<EntityIDRecord>(obj);

			if (this->GetSiteAndApp()->Equals(other.GetSiteAndApp()) &&
				getEntityID() == other.getEntityID())
			{
				return true;
			}
		}

		return false;
	}

	int EntityIDRecord::GetHashCode() const
	{
		auto hashCode = -697791091;
		hashCode = hashCode * -1521134295 + GetSiteAndApp()->GetHashCode();
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getEntityID());
		return hashCode;
	}

	bool EntityIDRecord::operator == (const EntityIDRecord &record2) const
	{
		return this->Equals(std::make_shared<EntityIDRecord>(record2));
	}

	bool EntityIDRecord::operator != (const EntityIDRecord &record2) const
	{
		return !(*this == record2);
	}

	size_t EntityIDRecord::HashFunction::operator()(const EntityIDRecord& id) const
	{
		return id.GetHashCode();
	}
}
