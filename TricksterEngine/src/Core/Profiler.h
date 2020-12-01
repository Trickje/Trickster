#pragma once
namespace Trickster {
	class Profiler
	{
	public:
		Profiler(std::string a_Name);
		Profiler() = delete;
		~Profiler();

	private:
		Timer m_Timer;
		std::string m_Name;
	};

}