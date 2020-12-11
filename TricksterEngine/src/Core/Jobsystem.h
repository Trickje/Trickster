#pragma once

//*********************.*********************//
//********************/ \********************//
//*******************/   \*******************//
//***Intellectual property of Rick Pijpers***//
//*******************\   /*******************//
//********************\ /********************//
//*********************`*********************//
//          Created December 5 2020          //

//To clarify: I did not make all of this code myself, but parts of it are mine.


// https://en.cppreference.com/w/cpp/thread/condition_variable
// https://en.cppreference.com/w/cpp/thread/condition_variable/notify_one





//TODO Categorization of this, separate Render jobs and so on.
namespace Trickster {
	//Class for Multithreading
	class Jobsystem
	{
	public:
		void Initialize();
		void OnUpdate(float a_DeltaTime);
		//This function will add a task (a function pointer or lambda) to the task queue for worker threads.
		//ONLY USE THIS IF YOU KNOW THAT YOUR FUNCTION IS THREAD-SAFE
		template<class F, class... Args>
		auto Enqueue(F&& function, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
		Jobsystem();
		~Jobsystem();
		//This function will pause the main thread until all worker threads are idle
		void AwaitAll();
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
	auto Jobsystem::Enqueue(F&& function, Args&&... args)
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
	inline Jobsystem::Jobsystem()
	{
		this->Initialize();
	}
	// the destructor joins all threads
	inline Jobsystem::~Jobsystem()
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			stop = true;
		}
		condition.notify_all();
		for (std::thread& t : m_Threads)
			t.join();
	}
}