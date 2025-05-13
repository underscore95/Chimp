#pragma once

#include "stdafx.h"
#include "AudioFormat.h"

namespace Chimp {
	class IAudioImporter {
	public:
		struct AudioData {
			DISABLE_COPY(AudioData);
			AudioData() :
				Data(),
				Format(AudioFormats::MONO16),
				Frequency(44100.0f) {
			}

			std::vector<short> Data;
			AudioFormat Format;
			int Frequency;
			float DurationSeconds;
		};

	public:
		virtual std::unique_ptr<AudioData> Load(const std::filesystem::path& path) const = 0;

	private:
	};
}