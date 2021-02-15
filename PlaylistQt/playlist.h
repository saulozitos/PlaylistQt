#pragma once

#include "musictrack.h"
#include <QString>
#include <QVector>
#include <QJsonArray>
#include <QMediaPlaylist>

class Playlist
{
public:
    explicit Playlist(QString playlistName);
    ~Playlist();
    void addtrack(const MusicTrack &musicTrack);
    void removeTrack(const int index);
    void removeAll();

    QVector<MusicTrack> getPlaylistData() const;
    QMediaPlaylist *getPlaylist() const;

private:
    QString playlistFile;
    QVector<MusicTrack>playlistData;
    QMediaPlaylist *playlist;

    QJsonArray list;
    QString path;
    void addToJsonArray(const MusicTrack &musicTrack);
    void updateJsonFile();
    void saveJsonFile(const QJsonDocument &doc);
};

