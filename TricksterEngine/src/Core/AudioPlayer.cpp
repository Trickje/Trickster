#include "pch.h"
#include "AudioPlayer.h"
using namespace Trickster;
AudioPlayer::AudioPlayer()
{
	SoundDir = "Resources/Sounds/";
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
	auto it = m_LoadedSounds.find(a_FileName);
	if(it != m_LoadedSounds.end())
	{
		FMOD::Channel* rawChannelPointer;
		FMOD_RESULT result = m_System->playSound(it->second, nullptr, false, &rawChannelPointer);
		if(result != FMOD_OK)
		{
			LOG_ERROR("[Audio Player] Failed to play sound " + a_FileName + "!");
		}else
		{
			auto sub = a_FileName.find_last_of('/');
			if(sub >= sizeof(a_FileName) - 1)
			{

				LOG_USELESS("[Audio Player] Playing sound " + a_FileName);
			}
			else {
				std::string str = a_FileName.substr(sub + 1);
				LOG_USELESS("[Audio Player] Playing sound " + str);
			}
		}
	}
}
bool Trickster::AudioPlayer::LoadSound(std::string a_FileName, bool a_3D, bool a_Looping, bool a_Stream)
{
	if (m_LoadedSounds.find(a_FileName) == m_LoadedSounds.end())
	{
		FMOD::Sound* rawSoundPointer;
		FMOD_MODE mode = FMOD_DEFAULT;
		mode |= a_3D ? FMOD_3D : FMOD_2D;
		mode |= a_Looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		mode |= a_Stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
		FMOD_RESULT result = m_System->createSound((SoundDir + a_FileName).c_str(), mode, nullptr, &rawSoundPointer);
		if (result == FMOD_OK) {
			m_LoadedSounds.insert(std::pair<std::string, FMOD::Sound*>(a_FileName, rawSoundPointer));
			LOG_USELESS("[Audio Player] Loaded sound " + a_FileName);
		}else
		{
			LOG_ERROR("[Audio Player] Failed to load sound " + SoundDir + a_FileName +"!");
			return false;
		}
	}else
	{
		LOG_USELESS("[Audio Player] Already loaded sound " + a_FileName + "!");
		return false;
	}
	return true;
}
bool Trickster::AudioPlayer::LoadSoundAsync(std::string a_FileName, bool a_3D, bool a_Looping, bool a_Stream)
{
	if (m_LoadedSounds.find(a_FileName) == m_LoadedSounds.end())
	{
		FMOD::Sound* rawSoundPointer;
		FMOD_MODE mode = FMOD_NONBLOCKING;
		mode |= a_3D ? FMOD_3D : FMOD_2D;
		mode |= a_Looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		mode |= a_Stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
		FMOD_RESULT result = m_System->createSound((SoundDir + a_FileName).c_str(), mode, nullptr, &rawSoundPointer);
		if (result == FMOD_OK) {
			m_LoadedSounds.insert(std::pair<std::string, FMOD::Sound*>(a_FileName, rawSoundPointer));
			LOG_USELESS("[Audio Player] Loaded sound " + a_FileName);
		}
		else
		{
			LOG_ERROR("[Audio Player] Failed to load sound " + SoundDir + a_FileName + "!");
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
	result = FMOD::System_Create(&m_System);
	if(result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}
	result = m_System->init(512, FMOD_INIT_NORMAL, 0);
	if(result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}
	LOG("[Audio Player] Initialized.");
	return true;
}


void AudioPlayer::Update(float a_DeltaTime)
{
	
}
