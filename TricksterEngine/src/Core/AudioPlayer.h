#pragma once
//I'm making this the audio handling class. Basically the only wrapper class we need
//Since this will just take care of placing audio, and playing it is handled by FMOD
//For projects that make 200,000$ there has to be money paid to the makers of FMOD.



//Hello World
namespace Trickster {
	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();
		void Play(std::string a_FileName);
		bool LoadSound(std::string a_FileName);
		bool LoadSoundAsync(std::string a_FileName);
		bool Initialize();
		bool Destroy();
		void Update(float a_DeltaTime);

	private:
		FMOD::Studio::System* m_System;
		std::map<std::string, FMOD::Studio::Bank*> m_LoadedSounds;
		//String: Name
		//Int: ID
		std::vector<std::pair<std::string, int>> m_SoundsPlaying;
		//String: Name
		//Float: Time since ended
		std::vector<std::pair<std::string, float>> m_FinishedSounds;
		std::string SoundDir;
	};
}