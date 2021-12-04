#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <gameboard.h>
#include <rulespopup.h>
#include <winnerpopup.h>

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

    // media player for audio
    QMediaPlayer *media_player_;
    // timer for ai turns
    QTimer *timer_;

    // has a GameBoard to run the game
    GameBoard* gameboard_;

    // references to our popup windows
    RulesPopup* rules_pupup_;
    WinnerPopup* winner_popup_;

public:
    // constructor and destructor
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // reset the mainwindow to initial game state
    void Reset();

    // helper methods
    void playClickSound();
    void handleWinner(int winner);
    void handleMainMenuClick();

public slots:
    // slots to update labels and Pieces
    void updateTurnLabel_slot(int turn);
    void updatePiecesLabel_slot(bool red, int pieces);

    // slots to add, remove and update pieces in the scene
    void updatePiece_slot(PiecePrototype* p);
    void addPiece_slot(PiecePrototype* p);
    void removePiece_slot(PiecePrototype* p);

    // slot for when the game ends
    void gameOver_slot(int winner);

    // slots for handling popup windows
    void rules_Rejected_slot();
    void rules_Accepted_slot();
    void winner_PlayAgain_slot();
    void winner_Exit_slot();

    // slots for playing specific sounds
    void playSlideSound_slot();
    void playJumpSound_slot();
    void playDeniedSound_slot();

private slots:
    // default slots for button clicks
    void on_resetButton_clicked();
    void on_surrenderButton_clicked();
    void on_spButton_clicked();
    void on_mpButton_clicked();
    void on_easyButton_clicked();
    void on_mediumButton_clicked();
};

#endif // MAINWINDOW_H
