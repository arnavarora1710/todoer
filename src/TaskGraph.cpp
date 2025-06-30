#include "TaskGraph.h"

TaskGraph::TaskGraph(int numTasks) : numTasks(numTasks) {}

bool TaskGraph::taskIdExists(int taskId)
{
    return graph.count(taskId);
}

void TaskGraph::addTask(int taskId)
{
    if (taskIdExists(taskId))
    {
        throw std::invalid_argument("Task already in map!");
    }
    else
    {
        graph[taskId] = {};
        numTasks++;
    }
}

void TaskGraph::addDependency(int taskId, int dependentTaskId)
{
    if (!taskIdExists(taskId) and !taskIdExists(dependentTaskId))
    {
        throw std::invalid_argument("Both tasks not in map!");
    }
    else if (!taskIdExists(taskId))
    {
        throw std::invalid_argument("Task not in map!");
    }
    else if (!taskIdExists(dependentTaskId))
    {
        throw std::invalid_argument("Dependent task not in map!");
    }
    else
    {
        graph[taskId].push_back(dependentTaskId);
    }
}

void TaskGraph::printGraph(std::ostream &os)
{
    for (auto &[taskId, dependencies] : graph)
    {
        os << "Task " << taskId << " depends on:" << std::endl;
        for (int dependency : dependencies)
        {
            os << "Task " << dependency << std::endl;
        }
        os << std::endl;
    }
}

std::vector<int> TaskGraph::topologicalSort()
{
    std::vector<int> sortedTasks{};
    std::unordered_map<int, int> inDegree{};

    for (auto &[taskId, _] : graph)
    {
        inDegree[taskId] = 0;
    }

    for (auto &[taskId, dependencies] : graph)
    {
        for (int dependency : dependencies)
        {
            inDegree[dependency]++;
        }
    }

    std::queue<int> q{};

    for (auto &[taskId, degree] : inDegree)
    {
        if (degree == 0)
        {
            q.push(taskId);
        }
    }

    while (!q.empty())
    {
        int taskId = q.front();
        q.pop();
        sortedTasks.push_back(taskId);

        for (int dependency : graph[taskId])
        {
            inDegree[dependency]--;
            if (inDegree[dependency] == 0)
            {
                q.push(dependency);
            }
        }
    }

    if (sortedTasks.size() != numTasks)
    {
        throw std::invalid_argument("Graph has a cycle!");
    }

    return sortedTasks;
}

void TaskGraph::printTopologicalSort(std::ostream &os)
{
    std::vector<int> sortedTasks = topologicalSort();
    os << "Tasks in topological order:" << std::endl;
    for (int taskId : sortedTasks)
    {
        os << "Task " << taskId << std::endl;
    }
    os << std::endl;
}
