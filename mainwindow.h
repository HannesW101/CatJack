#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPushButton>
#include <QSoundEffect>

#include "databasehandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleMute();
    void addFrame();
    void closeFrame(QFrame *frame);
    void playSound(const QString &soundName);
    void startSinglePlayerGame();
    void handleAITurn_player1(QLabel* player_card_1, QLabel* player_card_2, QLabel* player_score, QStringList cardList);
    void handleAITurn_player3(QLabel* player_card_1, QLabel* player_card_2, QLabel* player_score, QStringList cardList);

private:
    Ui::MainWindow *ui;

    QLabel *backgroundLabel;
    QLabel *creditsLabel;
    QLabel *home_usernameLabel;

    QMediaPlayer *background_music;

    QAudioOutput *audioOutput;
    QAudioOutput *audioOutput_buttonSound;

    QPushButton *muteButton;
    QPushButton *exitButton;
    QPushButton *singlePlayerButton;
    QPushButton *loginButton;
    QPushButton *settingsButton;
    QPushButton *creditsButton;
    QPushButton *home_usernameButton;

    QMap<QString, QMediaPlayer*> soundEffects;

    QCursor customCursor_hover;

    DatabaseHandler dbhandler;
};
#endif // MAINWINDOW_H
