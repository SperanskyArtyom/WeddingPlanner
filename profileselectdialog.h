#ifndef PROFILESELECTDIALOG_H
#define PROFILESELECTDIALOG_H

#include <QDialog>

namespace Ui {
class ProfileSelectDialog;
}

class ProfileSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileSelectDialog(QWidget *parent = nullptr);
    ~ProfileSelectDialog();

    enum class ProfileType {
        None,
        User,
        Organizer
    };

    ProfileType selectedProfile() const;

private slots:
    void onBtnUserClicked();
    void onBtnOrganizerClicked();

private:
    Ui::ProfileSelectDialog *ui;
    ProfileType m_selectedProfile = ProfileType::None;
};

#endif // PROFILESELECTDIALOG_H
