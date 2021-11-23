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
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    // all methods will need to be callable from GameBoard
    GameBoard* gameboard_;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Reset();

public slots:
    void addPiece(PiecePrototype* p);
    void updateTurnLabel(int turn);
    void updatePiecesLabel(bool red, int pieces);
    void updatePiece(PiecePrototype* p);

private slots:
    void on_resetButton_clicked();
    void on_surrenderButton_clicked();
};

#endif // MAINWINDOW_H
