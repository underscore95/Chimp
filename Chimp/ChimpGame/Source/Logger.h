#pragma once

#include "Chimp.h"

inline Chimp::Logger& GetLogger()
{
	static Chimp::Logger Logger("Game");
	return Logger;
}