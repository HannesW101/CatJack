#include "Player.h"
#include <QDebug>
#include <QFileInfo>
#include <QRandomGenerator>

Player::Player() {
    score = 0;
}

void Player::addCard(const QString &cardPath) {
    hand.append(cardPath);
    qDebug() << "Added card:" << cardPath;
}

int Player::calculateScore() const {
    return calculateScoreForHand(hand);
}

int Player::calculateVisibleScore() const {
    // Only calculate the score based on the first card of the hand
    QVector<QString> visibleHand;
    if (!hand.isEmpty()) {
        visibleHand.append(hand.first());
    }
    return calculateScoreForHand(visibleHand);
}

bool Player::isSoftHand() const {
    for (const QString &card : hand) {
        QString cardValue = card.split("_").first();
        if (cardValue == "ace") {
            return true;
        }
    }
    return false;
}

QString Player::aiBestMove(int dealerVisibleScore) const {
    int currentScore = calculateScore();
    bool isSoft = isSoftHand();

    if (isSoft) {
        // Soft totals
        if (currentScore == 20) {
            return "Stand";
        } else if (currentScore == 19 && dealerVisibleScore == 6) {
            return "Stand";
        } else if (currentScore == 18) {
            if (dealerVisibleScore >= 2 && dealerVisibleScore <= 6) {
                return "Stand";
            } else if (dealerVisibleScore >= 9 || dealerVisibleScore == 1) {
                return "Hit";
            } else {
                return "Stand";
            }
        } else if (currentScore == 17 && dealerVisibleScore >= 3 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 16 && dealerVisibleScore >= 4 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 15 && dealerVisibleScore >= 4 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 14 && dealerVisibleScore >= 5 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 13 && dealerVisibleScore >= 5 && dealerVisibleScore <= 6) {
            return "Stand";
        } else {
            return "Hit";
        }
    } else {
        // Hard totals
        if (currentScore >= 17) {
            return "Stand";
        } else if (currentScore == 16 && dealerVisibleScore >= 2 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 15 && dealerVisibleScore >= 2 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 14 && dealerVisibleScore >= 2 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 13 && dealerVisibleScore >= 2 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 12 && dealerVisibleScore >= 4 && dealerVisibleScore <= 6) {
            return "Stand";
        } else if (currentScore == 11) {
            return "Hit"; // Always hit since doubling is not an option
        } else if (currentScore == 10 && dealerVisibleScore >= 2 && dealerVisibleScore <= 9) {
            return "Hit"; // Hit since doubling is not an option
        } else if (currentScore == 9 && dealerVisibleScore >= 3 && dealerVisibleScore <= 6) {
            return "Hit"; // Hit since doubling is not an option
        } else if (currentScore == 8) {
            return "Hit";
        } else {
            return "Hit";
        }
    }
}

void Player::resetScore()
{
    this->score = 0;
    this->hand.clear();
}

int Player::calculateScoreForHand(const QVector<QString>& hand) const {
    int tempScore = 0;
    int aceCount = 0;

    // Calculate the score based on the cards in the hand
    for (const QString &card : hand) {
        // Extract card file name without the path and extension (e.g., "ace", "4")
        QString cardFileName = QFileInfo(card).baseName();
        QString cardValue = cardFileName.split("_").first(); // Extract card value
        qDebug() << "Card value:" << cardValue;

        if (cardValue == "jack" || cardValue == "queen" || cardValue == "king") {
            tempScore += 10;
        } else if (cardValue == "ace") {
            tempScore += 11;  // Ace is initially worth 11
            aceCount++;
        } else {
            tempScore += cardValue.toInt();  // Convert number cards (e.g., "2", "3", ...)
        }
        qDebug() << "Current temp score:" << tempScore;
    }

    // Adjust score for aces if necessary (Ace can be worth 1 or 11)
    while (tempScore > 21 && aceCount > 0) {
        tempScore -= 10;
        aceCount--;
        qDebug() << "Adjusted temp score for ace:" << tempScore;
    }

    qDebug() << "Final calculated score:" << tempScore;
    return tempScore;
}

bool Player::isBusted() const {
    return calculateScore() > 21;
}

QVector<QString> Player::getHand() const {
    return hand;
}
