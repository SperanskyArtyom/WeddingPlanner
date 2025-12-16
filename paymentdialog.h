#ifndef PAYMENTDIALOG_H
#define PAYMENTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "databasemanager.h"
#include "weddingorder.h"

class PaymentDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PaymentDialog(DatabaseManager *dbManager, const WeddingOrder &order, QWidget *parent = nullptr);

private slots:
    void validateAmount();
    void onPayClicked();

private:
    void initUi();
    void updateLabels();

    DatabaseManager *m_dbManager;
    WeddingOrder m_order;

    QLabel *m_budgetLabel;
    QLabel *m_paidLabel;
    QLabel *m_dueLabel;
    QDoubleSpinBox *m_amountEdit;
    QPushButton *m_payButton;

    double m_paidAmount;
};

#endif // PAYMENTDIALOG_H
