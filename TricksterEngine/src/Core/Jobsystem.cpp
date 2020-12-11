#include "pch.h"
#include "Jobsystem.h"
using namespace Trickster;

void Trickster::Jobsystem::Initialize()
{
    stop = false;
    max_threads = std::thread::hardware_concurrency();
    LOG("Available threads: " + std::to_string(max_threads));
    if (max_threads == 0)
    {
        m_Threads.push_back(std::thread());
    }
    else 
    {
        //Found this code on  https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h
        for (size_t i = 0; i < max_threads; ++i)
            m_Threads.emplace_back(
                [this]
                {
                    while(!stop)
                    {
                        std::function<void()> task;

                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            this->condition.wait(lock,
                                [this] { return this->stop || !this->m_Tasks.empty(); });
                            if (this->stop && this->m_Tasks.empty())
                                return;
                            task = std::move(this->m_Tasks.front());
                            this->m_Tasks.pop();
                            lock.unlock();
                        }

                        task();
                    }
                }
                );
    }
	EventManager::GetInstance()->GameLoopEvents.OnUpdate.AddListener(std::bind(&Jobsystem::OnUpdate,this, std::placeholders::_1));
}


void Trickster::Jobsystem::OnUpdate(float a_DeltaTime)
{

}
