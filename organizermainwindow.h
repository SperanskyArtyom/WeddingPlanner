#ifndef ORGANIZERMAINWINDOW_H
#define ORGANIZERMAINWINDOW_H

#include "mainwindow.h"

#include "ordersearchwidget.h"
#include "orderlistwidget.h"
#include <QListWidget>

class OrganizerMainWindow : public MainWindow
{
    Q_OBJECT
public:
    explicit OrganizerMainWindow(QWidget *parent = nullptr);

private slots:
    void updateOrderList();
    void onOrderSelected(const WeddingOrder &order);
    void onAddOrderButtonClicked();


private:
    void initUi();
    void initConnections();

    OrderSearchWidget *m_searchWidget;
    OrderListWidget *m_orderListWidget;
    QPushButton *m_addOrderButton;
};

#endif // ORGANIZERMAINWINDOW_H
