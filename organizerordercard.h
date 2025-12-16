#ifndef ORGANIZERORDERCARD_H
#define ORGANIZERORDERCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include "weddingorder.h"
#include "databasemanager.h"
#include "tasklist.h"
#include "tasklistwidget.h"

class OrganizerOrderCard : public QWidget
{
    Q_OBJECT
public:
    explicit OrganizerOrderCard(DatabaseManager *dbManager, QWidget *parent = nullptr);

    void setOrder(const WeddingOrder &order);

private slots:
    void onPerformerChanged(Service::Type type, const QString &performerName);
    void onPaymentClicked();

private:
    void initUi();
    void initServiceRow(Service::Type type, const QString &performerName);

    DatabaseManager *m_dbManager;
    WeddingOrder m_order;

    QLabel *m_clientNameLabel;
    QLabel *m_dateLabel;
    QLabel *m_budgetLabel;
    QVBoxLayout *m_servicesLayout;
    QMap<Service::Type, QComboBox*> m_serviceCombos;
    QPushButton *m_paymentButton;
    TaskListWidget * m_taskListWidget;
};

#endif // ORGANIZERORDERCARD_H
