#pragma once

#include "stdafx.h"
#include "api/ecs/EntityId.h"

namespace Chimp {
	class Engine;
	class ECS;
	class IEntityScript;
	class Scene;

	class IGame {
		typedef std::function<std::unique_ptr<IEntityScript>(ECS&)> CreateScriptFunc;
	protected:
		IGame();

	public:
		static IGame& Instance();

		virtual ~IGame() = default;

		virtual void Setup(Engine& engine) = 0;
		virtual std::unique_ptr<Chimp::Scene> CreateRuntimeEntryScene(Chimp::Engine& engine) = 0; // when running game not in editor, set the scene

		const std::unordered_map<std::string, CreateScriptFunc>& GetScripts() const;
		std::unique_ptr<IEntityScript> CreateScript(const std::string& name, ECS& ecs); // Create script

	protected:
		void RegisterScript(const std::string& name, const CreateScriptFunc& func);

	private:
		std::unordered_map<std::string, CreateScriptFunc> m_Scripts;
		static IGame* g_Instance;
	};
}

#define CHIMP_SET_GAME(type) \
	namespace Unused { \
		const static type UnusedChimpGame;\
	}

#define CHIMP_GAME_REGISTER_SCRIPT(name, type) \
	RegisterScript(#name, [&engine](Chimp::ECS& ecs) { return UNIQUE_PTR_CAST_FROM_RAW_PTR(Chimp::IEntityScript, new type(engine, ecs)); })