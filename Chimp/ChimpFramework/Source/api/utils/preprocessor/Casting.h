#pragma once

#include "stdafx.h"

#define UNIQUE_PTR_CAST_AND_MOVE(newType, uniquePtr) std::unique_ptr<newType>(static_cast<newType*>(uniquePtr.release()))

#define UNIQUE_PTR_CAST_FROM_RAW_PTR(newType, rawPtr) std::unique_ptr<newType>((newType*)rawPtr)