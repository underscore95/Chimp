#pragma once

#include "buffers/IElementArray.h"
#include "shaders/IShader.h"
#include "meshes/Mesh.h"
#include "camera/Camera.h"

namespace Chimp
{
	class MainLoop;

	class IRenderer
	{
		friend class MainLoop;
	protected:
		IRenderer() = default;
	public:
		virtual ~IRenderer() = default;

		// Get the default camera
		[[nodiscard]] Camera& GetDefaultCamera();

		// Set the camera
		void SetCamera(std::shared_ptr<ICamera> camera);

		// Draw a mesh
		void Draw(const Mesh& mesh, const IShader& shader) const;

		// Draw a mesh section
		virtual void Draw(const Mesh::Section& meshSection, const IShader& shader) const = 0;

		// Set the clear color
		virtual void SetClearColor(float r, float g, float b) const = 0;

	protected:
		// Get the camera matrices
		[[nodiscard]] const CameraMatrices& GetCameraMatrices() const;

		// Prepare the renderer for drawing
		// clear color buffer, etc
		virtual void StartDrawing() const = 0;

	private:
		std::shared_ptr<Camera> m_DefaultCamera = std::make_shared<Camera>();
		std::shared_ptr<ICamera> m_Camera = m_DefaultCamera;
		std::shared_ptr<IBuffer> m_ModelBuffer;
	};
}