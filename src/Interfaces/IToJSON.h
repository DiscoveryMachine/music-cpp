/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_INTERFACES_ITO_JSON_H
#define DMI_INTERFACES_ITO_JSON_H

#include <memory>
#include <any> // Needed for any_casts in children Equals()
#include <boost/json.hpp>
#include "../tangible_string_helper.h"


namespace MUSICLibrary::Interfaces
{
	class IToJSON
	{
	public:
		virtual std::shared_ptr<boost::json::object> ToJsonObject() = 0;
	};
}

#endif // !DMI_INTERFACES_ITO_JSON_H