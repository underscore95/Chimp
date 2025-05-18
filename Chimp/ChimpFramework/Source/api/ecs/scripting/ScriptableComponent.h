#pragma once

#include "api/ecs/components/ComponentRegistry.h"
#include "stdafx.h"
#include "api/IGame.h"
#include "api/Engine.h"

namespace Chimp {
	class IEntityScript;

	struct ScriptableComponent {
		static constexpr size_t MAX_SCRIPT_NAME_LENGTH = 64;

		std::list<std::shared_ptr<IEntityScript>> Scripts;
	};

	namespace Unused {
		namespace Unused {
			class ScriptableComponentRegister : public ComponentRegister<ScriptableComponent> {
			public:
				ScriptableComponentRegister() : ComponentRegister() {
				}

				void RenderInspectorUI(EntityId id, ScriptableComponent& comp) override {
#ifdef CHIMP_EDITOR
					// List scripts
					for (const auto& script : comp.Scripts) {
						ImGui::Text(script->GetName().c_str());
						ImGui::SameLine();
						if (ImGui::Button("-")) {
							comp.Scripts.remove(script);
						}
					}

					// New script
					if (IGame::Instance().GetScripts().empty()) {
						ImGui::Text("No registered scripts!");
					}
					else if (ImGui::BeginCombo("New Script", "...")) {
						for (const auto& [name, _] : IGame::Instance().GetScripts()) {
							// Do we already have this script?
							bool alreadyAttached = false;
							for (const auto& existingScript : comp.Scripts) {
								if (existingScript->GetName() == name) {
									alreadyAttached = true;
									break;
								}
							}
							if (alreadyAttached) continue;

							// Dropdown option
							if (ImGui::Selectable(name.c_str(), false)) {
								auto script = IGame::Instance().CreateScript(name, Engine::GetEngine(), GetECS());
								assert(script);
								GetECS().GetScripts().AttachScript(id, std::move(script));
							}
						}
						ImGui::EndCombo();
					}
#endif
				}

				ScriptableComponent Deserialise(const Json& json) override {
					ScriptableComponent comp;
					for (const std::string& script : json["Scripts"]) {
						comp.Scripts.push_back(IGame::Instance().CreateScript(script, Engine::GetEngine(), GetECS()));
					}
					return comp;
				}

				void Serialise(Json& json, const ScriptableComponent& comp) override {
					std::vector<std::string> scripts;
					scripts.reserve(comp.Scripts.size());
					for (auto& script : comp.Scripts) {
						scripts.push_back(script->GetName());
					}
					json["Scripts"] = scripts;
				}
			};
			COMPONENT_REGISTER(ScriptableComponentRegister);
		}
	}
}