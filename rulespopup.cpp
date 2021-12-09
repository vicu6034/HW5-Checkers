#include "rulespopup.h"
#include "ui_rulespopup.h"

#include <QDebug>

// default constructor sets up ui
RulesPopup::RulesPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RulesPopup)
{
    ui->setupUi(this);

}

// when rules are rejected emit signal to mainwindow to go back to main menu
void RulesPopup::on_rulesButtonBox_rejected() {
    emit rulesRejected();
}

// when rules are acceped emit signal to mainwindow to continue to game
void RulesPopup::on_rulesButtonBox_accepted() {
    emit rulesAccepted();
}

// destructor
RulesPopup::~RulesPopup() {
    delete ui;
}
