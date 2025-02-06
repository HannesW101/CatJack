#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "databasehandler.h"
#include "imagewidget.h"
#include "typewriterlabel.h"
#include "player.h"

#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QImageReader>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QFontDatabase>
#include <QRandomGenerator>
#include <QSlider>
#include <QFileInfo>
#include <QThread>

// global variable for current logged in user
QVariantMap userDetails;

// create Player objects for players/dealer
Player player1;
Player player2;
Player player3;
Player dealer;

// global cards dealt to keep track of which cards is deal in the current round
QVector<int> cardsDealt;

// int to hold the current bet value
int betValue = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // setup Home screen menu

    // set image allocation to 1 GB
    QImageReader::setAllocationLimit(524 * 524 * 524);

    // setup custom font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/resources/fonts/Casino.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont customFont(fontFamily, 12);

    // set custom cursor for hover ( custom cursor for window is set in main.cpp
    QPixmap pixmap_cursor_hover = QPixmap(":/images/resources/images/Cat_Cursor_hover.png");
    customCursor_hover = QCursor(pixmap_cursor_hover, 0, 0);

    // Remove window decorations
    this->setWindowFlags(Qt::FramelessWindowHint); // Remove the title bar and borders
    // Set the fixed window size to 1024x720 pixels
    this->setFixedSize(1024, 720);
    // Center the window on the screen using QScreen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    // Create QLabel for the background image
    backgroundLabel = new QLabel(this);
    QPixmap pixmap_backgroundLabel(":/images/resources/images/CatJack_Home_Menu_Background.jpeg");
    backgroundLabel->setPixmap(pixmap_backgroundLabel);
    backgroundLabel->setScaledContents(true); // Ensure the image scales to the label size
    backgroundLabel->setGeometry(0, 0, this->width(), this->height()); // Cover the entire window
    backgroundLabel->lower(); // Ensure the label is behind all other widgets
    backgroundLabel->setStyleSheet(
        "border: 2px solid #E4A700;"
    );

    // Set up QMediaPlayer for background music
    background_music = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    background_music->setAudioOutput(audioOutput);
    background_music->setSource(QUrl("qrc:/sounds/resources/sounds/cat_jack_21_background_music.mp3"));
    audioOutput->setVolume(0.05);
    background_music->setLoops(QMediaPlayer::Infinite);
    background_music->play();

    // Create QPushButton for mute toggle
    muteButton = new QPushButton(this);
    muteButton->setGeometry(10, 10, 85, 85); // x y width height
    muteButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    image: url(:/images/resources/images/Sound_Normal2.png);"
        "}"
        "QPushButton:hover {"
        "    border: none;"
        "    image: url(:/images/resources/images/Sound_Hover2.png);"
        "}"
        );
    muteButton->setCursor(customCursor_hover);
    // connect button to toggle mute
    connect(muteButton, &QPushButton::clicked, this, &MainWindow::toggleMute);
    connect(muteButton, &QPushButton::clicked, [this]() { playSound("button");});

    // Create QPushButton for exit app
    exitButton = new QPushButton(this);
    exitButton->setGeometry(934, 10, 85, 85); // x y width height
    exitButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    image: url(:/images/resources/images/Exit_Normal2.png);"
        "}"
        "QPushButton:hover {"
        "    border: none;"
        "    image: url(:/images/resources/images/Exit_Hover2.png);"
        "}"
        );
    exitButton->setCursor(customCursor_hover);
    // connect button to exit app
    connect(exitButton, &QPushButton::clicked, QCoreApplication::quit);
    connect(exitButton, &QPushButton::clicked, [this]() { playSound("button");});

    // Create Single Player QPushButton
    singlePlayerButton = new QPushButton(this);
    singlePlayerButton->setGeometry(365, 475, 295, 80); // x y width height
    singlePlayerButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    image: url(:/images/resources/images/SinglePlayer_Normal.png);"
        "}"
        "QPushButton:hover {"
        "    border: none;"
        "    image: url(:/images/resources/images/SinglePlayer_Hover.png);"
        "}"
        );
    singlePlayerButton->setCursor(customCursor_hover);
    singlePlayerButton->hide();
    connect(singlePlayerButton, &QPushButton::clicked, [this]() { playSound("button");});
    connect(singlePlayerButton, &QPushButton::clicked, this, &MainWindow::startSinglePlayerGame);

    // Create Login QPushButton
    loginButton = new QPushButton(this);
    loginButton->setGeometry(380, 475, 295, 80); // x y width height
    loginButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    image: url(:/images/resources/images/LogIn_Normal.png);"
        "}"
        "QPushButton:hover {"
        "    border: none;"
        "    image: url(:/images/resources/images/LogIn_Hover.png);"
        "}"
        );
    loginButton->setCursor(customCursor_hover);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::addFrame);
    connect(loginButton, &QPushButton::clicked, [this]() { playSound("button");});

    // Create Credits QPushButton
    creditsButton = new QPushButton(this);
    creditsButton->setGeometry(752, 652, 295, 80); // x y width height
    creditsButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    image: url(:/images/resources/images/Fish_credits.png);"
        "}"
    );
    creditsButton->hide();

    // Create Credits QLabel
    creditsLabel = new QLabel("0", this);
    creditsLabel->setGeometry(755, 682, 245, 60); // x y width height
    creditsLabel->setFont(customFont);
    creditsLabel->setAlignment(Qt::AlignRight);
    creditsLabel->setStyleSheet(
        "border: none;"
        "font-size: 30px;"
    );
    creditsLabel->hide();

    // Create username QPushButton
    home_usernameButton = new QPushButton(this);
    home_usernameButton->setGeometry(-52, 652, 295, 80); // x y width height
    home_usernameButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    image: url(:/images/resources/images/Fish_credits.png);"
        "}"
        );
    home_usernameButton->hide();

    // Create username QLabel
    home_usernameLabel = new QLabel("", this);
    home_usernameLabel->setGeometry(-52, 682, 245, 60); // x y width height
    home_usernameLabel->setFont(customFont);
    home_usernameLabel->setAlignment(Qt::AlignRight);
    home_usernameLabel->setStyleSheet(
        "border: none;"
        "font-size: 30px;"
    );
    home_usernameLabel->hide();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::toggleMute() {
    if (audioOutput->isMuted()) {
        audioOutput->setMuted(false);
        muteButton->setStyleSheet(
            "QPushButton {"
            "    border: none;"
            "    image: url(:/images/resources/images/Sound_Normal2.png);"
            "}"
            "QPushButton:hover {"
            "    border: none;"
            "    image: url(:/images/resources/images/Sound_Hover2.png);"
            "}"
            ); // Icon for unmuted state
    } else {
        audioOutput->setMuted(true);
        muteButton->setStyleSheet(
            "QPushButton {"
            "    border: none;"
            "    image: url(:/images/resources/images/Mute_Normal2.png);"
            "}"
            "QPushButton:hover {"
            "    border: none;"
            "    image: url(:/images/resources/images/Mute_Hover2.png);"
            "}"
            ); // Icon for muted state
    }
}

