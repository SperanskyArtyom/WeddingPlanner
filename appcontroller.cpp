#include "appcontroller.h"
#include "usermainwindow.h"
#include "organizermainwindow.h"

AppController::AppController(QObject *parent)
    : QObject{parent}
{}

void AppController::start()
{
    showProfileDialog();
}

void AppController::showProfileDialog()
{
    ProfileSelectDialog dialog;
    if (dialog.exec() != QDialog::Accepted) return;

    createMainWindow(dialog.selectedProfile());
}

void AppController::createMainWindow(ProfileSelectDialog::ProfileType type)
{
    if (m_mainWindow) {
        m_mainWindow->close();
        m_mainWindow->deleteLater();
        m_mainWindow = nullptr;
    }

    switch (type) {
    case ProfileSelectDialog::ProfileType::User:
        m_mainWindow = new UserMainWindow;
        break;
    case ProfileSelectDialog::ProfileType::Organizer:
        m_mainWindow = new OrganizerMainWindow;
        break;
    default:
        return;
    }

    connect(m_mainWindow, &MainWindow::profileSwitched,
            this, &AppController::switchProfile);

    m_mainWindow->show();
}

void AppController::switchProfile()
{
    showProfileDialog();
}
