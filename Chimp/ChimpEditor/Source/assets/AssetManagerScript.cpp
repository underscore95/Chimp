#include "AssetManagerScript.h"

ChimpEditor::AssetManagerScript::AssetManagerScript(Chimp::EntityId entity,
	Chimp::Engine& engine,
	Chimp::ECS& ecs) :
	IEntityScript(entity, engine, ecs),
	m_currentPath(GAME_DATA_FOLDER),
	m_selectedFile("")
{
}

bool ChimpEditor::AssetManagerScript::HasFileSelected() const
{
	return m_selectedFile != "";
}

const std::filesystem::path& ChimpEditor::AssetManagerScript::GetSelectedFile() const
{
	return m_selectedFile;
}

void ChimpEditor::AssetManagerScript::OnInit()
{
}

void ChimpEditor::AssetManagerScript::OnUpdate()
{
}

void ChimpEditor::AssetManagerScript::OnRender()
{
}

void ChimpEditor::AssetManagerScript::OnRenderUI()
{
	ImGui::SetNextWindowPos(
		Chimp::ComponentMultiply(GetEngine().GetWindow().GetSize(), { 0.25f, 0.75f })
	);
	ImGui::SetNextWindowSize(
		Chimp::ComponentMultiply(GetEngine().GetWindow().GetSize(), { 0.5f, 0.25f })
	);
	ImGui::Begin("Asset Manager");

	RenderFileSystem();

	ImGui::End();
}

void ChimpEditor::AssetManagerScript::RenderFileSystem()
{
	// Go to parent
	bool hasParent = m_currentPath.has_parent_path() && m_currentPath != GAME_DATA_FOLDER;
	if (!hasParent) ImGui::BeginDisabled();
	auto label = hasParent ? std::format("< {}", m_currentPath.parent_path().filename().string()) : "In Root Directory";
	if (ImGui::Button(label.c_str())) {
		m_currentPath = m_currentPath.parent_path();
	}
	if (!hasParent) ImGui::EndDisabled();

	// Render file system
	for (auto& entry : std::filesystem::directory_iterator(m_currentPath)) {
		if (entry.is_directory()) RenderDirectoryUI(entry.path());
		else RenderFileUI(entry.path());
	}
}

void ChimpEditor::AssetManagerScript::RenderFileUI(const std::filesystem::path& path)
{
	bool isCurrentlySelected = m_selectedFile == path;
	auto fileName = path.filename().string();

	auto cursorPos = ImGui::GetCursorPos();
	auto label = std::format("{}", path.string());
	if (ImGui::InvisibleButton(label.c_str(), ImGui::CalcTextSize(fileName.c_str()))) {
		m_selectedFile = isCurrentlySelected ? "" : path;
	}
	ImGui::SetCursorPos(cursorPos);
	ImGui::TextColored(isCurrentlySelected ? ImVec4{ 0.7f, 0.7f, 1.0f, 1.0f } : ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f }, fileName.c_str());
}

void ChimpEditor::AssetManagerScript::RenderDirectoryUI(const std::filesystem::path& path)
{
	auto label = std::format(">##{}", path.string());
	if (ImGui::SmallButton(label.c_str())) {
		m_currentPath = path;
	}
	ImGui::SameLine();
	auto directoryName = path.filename().string();
	ImGui::Text(directoryName.c_str());
}
