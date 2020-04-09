#pragma once
namespace Trickster {
	class Timer
	{
	public:
		Timer();
		~Timer();
		//Gets the elapsed time in seconds
		float GetSeconds();
		//Gets the elapsed time in milliseconds
		float GetMilliSeconds();
		//Starts counting from 0
		void Start();
		//Returns time in Seconds and resets the timer to 0
		float Reset();
	private:
		float m_Time;
		float m_StartTime;
	};

}