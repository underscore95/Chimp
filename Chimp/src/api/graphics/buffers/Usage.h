#pragma once

namespace Chimp {
	struct Usage {
		enum class UpdateFrequency {
			VERY_OCCASIONAL, // Less than once per frame
			OCCASIONAL, // Once per frame
			OFTEN // Multiple times per frame
		};

		enum class Access {
			CPU_READ,
			CPU_WRITE,
			CPU_READ_WRITE
		};

		UpdateFrequency UpdateFrequency;
		Access Access;

		bool CanRead() const {
			return Access == Access::CPU_READ || Access == Access::CPU_READ_WRITE;
		}

		bool CanWrite() const {
			return Access == Access::CPU_WRITE || Access == Access::CPU_READ_WRITE;
		}
	};

	constexpr static int NUMBERS_OF_BIND_TARGETS = 2;
	enum class BindTarget {
		VERTEX_BUFFER,
		INDEX_BUFFER,
		SHADER_BUFFER
	};
}