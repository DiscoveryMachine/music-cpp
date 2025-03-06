/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_MUSIC_VECTOR_3_H
#define DMI_MUSIC_MESSAGES_MUSIC_VECTOR_3_H

#include <codecvt>
#include "../Interfaces/IToJSON.h"


namespace MUSICLibrary::MUSIC_Messages
{
	class MUSICVector3 : public MUSICLibrary::Interfaces::IToJSON
	{
	private:
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

	public:
		double getX() const;
		void setX(double value);

		double getY() const;
		void setY(double value);

		double getZ() const;
		void setZ(double value);

		MUSICVector3();

		MUSICVector3(double x, double y, double z);

		MUSICVector3(std::shared_ptr<boost::json::object> obj);

		std::shared_ptr<boost::json::object> ToJsonObject() override;

		std::shared_ptr<boost::json::object> ToOrientationObject();

		bool Equals(std::any obj);

		bool Equals(std::shared_ptr<MUSICVector3> other);

		bool operator == (const MUSICVector3 &vector2);

		bool operator != (const MUSICVector3 &vector2);

		int GetHashCode() const;

		struct HashFunction
		{
			size_t operator()(const MUSICVector3& obj) const;
		};
	};
}

#endif // !DMI_MUSIC_MESSAGES_MUSIC_VECTOR_3_H