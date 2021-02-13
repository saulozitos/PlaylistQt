#pragma once

#include <QObject>
#include <QString>
#include <QtNetwork>
#include <QJsonObject>
#include <memory>

class Spotify : public QObject
{
    Q_OBJECT
public:
    explicit Spotify(QObject *parent = nullptr);
    ~Spotify() = default;
    QJsonObject search(const QString &value);

private:
    std::unique_ptr<QNetworkAccessManager>networkManager;
    QNetworkRequest request(const QString &url);
};

