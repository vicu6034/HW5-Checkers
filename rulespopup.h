/*
 * RulesPopup header
 * Implements a popup to display rules when the game starts
 * CSCI 3010 Homework 5
 * By: Vincent Curran & Philip Knott
*/

#ifndef RULESPOPUP_H
#define RULESPOPUP_H

#include <QDialog>

namespace Ui { class RulesPopup; }

class RulesPopup : public QDialog
{
    Q_OBJECT

public:
    // default contstructor and destructor
    explicit RulesPopup(QWidget *parent = nullptr);
    ~RulesPopup();

private:
    // popup has a ui
    Ui::RulesPopup *ui;

signals:
    // send signals to mainwindow if the button box is accepted or denied
    void rulesAccepted();
    void rulesRejected();

private slots:
    // slot for when the buttons are actually clicked
    void on_rulesButtonBox_accepted();
    void on_rulesButtonBox_rejected();

};

#endif // RULESPOPUP_H
