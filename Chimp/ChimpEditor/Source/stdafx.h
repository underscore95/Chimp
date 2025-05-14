#pragma once

#ifdef NDEBUG
static_assert(false); // Editor should only ever be built in debug mode
#endif

#include <Chimp.h>
#include <imgui.h>
#include "Logger.h"