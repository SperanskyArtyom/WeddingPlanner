#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "databasemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

protected:
    QVBoxLayout *mainLayout;
    DatabaseManager *m_dbManager;

signals:
    void profileSwitched();

private slots:
    void onChangeProfileButtonClicked();

private:
    void initUi();

    QPushButton *m_changeProfileButton;
};
#endif // MAINWINDOW_H
