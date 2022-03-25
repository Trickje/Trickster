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
#include "Core/Profiler.h"
using namespace TE;

Profiler::Profiler(std::string a_Name) : m_Timer(), m_Name(a_Name)
{
	m_Timer.Start();
}


Profiler::~Profiler()
{
	LOG("[Profiler] " + m_Name + ": " + std::to_string(m_Timer.GetSeconds()) + " seconds." );
}
 