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
    // connect our custom slots and signals
    connect(gameboard_, SIGNAL(updateTurnLabel(int)), this, SLOT(updateTurnLabel(int)));
    connect(gameboard_, SIGNAL(updatePiecesLabel(bool,int)), this, SLOT(updatePiecesLabel(bool,int)));
    connect(gameboard_, SIGNAL(updatePiece(PiecePrototype*)), this, SLOT(updatePiece(PiecePrototype*)));

    // add all tiles to the scene
    for (Tile* tile : gameboard_->getTiles() ) {
        scene->addItem(tile);
    }

    // add all pieces to the scene
    for (PiecePrototype* piece : gameboard_->getPieces() ) {
        scene->addItem(piece);
    }

    // add all powerups to the scene
    for (PowerUp* powerup : gameboard_->getPowerUps() ) {
        scene->addItem(powerup);
    }

    ui->turnLabel->setText("TURN: RED");
}

// update turn label with whoevers turn it is
void MainWindow::updateTurnLabel(int turn) {
    if (!turn) {
        ui->turnLabel->setText("TURN: RED");
    } else {
        ui->turnLabel->setText("TURN: BLACK");
    }
}

// update label displaying how many pieces a player has
void MainWindow::updatePiecesLabel(bool red, int pieces) {
    if (red) {
        std::string s= "RED: " + std::to_string(pieces) + " Pieces Remaining";
        QString pop_q(const_cast<char*>(s.c_str()));
        ui->redPiecesLabel->setText(pop_q);
    } else {
        std::string s= "BLACK: " + std::to_string(pieces) + " Pieces Remaining";
        QString pop_q(const_cast<char*>(s.c_str()));
        ui->blackPiecesLabel->setText(pop_q);
    }
}

// update a piece after movement
void MainWindow::updatePiece(PiecePrototype* p) {
    scene->removeItem(p);
    p->update();
    scene->addItem(p);
}

// reset the mainwindow
void MainWindow::Reset() {
    // remove all pieces
    for (PiecePrototype* piece : gameboard_->getPieces() ) {
         scene->removeItem(piece);
    }
    // reset gameboard
    gameboard_->Reset();
    // add new pieces
    for (PiecePrototype* piece : gameboard_->getPieces() ) {
        scene->addItem(piece);
    }
    // reset piece and turn labels
    ui->turnLabel->setText("TURN: RED");
    std::string s= "RED: " + std::to_string(15) + " Pieces Remaining";
    QString pop_q(const_cast<char*>(s.c_str()));
    ui->redPiecesLabel->setText(pop_q);

    std::string s2= "BLACK: " + std::to_string(15) + " Pieces Remaining";
    QString pop2_q(const_cast<char*>(s2.c_str()));
    ui->blackPiecesLabel->setText(pop2_q);
}

// when reset button is clicked, just reset
void MainWindow::on_resetButton_clicked() {
    Reset();
}

// when surrender is clicked, give a win to the non-surrenderer & reset
void MainWindow::on_surrenderButton_clicked() {
   Player * p = gameboard_->getOtherPlayer();
   p->set_num_wins(p->get_num_wins()+1);
   if (p->get_is_red()) {
       std::string s= "RED: " + std::to_string(p->get_num_wins()) + " Wins";
       QString pop_q(const_cast<char*>(s.c_str()));
       ui->redWinsLabel->setText(pop_q);
   } else {
       std::string s= "BLACK: " + std::to_string(p->get_num_wins()) + " Wins";
       QString pop_q(const_cast<char*>(s.c_str()));
       ui->blackWinsLabel->setText(pop_q);
   }
   Reset();
}

// destructor
MainWindow::~MainWindow() {
    delete ui;
}
