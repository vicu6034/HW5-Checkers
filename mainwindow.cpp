#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // we need to set up the ui before we draw on our scene
    ui->setupUi(this);

    // must create gameGraphicsView in UI


    // the QGraphicsView is the UI element that contains the
    // scene that we will actually get to draw on.
    QGraphicsView * view = ui->gameGraphicsView;

    // scene is a QGraphicsScene pointer field of the PlotWindow class
    // this makes our lives easier by letting us easily access it
    // from other functions in this class.
    scene = new QGraphicsScene;
    view->setScene(scene);
    // make the scene the same size as the view containing it
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());


    gameboard_ = new GameBoard();

}

MainWindow::~MainWindow()
{
    delete ui;
}

