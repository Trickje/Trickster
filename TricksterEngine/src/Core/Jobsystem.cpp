#include "pch.h"
#include "Jobsystem.h"

void Jobsystem::Initialize()
{
 
}

void Jobsystem::Execute(const std::function<void()>& job)
{
}

void Jobsystem::Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job)
{
}

bool Jobsystem::IsBusy()
{
	return false;
}

void Jobsystem::Wait()
{
}
