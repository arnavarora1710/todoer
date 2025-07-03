#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include "Task.hpp"

class ThreadPool
{
private:
    std::vector<std::thread> m_threads;
    std::queue<Task> m_tasks;
    std::mutex m_mtx;
    std::condition_variable m_cv;
    bool m_stop;

public:
};