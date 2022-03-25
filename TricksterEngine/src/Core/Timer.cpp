/*
================================================================================
		Copyright 2020 Rick Pijpers

		Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at

			http://www.apache.org/licenses/LICENSE-2.0

		Unless required by applicable law or agreed to in writing, software
		distributed under the License is distributed on an "AS IS" BASIS,
		WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		See the License for the specific language governing permissions and
		limitations under the License.
=================================================================================
 */
#include "pch.h"
#include "Core/Timer.h"



using namespace TE;
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

float Timer::GetMilliSeconds()
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

void Timer::Pause(bool a_Pause)
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
