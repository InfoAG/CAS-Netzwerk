#include "simpsongui.h"
#include "ui_simpsongui.h"

SimpsonGui::SimpsonGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimpsonGui)
{
    ui->setupUi(this);
}

SimpsonGui::~SimpsonGui()
{
    delete ui;
}

void SimpsonGui::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString SimpsonGui::getSimpsonString() const {
    return ui->functionLineEdit->text() + "," + ui->intvarLineEdit->text() + "," + ui->lboundLineEdit->text() + "," + ui->uboundLineEdit->text() + "," + ui->intervalsLineEdit->text();
}

QString SimpsonGui::SimpsonDlg() {
    SimpsonGui SDlg;
    if (SDlg.exec() == QDialog::Accepted) {
        return SDlg.getSimpsonString();
    }
    else return QString();
}
