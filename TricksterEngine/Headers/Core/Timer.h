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
namespace TE {
	class Timer
	{
	public:
		TRICKSTER_API Timer();
		TRICKSTER_API ~Timer();
		//Gets the elapsed time in seconds
		TRICKSTER_API float GetSeconds();
		//Gets the elapsed time in milliseconds
		TRICKSTER_API float GetMilliSeconds();
		//Starts counting from 0
		TRICKSTER_API void Start();
		//Returns time in Seconds and resets the timer to 0
		TRICKSTER_API float Reset();
		TRICKSTER_API void Pause(bool a_Pause = true);
	private:
		float m_Time;
		float m_StartTime;
		bool m_Paused;
	};

}