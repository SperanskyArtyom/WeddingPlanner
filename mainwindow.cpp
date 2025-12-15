#include "mainwindow.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Свадебное агенство");

    initUi();
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
