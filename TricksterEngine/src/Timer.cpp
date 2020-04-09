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
	return m_Time * 0.001f;
}

float Trickster::Timer::GetMilliSeconds()
{
	m_Time = static_cast<float>(glfwGetTime()) - m_StartTime;
	return m_Time;
}

void Timer::Start()
{
	m_StartTime = glfwGetTime();  // NOLINT(bugprone-narrowing-conversions)
	
	m_Time = m_StartTime;
}

float Timer::Reset()
{
	const auto NewTime = static_cast<float>(glfwGetTime());
	m_Time = NewTime - m_StartTime;
	m_StartTime = NewTime;
	return m_Time * 0.001f;
}
