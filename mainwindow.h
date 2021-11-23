#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <gameboard.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // mainwindow must have the ui and scene so we can do things
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    // has a GameBoard to run the game
    GameBoard* gameboard_;

public:
    // constructor and destructor
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // reset the mainwindow to initial game state
    void Reset();

public slots:
    // custom slots to update labels and Pieces
    void updateTurnLabel(int turn);
    void updatePiecesLabel(bool red, int pieces);
    void updatePiece(PiecePrototype* p);

private slots:
    // default slots for button clicks
    void on_resetButton_clicked();
    void on_surrenderButton_clicked();
};

#endif // MAINWINDOW_H
