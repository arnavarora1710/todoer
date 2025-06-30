#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>
#include <queue>

class TaskGraph
{
private:
    // map task_id to vector of task_ids that are dependent on it
    std::unordered_map<int, std::vector<int>> graph{};
    int numTasks{};

public:
    TaskGraph(int numTasks);
    bool taskIdExists(int taskId);
    void addTask(int taskId);
    void addDependency(int taskId, int dependentTaskId);
    void printGraph(std::ostream &os);
    std::vector<int> topologicalSort();
    void printTopologicalSort(std::ostream &os);
};
