#pragma once

#include "stdafx.h"
#include "api/audio/music/MusicTracksContainer.h"

namespace Chimp {
	class Engine;
	class ISound;
	class MainLoop;
	class IPlayingAudio;

	// This is a music player
	// it handles fading between tracks
	// it doesn't stream from disk but it will load the next track in the background (thus max 2 tracks are loaded at a time) so thats good at least
	class MusicPlayer {
		friend class MainLoop;
		struct Music {
			DISABLE_COPY_AND_MOVE(Music);
			Music() : Sound(nullptr), PlayingAudio(nullptr) {}

			~Music() {
				Reset();
			}

			void Reset() {
				if (PlayingAudio) {
					PlayingAudio = nullptr;
				}
				if (Sound) {
					Sound = nullptr;
				}
			}

			std::unique_ptr<ISound> Sound;
			std::shared_ptr<IPlayingAudio> PlayingAudio;
		};
	public:
		MusicPlayer(Engine& engine);

		// Returns true if there is any music tracks
		[[nodiscard]] bool HasTracks() const;
		
		// Fade to another music track
		void SwitchMusic(std::shared_ptr<MusicTracksContainer> tracksContainer);
		void SwitchMusic(const MusicTracksContainer& tracksContainer);

		// Get amount of milliseconds it will take to fade from min to max volume
		[[nodiscard]] float GetMillisToFade() const;

		// Set min and max volume
		[[nodiscard]] void SetVolumeRange(float minVolume, float maxVolume);

		// Set music fade speed (this is the volume change per second)
		// e.g if min volume is 0 and max volume is 1 and fade speed is 0.25 then it will take 4 seconds ((1- / 0.25) to fully fade a track in or out
		void SetMusicFadeSpeed(float fadeSpeed);

		// Set position of the sound source
		void SetPosition(const Vector3f& position);

		// Set velocity of the sound source
		void SetVelocity(const Vector3f& velocity);

		// Call this once per frame if you aren't using the music player in the engine instance
		void Update();

	private:
		bool IsCurrentTrackStopped() const;

		void PlayNewTrack();

		void AsyncLoadNextTrack();

		float GetStartingVolume() const;

	private:
		Engine& m_Engine;
		std::shared_ptr<MusicTracksContainer> m_TracksContainer;
		Music m_CurrentTrack;
		Music m_NextTrack;
		float m_MusicFadeSpeed = 0.25f; // volume change per second
		float m_MinVolume = 0;
		float m_MaxVolume = 1;
		std::chrono::system_clock::time_point m_MusicFadeOutStartTime; // Time stamp which fading out has or should start
		Vector3f m_Position = { 0, 0, 0 };
		Vector3f m_Velocity = { 0, 0, 0 };

		std::recursive_mutex m_NextTrackWriteMutex;
		size_t m_NextTrackLoadId = 0;
	};
}