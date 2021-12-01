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
    connect(gameboard_, SIGNAL(updateTurnLabel(int)), this, SLOT(updateTurnLabel_slot(int)));
    connect(gameboard_, SIGNAL(updatePiecesLabel(bool,int)), this, SLOT(updatePiecesLabel_slot(bool,int)));
    connect(gameboard_, SIGNAL(updatePiece(PiecePrototype*)), this, SLOT(updatePiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(addPiece(PiecePrototype*)), this, SLOT(addPiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(removePiece(PiecePrototype*)), this, SLOT(removePiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(gameOver()), this, SLOT(gameOver_slot()));

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

// reset the mainwindow
void MainWindow::Reset() {
    // remove all pieces
    for (PiecePrototype* piece : gameboard_->getPieces() ) {
         scene->removeItem(piece);
    }
    for (PowerUp* powerup : gameboard_->getPowerUps() ) {
        scene->removeItem(powerup);
    }
    // reset gameboard
    gameboard_->NewGame();
    // add new pieces
    for (PiecePrototype* piece : gameboard_->getPieces() ) {
        scene->addItem(piece);
    }
    // add powerups
    for (PowerUp* powerup : gameboard_->getPowerUps() ) {
        scene->addItem(powerup);
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

void MainWindow::iterateWinLabel(Player * p) {
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
}

// update turn label with whoevers turn it is
void MainWindow::updateTurnLabel_slot(int turn) {
    QString turn_str = turn ? "BLACK" : "RED";
    ui->turnLabel->setText("TURN: " + turn_str);
}

// update label displaying how many pieces a player has
void MainWindow::updatePiecesLabel_slot(bool red, int pieces) {
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
void MainWindow::updatePiece_slot(PiecePrototype* p) {
    scene->removeItem(p);
    p->update();
    scene->addItem(p);
}

// update a piece after movement
void MainWindow::addPiece_slot(PiecePrototype* p) {
    scene->addItem(p);
}

// update a piece after movement
void MainWindow::removePiece_slot(PiecePrototype* p) {
    scene->removeItem(p);
}

// when somebody wins update ther wins label and reset game
void MainWindow::gameOver_slot() {
    iterateWinLabel(gameboard_->getCurrentPlayer());
    Reset();
}

// when surrender is clicked, give a win to the non-surrenderer & reset
void MainWindow::on_surrenderButton_clicked() {
   iterateWinLabel(gameboard_->getOtherPlayer());
   Reset();
}

// when reset button is clicked, just reset
void MainWindow::on_resetButton_clicked() {
    Reset();
}

// destructor
MainWindow::~MainWindow() {
    delete ui;
}
