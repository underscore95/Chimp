#pragma once

#include "stdafx.h"
#include "api/ecs/EntityId.h"

namespace Chimp {
	class Engine;
	class ECS;
	class IEntityScript;

	class IGame {
		typedef std::function<std::unique_ptr<IEntityScript>(Engine&, ECS&)> CreateScriptFunc;
	protected:
		IGame();

	public:
		static IGame& Instance();

		virtual ~IGame() = default;

		virtual void Setup(Engine& engine) = 0;

		const std::unordered_map<std::string, CreateScriptFunc>& GetScripts() const;
		std::unique_ptr<IEntityScript> CreateScript(const std::string& name, Engine& engine, ECS& ecs); // Create script

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