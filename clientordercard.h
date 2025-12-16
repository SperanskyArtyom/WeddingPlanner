#ifndef CLIENTORDERCARD_H
#define CLIENTORDERCARD_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "weddingorder.h"

class ClientOrderCard : public QWidget
{
    Q_OBJECT
public:
    explicit ClientOrderCard(QWidget *parent = nullptr);

    void setOrder(const WeddingOrder &order);

private:
    QLabel *m_titleLabel;
    QLabel *m_dateLabel;
    QLabel *m_budgetLabel;
    QLabel *m_photographerLabel;
    QLabel *m_hostLabel;
    QLabel *m_decorLabel;
    QLabel *m_transportLabel;

    QVBoxLayout *m_layout;
};

#endif // CLIENTORDERCARD_H
