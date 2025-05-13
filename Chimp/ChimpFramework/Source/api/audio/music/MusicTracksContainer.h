#pragma once

#include "stdafx.h"

namespace Chimp {
	class Random;

	class MusicTracksContainer {
	public:
		MusicTracksContainer(const std::vector<std::string>& trackList);
		MusicTracksContainer(const MusicTracksContainer& other) = default;

		[[nodiscard]] const std::string& GetRandomTrack(Chimp::Random& random) const;

		[[nodiscard]] const std::vector<std::string>& GetTrackList() const;

		[[nodiscard]] size_t GetTrackCount() const;

		[[nodiscard]] bool IsEmpty() const;
		
	private:
		std::vector<std::string> m_TrackList;
	};
}