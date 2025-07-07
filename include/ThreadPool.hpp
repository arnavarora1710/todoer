#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <memory>
#include <any>
#include "Task.hpp"

// Wrapper class to connect Task execution with futures
struct TaskWrapper
{
    Task task;
    std::promise<std::any> promise;

    TaskWrapper(Task &&t) : task(std::move(t)) {}

    std::any execute()
    {
        try
        {
            std::any result = task.execute();
            promise.set_value(result);
            return result;
        }
        catch (...)
        {
            promise.set_exception(std::current_exception());
            throw;
        }
    }

    std::future<std::any> get_future()
    {
        return promise.get_future();
    }
};

class ThreadPool
{
public:
    explicit ThreadPool(std::size_t num_threads)
    {
        startThreadPool(num_threads);
    }

    ~ThreadPool()
    {
        stopThreadPool();
    }

    // Enqueue a Task object and return a future for its result
    std::future<std::any> enqueueTask(Task &&task)
    {
        auto wrapper = std::make_shared<TaskWrapper>(std::move(task));
        auto future = wrapper->get_future();

        {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_tasks.emplace(wrapper);
        }
        m_cv.notify_one();
        return future;
    }

private:
    std::vector<std::thread> m_threads{};
    std::queue<std::shared_ptr<TaskWrapper>> m_tasks{};
    std::mutex m_mtx;
    std::condition_variable m_cv;
    bool m_stop = false;

    void startThreadPool(std::size_t num_threads)
    {
        m_threads.reserve(num_threads);
        for (auto i = 0u; i < num_threads; ++i)
        {
            m_threads.emplace_back([this]
                                   {
                while (true)
                {
                    std::shared_ptr<TaskWrapper> task_wrapper;
                    {
                        std::unique_lock<std::mutex> lock(m_mtx);
                        m_cv.wait(lock, [this]
                                  { return m_stop || !m_tasks.empty(); });

                        if (m_stop && m_tasks.empty())
                        {
                            break;
                        }
                        task_wrapper = m_tasks.front();
                        m_tasks.pop();
                    }
                    task_wrapper->execute();
                } });
        }
    }

    void
    stopThreadPool() noexcept
    {
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_stop = true;
        }
        m_cv.notify_all();

        for (auto &thread : m_threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }
};
