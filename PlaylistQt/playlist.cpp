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

///
/// \brief Playlist::addtrack - função para adicionar uma música a playlist
/// \param musicTrack - parametro contendo as informações da música
///
void Playlist::addtrack(const MusicTrack &musicTrack)
{
    playlistData.push_back(musicTrack);
    playlist->addMedia(QUrl(musicTrack.urlPreview));
    addToJsonArray(musicTrack);
}

///
/// \brief Playlist::removeTrack - função para remover 1 música da playlist
/// \param index - posição da música na playlist
///
void Playlist::removeTrack(const int index)
{
    playlistData.remove(index);
    list.removeAt(index);
    playlist->removeMedia(index);
    updateJsonFile();
}

///
/// \brief Playlist::removeAll - remove todas as músicas da playlist e ao final exclui a playlist
///
void Playlist::removeAll()
{
    playlistData.clear();

    while (!list.isEmpty())
        list.removeLast();

    playlist->clear();

    updateJsonFile();
}

///
/// \brief Playlist::getPlaylistData
/// \return vector contendo todas as dados de músicas da playlist
///
QVector<MusicTrack> Playlist::getPlaylistData() const
{
    return playlistData;
}

///
/// \brief Playlist::getPlaylist
/// \return lista de conteúdo de mídia a ser reproduzido.
///
QMediaPlaylist *Playlist::getPlaylist() const
{
    return playlist;
}


///
/// \brief Playlist::addToJsonArray - adiciona os dados de uma música em uma lista
/// para serem inseridos em um arquivo json
/// \param musicTrack - dados de uma música a serem inseridos no json
///
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

///
/// \brief Playlist::updateJsonFile - atualiza o arquivo json a partir de uma lista
///
void Playlist::updateJsonFile()
{
    QJsonObject tracks;
    tracks[QStringLiteral("tracks")] = list;

    QJsonDocument doc(tracks);
    saveJsonFile(doc);
}

///
/// \brief Playlist::saveJsonFile - salva o arquivo json
/// \param doc - lista de arquivos músicais
///
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
