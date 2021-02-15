#include "playlist.h"
#include "settings.h"
#include <algorithm>
#include <utility>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QUrl>

Playlist::Playlist(QString playlistName) : playlist(new QMediaPlaylist())
{
    const auto cache = Settings::getInstance().getCacheLocation();
    const auto id = Settings::getInstance().getClientId();
    path = QStringLiteral("%1/playlist/%2").arg(cache, id);
    playlistFile = QStringLiteral("%1/%2.json").arg(path, playlistName);
}

Playlist::~Playlist()
{
    delete playlist;
}

void Playlist::addtrack(const MusicTrack &musicTrack)
{
    playlistData.push_back(musicTrack);
    playlist->addMedia(QUrl(musicTrack.urlPreview));
    addToJsonArray(musicTrack);
}

void Playlist::removeTrack(const int index)
{
    playlistData.remove(index);
    list.removeAt(index);
    playlist->removeMedia(index);
    updateJsonFile();
}

void Playlist::removeAll()
{
    playlistData.clear();

    while (!list.isEmpty())
        list.removeLast();

    playlist->clear();

    updateJsonFile();
}

QVector<MusicTrack> Playlist::getPlaylistData() const
{
    return playlistData;
}

QMediaPlaylist *Playlist::getPlaylist() const
{
    return playlist;
}

void Playlist::addToJsonArray(const MusicTrack &musicTrack)
{
    QJsonObject newTrack
    {
        {QStringLiteral("artistName"), musicTrack.artistName},
        {QStringLiteral("trackName"), musicTrack.musicaName},
        {QStringLiteral("urlPreview"), musicTrack.urlPreview},
    };

    list.append(newTrack);
    updateJsonFile();
}

void Playlist::updateJsonFile()
{
    QJsonObject tracks;
    tracks[QStringLiteral("tracks")] = list;

    QJsonDocument doc(tracks);
    saveJsonFile(doc);
}

void Playlist::saveJsonFile(const QJsonDocument &doc)
{
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
