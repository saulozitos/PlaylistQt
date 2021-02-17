#include "spotify.h"

#include "settings.h"
#include <QNetworkRequest>

Spotify::Spotify(QObject *parent) : QObject(parent),
    networkManager(std::make_unique<QNetworkAccessManager>(this))
{}


///
/// \brief Spotify::request - monta a requisição para a API do Spotify
/// \param url - url de complemento
/// \return solicitação a ser enviada
///
QNetworkRequest Spotify::request(const QString &url)
{
    QUrl urlRequest("https://api.spotify.com/v1/" + url);

    QNetworkRequest request(urlRequest);
    const auto accessToken = Settings::getInstance().getAccessToken();
    request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(accessToken).toUtf8());

    return request;
}

///
/// \brief Spotify::search - função para bucar músicas ou artistas
/// consumindo a API do Spotify
/// \param value - nome a ser buscado
/// \return json com o resultado gerado pela API
///
QJsonObject Spotify::search(const QString &value)
{
    const QString url = QStringLiteral("search?q=%1&type=track").arg(value);
    auto *reply = networkManager->get(request(url));

    while(!reply->isFinished())
        QCoreApplication::processEvents();

    const auto json = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();
    return json;
}
