#include "pch.h"
#include "Core/Timer.h"



using namespace Trickster;
Timer::Timer()
{
	Start();
}


Timer::~Timer()
{
}

float Timer::GetSeconds() 
{
	m_Time = static_cast<float>(glfwGetTime()) - m_StartTime;
	return m_Time;
}

float Trickster::Timer::GetMilliSeconds()
{
	m_Time = static_cast<float>(glfwGetTime()) - m_StartTime;
	return m_Time * 1000.f;
}

void Timer::Start()
{
	m_StartTime = static_cast<float>(glfwGetTime());  // NOLINT(bugprone-narrowing-conversions)
	
	m_Time = m_StartTime;
	m_Paused = false;
}

float Timer::Reset()
{
	if(m_Paused)
	{
		return 0.f;
	}
	const auto NewTime = static_cast<float>(glfwGetTime());
	m_Time = NewTime - m_StartTime;
	m_StartTime = NewTime;
	return m_Time;
}

void Trickster::Timer::Pause(bool a_Pause)
{
	if(m_Paused && !a_Pause)
	{
		//Unpause
		m_StartTime = static_cast<float>(glfwGetTime()) - m_Time;
		m_Paused = false;
	}
	if(!m_Paused && a_Pause)
	{
		const auto NewTime = static_cast<float>(glfwGetTime());
		m_Time = NewTime - m_StartTime;
		m_Paused = true;
	}
}
