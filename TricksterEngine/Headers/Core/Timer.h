#pragma once
namespace Trickster {
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