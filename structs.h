#pragma once

#include <QString>

struct Track
{
    QString id;
    QString name;
    uint duration;
};

struct Artist
{
    QString id;
    QString name;
};

struct TrackData
{
    Track track;
    Artist artist;
    QString previewUrl;
};
