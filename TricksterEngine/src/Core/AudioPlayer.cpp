#include "pch.h"
#include "AudioPlayer.h"
using namespace Trickster;
AudioPlayer::AudioPlayer()
{
	SoundDir = "Resources/Sounds/Music/MainMenu/";
}

AudioPlayer::~AudioPlayer()
{
	
	const FMOD_RESULT result = m_System->release();
	if (result != FMOD_OK)
	{
		LOG_ERROR("[Audio Player] Failed to destroy!");
	}else
	{
		LOG("[Audio Player] Destroyed.");
	}
}

void AudioPlayer::Play(std::string a_FileName)
{
	
}
bool Trickster::AudioPlayer::LoadSound(std::string a_FileName)
{
	if (m_LoadedSounds.find(a_FileName) == m_LoadedSounds.end())
	{
		FMOD::Studio::Bank* rawBankPointer;
		FMOD_RESULT result = m_System->loadBankFile((SoundDir + a_FileName).c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &rawBankPointer);
		if (result == FMOD_OK) {
			m_LoadedSounds.insert(std::pair<std::string, FMOD::Studio::Bank*>(a_FileName, rawBankPointer));
			LOG_USELESS("[Audio Player] Loaded sound " + a_FileName);
		}else
		{
			LOG_ERROR("[Audio Player] Failed to load sound " + a_FileName + "!");
			return false;
		}
	}else
	{
		LOG_USELESS("[Audio Player] Already loaded sound " + a_FileName + "!");
		return false;
	}
	return true;
}
bool Trickster::AudioPlayer::LoadSoundAsync(std::string a_FileName)
{
	if (m_LoadedSounds.find(a_FileName) == m_LoadedSounds.end())
	{
		FMOD::Studio::Bank* rawBankPointer;
		FMOD_RESULT result = m_System->loadBankFile((SoundDir + a_FileName).c_str(), FMOD_STUDIO_LOAD_BANK_NONBLOCKING, &rawBankPointer);
		if (result == FMOD_OK) {
			m_LoadedSounds.insert(std::pair<std::string, FMOD::Studio::Bank*>(a_FileName, rawBankPointer));
			LOG_USELESS("[Audio Player] Loaded sound " + a_FileName);
		}
		else
		{
			LOG_ERROR("[Audio Player] Failed to load sound " + a_FileName + "!");
			return false;
		}
	}
	else
	{
		LOG_USELESS("[Audio Player] Already loaded sound " + a_FileName + "!");
		return false;
	}
	return true;
}
bool AudioPlayer::Initialize()
{
	FMOD_RESULT result;
	m_System = nullptr;
	result = FMOD::Studio::System::create(&m_System);
	if(result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}
	result = m_System->initialize(512, FMOD_STUDIO_INIT_NORMAL,FMOD_INIT_NORMAL, 0);
	if(result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	return true;
}
bool AudioPlayer::Destroy()
{
	/*FMOD_RESULT result;
	result = m_System->release();
	if(result != FMOD_OK)
	{
		return false;
	}
	return true;*/
	return true;
}

void AudioPlayer::Update(float a_DeltaTime)
{
	
}
