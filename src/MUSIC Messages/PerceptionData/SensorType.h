/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_PERCEPTION_DATA_SENSOR_TYPE_H
#define DMI_PERCEPTION_DATA_SENSOR_TYPE_H

namespace MUSICLibrary::MUSIC_Messages
{
	enum class SensorType
	{
		Other,
		ESM,
		Radar,
		Sonar,
		Visual,
		Auditory
	};
}

#endif // !DMI_PERCEPTION_DATA_SENSOR_TYPE_H