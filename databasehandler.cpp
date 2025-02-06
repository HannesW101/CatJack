#include "databasehandler.h"
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

DatabaseHandler::DatabaseHandler(QObject *parent)
    : QObject{parent}
{
    m_networkManager = new QNetworkAccessManager(this);
    m_networkReply = m_networkManager->get(QNetworkRequest(QUrl("https://cat-jack-21-default-rtdb.europe-west1.firebasedatabase.app/Users.json")));
    connect(m_networkReply, &QNetworkReply::readyRead, this, &DatabaseHandler::networkReplyReadyRead); // connect to show all Users in the qDebug terminal
}

void DatabaseHandler::addUser(QVariantMap newUser, QString customKey)
{
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(newUser);
    QUrl url(QString("https://cat-jack-21-default-rtdb.europe-west1.firebasedatabase.app/Users/%1.json").arg(customKey));
    QNetworkRequest newUserRequest(url);
    newUserRequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QNetworkReply *reply = m_networkManager->put(newUserRequest, jsonDoc.toJson());

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "New user created successfully:" << customKey;
            qDebug() << "Response:" << reply->readAll();
        } else {
            qWarning() << "Error creating new user:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

DatabaseHandler::~DatabaseHandler()
{
    m_networkManager->deleteLater();
}

void DatabaseHandler::networkReplyReadyRead()
{
    qDebug() << m_networkReply->readAll();
}

bool DatabaseHandler::userExists(const QString &username)
{
    QEventLoop eventLoop;
    bool exists = false;

    QNetworkRequest request(QUrl(QString("https://cat-jack-21-default-rtdb.europe-west1.firebasedatabase.app/Users/%1.json").arg(username)));
    QNetworkReply *reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObj = jsonDoc.object();
            exists = !jsonObj.isEmpty();
        } else {
            qWarning() << "Network error:" << reply->errorString();
        }
        eventLoop.quit();
    });

    eventLoop.exec(); // Wait for the network request to complete
    reply->deleteLater();
    return exists;
}

QVariantMap DatabaseHandler::getUserDetails(const QString &username)
{
    QEventLoop eventLoop;
    QVariantMap userDetails;

    QNetworkRequest request(QUrl(QString("https://cat-jack-21-default-rtdb.europe-west1.firebasedatabase.app/Users/%1.json").arg(username)));
    QNetworkReply *reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObj = jsonDoc.object();
            if (!jsonObj.isEmpty()) {
                userDetails["Username"] = jsonObj["Username"].toString();
                userDetails["Password"] = jsonObj["Password"].toString();
                userDetails["Credits"] = jsonObj["Credits"].toInt();
                userDetails["ProfilePic"] = jsonObj["ProfilePic"].toInt();
            } else {
                qDebug() << "User not found.";
            }
        } else {
            qWarning() << "Network error:" << reply->errorString();
        }
        eventLoop.quit();
    });

    eventLoop.exec(); // Wait for the network request to complete
    reply->deleteLater();
    return userDetails;
}

void DatabaseHandler::updateUserCredits(const QString &username, int newCredits)
{
    QEventLoop eventLoop;
    bool success = false;

    // Get the current user details
    QVariantMap userDetails = getUserDetails(username);
    if (userDetails.isEmpty()) {
        qWarning() << "User not found.";
        return;
    }

    // Update the "Credits" field
    userDetails["Credits"] = newCredits;

    // Convert the updated data into a QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(userDetails);
    QUrl url(QString("https://cat-jack-21-default-rtdb.europe-west1.firebasedatabase.app/Users/%1.json").arg(username));
    QNetworkRequest updateRequest(url);
    updateRequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    // Send the PUT request to update the user
    QNetworkReply *reply = m_networkManager->put(updateRequest, jsonDoc.toJson());

    connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Credits updated successfully for user:" << username;
            success = true;
        } else {
            qWarning() << "Error updating credits:" << reply->errorString();
        }
        eventLoop.quit();
    });

    eventLoop.exec(); // Wait for the network request to complete
    reply->deleteLater();

    if (!success) {
        qWarning() << "Failed to update credits for user:" << username;
    }
}

