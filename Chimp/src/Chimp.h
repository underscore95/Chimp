#pragma once

#include "api/scenes/SceneManager.h"
#include "api/Engine.h"
#include "api/window/IWindow.h"
#include "api/graphics/IRenderingManager.h"
#include "api/graphics/shaders/shaders/lit/LitShadowShader.h"
#include "api/graphics/meshes/TexturedQuad.h"
#include "api/graphics/camera/controllers/FreeCameraController.h"
#include "api/ecs/ECS.h"
#include "api/ecs/Components.h"
#include "api/networking/PacketTypeRegistry.h"
#include "api/graphics/GameShader.h"
#include "api/audio/IPlayingAudio.h"

// Utils
#include "api/utils/FileReader.h"
#include "api/utils/HashCombine.h"
#include "api/utils/Maths.h"
#include "api/utils/OptionalReference.h"
#include "api/utils/Path.h"
#include "api/utils/RawArray.h"
#include "api/threading/ThreadSafeQueue.h"
#include "api/utils/UpdateSubscriber.h"
#include "api/utils/InPlaceOptional.h"
#include "api/utils/waves/WaveManager.h"
#include "api/threading/ThreadPool.h"
#include "api/utils/preprocessor/Casting.h"