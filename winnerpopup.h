#ifndef WINNERPOPUP_H
#define WINNERPOPUP_H

#include <QDialog>

namespace Ui {
class WinnerPopup;
}

class WinnerPopup : public QDialog
{
    Q_OBJECT

public:
    explicit WinnerPopup(QWidget *parent = nullptr);
    ~WinnerPopup();

    void setLabelText(int current_player);

private:
    Ui::WinnerPopup *ui;

signals:
    void PlayAgain();
    void Exit();

private slots:
    void on_winnerButtonBox_rejected();
    void on_winnerButtonBox_accepted();

};

#endif // WINNERPOPUP_H
