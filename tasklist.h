#ifndef TASKLIST_H
#define TASKLIST_H

#include <QString>
#include <QList>

struct Task
{
    int id = -1;
    QString label;
    QString section;
    bool isDone = false;
};

class TaskList
{
public:
    TaskList() = default;

    const QList<Task> &tasks() const { return m_tasks; }
    void addTask(const Task &task) { m_tasks.append(task); }

    void setTasks(const QList<Task> &tasks) { m_tasks = tasks; }

private:
    QList<Task> m_tasks;
};

#endif // TASKLIST_H
