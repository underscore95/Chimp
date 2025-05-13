#pragma once

#include "stdafx.h"

namespace ChimpEditor {
	class AssetManagerScript : public Chimp::IEntityScript {
	public:
		AssetManagerScript(Chimp::EntityId entity, Chimp::Engine& engine, Chimp::ECS& ecs);

	public:
		bool HasFileSelected() const;
		const std::filesystem::path& GetSelectedFile() const;

	private:
		void OnInit() override;
		void OnUpdate() override;
		void OnRender() override;
		void OnRenderUI() override;
		
		void RenderFileSystem();
		void RenderFileUI(const std::filesystem::path& path);
		void RenderDirectoryUI(const std::filesystem::path& path);

	private:
		std::filesystem::path m_currentPath;
		std::filesystem::path m_selectedFile;
	};
}