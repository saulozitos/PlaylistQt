#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>
#include <QPair>

class TrackData;

class SearchFormatter
{
public:
    SearchFormatter();
    QVector<TrackData> track(const QJsonObject &jsonObject);
    QPair<QString, QVector<TrackData>>getPlaylistData(const QString &path, const QString &file);
};

