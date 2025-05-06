#pragma once

#include "stdafx.h"

namespace Chimp {
	typedef int AudioFormat;
	namespace AudioFormats {
		constexpr AudioFormat INVALID = -1;

		constexpr AudioFormat MONO8 =
#ifdef CHIMP_OPENAL
			AL_FORMAT_MONO8;
#else
			INVALID;
#endif

		constexpr AudioFormat MONO16 =
#ifdef CHIMP_OPENAL
			AL_FORMAT_MONO16;
#else
			INVALID;
#endif

		constexpr AudioFormat STEREO8 =
#ifdef CHIMP_OPENAL
			AL_FORMAT_STEREO8;
#else
			INVALID;
#endif

		constexpr AudioFormat STEREO16 =
#ifdef CHIMP_OPENAL
			AL_FORMAT_STEREO16;
#else
			INVALID;
#endif 
	}
}