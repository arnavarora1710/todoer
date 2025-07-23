#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>
#include <atomic>
#include <deque>
#include "Task.hpp"

class ThreadPool
{
public:
    explicit ThreadPool(std::size_t num_threads = std::thread::hardware_concurrency())
        : m_stop(false)
    {
        startThreadPool(num_threads);
    }

    ~ThreadPool()
    {
        stopThreadPool();
    }

    // Enqueue a Task object and return a future for its result
    std::future<std::variant<int, double>> enqueueTask(Task&& task)
    {
        auto task_ptr = std::make_shared<std::packaged_task<std::variant<int, double>()>>(
            [task = std::move(task)]() mutable {
                return task.execute();
            }
        );

        auto wrapper_func = [task_ptr]() {
            (*task_ptr)();
        };

        {
            std::lock_guard lock(m_mtx);
            m_tasks.emplace_back(wrapper_func);
        }
        m_cv.notify_one();

        return task_ptr->get_future();
    }

private:
    class ThreadWorker {
    public:
        explicit ThreadWorker(ThreadPool* pool) : m_pool { pool } {}

        void operator()() const
        {
            std::function<void()> task;
            
            while (true) {
                {
                    std::unique_lock lock(m_pool->m_mtx);
                    
                    m_pool->m_cv.wait(lock, [this] {
                        return m_pool->m_stop || !m_pool->m_tasks.empty();
                    });
                    
                    if (m_pool->m_stop && m_pool->m_tasks.empty()) {
                        break;
                    }
                    
                    if (!m_pool->m_tasks.empty()) {
                        task = std::move(m_pool->m_tasks.front());
                        m_pool->m_tasks.pop_front();
                    }
                }

                task();
            }
        }
        
    private:
        ThreadPool* m_pool;
    };

    std::vector<std::thread> m_threads{};
    mutable std::mutex m_mtx;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop;
    std::deque<std::function<void()>> m_tasks{};

    void startThreadPool(const std::size_t num_threads)
    {
        m_threads.reserve(num_threads);
        for (std::size_t i = 0; i < num_threads; ++i) {
            m_threads.emplace_back(ThreadWorker(this));
        }
    }

    void stopThreadPool() noexcept
    {
        {
            std::lock_guard lock(m_mtx);
            m_stop = true;
        }
        m_cv.notify_all();

        for (auto& thread : m_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
};
