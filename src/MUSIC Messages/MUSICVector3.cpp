/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MUSICVector3.h"
#include "../tangible_string_helper.h"

using namespace MUSICLibrary::Interfaces;
using namespace boost::json;

namespace MUSICLibrary::MUSIC_Messages
{

	double MUSICVector3::getX() const
	{
		return x;
	}

	void MUSICVector3::setX(double value)
	{
		x = value;
	}

	double MUSICVector3::getY() const
	{
		return y;
	}

	void MUSICVector3::setY(double value)
	{
		y = value;
	}

	double MUSICVector3::getZ() const
	{
		return z;
	}

	void MUSICVector3::setZ(double value)
	{
		z = value;
	}

	MUSICVector3::MUSICVector3()
	{
	}

	MUSICVector3::MUSICVector3(double x, double y, double z)
	{
		this->setX(x);
		this->setY(y);
		this->setZ(z);
	}

	MUSICVector3::MUSICVector3(std::shared_ptr<object> obj)
	{
		object objVal = *obj;

		if (objVal.contains("x"))
		{
			setX(objVal["x"].as_double());
			setY(objVal["y"].as_double());
			setZ(objVal["z"].as_double());
		}
		else
		{
			setX(objVal["psi"].as_double());
			setY(objVal["theta"].as_double());
			setZ(objVal["phi"].as_double());
		}
	}

	std::shared_ptr<object> MUSICVector3::ToJsonObject()
	{
		object objVal;

		objVal["x"] = getX();
		objVal["y"] = getY();
		objVal["z"] = getZ();

		return std::make_shared<object>(objVal);
	}

	std::shared_ptr<object> MUSICVector3::ToOrientationObject()
	{
		object objVal;

		objVal["psi"] = getX();
		objVal["theta"] = getY();
		objVal["phi"] = getZ();

		return std::make_shared<object>(objVal);
	}

	bool MUSICVector3::Equals(std::any obj)
	{
		if (obj.type() == typeid(std::shared_ptr<MUSICVector3>))
		{
			std::shared_ptr<MUSICVector3> other = std::any_cast<std::shared_ptr<MUSICVector3>>(obj);

			if (getX() == other->getX() && 
				getY() == other->getY() && 
				getZ() == other->getZ())
			{
				return true;
			}
		}
		else if (obj.type() == typeid(MUSICVector3))
		{
			MUSICVector3 vect = std::any_cast<MUSICVector3>(obj);

			if (getX() == vect.getX() &&
				getY() == vect.getY() &&
				getZ() == vect.getZ())
			{
				return true;
			}
		}

		return false;
	}

	bool MUSICVector3::Equals(std::shared_ptr<MUSICVector3> other)
	{
		return other != nullptr && getX() == other->getX() && getY() == other->getY() && getZ() == other->getZ();
	}

	int MUSICVector3::GetHashCode() const
	{
		auto hashCode = 373119288;
		hashCode = hashCode * -1521134295 + std::hash<double>{}(getX());
		hashCode = hashCode * -1521134295 + std::hash<double>{}(getY());
		hashCode = hashCode * -1521134295 + std::hash<double>{}(getZ());
		return hashCode;
	}

	size_t MUSICVector3::HashFunction::operator()(const MUSICVector3& obj) const
	{
		return obj.GetHashCode();
	}

	bool MUSICVector3::operator == (const MUSICVector3 &vector2)
	{
		return MUSICVector3::Equals(std::make_shared<MUSICVector3>(vector2));
	}

	bool MUSICVector3::operator != (const MUSICVector3 &vector2)
	{
		return !MUSICVector3::operator==(vector2);

	}
}
