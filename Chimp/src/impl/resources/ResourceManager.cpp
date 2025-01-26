#include "api/resources/ResourceManager.h"
#include "api/Engine.h"
#include "api/graphics/meshes/TexturedQuad.h"
#include "Loggers.h"
#ifdef CHIMP_ASSIMP
#include "impl/resources/assimp/ModelImporter.h"
#endif

namespace Chimp {
	ResourceManager::ResourceManager(Engine& engine)
		: m_Engine(engine),
		m_MeshStorage(engine, *this),
		m_Shaders(engine),
		m_Textures(engine),
		m_Models(engine),
		m_Sprites(engine, m_Textures),
		m_Images(engine),
		m_Sounds(engine),
		m_SoundEffects(engine, m_Sounds)
	{
		InitModelImporter();
	}

	ResourceContainer<ShaderFilePaths, IShader>& ResourceManager::GetShaders()
	{
		return m_Shaders;
	}

	ResourceContainer<TextureResourcePath, ITexture>& ResourceManager::GetTextures()
	{
		return m_Textures;
	}

	ModelResourceContainer& ResourceManager::GetModels()
	{
		assert(m_ModelImporter != nullptr);
		assert(m_Models.m_ModelImporter.HasValue());
		return m_Models;
	}

	ResourceContainer<TextureResourcePath, Mesh>& ResourceManager::GetSprites()
	{
		return m_Sprites;
	}

	ResourceContainer<ImageResourcePath, IImageLoader::LoadedImage>& ResourceManager::GetImages()
	{
		return m_Images;
	}

	ResourceContainer<SoundResourcePath, ISound>& ResourceManager::GetBasicSounds() {
		return m_Sounds;
	}

	SoundEffectResourceContainer& ResourceManager::GetSoundEffects()
	{
		return m_SoundEffects;
	}

	MeshStorage& ResourceManager::GetMeshStorage()
	{
		return m_MeshStorage;
	}

	void ResourceManager::InitModelImporter()
	{
#ifdef CHIMP_ASSIMP
		m_ModelImporter = std::unique_ptr<ModelImporter>(new ModelImporter(m_Engine.GetRenderingManager(), *this));
#else
		Loggers::Resources().Error("No model importer available, can't load models.");
#endif
		m_Models.m_ModelImporter = m_ModelImporter.get();
	}

	void ResourceManager::Update() {
		m_Sounds.Update();
		m_SoundEffects.Update();
	}

	void ResourceManager::UnloadUnusedResources()
	{
		m_Shaders.UnloadUnused();
		m_Sprites.UnloadUnused();
		m_Textures.UnloadUnused();
		m_Models.UnloadUnused();
		m_Images.UnloadUnused();
		m_Sounds.UnloadUnused();
		m_SoundEffects.UnloadUnused();
	}

	void ResourceManager::LoadRequiredResources()
	{
		m_Shaders.LoadDependencies();
		m_Sprites.LoadDependencies();
		m_Textures.LoadDependencies();
		m_Models.LoadDependencies();
		m_Images.LoadDependencies();
		m_Sounds.LoadDependencies();
		m_SoundEffects.LoadDependencies();
	}
}