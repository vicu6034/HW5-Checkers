#include "winnerpopup.h"
#include "ui_winnerpopup.h"

#include <QPushButton>

WinnerPopup::WinnerPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WinnerPopup)
{
    ui->setupUi(this);
    // change text for the buttons in the button box
    ui->winnerButtonBox->button(QDialogButtonBox::Ok)->setText("Play Again");
    ui->winnerButtonBox->button(QDialogButtonBox::Cancel)->setText("Exit Game");
}

/* Set text displayed to winner
 * @param int of the player of who won
*/
void WinnerPopup::setLabelText(int current_player) {
    if (!current_player) {
        ui->winnerPopupLabel->setText("Red Wins!");
    } else {
        ui->winnerPopupLabel->setText("Black Wins!");
    }
}

// when accepted button is clicked reset game
void WinnerPopup::on_winnerButtonBox_accepted() {
    emit PlayAgain();
}

// when denied is clicked exit the whole window
void WinnerPopup::on_winnerButtonBox_rejected() {
    emit Exit();
}

// destructor
WinnerPopup::~WinnerPopup() {
    delete ui;
}
