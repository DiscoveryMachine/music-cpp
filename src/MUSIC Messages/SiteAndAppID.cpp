/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "SiteAndAppID.h"

using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	SiteAndAppID::SiteAndAppID()
	{
	}

	SiteAndAppID::SiteAndAppID(unsigned int siteID, unsigned int appID)
	{
		setSiteID(siteID);
		setAppID(appID);
	}

	unsigned int SiteAndAppID::getSiteID() const
	{
		return SiteID;
	}

	void SiteAndAppID::setSiteID(unsigned int value)
	{
		SiteID = value;
	}

	unsigned int SiteAndAppID::getAppID() const
	{
		return AppID;
	}

	void SiteAndAppID::setAppID(unsigned int value)
	{
		AppID = value;
	}

	bool SiteAndAppID::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<SiteAndAppID>))
		{
			std::shared_ptr<SiteAndAppID> other = std::any_cast<std::shared_ptr<SiteAndAppID>>(obj);

			if (getSiteID() == other->getSiteID() && 
				getAppID() == other->getAppID())
			{
				return true;
			}
		}

		return false;
	}

	int SiteAndAppID::GetHashCode() const
	{
		auto hashCode = -421393812;
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getSiteID());
		hashCode = hashCode * -1521134295 + std::hash<unsigned int>{}(getAppID());
		return hashCode;
	}

	size_t SiteAndAppID::HashFunction::operator()(const SiteAndAppID& obj) const
	{
		return obj.GetHashCode();
	}

	std::shared_ptr<object> SiteAndAppID::ToJsonObject()
	{
		object objVal;

		objVal["siteID"] = getSiteID();
		objVal["appID"] = getAppID();

		return std::make_shared<object>(objVal);
	}

	bool SiteAndAppID::operator == (const SiteAndAppID &iD2)
	{
		return this->Equals(std::make_shared<SiteAndAppID>(iD2));
	}

	bool SiteAndAppID::operator != (const SiteAndAppID &iD2)
	{
		return !(*this == iD2);
	}
}
