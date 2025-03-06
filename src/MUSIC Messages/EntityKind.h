/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_MUSIC_MESSAGES_ENTITY_KIND_H
#define DMI_MUSIC_MESSAGES_ENTITY_KIND_H


namespace MUSICLibrary::MUSIC_Messages
{
	enum class EntityKind
	{
		Other,
		Platform,
		Munition,
		LifeForm,
		Environmental,
		CulturalFeature,
		Supply,
		Radio,
		Expendable,
		SensorEmitter
	};
}

#endif // !DMI_MUSIC_LIBRARY_ENTITY_KIND_H