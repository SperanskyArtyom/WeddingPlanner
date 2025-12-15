#include "profileselectdialog.h"
#include "ui_profileselectdialog.h"

ProfileSelectDialog::ProfileSelectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ProfileSelectDialog)
{
    ui->setupUi(this);

    setWindowTitle("Свадебное агенство");

    connect(ui->btnUser, &QPushButton::clicked,
            this, &ProfileSelectDialog::onBtnUserClicked);
    connect(ui->btnOrganizer, &QPushButton::clicked,
            this, &ProfileSelectDialog::onBtnOrganizerClicked);
}

ProfileSelectDialog::~ProfileSelectDialog()
{
    delete ui;
}

void ProfileSelectDialog::onBtnUserClicked()
{
    m_selectedProfile = ProfileType::User;
    accept();
}

void ProfileSelectDialog::onBtnOrganizerClicked()
{
    m_selectedProfile = ProfileType::Organizer;
    accept();
}

ProfileSelectDialog::ProfileType ProfileSelectDialog::selectedProfile() const
{
    return m_selectedProfile;
}
