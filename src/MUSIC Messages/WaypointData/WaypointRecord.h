/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_WAYPOINT_DATA_WAYPOINT_RECORD_H
#define DMI_WAYPOINT_DATA_WAYPOINT_RECORD_H

#include <string>
#include "../../Interfaces/IToJSON.h"
#include "../MUSICVector3.h"



namespace MUSICLibrary::MUSIC_Messages
{
	class WaypointRecord : public MUSICLibrary::Interfaces::IToJSON
	{
	private:
		std::shared_ptr<MUSICVector3> worldCoordinateRecord;
		unsigned int estimatedArrivalTime = 0;
		unsigned int arrivalTimeError = 0;

	public:
		std::shared_ptr<MUSICVector3> getWorldCoordinateRecord() const;
		void setWorldCoordinateRecord(std::shared_ptr<MUSICVector3> value);
		unsigned int getEstimatedArrivalTime() const;
		void setEstimatedArrivalTime(unsigned int value);
		unsigned int getArrivalTimeError() const;
		void setArrivalTimeError(unsigned int value);

		// Constants for Key lookups.
	private:
		static const std::wstring WORLD_COORDINATE_RECORD;
		static const std::wstring ESTIMATED_ARRIVAL_TIME;
		static const std::wstring ARRIVAL_TIME_ERROR;

	public:
		WaypointRecord();

		WaypointRecord(std::shared_ptr<MUSICVector3> worldCoordinateRecord, unsigned int estimatedArrivalTime, unsigned int arrivalTimeError);

		WaypointRecord(std::shared_ptr<boost::json::object> jobj);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const WaypointRecord& obj) const;
		};

		bool operator == (const WaypointRecord &record2);

		bool operator != (const WaypointRecord &record2);
	};
}

#endif // !DMI_WAYPOINT_DATA_WAYPOINT_RECORD_H