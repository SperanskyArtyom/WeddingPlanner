#ifndef USERMAINWINDOW_H
#define USERMAINWINDOW_H

#include "mainwindow.h"

#include <QLabel>

class UserMainWindow : public MainWindow
{
    Q_OBJECT
public:
    explicit UserMainWindow(QWidget *parent = nullptr);

private slots:
    void onPayButtonClicked();

private:
    void initUi();
    void initConnections();

    QLabel *m_orderLabel;
    QPushButton *m_payButton;
};

#endif // USERMAINWINDOW_H
