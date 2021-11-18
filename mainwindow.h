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


};
#endif // MAINWINDOW_H
