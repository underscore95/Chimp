#include "api/audio/music/MusicTracksContainer.h"
#include "api/utils/Random.h"

namespace Chimp {
	MusicTracksContainer::MusicTracksContainer(const std::vector<std::string>& trackList)
		: m_TrackList(trackList)
	{
	}

	const std::string& MusicTracksContainer::GetRandomTrack(Chimp::Random& random) const
	{
		assert(!IsEmpty());

		return m_TrackList[random.GetRandomIntExclusive(0, m_TrackList.size())];
	}

	const std::vector<std::string>& MusicTracksContainer::GetTrackList() const
	{
		return m_TrackList;
	}

	size_t MusicTracksContainer::GetTrackCount() const
	{
		return m_TrackList.size();
	}

	bool MusicTracksContainer::IsEmpty() const
	{
		return m_TrackList.empty();
	}
}