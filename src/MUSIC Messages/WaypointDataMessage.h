/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_WAYPOINT_DATA_MESSAGE_H
#define DMI_MUSIC_MESSAGES_WAYPOINT_DATA_MESSAGE_H

#include <string>
#include <vector>
#include <algorithm>
#include "MUSICMessage.h"
#include "MUSICVector3.h"
#include "WaypointData/WaypointRecord.h"
#include "../Interfaces/IMUSICMessageVisitor.h"



namespace MUSICLibrary::MUSIC_Messages
{
	class WaypointDataMessage : public MUSICMessage
	{
	private:
		unsigned short CurrentWaypointIndex = 0;
		std::vector<std::shared_ptr<WaypointRecord>> Waypoints;

	public:
		unsigned short getCurrentWaypointIndex() const;
		void setCurrentWaypointIndex(unsigned short value);
		std::vector<std::shared_ptr<WaypointRecord>> getWaypoints() const;
		void setWaypoints(const std::vector<std::shared_ptr<WaypointRecord>> &value);

	private:
		static const std::wstring HEADER_KEY;
		static const std::wstring ORIGIN_ID_KEY;
		static const std::wstring CURRENT_WAYPOINT_INDEX_KEY;
		static const std::wstring WAYPOINT_RECORDS_KEY;
		static const std::wstring NUM_WAYPOINTS_KEY;

	public:
		WaypointDataMessage();

		WaypointDataMessage(unsigned int exerciseID, std::shared_ptr<EntityIDRecord> entityIDRecord, unsigned short currentWaypointIndex, std::vector<std::shared_ptr<WaypointRecord>> &waypoints);

		WaypointDataMessage(std::shared_ptr<boost::json::object> jObject);

		void AcceptVisitor(std::shared_ptr<MUSICLibrary::Interfaces::IMUSICMessageVisitor> visitor) override;

		std::any Clone() override;

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const WaypointDataMessage& obj) const;
		};

		bool Equals(std::any obj);

		bool operator == (const WaypointDataMessage &message2);

		bool operator != (const WaypointDataMessage &message2);

	protected:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::WaypointDataMessage> shared_from_this()
		{
			return std::static_pointer_cast<MUSICLibrary::MUSIC_Messages::WaypointDataMessage>(MUSICMessage::shared_from_this());
		}
	};
}

#endif // !DMI_MUSIC_MESSAGES_WAYPOINT_DATA_MESSAGE_H