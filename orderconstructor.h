#ifndef ORDERCONSTRUCTOR_H
#define ORDERCONSTRUCTOR_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMap>
#include "databasemanager.h"
#include "service.h"
#include "weddingorder.h"

class OrderConstructor : public QDialog
{
    Q_OBJECT
public:
    explicit OrderConstructor(DatabaseManager *dbManager, QWidget *parent = nullptr);

signals:
    void orderAdded(const WeddingOrder &order);

private slots:
    void onAddClicked();
    void validateInputs();
    void updateAvailablePerformers(Service::Type type);
    void onDateChanged(const QDate &date);

private:
    void initUi();
    void initServiceRow(Service::Type type);

    DatabaseManager *m_dbManager;

    QLineEdit *m_clientNameEdit;
    QDateEdit *m_dateEdit;
    QDoubleSpinBox *m_budgetEdit;
    QTextEdit *m_commentEdit;
    QPushButton *m_addButton;

    QMap<Service::Type, QComboBox*> m_serviceCombos;
    QVBoxLayout *m_servicesLayout;
};

#endif // ORDERCONSTRUCTOR_H
