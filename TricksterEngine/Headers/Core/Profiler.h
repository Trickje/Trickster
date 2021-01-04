#pragma once
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
