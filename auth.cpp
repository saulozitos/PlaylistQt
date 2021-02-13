#include "auth.h"
#include <QMessageBox>
#include <QStringList>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QDebug>

Auth::Auth(QWidget *parent) : QDialog(parent),
    networkManager(std::make_unique<QNetworkAccessManager>()),
    server(std::make_unique<QTcpServer>(this)),
    settings(Settings::getInstance())
{

    if(server != nullptr)
    {
        if(!server->listen(QHostAddress::LocalHost, 8888))
        {
            QMessageBox::warning(this, QStringLiteral("server error"),
                QStringLiteral("failed to start a temporary server on port 8888: %1").arg(server->errorString()));
            return;
        }

        QTcpServer::connect(server.get(), &QTcpServer::newConnection, [this]()
        {
            // Read
            auto *socket = server->nextPendingConnection();
            socket->waitForReadyRead();
            auto response = QString(socket->readAll());

            // Client might want to request favicon or something
            if (!response.contains(QLatin1String("?code=")))
            {
                socket->close();
                return;
            }

            // GET /?code=<code> HTTP...
            auto left = response.left(response.indexOf(QLatin1String(" HTTP")));
            auto code = left.right(left.length() - left.indexOf(QLatin1String("?code=")) - 6);
            settings.setCode(code);
            auto status = auth();

            QString msn;
            if(status.isEmpty())
                msn = QStringLiteral("success, you can now return to app");
            else
            {
                msn = QStringLiteral("failed to authenticate: %1").arg(status);
                emit connectionError(msn);
            }


            // Write
            socket->write(QStringLiteral("HTTP/1.1 200 OK\r\n\r\n%1").arg(msn).toUtf8());

            socket->flush();
            socket->waitForBytesWritten(3000);
            socket->close();

            // Close it all down if ok
            if (status.isEmpty())
            {
                server->close();
                accept();
            }

            emit connectionOk();
        });
    }

    const auto url = authUrl();

    if(!QDesktopServices::openUrl(QUrl(url)))
        QMessageBox::critical(this, QStringLiteral("openUrl error"),
            QStringLiteral("failed to open Url: %1").arg(url));
}

QString Auth::authUrl()
{
    QStringList scopes = {
        "playlist-read-collaborative",
        "playlist-read-private",
        "playlist-modify-private",
        "playlist-modify-public",
        "user-follow-read",
        "user-follow-modify",
        "user-library-modify",
        "user-library-read",
        "user-modify-playback-state",
        "user-read-currently-playing",
        "user-read-playback-state",
        "user-read-private",
        "user-read-recently-played",
        "streaming",
        "user-read-email",
        "user-top-read"
    };

    // Prepare url and open browser
    QUrl redirectUrl(settings.getRedirect());
    return QStringLiteral("https://accounts.spotify.com/authorize?client_id=%1&response_type=code&redirect_uri=%2&scope=%3")
            .arg(settings.getClientId(), QString(redirectUrl.toEncoded()), scopes.join(QStringLiteral("%20")));
}

QString Auth::auth()
{
    if (settings.getCode().isEmpty())
        return QStringLiteral("no code specified");

    QUrl redirectUrl(settings.getRedirect());

    // Prepare form to send
    const auto postData = QStringLiteral("grant_type=authorization_code&code=%1&redirect_uri=%2").arg(settings.getCode(), QString(redirectUrl.toEncoded())).toUtf8();

    // Prepare request
    QNetworkRequest request(QUrl(QStringLiteral("https://accounts.spotify.com/api/token")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", "Basic " + QStringLiteral("%1:%2").arg(settings.getClientId(), settings.getClientSecret()).toUtf8().toBase64());

    // Send request
    auto *reply = networkManager->post(request, postData);

    // Wait for response
    while (!reply->isFinished())
        QCoreApplication::processEvents();

    const auto jsonData = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();

    if (jsonData.contains(QStringLiteral("error_description")))
        return jsonData[QStringLiteral("error_description")].toString();

    // Save access/refresh token
    settings.setAccessToken(jsonData[QStringLiteral("access_token")].toString());

    return QString();
}
