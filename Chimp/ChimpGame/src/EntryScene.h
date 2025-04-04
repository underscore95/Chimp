#pragma once

#include "stdafx.h"

class EntryScene : public Chimp::Scene {
public:
	EntryScene(Chimp::Engine& engine);
	~EntryScene();

public:

protected:
	void OnInit() override;
	void OnActivate(std::unique_ptr<Chimp::Scene> previousScene) override;
	void OnDeactivate() override;

	void OnUpdate() override;
	void OnRender() override;
	void OnRenderUI() override;

	void LoadResources() override;
	void UnloadResources() override;

private:
	void ResetLighting(Chimp::Matrix view, Chimp::SceneLighting& lights);
	void ShadowPass(Chimp::LitShader& shader, Chimp::ECS::View<Chimp::TransformComponent, Chimp::EntityIdComponent, Chimp::MeshComponent>& view);

private:
	Chimp::Engine& m_Engine;
	Chimp::ECS m_ECS;
	Chimp::Reference<Chimp::Mesh> m_TestMesh;
	Chimp::Reference<Chimp::Mesh> m_TestSprite;
	Chimp::FreeCameraController m_Controller;
	Chimp::CameraPerspective m_Camera;

	Chimp::ModelResourcePath m_ModelPath;
	Chimp::TextureResourcePath m_TexPath;
	std::unique_ptr<Chimp::IShadowMap> m_ShadowMap;
};