#include "spotify.h"

#include "settings.h"
#include <QNetworkRequest>

Spotify::Spotify(QObject *parent) : QObject(parent),
    networkManager(std::make_unique<QNetworkAccessManager>(this))
{}

QNetworkRequest Spotify::request(const QString &url)
{
    QUrl urlRequest("https://api.spotify.com/v1/" + url);

    qDebug() << "URL" << urlRequest;

    QNetworkRequest request(urlRequest);
    const auto accessToken = Settings::getInstance().getAccessToken();
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(accessToken).toUtf8());

    return request;
}

QJsonObject Spotify::search(const QString &value)
{
    const QString url = QStringLiteral("search?q=%1&type=track&limit=10").arg(value);
    auto *reply = networkManager->get(request(url));

    while(!reply->isFinished())
        QCoreApplication::processEvents();

    const auto json = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();
    return json;
}

void Spotify::play(const QString &value)
{
    //return put("me/player/play");
}

void Spotify::pause()
{
    //return put("me/player/pause");
}

void Spotify::stop()
{

}

void Spotify::next()
{
    //return post("me/player/next");
}

void Spotify::previous()
{
    //return post("me/player/previous");
}
