/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_PERCEPTION_DATA_PERCEPTION_RECORD_H
#define DMI_PERCEPTION_DATA_PERCEPTION_RECORD_H

#include <string>
#include "../EntityIDRecord.h"
#include "../Force.h"
#include "../EntityTypeRecord.h"
#include "SensorType.h"
#include "PerceptionErrors.h"

namespace MUSICLibrary::MUSIC_Messages::PerceptionData
{
	class PerceptionRecord : public MUSICLibrary::Interfaces::IToJSON
	{
	private:

		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> perceptionID;
		MUSICLibrary::MUSIC_Messages::Force forceIDField = static_cast<MUSICLibrary::MUSIC_Messages::Force>(0);
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityTypeRecord> entityTypeRecord;
		MUSICLibrary::MUSIC_Messages::SensorType perceptionSystem = static_cast<MUSICLibrary::MUSIC_Messages::SensorType>(0);
		float accuracy = 0.0F;
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionErrors> perceptionErrors;

	public:
		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> getPerceptionID() const;
		void setPerceptionID(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> value);

		MUSICLibrary::MUSIC_Messages::Force getForceIDField() const;
		void setForceIDField(MUSICLibrary::MUSIC_Messages::Force value);

		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityTypeRecord> getEntityTypeRecord() const;
		void setEntityTypeRecord(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityTypeRecord> value);

		MUSICLibrary::MUSIC_Messages::SensorType getPerceptionSystem() const;
		void setPerceptionSystem(MUSICLibrary::MUSIC_Messages::SensorType value);

		float getAccuracy() const;
		void setAccuracy(float value);

		std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionErrors> getPerceptionErrors() const;
		void setPerceptionErrors(std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionErrors> value);

		// Constants for Key look ups.
	private:
		const std::wstring PERCEPTION_ID = L"perceptionID";
		const std::wstring FORCE_ID_FIELD = L"force";
		const std::wstring ENTITY_TYPE_RECORD = L"entityType";
		const std::wstring PERCEPTION_SYSTEM = L"system";
		const std::wstring ACCURACY = L"accuracy";
		const std::wstring PERCEPTION_ERRORS = L"perceptionErrors";

	public:
		PerceptionRecord();

		//[JsonConstructor]
		PerceptionRecord(std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> perceptionID, MUSICLibrary::MUSIC_Messages::Force forceIDField, std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityTypeRecord> entityTypeRecord, MUSICLibrary::MUSIC_Messages::SensorType perceptionSystem, float accuracy, std::shared_ptr<MUSICLibrary::MUSIC_Messages::PerceptionErrors> perceptionErrors);

		//[JsonConstructor]
		PerceptionRecord(std::shared_ptr<boost::json::object> jobj);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		std::shared_ptr<MUSICLibrary::MUSIC_Messages::EntityIDRecord> extractEntityIDRecordFromJson(boost::json::object json, std::string entityIDKey);

		bool Equals(std::any obj);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const PerceptionRecord& obj) const;
		};

		bool operator == (const PerceptionRecord &pr2);

		bool operator != (const PerceptionRecord &pr2);
	};
}

#endif // !DMI_PERCEPTION_DATA_PERCEPTION_RECORD_H