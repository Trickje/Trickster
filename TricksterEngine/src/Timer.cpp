#include "pch.h"
#include "Timer.h"

#include <GLFW/glfw3.h>


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
}

float Timer::Reset()
{
	const auto NewTime = static_cast<float>(glfwGetTime());
	m_Time = NewTime - m_StartTime;
	m_StartTime = NewTime;
	return m_Time;
}
