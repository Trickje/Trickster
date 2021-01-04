#include "pch.h"

#include "Events/EventManager.h"
#include "Core/JobSystem.h"
using namespace Trickster;

JobSystem::JobSystem()
{
    this->Initialize();
}
// the destructor joins all threads
JobSystem::~JobSystem()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& t : m_Threads)
        t.join();
}
void Trickster::JobSystem::Initialize()
{
    stop = false;
    max_threads = std::thread::hardware_concurrency();
    LOG("[Multi-Threading] Available threads: " + std::to_string(max_threads));
    if (max_threads == 0)
    {
        m_Threads.push_back(std::thread());
    }
    else 
    {
        //Found this code on  https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h
        for (size_t i = 0; i < max_threads; i++) {
            m_Working.emplace_back(false);
            m_Threads.emplace_back(
                [this, i]
                {
            		
                    for (;;)
                    {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            this->condition.wait(lock,
                                [this] { return this->stop || !this->m_Tasks.empty(); });
                            if (this->stop && this->m_Tasks.empty())
                                return;
                            task = std::move(this->m_Tasks.front());
                            m_Working[i] = true;
                            this->m_Tasks.pop();
                        }
                        task();
                        m_Working[i] = false;
                    }
                }
                );
        }
    }
	EventManager::GetInstance()->GameLoopEvents.OnUpdate.AddListener(std::bind(&JobSystem::OnUpdate,this, std::placeholders::_1));
}


void Trickster::JobSystem::OnUpdate(float a_DeltaTime)
{

}

void JobSystem::AwaitAll()
{
    bool Working = true;
	while(Working)
	{
        Working = false;
		for(auto b: m_Working)
		{
			if(b)
			{
                Working = true;
			}
		}
	}
}