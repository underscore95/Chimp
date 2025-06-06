#pragma once

#include "stdafx.h"

namespace ChimpEditor {
	class AssetManagerScript;

	class AssetImporterScript : public Chimp::IEntityScript {
	public:
		AssetImporterScript(Chimp::Engine& engine, Chimp::ECS& ecs, AssetManagerScript& assetManager);

	public:

	private:
		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;

		void RenderImportAssetButton(Chimp::AssetImportState state, Chimp::AssetType& assetType);
		void RenderInvalidUI();
	private:
		AssetManagerScript& m_assetManager;
	};
}