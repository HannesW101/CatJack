#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QVector>

class Player {
public:
    Player();  // Constructor
    void addCard(const QString &cardPath);  // Add a card to the player's hand
    int calculateScore() const;  // Calculate the player's hand score
    bool isBusted() const;  // Check if the player has busted
    QVector<QString> getHand() const;  // Get the player's hand (cards)
    int calculateScoreForHand(const QVector<QString>& hand) const;
    int calculateVisibleScore() const;
    QString aiBestMove(int dealerVisibleScore) const;
    void resetScore();

private:
    QVector<QString> hand;  // Player's hand (list of card paths)
    int score;  // The player's score
    void updateScore();  // Helper function to update the score based on the hand
    QVector<int> cardsDealt;
    bool isSoftHand() const;
};

#endif // PLAYER_H
