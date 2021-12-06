#ifndef WINNERPOPUP_H
#define WINNERPOPUP_H

#include <QDialog>

namespace Ui { class WinnerPopup; }

class WinnerPopup : public QDialog
{
    Q_OBJECT

public:
    // default constructor and destructor
    explicit WinnerPopup(QWidget *parent = nullptr);
    ~WinnerPopup();

    // change text of the label on the popup
    void setLabelText(int current_player);

private:
    Ui::WinnerPopup *ui;

signals:
    // signals to main window wether to run another game or exit
    // based off the what slots selected
    void PlayAgain();
    void Exit();

private slots:
    // default slots for the play again and exit buttons
    void on_winnerButtonBox_rejected();
    void on_winnerButtonBox_accepted();

};

#endif // WINNERPOPUP_H
