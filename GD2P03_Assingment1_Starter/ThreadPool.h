#pragma once


#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;

    public:
       

        // Template function to add a task to the thread pool's queue
        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
            // Determine the return type of the function to be executed
            using return_type = typename std::result_of<F(Args...)>::type;

            // Package the function and its arguments into a task
            auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            // Get a future from the task to allow the caller to wait for the task to complete
            std::future<return_type> res = task->get_future();

            // Lock the queue mutex to safely add the task to the queue
            {
                std::unique_lock<std::mutex> lock(queue_mutex);

                // If the thread pool is stopped, throw an exception
                if (stop) {
                    throw std::runtime_error("enqueue on stopped ThreadPool");
                }

                // Add the task to the queue
                tasks.emplace([task]() { (*task)(); });
            }

            // Notify one worker thread that a new task is available
            condition.notify_one();

            // Return the future associated with the task
            return res;
        }

        ThreadPool(size_t threads);
        ~ThreadPool();
};