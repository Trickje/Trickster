#pragma once
//I'm making this the audio handling class. Basically the only wrapper class we need
//Since this will just take care of placing audio, and playing it is handled by FMOD
//For projects that make 200,000$ there has to be money paid to the makers of FMOD.



namespace Trickster {
	//Audio handling class
	//Should only be accessed through the Application class!
	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();
		void Play(std::string a_FileName);
		bool LoadSound(std::string a_FileName, bool a_3D = false, bool a_Looping = false, bool a_Stream = false);
		bool LoadSoundAsync(std::string a_FileName, bool a_3D = false, bool a_Looping = false, bool a_Stream = false);
		bool Initialize();
		void Update(float a_DeltaTime);

	private:
		FMOD::System* m_System;
		//String: Sound Name
		//FMOD::Sound*: Sound handle
		std::map<std::string, FMOD::Sound*> m_LoadedSounds;
		//String: Name
		//Int: ID
		std::vector<std::pair<std::string, int>> m_SoundsPlaying;
		//String: Name
		//Float: Time since ended
		std::vector<std::pair<std::string, float>> m_FinishedSounds;
		std::string SoundDir;
	};
}