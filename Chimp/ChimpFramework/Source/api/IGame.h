#pragma once

#include "stdafx.h"
#include "api/ecs/EntityId.h"

namespace Chimp {
	class Engine;
	class ECS;
	class IEntityScript;

	class IGame {
		typedef std::function<std::unique_ptr<IEntityScript>(Engine&, ECS&, Entity)> CreateScriptFunc;
	public:
		virtual void Setup(Engine& engine) = 0;

		const std::unordered_map<std::string, CreateScriptFunc>& GetScripts() const;

	protected:
		void RegisterScript(const std::string& name, const CreateScriptFunc& func);

	private:
		std::unordered_map<std::string, CreateScriptFunc> m_Scripts;
	};

	extern std::unique_ptr<IGame> ChimpGame; // ChimpGame.dll implements this using CHIMP_SET_GAME macro
}

#define CHIMP_SET_GAME(type)\
	namespace Chimp {\
		std::unique_ptr<IGame> ChimpGame = UNIQUE_PTR_CAST_FROM_RAW_PTR(type, new type());\
	}