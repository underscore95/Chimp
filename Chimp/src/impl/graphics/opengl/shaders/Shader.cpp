#pragma once

#include "Shader.h"
#include "api/graphics/buffers/IBuffer.h"
#include "impl/graphics/opengl/textures/Texture.h"

namespace Chimp::GL {
	Shader::Shader(const ShaderFilePaths& paths,
		const ShaderCompiler& compiler)
		: m_ProgramID(0),
		m_IsValid(false),
		m_ShaderBuffers(*this)
	{
		// Compile the shaders
		const auto pathsMap = paths.AsMap();
		std::vector<GLuint> shaderIds;
		shaderIds.reserve(pathsMap.size());

		for (const auto& [type, path] : pathsMap)
		{
			if (path.empty()) continue;
			const auto result = compiler.Compile(type, FileReader::Read(path));
			if (!result.Success)
			{
				assert(false);
				HelperDeleteShaders(shaderIds);
				return;
			}
			shaderIds.push_back(result.Id);
		}

		// Combine them into a program
		m_ProgramID = glCreateProgram();
		for (const auto shaderId : shaderIds)
		{
			glAttachShader(m_ProgramID, shaderId);
		}
		glLinkProgram(m_ProgramID);

		// Check for success
		{
			GLint success;
			glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
			m_IsValid = success == GL_TRUE;
		}

		if (!m_IsValid)
		{
			std::cerr << "Failed to link shader program " << m_ProgramID << std::endl;
			GLint logLength = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0)
			{
				std::vector<char> log(logLength);
				glGetProgramInfoLog(m_ProgramID, logLength, nullptr, log.data());
				std::cerr << "Shader Program Link Error: " << log.data() << std::endl;
			}
			assert(false);
			HelperDeleteShaders(shaderIds);
			m_ProgramID = 0;
			return;
		}

		// Delete the shaders, the program contains all required data
		HelperDeleteShaders(shaderIds);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	bool Shader::IsValid() const
	{
		return m_IsValid;
	}

	IShaderBuffers& Shader::GetShaderBuffers()
	{
		return m_ShaderBuffers;
	}

	void Shader::UpdateShaderBuffer(IShaderBuffers::Index id) const
	{
		const auto& buffer = m_ShaderBuffers.GetBuffer(id);
		GLuint blockIndex = glGetUniformBlockIndex(m_ProgramID, buffer.Name.c_str()); // TODO: could we put this in the shader buffer struct?
		assert(blockIndex < std::numeric_limits<unsigned int>().max()); // Your CPU buffer name doesn't match the name in the shader
		glUniformBlockBinding(m_ProgramID, blockIndex, static_cast<GLuint>(buffer.Index));
		buffer.Buffer->BindBufferBase(buffer.Index);
	}

	void Shader::SetTextureSampler(const std::string& name, TextureSlot slot) const
	{
		Bind();

		const auto location = glGetUniformLocation(m_ProgramID, name.c_str()); // todo cache this maybe?
		assert(location != -1); // invalid uniform
		glUniform1i(location, slot);
	}

	void Shader::HelperDeleteShaders(std::vector<GLuint>& shaderIds)
	{
		for (const auto shaderId : shaderIds)
		{
			glDeleteShader(shaderId);
		}
		shaderIds.clear();
	}
}