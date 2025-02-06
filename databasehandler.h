#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVariantMap>

class DatabaseHandler : public QObject {
    Q_OBJECT

public:
    explicit DatabaseHandler(QObject *parent = nullptr);
    ~DatabaseHandler();
    void addUser(QVariantMap newUser, QString customKey);
    bool userExists(const QString &username);
    QVariantMap getUserDetails(const QString &username);
    void updateUserCredits(const QString &username, int newCredits);

private slots:
    void networkReplyReadyRead();

private:
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_networkReply;
};

#endif // DATABASEHANDLER_H
