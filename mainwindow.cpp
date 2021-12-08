#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QMediaPlayer>
#include <QStackedWidget>

#include <gameboard.h>
#include <tile.h>
#include <pieceprototype.h>
#include <powerup.h>
#include <rulespopup.h>
#include <winnerpopup.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // we need to set up the ui before we draw on our scene
    ui->setupUi(this);

    // set widget to 0 (display main menu)
    ui->stackedWidget->setCurrentIndex(0);

    // set up audio player
    media_player_ = new QMediaPlayer(this);

    // set up popup for displaying rules
    rules_pupup_ = new RulesPopup(this);
    connect(rules_pupup_, SIGNAL(rulesRejected()), this, SLOT(rules_Rejected_slot()));
    connect(rules_pupup_, SIGNAL(rulesAccepted()), this, SLOT(rules_Accepted_slot()));

    // set up popup for displaying winner
    winner_popup_ = new WinnerPopup(this);
    connect(winner_popup_, SIGNAL(PlayAgain()), this, SLOT(winner_PlayAgain_slot()));
    connect(winner_popup_, SIGNAL(Exit()), this, SLOT(winner_Exit_slot()));

    // set up game with custom slots and signals
    gameboard_ = new GameBoard();
    connect(gameboard_, SIGNAL(updateTurnLabel(int)), this, SLOT(updateTurnLabel_slot(int)));
    connect(gameboard_, SIGNAL(updatePiecesLabel(bool,int)), this, SLOT(updatePiecesLabel_slot(bool,int)));
    connect(gameboard_, SIGNAL(updatePiece(PiecePrototype*)), this, SLOT(updatePiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(addPiece(PiecePrototype*)), this, SLOT(addPiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(removePiece(PiecePrototype*)), this, SLOT(removePiece_slot(PiecePrototype*)));
    connect(gameboard_, SIGNAL(gameOver(int)), this, SLOT(gameOver_slot(int)));
    connect(gameboard_, SIGNAL(playSlideSound()), this, SLOT(playSlideSound_slot()));
    connect(gameboard_, SIGNAL(playJumpSound()), this, SLOT(playJumpSound_slot()));
    connect(gameboard_, SIGNAL(playDeniedSound()), this, SLOT(playDeniedSound_slot()));

    // the QGraphicsView is the UI element that contains the
    // scene that we will actually get to draw on.
    QGraphicsView * view = ui->gameGraphicsView;
    scene = new QGraphicsScene;
    view->setScene(scene);
    // make the scene the same size as the view containing it
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());

    // add all graphics items to game scene
    for (Tile* tile : gameboard_->get_tiles() ) {
        connect(tile, SIGNAL(playDeniedSound()), this, SLOT(playDeniedSound_slot()));
        // add all tiles to the scene
        scene->addItem(tile);
    }
    for (PiecePrototype* piece : gameboard_->get_pieces() ) {
        // add all pieces to the scene
        scene->addItem(piece);
    }
    for (PowerUp* powerup : gameboard_->get_powerups()) {
         // add all powerups to the scene
        scene->addItem(powerup);
    }

    // set initial turn label
    ui->turnLabel->setText("TURN: RED");
    // gray out hard difficulty button
    ui->hardButton->setEnabled(false);

}

