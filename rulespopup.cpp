#include "rulespopup.h"
#include "ui_rulespopup.h"

#include <QDebug>

RulesPopup::RulesPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RulesPopup)
{
    ui->setupUi(this);

}

RulesPopup::~RulesPopup()
{
    delete ui;
}

void RulesPopup::on_buttonBox_rejected()
{
    emit rulesRejected();
}

