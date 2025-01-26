#pragma once

#include "api/audio/IAudioImporter.h"
#include "Loggers.h"

#ifdef CHIMP_LIBSNDFILE
#include <../vendor/libsndfile/libsndfile-src/include/sndfile.h>
#endif

namespace Chimp {
	class AudioImporter : public IAudioImporter {
	public:
		std::unique_ptr<AudioData> Load(const std::filesystem::path& path) const override {
			SF_INFO info;
			SNDFILE* file = sf_open(path.string().c_str(), SFM_READ, &info);

			if (!file) {
				Loggers::Audio().Error("Failed to open file: " + path.string());
				Loggers::Audio().Error(sf_strerror(file));
				return nullptr;
			}

			std::vector<short> data(info.frames * info.channels);
			sf_read_short(file, data.data(), data.size());
			sf_close(file);

			std::unique_ptr<AudioData> audioData = std::make_unique<AudioData>();
			audioData->Data = std::move(data);
			audioData->Format = info.channels == 1 ? (info.format & SF_FORMAT_PCM_S8 ? AudioFormats::MONO8 : AudioFormats::MONO16) : (info.format & SF_FORMAT_PCM_S8 ? AudioFormats::STEREO8 : AudioFormats::STEREO16);
			audioData->Frequency = info.samplerate;
			audioData->DurationSeconds = static_cast<float>(info.frames) / info.samplerate;

			return std::move(audioData);
		}
	};
}