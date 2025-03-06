/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_MUSIC_HEADER_H
#define DMI_MUSIC_MESSAGES_MUSIC_HEADER_H

#include "../Interfaces/IToJSON.h"
#include <codecvt>


namespace MUSICLibrary::MUSIC_Messages
{
	class MUSICHeader : public MUSICLibrary::Interfaces::IToJSON
	{
	private:
		unsigned int ExerciseID = 0;
		unsigned long long Timestamp = 0;

	public:
		unsigned int getExerciseID() const;
		void setExerciseID(unsigned int value);
		unsigned long long getTimestamp() const;
		void setTimestamp(unsigned long long value);

		MUSICHeader(unsigned int exerciseID, unsigned long long timestamp);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const MUSICHeader& obj) const;
		};

		bool Equals(std::any obj);

		bool operator == (const MUSICHeader &header2);

		bool operator != (const MUSICHeader &header2);
	};
}

#endif // !DMI_MUSIC_MESSAGES_MUSIC_HEADER_H