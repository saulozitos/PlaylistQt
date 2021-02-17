#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>
#include <QPair>

class MusicTrack;

class SearchEngine
{
public:
    SearchEngine() = default;
    ~SearchEngine() = default;
    QVector<MusicTrack> getDataTrackFromSpotifyJson(const QJsonObject &jsonObject);
    QPair<QString, QVector<MusicTrack>>loadPlaylistFromJsonFile(const QString &path, const QString &file);
};

