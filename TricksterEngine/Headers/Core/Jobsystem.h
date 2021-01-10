#pragma once

//*********************.*********************//
//********************/ \********************//
//*******************/   \*******************//
//***Intellectual property of Rick Pijpers***//
//*******************\   /*******************//
//********************\ /********************//
//*********************`*********************//
//          Created December 5 2020          //

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
//To clarify: I did not make all of this code myself, but parts of it are mine.


// https://en.cppreference.com/w/cpp/thread/condition_variable
// https://en.cppreference.com/w/cpp/thread/condition_variable/notify_one





//TODO Categorization of this, separate Render jobs and so on.
namespace Trickster {
	//Class for Multithreading
	class JobSystem
	{
	public:
		TRICKSTER_API void Initialize();
		TRICKSTER_API void OnUpdate(float a_DeltaTime);
		//This function will add a task (a function pointer or lambda) to the task queue for worker threads.
		//ONLY USE THIS IF YOU KNOW THAT YOUR FUNCTION IS THREAD-SAFE
		template<class F, class... Args>
		auto Enqueue(F&& function, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
		TRICKSTER_API JobSystem();
		TRICKSTER_API ~JobSystem();
		//This function will pause the main thread until all worker threads are idle
		TRICKSTER_API void AwaitAll();
		private:





		//Joinable threads
		std::vector<std::thread> m_Threads;
		std::vector<bool> m_Working;
		std::queue<std::function<void()>> m_Tasks;
		//Synchronization
		std::mutex queue_mutex;
		std::condition_variable condition;
		bool stop;
		size_t max_threads;
	};


	// https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h 
	// add new work item to the pool
	template<class F, class... Args>
	auto JobSystem::Enqueue(F&& function, Args&&... args)
		-> std::future<typename std::result_of<F(Args...)>::type>
	{
		using return_type = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared< std::packaged_task<return_type()> >(
			std::bind(std::forward<F>(function), std::forward<Args>(args)...)
			);

		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);

			// don't allow enqueueing after stopping the pool
			if (stop)
				throw std::runtime_error("enqueue on stopped ThreadPool");

			m_Tasks.emplace([task]() { (*task)(); });
		}
		condition.notify_one();
		return res;
	}
	
}