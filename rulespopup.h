#ifndef RULESPOPUP_H
#define RULESPOPUP_H

#include <QDialog>

namespace Ui {
class RulesPopup;
}

class RulesPopup : public QDialog
{
    Q_OBJECT

public:
    explicit RulesPopup(QWidget *parent = nullptr);
    ~RulesPopup();

private:
    Ui::RulesPopup *ui;

signals:
    void rulesRejected();

private slots:
    void on_buttonBox_rejected();
};

#endif // RULESPOPUP_H
