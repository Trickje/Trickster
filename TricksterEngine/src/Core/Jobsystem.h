#pragma once

//TODO(Rick) Categorization of this, separate Render jobs and so on.

// Fixed size very simple thread safe ring buffer
template <typename T, size_t capacity>
class ThreadSafeRingBuffer
{
public:
    // Push an item to the end if there is free space
    //  Returns true if successful
    //  Returns false if there is not enough space
    inline bool push_back(const T& item)
    {
        bool result = false;
        lock.lock();
        size_t next = (head + 1) % capacity;
        if (next != tail)
        {
            data[head] = item;
            head = next;
            result = true;
        }
        lock.unlock();
        return result;
    }

    // Get an item if there are any
    //  Returns true if successful
    //  Returns false if there are no items
    inline bool pop_front(T& item)
    {
        bool result = false;
        lock.lock();
        if (tail != head)
        {
            item = data[tail];
            tail = (tail + 1) % capacity;
            result = true;
        }
        lock.unlock();
        return result;
    }

private:
    T data[capacity];
    size_t head = 0;
    size_t tail = 0;
    std::mutex lock; // this just works better than a spinlock here (on windows)
};
// A Dispatched job will receive this as function argument:
struct JobDispatchArgs
{
    uint32_t jobIndex;
    uint32_t groupIndex;
};
class Jobsystem
{
public:
	// Create the internal resources such as worker threads, etc. Call it once when initializing the application.
    void Initialize();

    // Add a job to execute asynchronously. Any idle thread will execute this job.
    void Execute(const std::function<void()>& job);

    // Divide a job onto multiple jobs and execute in parallel.
    //  jobCount    : how many jobs to generate for this task.
    //  groupSize   : how many jobs to execute per thread. Jobs inside a group execute serially. It might be worth to increase for small jobs
    //  func        : receives a JobDispatchArgs as parameter
    void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);

    // Check if any threads are working currently or not
    bool IsBusy();

    // Wait until all threads become idle
    void Wait();

private:

};

