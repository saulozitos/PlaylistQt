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
    QVector<MusicTrack> getDataTrackFromJson(const QJsonObject &jsonObject);
    QPair<QString, QVector<MusicTrack>>getPlaylistData(const QString &path, const QString &file);
};

