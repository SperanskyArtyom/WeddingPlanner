#include "tasklistwidget.h"
#include <QLabel>
#include <QInputDialog>
#include <QDebug>

TaskListWidget::TaskListWidget(DatabaseManager *dbManager, int orderId, QWidget *parent)
    : QWidget(parent), m_dbManager(dbManager), m_orderId(orderId)
{
    m_mainLayout = new QVBoxLayout(this);
    setLayout(m_mainLayout);

    // создаём три секции
    createSection("До свадьбы");
    createSection("Во время свадьбы");
    createSection("После свадьбы");

    loadTasks();
}

void TaskListWidget::createSection(const QString &section)
{
    QLabel *sectionLabel = new QLabel(section, this);
    sectionLabel->setStyleSheet("font-weight: bold;");
    m_mainLayout->addWidget(sectionLabel);

    QVBoxLayout *sectionLayout = new QVBoxLayout();
    m_mainLayout->addLayout(sectionLayout);
    m_sectionLayouts[section] = sectionLayout;

    QPushButton *addButton = new QPushButton("Добавить задачу", this);
    m_mainLayout->addWidget(addButton);
    connect(addButton, &QPushButton::clicked, this, [this, section](){
        bool ok;
        QString text = QInputDialog::getText(this, "Новая задача", "Описание задачи:", QLineEdit::Normal, "", &ok);
        if(ok && !text.isEmpty()) {
            addTask(section, text);
        }
    });
}

void TaskListWidget::loadTasks()
{
    if(!m_dbManager) return;

    m_taskList.setTasks(m_dbManager->getTasksForOrder(m_orderId));
    refreshUi();
}

void TaskListWidget::refreshUi()
{
    // очищаем все виджеты
    for(auto layout : m_sectionLayouts) {
        QLayoutItem *child;
        while((child = layout->takeAt(0)) != nullptr) {
            if(child->widget()) child->widget()->deleteLater();
            delete child;
        }
    }

    for(const Task &task : m_taskList.tasks()) {
        if(!m_sectionLayouts.contains(task.section)) continue;

        QCheckBox *check = new QCheckBox(task.label, this);
        check->setChecked(task.isDone);
        check->setProperty("taskId", task.id);

        connect(check, &QCheckBox::toggled, this, &TaskListWidget::onTaskToggled);

        m_sectionLayouts[task.section]->addWidget(check);
    }
}

void TaskListWidget::addTask(const QString &section, const QString &label)
{
    Task task;
    task.label = label;
    task.section = section;
    task.isDone = false;

    if(m_dbManager->saveTask(m_orderId, task)) {
        m_taskList.addTask(task);
        refreshUi();
    } else {
        qDebug() << "Не удалось добавить задачу в базу";
    }
}

void TaskListWidget::onTaskToggled(bool checked)
{
    QCheckBox *check = qobject_cast<QCheckBox*>(sender());
    if(!check) return;

    int taskId = check->property("taskId").toInt();
    QString label = check->text();

    Task task;
    task.id = taskId;
    task.label = label;
    task.isDone = checked;

    if(!m_dbManager->saveTask(m_orderId, task)) {
        qDebug() << "Не удалось обновить задачу в базе";
    }
}
