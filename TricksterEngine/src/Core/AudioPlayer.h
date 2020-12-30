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
		float Volume;
	};



	
	//Audio handling class
	//Should only be accessed through the Application class!
	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();
		//Returns the ID of the sound that is being played
		int Play(std::string a_FileName, FMOD::ChannelGroup* a_Group = nullptr);
		//Percentage??
		bool ChangeVolume(int a_ID, float a_DeltaVolume);
		void SetVolume(int a_ID, float a_Volume);
		void GetVolume(int a_ID, float* a_Volume);
		bool LoadSound(std::string a_FileName, bool a_3D = false, bool a_Looping = false, bool a_Stream = false);
		bool LoadSoundAsync(std::string a_FileName, bool a_3D = false, bool a_Looping = false, bool a_Stream = false);
		bool Initialize();
		void Update(float a_DeltaTime);

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
		std::string SoundDir;
		int NextID;
	};
}