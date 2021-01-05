#pragma once
//I'm making this the audio handling class. Basically the only wrapper class we need
//Since this will just take care of placing audio, and playing it is handled by FMOD
//For projects that make 200,000$ there has to be money paid to the makers of FMOD.



namespace Trickster {

	struct SoundInfo
	{
		FMOD::Sound* Sound;
		FMOD::Channel* Channel;
		FMOD::ChannelGroup* ChannelGroup;
		float Volume = 1.f;
		float PlaybackRate = 1.f;
	};

	enum class ChannelGroups
	{
		Music,
		SoundEffects,
		Environment
	};


	
	//Audio handling class
	//Should only be accessed through the Application class!
	class AudioPlayer
	{
	public:
		TRICKSTER_API AudioPlayer();
		TRICKSTER_API ~AudioPlayer();
		//Returns the ID of the sound that is being played
		TRICKSTER_API int PlayInGroup(std::string a_FileName, FMOD::ChannelGroup* a_Group = nullptr);
		//Changes the volume of the sound instance with a delta
		//This is capped at the sound of m_MasterVolume
		TRICKSTER_API bool ChangeVolume(int a_ID, float a_DeltaVolume);
		//Sets the volume of the sound instance (should be between 0 and 1)
		TRICKSTER_API bool SetVolume(int a_ID, float a_Volume);
		//Gets the volume of the sound instance (should be between 0 and 1)
		TRICKSTER_API float GetVolume(int a_ID);
		//Plays the sound at the filename
		//If the sound isn't loaded yet, it loads
		//and saves that for later use. Plays it after
		//loading the sound.
		TRICKSTER_API int Play(std::string a_FileName, bool a_3D = false, bool a_Looping = false, bool a_Stream = false);
		//Sets the playback speed to 0
		TRICKSTER_API void Pause(int a_ID);
		//Sets the playback speed to what it was before pausing
		TRICKSTER_API void Unpause(int a_ID);
		//Sets the playback rate of the sound instance
		//Instance specific!
		//If you want to change the playback rate of the original sound
		//Use AudioPlayer::SetBasePlaybackRate
		TRICKSTER_API void SetPlaybackRate(int a_ID, float a_PlaybackRate);
		//Sets the playback rate of the loaded sound
		//Should not be used if you want to change 1 specific instance
		//See AudioPlayer::SetPlaybackRate for that usecase
		TRICKSTER_API void SetBasePlaybackRate(std::string a_FileName, float a_PlaybackRate);
		//You don't need to load sounds manually.
		//But you can load the sounds you want to use beforehand.
		//So that you won't block the main thread on runtime
		//when you want to use a sound for the first time.
		TRICKSTER_API bool LoadSound(std::string a_FileName, bool a_3D = false, bool a_Looping = false, bool a_Stream = false);
		//This should never be used on a different thread, that would be really useless
		//Note that you won't be able to use the sound immediately after calling
		TRICKSTER_API bool LoadSoundAsync(std::string a_FileName, bool a_3D = false, bool a_Looping = false, bool a_Stream = false);
		//Should be called after making an instance of this class
		TRICKSTER_API bool Initialize();
		//Updates the sounds that are playing
		TRICKSTER_API void Update(float a_DeltaTime);

	private:
		FMOD::System* m_System;
		//String: Sound Name
		//FMOD::Sound*: Sound handle
		std::unordered_map<std::string, FMOD::Sound*> m_LoadedSounds;
		
		//Int ID
		//FMOD::Sound* Sound
		//FMOD::Channel* Channel
		//FMOD::ChannelGroup* ChannelGroup
		std::unordered_map<int, SoundInfo*> m_SoundsPlaying;
		
		//Int: ID
		//Float: Time since ended
		std::unordered_map<int, float> m_FinishedSounds;
		std::map<ChannelGroups, FMOD::ChannelGroup*> m_ChannelGroups;
		std::string SoundDir;
		float m_VolumeMultiplier = 0.5f;
		float m_MasterVolume = 0.7f;
		int NextID;
	};
}