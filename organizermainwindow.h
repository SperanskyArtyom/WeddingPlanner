#ifndef ORGANIZERMAINWINDOW_H
#define ORGANIZERMAINWINDOW_H

#include "mainwindow.h"

#include "ordersearchwidget.h"
#include <QListWidget>

class OrganizerMainWindow : public MainWindow
{
    Q_OBJECT
public:
    explicit OrganizerMainWindow(QWidget *parent = nullptr);

private slots:
void onSearchWidgetChanged(const QString &text,
                           OrderSearchWidget::FilterType filter,
                           OrderSearchWidget::SortType sort);

private:
    void initUi();
    void initConnections();

    OrderSearchWidget *m_searchWidget;
    QListWidget *m_orderListWidget;
    QPushButton *m_addOrderButton;
};

#endif // ORGANIZERMAINWINDOW_H
