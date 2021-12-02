#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QTime>
#include <QStackedWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // we need to set up the ui before we draw on our scene
    ui->setupUi(this);
    // set widget to 0 (display main menu)
    ui->stackedWidget->setCurrentIndex(0);

    // set up audio player
    media_player = new QMediaPlayer();
    // set media to the game start click sound
    media_player->setMedia(QUrl("qrc:/audio/game_start.mp3"));
    media_player->setPlaybackRate(1.5);
    // set up popup for displaying rules
    rules_pupup_ = new RulesPopup();
    connect(rules_pupup_, SIGNAL(rulesRejected()), this, SLOT(rules_Rejected_slot()));
    connect(rules_pupup_, SIGNAL(rulesAccepted()), this, SLOT(rules_Accepted_slot()));

    // set up popup for displaying winner
    winner_popup_ = new WinnerPopup();
    connect(winner_popup_, SIGNAL(PlayAgain()), this, SLOT(winner_PlayAgain_slot()));
    connect(winner_popup_, SIGNAL(Exit()), this, SLOT(winner_Exit_slot()));

    // set up game with custom slots and signals
    gameboard_ = new GameBoard();
    connect(gameboard_, SIGNAL(updateTurnLabel(int)), this, SLOT(updateTurnLabel_slot(int)));
    connect(gameboard_, SIGNAL(updatePiecesLabel(bool,int)), this, SLOT(updatePiecesLabel_slot(bool,int)));
    connect(gameboard_, SIGNAL(updatePiece(PiecePrototype*)), this, SLOT(updatePiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(addPiece(PiecePrototype*)), this, SLOT(addPiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(removePiece(PiecePrototype*)), this, SLOT(removePiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(gameOver()), this, SLOT(gameOver_slot()));
    connect(gameboard_, SIGNAL(playSlideSound()), this, SLOT(playSlideSound_slot()));
    connect(gameboard_, SIGNAL(playJumpSound()), this, SLOT(playJumpSound_slot()));

    // the QGraphicsView is the UI element that contains the
    // scene that we will actually get to draw on.
    QGraphicsView * view = ui->gameGraphicsView;
    scene = new QGraphicsScene;
    view->setScene(scene);
    // make the scene the same size as the view containing it
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());

    // add all graphics items to game scene
    for (Tile* tile : gameboard_->getTiles() ) {
        // add all tiles to the scene
        scene->addItem(tile);
    }
    for (PiecePrototype* piece : gameboard_->getPieces() ) {
        // add all pieces to the scene
        scene->addItem(piece);
    }
    for (PowerUp* powerup : gameboard_->getPowerUps() ) {
         // add all powerups to the scene
        scene->addItem(powerup);
    }

    // set initial turn label
    ui->turnLabel->setText("TURN: RED");
    // gray out simulation button
    ui->simButton->setEnabled(false);

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

// helper method to give a player a win and update the label
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

void MainWindow::playClickSound() {
    media_player->setMedia(QUrl("qrc:/audio/menu_click.mp3"));
    media_player->setPlaybackRate(1);
    media_player->play();
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
    winner_popup_->setLabelText(gameboard_->getCurrentPlayerInt());
    winner_popup_->exec();
}

// when surrender is clicked, give a win to the non-surrenderer & reset
void MainWindow::on_surrenderButton_clicked() {
    playClickSound();
    iterateWinLabel(gameboard_->getOtherPlayer());
    int other_player = 0;
    if (gameboard_->getCurrentPlayerInt() == 0) {
        other_player = 1;
    }
    winner_popup_->setLabelText(other_player);
    winner_popup_->exec();
}

// when reset button is clicked, just reset
void MainWindow::on_resetButton_clicked() {
    playClickSound();
    Reset();
}

// destructor
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_spButton_clicked() {
    // play start song and change view to game screen
    media_player->play();
    ui->stackedWidget->setCurrentIndex(1);
    rules_pupup_->exec();
}

void MainWindow::on_mpButton_clicked() {
    // play start song and change view to game screen
    media_player->play();
    ui->stackedWidget->setCurrentIndex(1);
    rules_pupup_->exec();
}

void MainWindow::rules_Rejected_slot() {
    // if rules get rejected go back to main menu
    playClickSound();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::rules_Accepted_slot() {
    playClickSound();
}

void MainWindow::winner_Exit_slot() {
    // exit application when exit game is clicked
    playClickSound();
    exit(1);
}

void MainWindow::winner_PlayAgain_slot() {
    playClickSound();
    Reset();
}

void MainWindow::playSlideSound_slot() {
    media_player->setMedia(QUrl("qrc:/audio/piece_slide.mp3"));
    media_player->setPlaybackRate(2.5);
    media_player->play();
}

void MainWindow::playJumpSound_slot() {
    media_player->setMedia(QUrl("qrc:/audio/piece_jump.mp3"));
    media_player->setPlaybackRate(1.5);
    media_player->play();
}
