#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QMediaPlayer>
#include <QStackedWidget>

#include <pieceprototype.h>
#include <pieces.h>
#include <tile.h>
#include <powerup.h>
#include <gameboard.h>
#include <rulespopup.h>
#include <winnerpopup.h>

// setup mainwindow componenets
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
        // connect Tile signal and add all tiles to the scene
        connect(tile, SIGNAL(playDeniedSound()), this, SLOT(playDeniedSound_slot()));
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

/* Reset the gameboard and labels
*/
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
void MainWindow::PlayClickSound() {
    media_player_->setMedia(QUrl("qrc:/audio/audio/menu_click.mp3"));
    media_player_->setPlaybackRate(1);
    media_player_->play();
}

/* Update labels and display popup for winner
 * @param int winner of the game
*/
void MainWindow::HandleWinner(int winner) {
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

/* Update turn label
 * @param int whos turn it is
*/
void MainWindow::updateTurnLabel_slot(int turn) {
    QString turn_str = turn ? "BLACK" : "RED";
    ui->turnLabel->setText("TURN: " + turn_str);
}

/* Update pieces label
 * @param bool player to update for
 * @int num pieces they have left
*/
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

/* Manually update a piece after movement
 * @param Piece to update
*/
void MainWindow::updatePiece_slot(PiecePrototype* p) {
    if (gameboard_->get_difficulty() == Difficulty::Simulation) {
        scene->update();
    } else {
        scene->removeItem(p);
        scene->addItem(p);
    }
}

/* Add a piece to the scene
 * @param Piece to add
*/
void MainWindow::addPiece_slot(PiecePrototype* p) {
    scene->addItem(p);
}

/* Remove a piece from the scene
 * @param Piece to remove
*/
void MainWindow::removePiece_slot(PiecePrototype* p) {
    scene->removeItem(p);
}

/* Slot to handle the game ending
 * @param int winner of the game
*/
void MainWindow::gameOver_slot(int winner) {
    HandleWinner(winner);
}

/* When surrender is clicked give a win to the 'other' player
*/
void MainWindow::on_surrenderButton_clicked() {
    PlayClickSound();
    // check its not the computers turn
    if (!(gameboard_->get_difficulty() != Difficulty::None && gameboard_->get_current_player() == 1)) {
        // make the player whos turn its NOT the winner
        int other_player = 0;
        if (gameboard_->get_current_player() == 0) {
            other_player = 1;
        }
        // give the player a win and handle winner
        gameboard_->get_player(other_player)->set_num_wins(gameboard_->get_player(other_player)->get_num_wins()+1);
        HandleWinner(other_player);
    }
}

/* When reset button is clicked reset the game
*/
void MainWindow::on_resetButton_clicked() {
    PlayClickSound();
    Reset();
}

/* Display game rules when game is about to start
 * If they accept start game, else go back
*/
void MainWindow::HandleMainMenuClick() {
    // make sure games reset
    Reset();
    ui->surrenderButton->setDisabled(false);
    // play start song and change view to game screen
    media_player_->setMedia(QUrl("qrc:/audio/audio/game_start.mp3"));
    media_player_->setPlaybackRate(2);
    media_player_->play();
    ui->stackedWidget->setCurrentIndex(2);
    // open popup window to display rules
    rules_pupup_->exec();

}

/* When single player button clicked
 * Switch to difficulty menu
*/
void MainWindow::on_spButton_clicked() {
    // set page to difficulty screen
    PlayClickSound();
    ui->stackedWidget->setCurrentIndex(1);
}

/* When multiplayer button clicked
 * Set game difficulty to None and display rules
*/
void MainWindow::on_mpButton_clicked() {
    gameboard_->set_difficulty(Difficulty::None);
    HandleMainMenuClick();
}

/* When simulation button clicked run a simulation of the game
*/
void MainWindow::on_simButton_clicked() {
    // disbale surrender button during simulations
    ui->surrenderButton->setDisabled(true);
    // set difficulty to simulation and screen to game
    gameboard_->set_difficulty(Difficulty::Simulation);
    ui->stackedWidget->setCurrentIndex(2);
    // make sure game is reset so we dont miss anything
    Reset();
}

/* When Easy button clicked set difficulty to easy and display rules
*/
void MainWindow::on_easyButton_clicked() {
    // set difficulty and start game
    gameboard_->set_difficulty(Difficulty::Easy);
    HandleMainMenuClick();
}

/* When Medium button clicked set difficulty to medium and display rules
*/
void MainWindow::on_mediumButton_clicked() {
    // set difficulty and start game
    gameboard_->set_difficulty(Difficulty::Medium);
    HandleMainMenuClick();
}

/* When rules are rejected go back to menu
*/
void MainWindow::rules_Rejected_slot() {
    // play click sound and go back to main menu
    PlayClickSound();
    ui->stackedWidget->setCurrentIndex(0);
}

/* When rules are accpeted continue to the game screen
 *
*/
void MainWindow::rules_Accepted_slot() {
    // play click sound and continue
    PlayClickSound();
}

/* When the winner popup returns exit, exit the application
*/
void MainWindow::winner_Exit_slot() {
    // play click and exit application
    PlayClickSound();
    exit(1);
}

/* When the winner popup returns play again, reset the game
*/
void MainWindow::winner_PlayAgain_slot() {
    // play click sound and reset the game
    PlayClickSound();
    Reset();
}

/* When we hit back button from difficulty menu, go to main menu
*/
void MainWindow::on_difficultyBackButton_clicked() {
    // play click sound and go back to main menu
    PlayClickSound();
    ui->stackedWidget->setCurrentIndex(0);
}


/* When we click main menu button, stop all timers and go to main menu
*/
void MainWindow::on_mainmenuButton_clicked() {
    // play click sound and go back to main menu
    PlayClickSound();
    gameboard_->StopTimers();
    ui->stackedWidget->setCurrentIndex(0);
}

/* Slots for playing different sounds
*/
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

// destructor
MainWindow::~MainWindow() {
    delete ui;
}
