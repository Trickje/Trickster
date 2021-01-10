#pragma once
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
#include "Timer.h"

namespace Trickster {
	class Profiler
	{
	public:
		TRICKSTER_API Profiler(std::string a_Name);
		TRICKSTER_API Profiler() = delete;
		TRICKSTER_API ~Profiler();
	private:
		Timer m_Timer;
		std::string m_Name;
	};

}
