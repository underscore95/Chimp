#pragma once

#include "api/graphics/buffers/Usage.h"
#include "api/graphics/GraphicsType.h"
#include "api/graphics/shaders/ShaderTypes.h"
#include "stdafx.h"
#include "api/graphics/PrimitiveType.h"

namespace Chimp::GL {
	[[nodiscard]] static constexpr GLenum BindTargetTranslation(const BindTarget target) {
		switch (target) {
		case BindTarget::VERTEX_BUFFER:
			return GL_ARRAY_BUFFER;
		case BindTarget::INDEX_BUFFER:
			return GL_ELEMENT_ARRAY_BUFFER;
		case BindTarget::SHADER_BUFFER:
			return GL_UNIFORM_BUFFER;
		default:
			assert(false);
			__assume(false);
		}
	}

	[[nodiscard]] static constexpr GLenum NumberTextureChannelsTranslation(const unsigned int numberTextureChannels) {
		switch (numberTextureChannels) {
		case 1:
			return GL_RED;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
		default:
			assert(false);
			__assume(false);
		}
	}

	[[nodiscard]] static GLenum PrimitiveTypeTranslation(const PrimitiveType primitiveType) {
		if (primitiveType == PrimitiveType::POINTS) {
			return GL_POINTS;
		}
		else if (primitiveType == PrimitiveType::LINES) {
			return GL_LINES;
		}
		else if (primitiveType == PrimitiveType::LINE_STRIP) {
			return GL_LINE_STRIP;
		}
		else if (primitiveType == PrimitiveType::TRIANGLES) {
			return GL_TRIANGLES;
		}
		else if (primitiveType == PrimitiveType::TRIANGLE_STRIP) {
			return GL_TRIANGLE_STRIP;
		}
		assert(false);
		__assume(false);
	}

	[[nodiscard]] static constexpr GLenum ShaderTypeTranslation(const ShaderType shaderType) {
		switch (shaderType) {
		case ShaderType::VERTEX:
			return GL_VERTEX_SHADER;
		case ShaderType::FRAGMENT:
			return GL_FRAGMENT_SHADER;
		default:
			assert(false);
			__assume(false);
		}
	}

	[[nodiscard]] static GLenum TypeTranslation(const GraphicsType& type) {
		if (type == GraphicsType::FLOAT) {
			return GL_FLOAT;
		}
		else if (type == GraphicsType::UNSIGNED_INT) {
			return GL_UNSIGNED_INT;
		}
		else if (type == GraphicsType::UNSIGNED_SHORT) {
			return GL_UNSIGNED_SHORT;
		}
		else if (type == GraphicsType::UNSIGNED_BYTE) {
			return GL_UNSIGNED_BYTE;
		}
		assert(false);
		__assume(false);
	}

	[[nodiscard]] static constexpr GLenum UsageTranslation(const Usage& usage) {
		if (usage.Access == Usage::Access::CPU_READ) {
			if (usage.UpdateFrequency == Usage::UpdateFrequency::VERY_OCCASIONAL) {
				return GL_STATIC_READ;
			}
			else if (usage.UpdateFrequency == Usage::UpdateFrequency::OCCASIONAL) {
				return GL_DYNAMIC_READ;
			}
			else {
				return GL_STREAM_READ;
			}
		}
		else if (usage.Access == Usage::Access::CPU_WRITE) {
			if (usage.UpdateFrequency == Usage::UpdateFrequency::VERY_OCCASIONAL) {
				return GL_STATIC_DRAW;
			}
			else if (usage.UpdateFrequency == Usage::UpdateFrequency::OCCASIONAL) {
				return GL_DYNAMIC_DRAW;
			}
			else {
				return GL_STREAM_DRAW;
			}
		}
		else {
			if (usage.UpdateFrequency == Usage::UpdateFrequency::VERY_OCCASIONAL) {
				return GL_STATIC_COPY;
			}
			else if (usage.UpdateFrequency == Usage::UpdateFrequency::OCCASIONAL) {
				return GL_DYNAMIC_COPY;
			}
			else {
				return GL_STREAM_COPY;
			}
		}
		assert(false);
		__assume(false);
	}
}