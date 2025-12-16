#include "mainwindow.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Свадебное агенство");

    initUi();

    m_dbManager = new DatabaseManager(this);
    m_dbManager->openDatabase("wedding_planner.db");

    connect(m_changeProfileButton, &QPushButton::clicked,
            this, &MainWindow::onChangeProfileButtonClicked);
}

MainWindow::~MainWindow() = default;

void MainWindow::onChangeProfileButtonClicked()
{
    emit profileSwitched();
}

void MainWindow::initUi()
{
    setMinimumSize(800, 600);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    m_changeProfileButton = new QPushButton("Сменить профиль", this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(m_changeProfileButton);

    mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(topLayout);
}
