#ifndef GRAPH_H
#define GRAPH_H

#include "Task.h"
#include <unordered_map>
#include <vector>

class TaskGraph
{
private:
    // map task_id to vector of task_ids that are dependent on it
    std::unordered_map<int, std::vector<int>> graph{};

public:
    TaskGraph(int numTasks);
    void addTask(int taskId);
    void addDependency(int taskId, int dependentTaskId);
    void printGraph();
    std::vector<int> topologicalSort();
    void printTopologicalSort();
};

#endif