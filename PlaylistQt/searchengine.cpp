#include "searchengine.h"
#include "musictrack.h"
#include <QJsonDocument>
#include <QFile>

///
/// \brief SearchEngine::getDataTrackFromSpotifyJson
/// \param jsonObject - json recebido pela API do Spotify em sua busca
/// \return vector contendo os dados das músicas
///
QVector<MusicTrack> SearchEngine::getDataTrackFromSpotifyJson(const QJsonObject &jsonObject)
{
    const auto variantMap = jsonObject.toVariantMap();
    const auto items = variantMap[QStringLiteral("tracks")].toMap()[QStringLiteral("items")].toList();

    QVector<MusicTrack> out;

    for(const auto &it : items)
    {
        MusicTrack musicTrack;
        const auto map = it.toMap();

        const auto artists = map[QStringLiteral("album")].toMap()[QStringLiteral("artists")].toList();
        for(const auto &at : artists)
        {
            const auto map = at.toMap();
            musicTrack.artistName = map[QStringLiteral("name")].toString();
        }

        musicTrack.musicaName = map[QStringLiteral("name")].toString();
        musicTrack.urlPreview = map[QStringLiteral("preview_url")].toString();

        out.push_back(musicTrack);
    }

    return out;
}

///
/// \brief SearchEngine::loadPlaylistFromJsonFile
/// \param path - diretório onde se encontram os arquivos json de playlists criadas
/// \param file - nome da playlist. ex.. playlist.json
/// \return pair com o nome da playlist e todas as suas músicas
///
QPair<QString, QVector<MusicTrack> > SearchEngine::loadPlaylistFromJsonFile(const QString &path, const QString &file)
{
    QPair<QString, QVector<MusicTrack> > out;
    QString playlistName = file;
    playlistName = playlistName.remove(playlistName.size()-5, 5);
    out.first = playlistName;

    const auto playlistFilePath = QStringLiteral("%1/%2").arg(path, file);
    QFile jsonFile(playlistFilePath);
    jsonFile.open(QIODevice::ReadOnly|QIODevice::Text);
    const auto data = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = jsonDoc.object();
    const auto variantMap = jsonObject.toVariantMap();
    const auto list = variantMap[QStringLiteral("tracks")].toList();
    for(const auto &it : list)
    {
        MusicTrack musicTrack;
        const auto map = it.toMap();
        musicTrack.artistName = map[QStringLiteral("artistName")].toString();
        musicTrack.musicaName = map[QStringLiteral("trackName")].toString();
        musicTrack.urlPreview = map[QStringLiteral("urlPreview")].toString();
        out.second.push_back(musicTrack);
    }

    return out;
}
