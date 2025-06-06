#pragma once

#include "Lights.h"
#include "api/ecs/components/ComponentRegistry.h"
#include "api/imgui/ImGuiMacros.h"

namespace Chimp {
	struct AmbientLight {
		Vector3f Color = { 0.1f, 0.1f, 0.1f };
	};

	namespace Unused {
		class PointLightComponentRegister : public ComponentRegister<PointLight> {
		public:
			void RenderInspectorUI(EntityId id, PointLight& comp) override {
				CHIMP_SET_IMGUI_CONTEXT_IN_DOMAIN_ONCE();

				ImGui::InputFloat3(std::format("Color##{}", (long)id).c_str(), comp.Color.Data());
				ImGui::InputFloat3(std::format("Attenuation##{}", (long)id).c_str(), comp.Attenuation.Data());

				ImGui::InputFloat(std::format("Far Plane##{}", (long)id).c_str(), &comp.FarPlane, 1, 10);
				ImGui::InputInt(std::format("Cube Root Shadow Samples##{}", (long)id).c_str(), &comp.CubeRootNumShadowSamples);
				ImGui::InputFloat(std::format("Shadow Bias##{}", (long)id).c_str(), &comp.ShadowBias, 0.01f, 0.05f);
				ImGui::InputFloat(std::format("Max Shadow Sample Distance##{}", (long)id).c_str(), &comp.ShadowMaxSampleDistance, 0.01f, 0.05f);
			}

			PointLight Deserialise(const Json& json) override {
				PointLight pl{};
				pl.Color = Vector3f(json["Color"]);
				pl.Attenuation = Vector3f(json["Attenuation"]);
				pl.FarPlane = json["FarPlane"];
				pl.CubeRootNumShadowSamples = json["CubeRootNumShadowSamples"];
				pl.ShadowBias = json["ShadowBias"];
				pl.ShadowMaxSampleDistance = json["ShadowMaxSampleDistance"];
				return pl;
			}

			void Serialise(Json& json, const PointLight& comp) override {
				json["Color"] = comp.Color;
				json["Attenuation"] = comp.Attenuation;
				json["FarPlane"] = comp.FarPlane;
				json["CubeRootNumShadowSamples"] = comp.CubeRootNumShadowSamples;
				json["ShadowBias"] = comp.ShadowBias;
				json["ShadowMaxSampleDistance"] = comp.ShadowMaxSampleDistance;
			}
		};
		COMPONENT_REGISTER(PointLightComponentRegister);
	}

	namespace Unused {
		class DirectionalLightComponentRegister : public ComponentRegister<DirectionalLight> {
		public:
			void RenderInspectorUI(EntityId id, DirectionalLight& comp) override {
				CHIMP_SET_IMGUI_CONTEXT_IN_DOMAIN_ONCE();

				ImGui::InputFloat3(std::format("Color##{}", (long)id).c_str(), comp.Color.Data());

				ImGui::InputFloat(std::format("Shadow Bias##{}", (long)id).c_str(), &comp.ShadowBias, 0.01f, 0.05f);
				ImGui::InputInt(std::format("Sqrt Shadow Samples##{}", (long)id).c_str(), &comp.SqrtNumShadowSamples);
			}

			DirectionalLight Deserialise(const Json& json) override {
				DirectionalLight dl{};
				dl.Color = Vector3f(json["Color"]);
				dl.ShadowBias = json["ShadowBias"];
				dl.SqrtNumShadowSamples = json["SqrtNumShadowSamples"];
				return dl;
			}

			void Serialise(Json& json, const DirectionalLight& comp) override {
				json["Color"] = comp.Color;
				json["ShadowBias"] = comp.ShadowBias;
				json["SqrtNumShadowSamples"] = comp.SqrtNumShadowSamples;
			}
		};
		COMPONENT_REGISTER(DirectionalLightComponentRegister);


		class SpotlightComponentRegister : public ComponentRegister<Spotlight> {
		public:
			void RenderInspectorUI(EntityId id, Spotlight& comp) override {
				CHIMP_SET_IMGUI_CONTEXT_IN_DOMAIN_ONCE();

				ImGui::InputFloat3(std::format("Color##{}", (long)id).c_str(), comp.Color.Data());
				ImGui::InputFloat3(std::format("Attenuation##{}", (long)id).c_str(), comp.Attenuation.Data());

				if (ImGui::InputFloat(std::format("Cutoff Angle (deg)##{}", (long)id).c_str(), comp.GetCutoffAngleDegreesPtr())) {
					comp.SetCutoffAngleDegrees(comp.GetCutoffAngleDegrees());
				}

				ImGui::InputFloat(std::format("Shadow Bias##{}", (long)id).c_str(), &comp.ShadowBias, 0.01f, 0.05f);
				ImGui::InputInt(std::format("Sqrt Shadow Samples##{}", (long)id).c_str(), &comp.SqrtNumShadowSamples);
			}

			Spotlight Deserialise(const Json& json) override {
				Spotlight sl{};
				sl.Color = Vector3f(json["Color"]);
				sl.Attenuation = Vector3f(json["Attenuation"]);
				sl.SetCutoffAngleDegrees(json["CutoffAngleDegrees"]);
				sl.ShadowBias = json["ShadowBias"];
				sl.SqrtNumShadowSamples = json["SqrtNumShadowSamples"];
				return sl;
			}

			void Serialise(Json& json, const Spotlight& comp) override {
				json["Color"] = comp.Color;
				json["Attenuation"] = comp.Attenuation;
				json["CutoffAngleDegrees"] = comp.GetCutoffAngleDegrees();
				json["ShadowBias"] = comp.ShadowBias;
				json["SqrtNumShadowSamples"] = comp.SqrtNumShadowSamples;
			}
		};
		COMPONENT_REGISTER(SpotlightComponentRegister);
	}

	class AmbientLightComponentRegister : public ComponentRegister<AmbientLight> {
	public:
		void RenderInspectorUI(EntityId id, AmbientLight& comp) override {
			CHIMP_SET_IMGUI_CONTEXT_IN_DOMAIN_ONCE();
			ImGui::InputFloat3(std::format("Ambient Color##{}", (long)id).c_str(), comp.Color.Data());
		}

		AmbientLight Deserialise(const Json& json) override {
			AmbientLight light{};
			light.Color = Vector3f(json["Color"]);
			return light;
		}

		void Serialise(Json& json, const AmbientLight& comp) override {
			json["Color"] = comp.Color;
		}
	};
	COMPONENT_REGISTER(AmbientLightComponentRegister);

}