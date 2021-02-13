#include "searchformatter.h"
#include "structs.h"
#include <QJsonDocument>
#include <QFile>

SearchFormatter::SearchFormatter()
{}

QVector<TrackData> SearchFormatter::track(const QJsonObject &jsonObject)
{
    auto variantMap = jsonObject.toVariantMap();
    const auto items = variantMap[QStringLiteral("tracks")].toMap()[QStringLiteral("items")].toList();

    QVector<TrackData> out;

    for(const auto &it : items)
    {
        TrackData td;
        const auto map = it.toMap();

        const auto artists = map[QStringLiteral("album")].toMap()[QStringLiteral("artists")].toList();
        for(const auto &at : artists)
        {
            const auto map = at.toMap();
            td.artist.name = map[QStringLiteral("name")].toString();
            td.artist.id = map[QStringLiteral("id")].toString();
        }

        td.track.name = map[QStringLiteral("name")].toString();
        td.track.id = map[QStringLiteral("id")].toString();
        td.track.duration = map[QStringLiteral("duration_ms")].toUInt();
        td.previewUrl = map[QStringLiteral("preview_url")].toString();

        out.push_back(td);
    }

    return out;
}

QPair<QString, QVector<TrackData> > SearchFormatter::getPlaylistData(const QString &path, const QString &file)
{
    QPair<QString, QVector<TrackData> > out;
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
        TrackData td;
        const auto map = it.toMap();
        td.artist.id = map[QStringLiteral("artistId")].toString();
        td.artist.name = map[QStringLiteral("artistName")].toString();
        td.track.duration = map[QStringLiteral("duration")].toUInt();
        td.track.id = map[QStringLiteral("trackId")].toString();
        td.track.name = map[QStringLiteral("trackName")].toString();
        out.second.push_back(td);
    }

    return out;
}
