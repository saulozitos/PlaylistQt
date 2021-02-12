#pragma once

#include "structs.h"
#include <QString>
#include <QVector>
#include <QJsonArray>

class Playlist
{
public:
    explicit Playlist(QString playlistName);
    void addtrack(const TrackData &trackData);
    void removeTrack(const int index);
    void removeAll();

    QString getName() const;

    QVector<TrackData> getPlaylist() const;

    QString getPreviewUrl(const int index);

private:
    QString name;
    QVector<TrackData>playlist;
    QJsonArray list;
    QString path;
    void addToJsonArray(const TrackData &trackData);
    void mountJsonFile();
    void saveJsonFile(const QJsonDocument &doc);
};

