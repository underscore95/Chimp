#include "api/IGame.h"

namespace Chimp {
	void IGame::RegisterScript(const std::string& name, const CreateScriptFunc& func)
	{
		m_Scripts[name] = func;
	}

	const std::unordered_map<std::string, IGame::CreateScriptFunc>& IGame::GetScripts() const
	{
		return m_Scripts;
	}
}