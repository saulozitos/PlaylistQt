#include "playlist.h"
#include "settings.h"
#include <algorithm>
#include <utility>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>

Playlist::Playlist(QString playlistName) : name(std::move(playlistName))
{
    const auto cache = Settings::getInstance().getCacheLocation();
    const auto id = Settings::getInstance().getClientId();
    path = QStringLiteral("%1/playlist/%2").arg(cache, id);
}

void Playlist::addtrack(const TrackData &trackData)
{
    playlist.push_back(trackData);
    addToJsonArray(trackData);
}

void Playlist::removeTrack(const int index)
{
    playlist.remove(index);
    list.removeAt(index);
    mountJsonFile();
}

void Playlist::removeAll()
{
    while (!playlist.isEmpty()) {
        playlist.removeLast();
    }

    while (!list.isEmpty()) {
        list.removeLast();
    }

    mountJsonFile();
}

QString Playlist::getName() const
{
    return name;
}

QVector<TrackData> Playlist::getPlaylist() const
{
    return playlist;
}

QString Playlist::getPreviewUrl(const int index)
{
    return playlist.at(index).previewUrl;
}

void Playlist::addToJsonArray(const TrackData &trackData)
{
    QJsonObject newTrack
    {
        {QStringLiteral("artistId"), trackData.artist.id},
        {QStringLiteral("artistName"), trackData.artist.name},
        {QStringLiteral("duration"), QString::number(trackData.track.duration)},
        {QStringLiteral("trackId"), trackData.track.id},
        {QStringLiteral("trackName"), trackData.track.name},
    };

    list.append(newTrack);
    mountJsonFile();
}

void Playlist::mountJsonFile()
{
    QJsonObject tracks;
    tracks[QStringLiteral("tracks")] = list;

    QJsonDocument doc(tracks);
    saveJsonFile(doc);
}

void Playlist::saveJsonFile(const QJsonDocument &doc)
{
    QString playlistFile = QStringLiteral("%1/%2.json").arg(path, name);

    QFile file(playlistFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    if(list.isEmpty())
    {
        file.remove();
        return;
    }

    QTextStream out(&file);
    out << doc.toJson(QJsonDocument::Indented);

    file.close();
}
