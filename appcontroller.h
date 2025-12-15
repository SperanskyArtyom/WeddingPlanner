#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include "profileselectdialog.h"
#include "mainwindow.h"

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);
    void start();

public slots:
    void switchProfile();

private:
    void showProfileDialog();
    void createMainWindow(ProfileSelectDialog::ProfileType type);

    MainWindow *m_mainWindow = nullptr;
};

#endif // APPCONTROLLER_H
