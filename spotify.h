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
    QJsonObject search(const QString &value);
    void play(const QString &value);
    void pause();
    void stop();
    void next();
    void previous();

private:
    std::unique_ptr<QNetworkAccessManager>networkManager;
    QNetworkRequest request(const QString &url);

signals:

};