#include <QPushButton>


void MainWindow::closeFrame(QFrame *frame) {
    frame->hide();
}

void MainWindow::startSinglePlayerGame() {
    // setup ui elements for singeplayer game

        singlePlayerButton->hide();
        // Remove the pixmap
        backgroundLabel->clear();
        // Set the background color to green
        backgroundLabel->setStyleSheet(
            "background-color: #0a9056;"
            "border: 2px solid #E4A700;"
        );
        backgroundLabel->setGeometry(0, 0, this->width(), this->height()); // Cover the entire window
        backgroundLabel->lower(); // Ensure the label is behind all other widgets

    // labels for the profile pics
        QLabel *player_1_pp = new QLabel("",this);
        player_1_pp->setGeometry(100,500,100,100);
        player_1_pp->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
        );
        player_1_pp->show();
        QLabel *player_2_pp = new QLabel("",this);
        player_2_pp->setGeometry(462,500,100,100);
        player_2_pp->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        player_2_pp->show();
        QLabel *player_3_pp = new QLabel("",this);
        player_3_pp->setGeometry(824,500,100,100);
        player_3_pp->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        player_3_pp->show();
        QLabel *dealer_pp = new QLabel("",this);
        dealer_pp->setGeometry(462,30,100,100);
        dealer_pp->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        dealer_pp->show();

    // labels for the usernames
        QLabel *player_1_username = new QLabel("MR MEOW",this);
        player_1_username->setGeometry(100,450,100,30);
        player_1_username->setStyleSheet(
            "border: 2px solid white;"
            "background-color: #E4A700;"
            "border-radius: 10px;"
            );
        player_1_username->setAlignment(Qt::AlignCenter);
        player_1_username->show();
        QLabel *player_2_username = new QLabel(userDetails["Username"].toString(),this);
        player_2_username->setGeometry(462,450,100,30);
        player_2_username->setStyleSheet(
            "border: 2px solid white;"
            "background-color: #E4A700;"
            "border-radius: 10px;"
            );
        player_2_username->setAlignment(Qt::AlignCenter);
        player_2_username->show();
        QLabel *player_3_username = new QLabel("Purrrfect",this);
        player_3_username->setGeometry(824,450,100,30);
        player_3_username->setStyleSheet(
            "border: 2px solid white;"
            "background-color: #E4A700;"
            "border-radius: 10px;"
            );
        player_3_username->setAlignment(Qt::AlignCenter);
        player_3_username->show();
        QLabel *dealer_username = new QLabel("DEALER",this);
        dealer_username->setGeometry(462,150,100,30);
        dealer_username->setStyleSheet(
            "border: 2px solid white;"
            "background-color: #E4A700;"
            "border-radius: 10px;"
            );
        dealer_username->setAlignment(Qt::AlignCenter);
        dealer_username->show();

    // set profile pics
        int userIndex = userDetails["ProfilePic"].toInt() + 1;
        int player_1_randomNumber;
        do {
            player_1_randomNumber = QRandomGenerator::global()->bounded(1,16);
        } while (player_1_randomNumber == userIndex);
        int player_2_randomNumber;
        do {
            player_2_randomNumber = QRandomGenerator::global()->bounded(1,16);
        } while (player_2_randomNumber == userIndex && player_2_randomNumber == player_1_randomNumber);

        QString player_2_path = QString(":/profile_pics/resources/profile_pics/pp_%1.png").arg(userIndex);
        QPixmap player_2_pixmap(player_2_path);
        player_2_pp->setPixmap(player_2_pixmap);
        player_2_pp->setAlignment(Qt::AlignCenter);

        QString player_1_path = QString(":/profile_pics/resources/profile_pics/pp_%1.png").arg(player_1_randomNumber);
        QPixmap player_1_pixmap(player_1_path);
        player_1_pp->setPixmap(player_1_pixmap);
        player_1_pp->setAlignment(Qt::AlignCenter);

        QString player_3_path = QString(":/profile_pics/resources/profile_pics/pp_%1.png").arg(player_2_randomNumber);
        QPixmap player_3_pixmap(player_3_path);
        player_3_pp->setPixmap(player_3_pixmap);
        player_3_pp->setAlignment(Qt::AlignCenter);

        QString dealer_path = QString(":/profile_pics/resources/profile_pics/pp_dealer.png");
        QPixmap dealer_pixmap(dealer_path);
        dealer_pp->setPixmap(dealer_pixmap);
        dealer_pp->setAlignment(Qt::AlignCenter);

    // player/dealer cards
        QLabel *player_1_card_1 = new QLabel("",this);
        player_1_card_1->setGeometry(90,350,50,70);
        player_1_card_1->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
        );
        player_1_card_1->show();

        QLabel *player_1_card_2 = new QLabel("",this);
        player_1_card_2->setGeometry(160,350,50,70);
        player_1_card_2->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        player_1_card_2->show();

        QLabel *player_2_card_1 = new QLabel("",this);
        player_2_card_1->setGeometry(452,350,50,70);
        player_2_card_1->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        player_2_card_1->show();
        QLabel *player_2_card_2 = new QLabel("",this);
        player_2_card_2->setGeometry(522,350,50,70);
        player_2_card_2->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        player_2_card_2->show();

        QLabel *player_3_card_1 = new QLabel("",this);
        player_3_card_1->setGeometry(814,350,50,70);
        player_3_card_1->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        player_3_card_1->show();
        QLabel *player_3_card_2 = new QLabel("",this);
        player_3_card_2->setGeometry(884,350,50,70);
        player_3_card_2->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        player_3_card_2->show();

        QLabel *dealer_card_1 = new QLabel("",this);
        dealer_card_1->setGeometry(452,210,50,70);
        dealer_card_1->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        dealer_card_1->show();
        QLabel *dealer_card_2 = new QLabel("",this);
        dealer_card_2->setGeometry(522,210,50,70);
        dealer_card_2->setStyleSheet(
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "border-radius: 10px;"
            );
        dealer_card_2->show();

    // QString list to hold the paths to all the cards, can be accessed by an index (randomly accessed for dealing the cards)
        QStringList cardList = { // 52 cards
            ":/cards/resources/cards/ace_clubs.png",
            ":/cards/resources/cards/ace_diamonds.png",
            ":/cards/resources/cards/ace_hearts.png",
            ":/cards/resources/cards/ace_spades.png",
            ":/cards/resources/cards/2_clubs.png",
            ":/cards/resources/cards/2_diamonds.png",
            ":/cards/resources/cards/2_hearts.png",
            ":/cards/resources/cards/2_spades.png",
            ":/cards/resources/cards/3_clubs.png",
            ":/cards/resources/cards/3_diamonds.png",
            ":/cards/resources/cards/3_hearts.png",
            ":/cards/resources/cards/3_spades.png",
            ":/cards/resources/cards/4_clubs.png",
            ":/cards/resources/cards/4_diamonds.png",
            ":/cards/resources/cards/4_hearts.png",
            ":/cards/resources/cards/4_spades.png",
            ":/cards/resources/cards/5_clubs.png",
            ":/cards/resources/cards/5_diamonds.png",
            ":/cards/resources/cards/5_hearts.png",
            ":/cards/resources/cards/5_spades.png",
            ":/cards/resources/cards/6_clubs.png",
            ":/cards/resources/cards/6_diamonds.png",
            ":/cards/resources/cards/6_hearts.png",
            ":/cards/resources/cards/6_spades.png",
            ":/cards/resources/cards/7_clubs.png",
            ":/cards/resources/cards/7_diamonds.png",
            ":/cards/resources/cards/7_hearts.png",
            ":/cards/resources/cards/7_spades.png",
            ":/cards/resources/cards/8_clubs.png",
            ":/cards/resources/cards/8_diamonds.png",
            ":/cards/resources/cards/8_hearts.png",
            ":/cards/resources/cards/8_spades.png",
            ":/cards/resources/cards/9_clubs.png",
            ":/cards/resources/cards/9_diamonds.png",
            ":/cards/resources/cards/9_hearts.png",
            ":/cards/resources/cards/9_spades.png",
            ":/cards/resources/cards/10_clubs.png",
            ":/cards/resources/cards/10_diamonds.png",
            ":/cards/resources/cards/10_hearts.png",
            ":/cards/resources/cards/10_spades.png",
            ":/cards/resources/cards/jack_clubs.png",
            ":/cards/resources/cards/jack_diamonds.png",
            ":/cards/resources/cards/jack_hearts.png",
            ":/cards/resources/cards/jack_spades.png",
            ":/cards/resources/cards/queen_clubs.png",
            ":/cards/resources/cards/queen_diamonds.png",
            ":/cards/resources/cards/queen_hearts.png",
            ":/cards/resources/cards/queen_spades.png",
            ":/cards/resources/cards/king_clubs.png",
            ":/cards/resources/cards/king_diamonds.png",
            ":/cards/resources/cards/king_hearts.png",
            ":/cards/resources/cards/king_spades.png"
        };


    // setup the bottom ui elements for placing a bet (this will also start each singleplayer round)
        // Create a vertical layout for the slider and button
        QVBoxLayout *mainLayout = new QVBoxLayout;
        // Create a horizontal layout for the min value label, slider, and max value label
        QHBoxLayout *sliderLayout = new QHBoxLayout;
        // Create labels for the min and max values of the slider
        QLabel *minValueLabel = new QLabel("1", this);
        QLabel *maxValueLabel = new QLabel(userDetails["Credits"].toString(), this);
        // Create the slider for betting
        QSlider *betSlider = new QSlider(Qt::Horizontal, this);
        betSlider->setRange(1, userDetails["Credits"].toInt());
        betSlider->setValue(1);     // Set a default value
        betSlider->setStyleSheet(
            "QSlider::groove:horizontal {"
            "background-color: #E4A700;"
            "height: 8px;"
            "border: 2px solid white;"
            "}"
            "QSlider::handle:horizontal {"
            "background-color: white;"
            "width: 15px;"
            "margin: -40px 0;"
            "}"
        );
        betSlider->setCursor(customCursor_hover);
        // Create a label to display the current value of the slider
        QLabel *currentValueLabel = new QLabel(QString::number(betSlider->value()), this);
        currentValueLabel->setAlignment(Qt::AlignCenter);
        // Connect the slider's valueChanged signal to update the current value label
        connect(betSlider, &QSlider::valueChanged, currentValueLabel, [=](int value) {
            currentValueLabel->setText(QString::number(value));
        });
        // Add the min value label, slider, and max value label to the slider layout
        sliderLayout->addWidget(minValueLabel);
        sliderLayout->addWidget(betSlider);
        sliderLayout->addWidget(maxValueLabel);
        // Create the button to place the bet
        QPushButton *placeBetButton = new QPushButton("Place Bet", this);
        placeBetButton->setFixedSize(120,40);
        placeBetButton->setStyleSheet(
            "QPushButton {"
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "}"
            "QPushButton:hover {"
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #B38300;"
            "border: 2px solid white;"
            "}"
        );
        placeBetButton->setCursor(customCursor_hover);
        connect(placeBetButton, &QPushButton::clicked, [this]() { playSound("button");});
        // Add the current value label, slider layout, and button to the main layout
        mainLayout->addWidget(currentValueLabel);
        mainLayout->addLayout(sliderLayout);
        mainLayout->addWidget(placeBetButton, 0, Qt::AlignCenter);
        // Create a container widget for the main layout
        QWidget *container = new QWidget(this);
        container->setLayout(mainLayout);
        container->setGeometry(260, this->height() - 100, 500, 100); // Adjust the position and size as needed
        container->show();

    // setup the labels for the current hands of players and dealer
        QLabel *player_1_score = new QLabel("Blackjack!",this);
        player_1_score->setGeometry(100,425,100,20);
        player_1_score->setAlignment(Qt::AlignCenter);
        player_1_score->hide();
        QLabel *player_2_score = new QLabel("Blackjack!",this);
        player_2_score->setGeometry(462,425,100,20);
        player_2_score->setAlignment(Qt::AlignCenter);
        player_2_score->hide();
        QLabel *player_3_score = new QLabel("Blackjack!",this);
        player_3_score->setGeometry(824,425,100,20);
        player_3_score->setAlignment(Qt::AlignCenter);
        player_3_score->hide();
        QLabel *dealer_score = new QLabel("Blackjack!",this);
        dealer_score->setGeometry(462,185,100,20);
        dealer_score->setAlignment(Qt::AlignCenter);
        dealer_score->hide();

    // UI elements for actions during the round
        QPushButton *hitButton = new QPushButton("Hit", this);
        hitButton->setFixedSize(120, 40);
        hitButton->setCursor(customCursor_hover);
        hitButton->setStyleSheet(
            "QPushButton {"
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "}"
            "QPushButton:hover {"
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #B38300;"
            "border: 2px solid white;"
            "}"
        );
        QPushButton *standButton = new QPushButton("Stand", this);
        standButton->setFixedSize(120, 40);
        standButton->setCursor(customCursor_hover);
        standButton->setStyleSheet(
            "QPushButton {"
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "}"
            "QPushButton:hover {"
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #B38300;"
            "border: 2px solid white;"
            "}"
        );
        // Layout for round actions
        QHBoxLayout *actionLayout = new QHBoxLayout();
        actionLayout->addWidget(hitButton);
        actionLayout->addWidget(standButton);
        QWidget *roundContainer = new QWidget(this);
        roundContainer->setLayout(actionLayout);
        roundContainer->setGeometry(260, this->height() - 100, 500, 100); // Adjust as needed

    // Lambda function to get the next unique random index
        auto getNextUniqueRandomIndex = []() {
            int randomNumber;
            do {
                randomNumber = QRandomGenerator::global()->bounded(52);
            } while (std::find(cardsDealt.begin(), cardsDealt.end(), randomNumber) != cardsDealt.end());
            cardsDealt.append(randomNumber);
            return randomNumber;
        };

    // Start the round when a bet is placed
        connect(placeBetButton, &QPushButton::clicked, this, [=]() {
            // Hide slider and UI elements at the bottom
            container->hide();
            container->lower();
            creditsLabel->setText(QString::number(userDetails["Credits"].toInt()-betSlider->value()));
            betValue = betSlider->value();

            // Initialize player_cards and dealer_cards
            QVector<QLabel*> player_cards[3] = {
                {player_1_card_1, player_1_card_2},
                {player_2_card_1, player_2_card_2},
                {player_3_card_1, player_3_card_2}
            };
            QVector<QLabel*> dealer_cards = {dealer_card_1, dealer_card_2};

            // Clear the pixmaps of all player and dealer card labels
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 2; ++j) {
                    player_cards[i][j]->clear();
                }
            }
            dealer_cards[0]->clear();
            dealer_cards[1]->clear();

            // clear all scores
            player_1_score->setText("0");
            player_2_score->setText("0");
            player_3_score->setText("0");
            dealer_score->setText("0");
            player1.resetScore();
            player2.resetScore();
            player3.resetScore();
            dealer.resetScore();

            // Create a list of all the cards to be dealt
            QList<QPair<QLabel*, QString>> cardsToDeal;
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 3; ++j) {
                    QString cardPath = cardList[getNextUniqueRandomIndex()];
                    cardsToDeal.append(qMakePair(player_cards[j][i], cardPath));
                    if (j == 0) player1.addCard(cardPath); // Player 1 gets the card
                    else if (j == 1) player2.addCard(cardPath); // Player 2 gets the card
                    else player3.addCard(cardPath); // Player 3 gets the card
                }
                QString dealerCardPath = cardList[getNextUniqueRandomIndex()];
                cardsToDeal.append(qMakePair(dealer_cards[i], dealerCardPath));
                dealer.addCard(dealerCardPath); // Deal to the dealer
            }

            // Debugging output to verify the hands
            qDebug() << "cards generated to be dealt initially:" << cardsDealt;
            qDebug() << "Player 1 initial hand:" << player1.getHand();
            qDebug() << "Player 2 initial hand:" << player2.getHand();
            qDebug() << "Player 3 initial hand:" << player3.getHand();
            qDebug() << "Dealer initial hand:" << dealer.getHand();

            // Function to deal the next card in the list
            auto dealNextCard = [=](int index) {
                if (index < cardsToDeal.size()) {
                    QLabel* cardLabel = cardsToDeal[index].first;
                    QString cardPath = cardsToDeal[index].second;
                    QFileInfo check_file(cardPath);
                    if (check_file.exists() && check_file.isFile()) {
                        if (cardLabel == dealer_cards[1]) {
                            // If it's the dealer's second card, set it to the card back after dealing
                            QTimer::singleShot(600, [=]() {
                                cardLabel->setPixmap(QPixmap(":/images/resources/images/card_back.png").scaled(50, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                                cardLabel->setAlignment(Qt::AlignCenter);
                                // Play the card dealing sound effect
                                playSound("card");
                            });
                        } else {
                            QPixmap card_pixmap(cardPath);
                            cardLabel->setPixmap(card_pixmap.scaled(50, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                            cardLabel->setAlignment(Qt::AlignCenter);
                            // Play the card dealing sound effect
                            playSound("card");
                        }
                    }
                }
            };

            // Use a single QTimer to deal the cards with delays
            for (int i = 0; i < cardsToDeal.size(); ++i) {
                QTimer::singleShot(i * 600, this, [=]() {
                    dealNextCard(i);
                });
            }

            qDebug() << player1.calculateScore();
            qDebug() << player2.calculateScore();
            qDebug() << player3.calculateScore();
            qDebug() << dealer.calculateScore();

            // Calculate and display hand values after all cards are dealt
            QTimer::singleShot(cardsToDeal.size() * 600 + 600, this, [=]() {
                if (player1.calculateScore() == 21){
                    player_1_score->setText("Blackjack!");
                } else {
                    player_1_score->setText(QString::number(player1.calculateScore()));
                }
                if (player2.calculateScore() == 21){
                    player_2_score->setText("Blackjack!");
                    hitButton->hide();
                } else {
                    player_2_score->setText(QString::number(player2.calculateScore()));
                }
                if (player3.calculateScore() == 21){
                    player_3_score->setText("Blackjack!");
                } else {
                    player_3_score->setText(QString::number(player3.calculateScore()));
                }
                dealer_score->setText(QString::number(dealer.calculateVisibleScore()));
                player_1_score->show();
                player_2_score->show();
                player_3_score->show();
                dealer_score->show();
                roundContainer->show();
                roundContainer->raise();
            });
        });

    // The hit button logic
        connect(hitButton, &QPushButton::clicked, this, [=]() {
            qDebug() << "Hit button clicked: dealing a new card";

            QTimer::singleShot(600, this, [=]() {
                player_2_card_2->setPixmap(player_2_card_1->pixmap());
                player_2_card_1->clear();
                playSound("card");

                QTimer::singleShot(600, this, [=]() {
                    // Get the next unique random card
                    int newCardIndex = getNextUniqueRandomIndex();
                    QString newCardPath = cardList[newCardIndex];
                    qDebug() << "New card index:" << newCardIndex << ", New card path:" << newCardPath;


                    QPixmap newCardPixmap(newCardPath);
                    player_2_card_1->setPixmap(newCardPixmap.scaled(50, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    player_2_card_1->setAlignment(Qt::AlignCenter);
                    playSound("card");

                    // Add the new card to Player 2's hand
                    player2.addCard(newCardPath);
                    qDebug() << "Player 2's updated hand:" << player2.getHand();

                    // Calculate new hand value
                    int newPlayer2Value = player2.calculateScore();
                    if (newPlayer2Value < 21){
                        player_2_score->setText(QString::number(newPlayer2Value));
                    } else if (newPlayer2Value == 21){
                        player_2_score->setText("Blackjack!");
                        hitButton->hide();
                    } else {
                        player_2_score->setText("Bust!");
                        hitButton->hide();
                    }
                });
            });
        });

        // The stand button logic
        connect(standButton, &QPushButton::clicked, this, [=]() {
            qDebug() << "Stand button clicked";
            // Hide round container
            roundContainer->hide();
            roundContainer->lower();

            // Start AI's turn for player 1 and player 3
            for (int i = 0; i < 3; i++) {
                QTimer::singleShot(2400 * i, this, [=]() mutable {
                    if (player1.aiBestMove(dealer.calculateVisibleScore()) == "Stand" &&
                        player3.aiBestMove(dealer.calculateVisibleScore()) == "Stand") {
                        // Flip over dealer card and see if the player won
                        dealer_card_2->setPixmap(QPixmap(dealer.getHand().at(1)).scaled(50, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        dealer_score->setText(QString::number(dealer.calculateScore()));
                    } else {
                        if (player1.aiBestMove(dealer.calculateVisibleScore()) != "Stand") {
                            handleAITurn_player1(player_1_card_1, player_1_card_2, player_1_score, cardList);
                            playSound("card");
                        }
                        if (player3.aiBestMove(dealer.calculateVisibleScore()) != "Stand") {
                            handleAITurn_player3(player_3_card_1, player_3_card_2, player_3_score, cardList);
                            playSound("card");
                        }
                    }
                });
            }

            // Schedule container to be shown after all iterations are complete
            QTimer::singleShot(2400 * 3, this, [=]() {

                // Check if player 2 wins
                bool player2Wins = (player_2_score->text() == "blackjack") ||
                                   (player_2_score->text().toInt() > dealer_score->text().toInt() && player_2_score->text().toInt() < 21);

                int newCredits = creditsLabel->text().toInt();

                if (player2Wins) {
                    // Player wins, update credits accordingly
                    newCredits += betValue * 1.5;
                } else {
                    // Player loses, subtract the bet value from credits
                    newCredits -= betValue;
                }

                // Update the creditsLabel with the new value
                creditsLabel->setText(QString::number(newCredits));
                // Update the userDetails map with the new credits value
                userDetails["Credits"] = newCredits;
                // Call the database update method to update the credits on the server
                dbhandler.updateUserCredits(userDetails["Username"].toString(), newCredits);

                // Update the creditsLabel with the updated credits in userDetails (should be the same as newCredits)
                creditsLabel->setText(QString::number(userDetails["Credits"].toInt()));

                // Show the container and set other UI elements
                container->show();
                container->raise();
                betSlider->setRange(1, userDetails["Credits"].toInt());
                maxValueLabel->setText(userDetails["Credits"].toString());
            });
        });
}

void MainWindow::handleAITurn_player1(QLabel* player_card_1, QLabel* player_card_2, QLabel* player_score, QStringList cardList) {

    auto getNextUniqueRandomIndex = []() {
        int randomNumber;
        do {
            randomNumber = QRandomGenerator::global()->bounded(52);
        } while (std::find(cardsDealt.begin(), cardsDealt.end(), randomNumber) != cardsDealt.end());
        cardsDealt.append(randomNumber);
        return randomNumber;
    };

    qDebug() << "AI best move:" << player1.aiBestMove(dealer.calculateVisibleScore());
    if (player1.aiBestMove(dealer.calculateVisibleScore()) == "Hit" && player1.calculateScore() < 21) {
        qDebug() << "AI decides to hit.";
        // Hit logic for player
        QTimer::singleShot(600, this, [=]() {
            player_card_2->setPixmap(player_card_1->pixmap());
            player_card_1->clear();
            playSound("card");

            QTimer::singleShot(600, this, [=]() {
                qDebug() << "Executing hit logic.";
                // Get the next unique random card
                int newCardIndex = getNextUniqueRandomIndex();
                QString newCardPath = cardList[newCardIndex];
                qDebug() << "New card index:" << newCardIndex << ", New card path:" << newCardPath;

                QPixmap newCardPixmap(newCardPath);
                player_card_1->setPixmap(newCardPixmap.scaled(50, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                player_card_1->setAlignment(Qt::AlignCenter);
                playSound("card");

                // Add the new card to Player's hand
                player1.addCard(newCardPath);
                qDebug() << "Player's updated hand:" << player1.getHand();

                // Calculate new hand value
                int newPlayerValue = player1.calculateScore();
                qDebug() << "Player's new hand value:" << newPlayerValue;

                if (newPlayerValue == 21) {
                    player_score->setText("Blackjack!");
                    qDebug() << "Player got Blackjack!";
                } else if (newPlayerValue > 21) {
                    player_score->setText("Bust!");
                    qDebug() << "Player busted.";
                } else {
                    player_score->setText(QString::number(newPlayerValue));
                    qDebug() << "Player new score:" << newPlayerValue;
                }
            });
        });
    } else {
        qDebug() << "Player AI decides to stand or player is bust.";
    }
}

void MainWindow::handleAITurn_player3(QLabel* player_card_1, QLabel* player_card_2, QLabel* player_score, QStringList cardList) {

    auto getNextUniqueRandomIndex = []() {
        int randomNumber;
        do {
            randomNumber = QRandomGenerator::global()->bounded(52);
        } while (std::find(cardsDealt.begin(), cardsDealt.end(), randomNumber) != cardsDealt.end());
        cardsDealt.append(randomNumber);
        return randomNumber;
    };

    qDebug() << "AI best move:" << player3.aiBestMove(dealer.calculateVisibleScore());
    if (player3.aiBestMove(dealer.calculateVisibleScore()) == "Hit" && player3.calculateScore() < 21) {
        qDebug() << "AI decides to hit.";
        // Hit logic for player
        QTimer::singleShot(600, this, [=]() {
            player_card_2->setPixmap(player_card_1->pixmap());
            player_card_1->clear();
            playSound("card");

            QTimer::singleShot(600, this, [=]() {
                qDebug() << "Executing hit logic.";
                // Get the next unique random card
                int newCardIndex = getNextUniqueRandomIndex();
                QString newCardPath = cardList[newCardIndex];
                qDebug() << "New card index:" << newCardIndex << ", New card path:" << newCardPath;

                QPixmap newCardPixmap(newCardPath);
                player_card_1->setPixmap(newCardPixmap.scaled(50, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                player_card_1->setAlignment(Qt::AlignCenter);
                playSound("card");

                // Add the new card to Player's hand
                player3.addCard(newCardPath);
                qDebug() << "Player's updated hand:" << player3.getHand();

                // Calculate new hand value
                int newPlayerValue = player3.calculateScore();
                qDebug() << "Player's new hand value:" << newPlayerValue;

                if (newPlayerValue == 21) {
                    player_score->setText("Blackjack!");
                    qDebug() << "Player got Blackjack!";
                } else if (newPlayerValue > 21) {
                    player_score->setText("Bust!");
                    qDebug() << "Player busted.";
                } else {
                    player_score->setText(QString::number(newPlayerValue));
                    qDebug() << "Player new score:" << newPlayerValue;
                }
            });
        });
    } else {
        qDebug() << "Player AI decides to stand or player is bust.";
    }
}

void MainWindow::addFrame() {

    // Create a new frame
    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::Box);
    frame->setStyleSheet(
        "background-color: #0a9056;"
        "border: 2px solid #E4A700;"
        "border-radius: 15px;"
    );
    frame->setFixedSize(800, 460); // width x height
    frame->move(112, 230);

    // Create the close button
    QPushButton *closeButton = new QPushButton("X", frame);
    closeButton->setFixedSize(50, 50); // Adjust size as needed
    closeButton->move(730, 20);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    image: url(:/images/resources/images/Exit_Normal2.png);"
        "}"
        "QPushButton:hover {"
        "    border: none;"
        "    image: url(:/images/resources/images/Exit_Hover2.png);"
        "}"
        );
    closeButton->setCursor(QCursor(QPixmap(":/images/resources/images/Cat_Cursor_hover.png"), 0, 0));
    // Connect the close button's clicked signal to the slot
    connect(closeButton, &QPushButton::clicked, [=]() { closeFrame(frame); });
    connect(closeButton, &QPushButton::clicked, [this]() { playSound("button");});

    // Identify the sender button and add different ui elements to frame based on the sender
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());

    // if login button is pressed
    if (senderButton == loginButton){
        // Create a label for "Username"
        QLabel *usernameLabel = new QLabel("Username:", frame);
        usernameLabel->setGeometry(215, 50, 150, 41); // 295 x 80
        usernameLabel->setAlignment(Qt::AlignCenter);
        usernameLabel->setStyleSheet(
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
        );

        // Create a text box (QLineEdit) for username input
        QLineEdit *usernameEdit = new QLineEdit(frame);
        usernameEdit->setGeometry(385, 50, 200, 41);
        usernameEdit->setAlignment(Qt::AlignCenter);
        usernameEdit->setMaxLength(11);
        usernameEdit->setStyleSheet(
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
        );
        QRegularExpression regex("[a-zA-Z0-9 ]+"); // allow letters, numbers, and spaces
        QValidator *validator = new QRegularExpressionValidator(regex, usernameEdit); usernameEdit->setValidator(validator);
        usernameEdit->setValidator(validator);

        // Create a label for "Password"
        QLabel *passwordLabel = new QLabel("Password:", frame);
        passwordLabel->setGeometry(215, 111, 150, 41); // 295 x 80
        passwordLabel->setAlignment(Qt::AlignCenter);
        passwordLabel->setStyleSheet(
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
        );

        // Create a label for "Password Confirmation"
        QLabel *password_confirmLabel = new QLabel("Confirm Password:", frame);
        password_confirmLabel->setGeometry(165, 172, 200, 41); // 295 x 80
        password_confirmLabel->setAlignment(Qt::AlignCenter);
        password_confirmLabel->setStyleSheet(
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
        );
        password_confirmLabel->hide(); // hide the element for non registering users

        // Create a text box (QLineEdit) for password input
        QLineEdit *passwordEdit = new QLineEdit(frame);
        passwordEdit->setGeometry(385, 111, 200, 41);
        passwordEdit->setAlignment(Qt::AlignCenter);
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordEdit->setMaxLength(20);
        passwordEdit->setStyleSheet(
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
        );

        // Create a text box (QLineEdit) for password confirmation input
        QLineEdit *password_confirmEdit = new QLineEdit(frame);
        password_confirmEdit->setGeometry(385, 172, 200, 41);
        password_confirmEdit->setAlignment(Qt::AlignCenter);
        password_confirmEdit->setEchoMode(QLineEdit::Password);
        password_confirmEdit->setMaxLength(20);
        password_confirmEdit->setStyleSheet(
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
        );
        password_confirmEdit->hide(); // hide the element for non registering users

        // button to toggle visibility of password
        QPushButton *toggleButton = new QPushButton("", frame);
        toggleButton->setGeometry(600, 118, 30, 30);
        toggleButton->setStyleSheet("border: none;");
        toggleButton->setIcon(QIcon(":/images/resources/images/password_eye.png"));
        toggleButton->setIconSize(QSize(30, 30));
        toggleButton->setCursor(customCursor_hover);
        // Connect the toggle button to toggle password visibility
        connect(toggleButton, &QPushButton::clicked, [=]() {
            if (passwordEdit->echoMode() == QLineEdit::Password) {
                passwordEdit->setEchoMode(QLineEdit::Normal);
                password_confirmEdit->setEchoMode(QLineEdit::Normal);
            } else {
                passwordEdit->setEchoMode(QLineEdit::Password);
                password_confirmEdit->setEchoMode(QLineEdit::Password);
            }
        });
        connect(toggleButton, &QPushButton::clicked, [this]() { playSound("button");});

        // login/register button ( will become register button when its a new user )
        QPushButton *login_register_Button = new QPushButton("login", frame);
        login_register_Button->setGeometry(325, 369, 150, 41);
        login_register_Button->setCursor(customCursor_hover);
        login_register_Button->setStyleSheet(
            "QPushButton {"
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "}"
            "QPushButton:hover {"
            "border-radius: 10px;"
            "font-size: 20px;"
            "background-color: #B38300;"
            "border: 2px solid white;"
            "}"
        );
        connect(login_register_Button, &QPushButton::clicked, [this]() { playSound("button");});

        // setup image widget for the user to select profile pic
        ImageWidget *imageWidget = new ImageWidget(frame);
        QStringList imagePaths = {
            ":/profile_pics/resources/profile_pics/pp_1.png",
            ":/profile_pics/resources/profile_pics/pp_2.png",
            ":/profile_pics/resources/profile_pics/pp_3.png",
            ":/profile_pics/resources/profile_pics/pp_4.png",
            ":/profile_pics/resources/profile_pics/pp_5.png",
            ":/profile_pics/resources/profile_pics/pp_6.png",
            ":/profile_pics/resources/profile_pics/pp_7.png",
            ":/profile_pics/resources/profile_pics/pp_8.png",
            ":/profile_pics/resources/profile_pics/pp_9.png",
            ":/profile_pics/resources/profile_pics/pp_10.png",
            ":/profile_pics/resources/profile_pics/pp_11.png",
            ":/profile_pics/resources/profile_pics/pp_12.png",
            ":/profile_pics/resources/profile_pics/pp_13.png",
            ":/profile_pics/resources/profile_pics/pp_14.png",
            ":/profile_pics/resources/profile_pics/pp_15.png"
        };
        imageWidget->setImageList(imagePaths);
        imageWidget->setGeometry(140, 233, 150, 150);
        connect(imageWidget->getNextButton(), &QPushButton::clicked, [this]() { playSound("button");});
        connect(imageWidget->getPrevButton(), &QPushButton::clicked, [this]() { playSound("button");});
        QPushButton *nextBtn = imageWidget->getNextButton();
        nextBtn->hide();
        QPushButton *prevBtn = imageWidget->getPrevButton();
        prevBtn->hide();
        imageWidget->hide();

        // setup speech bubble and text
        QPushButton *speechBubbleButton = new QPushButton("", frame);
        speechBubbleButton->setIcon(QIcon(":/images/resources/images/Speech_Bubble.png"));
        speechBubbleButton->setStyleSheet("border: none;");
        speechBubbleButton->setIconSize(QSize(400, 120));
        speechBubbleButton->setGeometry(270, 243, 320, 100);
        speechBubbleButton->hide();

        TypewriterLabel *typewriterLabel = new TypewriterLabel(frame);
        typewriterLabel->setGeometry(270, 243, 320, 100);
        typewriterLabel->setTextToShow("Choose your profile picture!");
        typewriterLabel->hide();

        // Create a button for new users
        QPushButton *newUserButton = new QPushButton("new user? click here!", frame);
        newUserButton->setGeometry(280, 173, 240, 41);
        newUserButton->setCursor(customCursor_hover);
        newUserButton->setStyleSheet(
            "QPushButton {"
            "border-radius: 15px;"
            "font-size: 18px;"
            "background-color: #E4A700;"
            "border: 2px solid white;"
            "}"
            "QPushButton:hover {"
            "border-radius: 15px;"
            "font-size: 18px;"
            "background-color: #B38300;"
            "border: 2px solid white;"
            "}"
        );
        connect(newUserButton, &QPushButton::clicked, [this]() { playSound("button");});
        // Connect the new user button to hide itself when clicked and load up the ui elements for registering
        connect(newUserButton, &QPushButton::clicked, [=]() {
            newUserButton->hide();

            login_register_Button->setText("register");

            password_confirmEdit->show();
            password_confirmLabel->show();

            imageWidget->show();
            nextBtn->show();
            prevBtn->show();

            typewriterLabel->show();
            typewriterLabel->setTextToShow("Choose your profile picture!");
            typewriterLabel->startTypewriterEffect();
            speechBubbleButton->show();
        });

        // create DatabaseHandler instance
        DatabaseHandler *db_handler = new DatabaseHandler(this);

        // Connect button click to login/registration
        connect(login_register_Button, &QPushButton::clicked, [=]() {
            if (login_register_Button->text() == "login") {
                // retrieve the user info
                QString username = usernameEdit->text().toUpper();
                QVariantMap checkPass = db_handler->getUserDetails(username);

                // calculating error codes
                int error_code = 0;
                if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty()) error_code = 2;
                if (!db_handler->userExists(username) && error_code == 0) error_code = 3;
                if (passwordEdit->text().toUpper() != checkPass["Password"].toString() && error_code == 0) error_code = 4;

                // show the cat speaking to the user
                imageWidget->show();
                nextBtn->hide();
                prevBtn->hide();

                switch (error_code) {
                case 0:
                    // log in the user
                    speechBubbleButton->show();
                    typewriterLabel->show();
                    typewriterLabel->setTextToShow("Successfully logged in!");
                    typewriterLabel->startTypewriterEffect();
                    userDetails = checkPass;

                    // close frame after a few seconds and change home ui elements
                    QTimer::singleShot(4000, this, [=](){ closeFrame(frame); });
                    loginButton->hide();
                    singlePlayerButton->show();
                    creditsLabel->setText(userDetails["Credits"].toString());
                    creditsButton->show();
                    creditsLabel->show();
                    home_usernameLabel->setText(userDetails["Username"].toString());
                    home_usernameButton->show();
                    home_usernameLabel->show();
                    break;
                case 2:
                    speechBubbleButton->show();
                    typewriterLabel->show();
                    typewriterLabel->setTextToShow("Empty field!");
                    typewriterLabel->startTypewriterEffect();
                    break;
                case 3:
                    speechBubbleButton->show();
                    typewriterLabel->show();
                    typewriterLabel->setTextToShow("User does not exist!");
                    typewriterLabel->startTypewriterEffect();
                    break;
                case 4:
                    speechBubbleButton->show();
                    typewriterLabel->show();
                    typewriterLabel->setTextToShow("Incorrect password!");
                    typewriterLabel->startTypewriterEffect();
                    break;
                }
            } else if (login_register_Button->text() == "register") {
                // calculating error codes
                int error_code = 0;
                if (passwordEdit->text() != password_confirmEdit->text()) error_code = 1;
                if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty() || password_confirmEdit->text().isEmpty()) error_code = 2;
                if (db_handler->userExists(usernameEdit->text().toUpper())) error_code = 3;

                // New user details to be added if no errors in switch statement
                QVariantMap newUser;
                newUser["Username"] = usernameEdit->text().toUpper();
                newUser["Password"] = passwordEdit->text().toUpper();
                newUser["Credits"] = 500;
                newUser["ProfilePic"] = imageWidget->get_currentIndex();
                QString customKey = usernameEdit->text().toUpper();

                switch (error_code) {
                case 0:
                    // Add the user
                    db_handler->addUser(newUser, customKey);
                    typewriterLabel->setTextToShow("Successfully registered!");
                    typewriterLabel->startTypewriterEffect();
                    userDetails = newUser;
                    // close frame after a few seconds and change home ui elements
                    QTimer::singleShot(4000, this, [=](){ closeFrame(frame); });
                    loginButton->hide();
                    singlePlayerButton->show();
                    creditsLabel->setText(userDetails["Credits"].toString());
                    creditsButton->show();
                    creditsLabel->show();
                    home_usernameLabel->setText(userDetails["Username"].toString());
                    home_usernameButton->show();
                    home_usernameLabel->show();
                    break;
                case 1:
                    typewriterLabel->setTextToShow("Two passwords do not match!");
                    typewriterLabel->startTypewriterEffect();
                    break;
                case 2:
                    typewriterLabel->setTextToShow("Empty field!");
                    typewriterLabel->startTypewriterEffect();
                    break;
                case 3:
                    typewriterLabel->setTextToShow("User already exists!");
                    typewriterLabel->startTypewriterEffect();
                    break;
                }
            }
        });
    }

    frame->show();
}

void MainWindow::playSound(const QString &soundName){
    // make button sound effect and output it to user when button is pressed
    QMediaPlayer *buttonSound = new QMediaPlayer(this);
    audioOutput_buttonSound = new QAudioOutput;
    buttonSound->setAudioOutput(audioOutput_buttonSound);

    if (soundName == "button"){
        buttonSound->setSource(QUrl("qrc:/sounds/resources/sounds/button_effect.mp3"));
    } else if (soundName == "card"){
        buttonSound->setSource(QUrl("qrc:/sounds/resources/sounds/cards_being_dealt.mp3"));
    }

    audioOutput_buttonSound->setVolume(1.0);
    buttonSound->stop();
    buttonSound->play();
}
