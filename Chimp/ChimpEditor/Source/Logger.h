#pragma once

#include "Chimp.h"

namespace ChimpEditor {
	inline Chimp::Logger& GetLogger()
	{
		static Chimp::Logger Logger("Editor");
		return Logger;
	}
}