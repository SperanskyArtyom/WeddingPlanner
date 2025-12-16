#ifndef TASKLISTWIDGET_H
#define TASKLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QScrollArea>
#include "tasklist.h"
#include "databasemanager.h"

class TaskListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskListWidget(DatabaseManager *dbManager, int orderId, QWidget *parent = nullptr);

    void loadTasks();               // загружает задачи из БД
    void addTask(const QString &section, const QString &label);

private slots:
    void onTaskToggled(bool checked);

private:
    void refreshUi();
    void createSection(const QString &section);

    DatabaseManager *m_dbManager;
    int m_orderId;
    TaskList m_taskList;
    QVBoxLayout *m_mainLayout;
    QMap<QString, QVBoxLayout*> m_sectionLayouts;
};

#endif // TASKLISTWIDGET_H
