#include "winnerpopup.h"
#include "ui_winnerpopup.h"

#include <QPushButton>

WinnerPopup::WinnerPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WinnerPopup)
{
    ui->setupUi(this);
    ui->winnerButtonBox->button(QDialogButtonBox::Ok)->setText("Play Again");
    ui->winnerButtonBox->button(QDialogButtonBox::Cancel)->setText("Exit Game");
}

void WinnerPopup::setLabelText(int current_player) {
    if (!current_player) {
        ui->winnerPopupLabel->setText("Red Wins!");
    } else {
        ui->winnerPopupLabel->setText("Black Wins!");
    }
}

void WinnerPopup::on_winnerButtonBox_accepted()
{
    emit PlayAgain();
}

void WinnerPopup::on_winnerButtonBox_rejected()
{
    emit Exit();
}

WinnerPopup::~WinnerPopup()
{
    delete ui;
}
