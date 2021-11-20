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
    connect(gameboard_, SIGNAL(addPiece(PiecePrototype*)), this, SLOT(addPiece(PiecePrototype*)));
    connect(gameboard_, SIGNAL(updateTurnLabel(int)), this, SLOT(updateTurnLabel(int)));

    // add all tiles to the scene
    for (Tile* tile : gameboard_->getTiles() ) {
        connect(tile, SIGNAL(gotSelected(Tile*)), gameboard_, SLOT(tileSelected(Tile*)));
        scene->addItem(tile);
    }

    // add all pieces to the scene
    for (PiecePrototype* piece : gameboard_->getPieces() ) {
        connect(piece, SIGNAL(gotSelected()), gameboard_, SLOT(pieceSelected()));
        scene->addItem(piece);
    }

    // add all powerups to the scene
    for (PowerUp* powerup : gameboard_->getPowerUps() ) {
        scene->addItem(powerup);
    }

    ui->turnLabel->setText("TURN: RED");
}

void MainWindow::addPiece(PiecePrototype* p) {
    scene->addItem(p);
}

void MainWindow::updateTurnLabel(int turn) {
    if (turn == 0) {
        ui->turnLabel->setText("TURN: RED");
    } else {
        ui->turnLabel->setText("TURN: BLACK");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

