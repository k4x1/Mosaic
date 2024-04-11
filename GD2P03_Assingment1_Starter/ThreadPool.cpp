#include "ThreadPool.h"


ThreadPool::ThreadPool(size_t threads) {
    stop = false; // Initialize the stop flag to false
    for (size_t i = 0; i < threads; ++i) {
        // For each thread, create a worker that will execute tasks from the queue
        workers.emplace_back([this] {
            for (;;) { // Infinite loop to keep the worker running
                std::function<void()> task; // Task to be executed
                {
                    // Lock the queue mutex to safely access the task queue
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    // Wait until there's a task in the queue or the thread pool is stopped
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    // If the thread pool is stopped and there are no tasks left, return from the worker
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }
                    // Get the next task from the queue
                    task = std::move(this->tasks.front());
                    // Remove the task from the queue
                    this->tasks.pop();
                }
                // Execute the task
                task();
            }
            });
    }
}

// Destructor for the ThreadPool class
ThreadPool::~ThreadPool() {
    {
        // Create a new scope for the mutex lock to ensure it's released before notifying the workers
        std::unique_lock<std::mutex> lock(queue_mutex);
        // Signal the workers to stop
        stop = true;
    }
    // Notify all waiting workers that they should stop
    condition.notify_all();
    // Wait for all workers to finish their current tasks
    for (std::thread& worker : workers) {
        worker.join();
    }
}