// reset the mainwindow
void MainWindow::Reset() {
    // remove all pieces
    for (PiecePrototype* piece : gameboard_->get_pieces() ) {
         scene->removeItem(piece);
    }
    for (PowerUp* powerup : gameboard_->get_powerups()) {
        scene->removeItem(powerup);
    }
    // reset gameboard
    gameboard_->NewGame();
    // add new pieces
    for (PiecePrototype* piece : gameboard_->get_pieces() ) {
        scene->addItem(piece);
    }
    // add powerups
    for (PowerUp* powerup : gameboard_->get_powerups() ) {
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

// play a regular click sound
void MainWindow::playClickSound() {
    media_player_->setMedia(QUrl("qrc:/audio/audio/menu_click.mp3"));
    media_player_->setPlaybackRate(1);
    media_player_->play();
}

// handle what happens when someone wins the game
void MainWindow::handleWinner(int winner) {
    // update win label
    if (winner == 0) {
        std::string s= "RED: " + std::to_string(gameboard_->get_player(winner)->get_num_wins()) + " Wins";
        QString pop_q(const_cast<char*>(s.c_str()));
        ui->redWinsLabel->setText(pop_q);
    } else {
        std::string s= "BLACK: " + std::to_string(gameboard_->get_player(winner)->get_num_wins()) + " Wins";
        QString pop_q(const_cast<char*>(s.c_str()));
        ui->blackWinsLabel->setText(pop_q);
    }

    // play winner sound
    media_player_->setMedia(QUrl("qrc:/audio/audio/player_won.mp3"));
    media_player_->setPlaybackRate(1.5);
    media_player_->play();

    // set text for winner popup and show it
    winner_popup_->setLabelText(winner);
    winner_popup_->exec();
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
void MainWindow::gameOver_slot(int winner) {
    handleWinner(winner);
}

// when surrender is clicked, give a win to the non-surrenderer & reset
void MainWindow::on_surrenderButton_clicked() {
    playClickSound();
    // check its not the computers turn
    if (!(gameboard_->get_difficulty() != Difficulty::None && gameboard_->get_current_player() == 1)) {
        // make the player whos turn its NOT the winner
        int other_player = 0;
        if (gameboard_->get_current_player() == 0) {
            other_player = 1;
        }
        // give the player a win and handle winner
        gameboard_->get_player(other_player)->set_num_wins(gameboard_->get_player(other_player)->get_num_wins()+1);
        handleWinner(other_player);
    }
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

// display rules and start setting up game
void MainWindow::handleMainMenuClick() {
    // make sure games reset
    Reset();
    // play start song and change view to game screen
    media_player_->setMedia(QUrl("qrc:/audio/audio/game_start.mp3"));
    media_player_->setPlaybackRate(2);
    media_player_->play();
    ui->stackedWidget->setCurrentIndex(2);
    // open popup window to display rules
    rules_pupup_->exec();

}

// slot for single player button being clicked
void MainWindow::on_spButton_clicked() {
    // set page to difficulty screen
    playClickSound();
    ui->stackedWidget->setCurrentIndex(1);
}

// slot for multiplayer button being clicked
void MainWindow::on_mpButton_clicked() {
    gameboard_->set_difficulty(Difficulty::None);
    handleMainMenuClick();
}

// slot for clicking easy difficulty button
void MainWindow::on_easyButton_clicked() {
    // set difficulty and start game
    gameboard_->set_difficulty(Difficulty::Easy);
    handleMainMenuClick();
}

// slot for clicking medium difficulty button
void MainWindow::on_mediumButton_clicked() {
    // set difficulty and start game
    gameboard_->set_difficulty(Difficulty::Medium);
    handleMainMenuClick();
}

// slot for when the rules get rejected
void MainWindow::rules_Rejected_slot() {
    // play click sound and go back to main menu
    playClickSound();
    ui->stackedWidget->setCurrentIndex(0);
}

// slot for when rules are accepted
void MainWindow::rules_Accepted_slot() {
    // play click sound and continue
    playClickSound();
}

// slot for when the winner popup chooses to exit the game
void MainWindow::winner_Exit_slot() {
    // play click and exit application
    playClickSound();
    exit(1);
}

// slot for when play again is chosen from the winner popup
void MainWindow::winner_PlayAgain_slot() {
    // play click sound and reset the game
    playClickSound();
    Reset();
}

// slots for playing different sounds
void MainWindow::playSlideSound_slot() {
    media_player_->setMedia(QUrl("qrc:/audio/audio/piece_slide.mp3"));
    media_player_->setPlaybackRate(2.5);
    media_player_->play();
}
void MainWindow::playJumpSound_slot() {
    media_player_->setMedia(QUrl("qrc:/audio/audio/piece_jump.mp3"));
    media_player_->setPlaybackRate(1.5);
    media_player_->play();
}
void MainWindow::playDeniedSound_slot() {
    media_player_->setMedia(QUrl("qrc:/audio/audio/piece_denied.mp3"));
    media_player_->setPlaybackRate(1);
    media_player_->play();
}

void MainWindow::on_difficultyBackButton_clicked() {
    // play click sound and go back to main menu
    playClickSound();
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_mainmenuButton_clicked() {
    // play click sound and go back to main menu
    playClickSound();
    gameboard_->StopTimers();
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_simButton_clicked() {
    gameboard_->set_difficulty(Difficulty::Simulation);
    ui->stackedWidget->setCurrentIndex(2);
    Reset();
}

