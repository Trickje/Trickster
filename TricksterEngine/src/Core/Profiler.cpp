#include "pch.h"
#include "Profiler.h"
using namespace Trickster;

Profiler::Profiler(std::string a_Name) : m_Timer(), m_Name(a_Name)
{
	m_Timer.Start();
}


Profiler::~Profiler()
{
	LOG("[Profiler] " + m_Name + ": " + std::to_string(m_Timer.GetSeconds()) + " seconds." );
}
