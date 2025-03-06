/**
 * Copyright (c)  Discovery Machine®, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef DMI_CONSTRUCT_DATA_CONSTRUCT_RENDER_H
#define DMI_CONSTRUCT_DATA_CONSTRUCT_RENDER_H


namespace MUSICLibrary::MUSIC_Messages
{
	enum class ConstructRender
	{
		NonPhysical, // = 0
		UnrenderedPhysical = 4,
		RenderedPhysical, // = 5 Due to how C++ enums work, start counting from last defined value
		GhostedConstruct, // = 6
		GhostedLegacy // = 7
	};
}


#endif // !DMI_CONSTRUCT_DATA_CONSTRUCT_RENDER_H