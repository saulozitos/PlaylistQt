#pragma once

#include <QWidget>
#include <QString>
#include <QPair>
#include <QVector>
#include "auth.h"
#include "spotify.h"
#include <memory>

namespace Ui {
class UiPlayer;
}

class Playlist;
class TrackData;

class UiPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit UiPlayer(QWidget *parent = nullptr);
    ~UiPlayer();

private slots:
    void on_btnBack_clicked();
    void on_btnPlay_clicked();
    void on_btnStop_clicked();
    void on_btnForward_clicked();
    void on_btnSearch_clicked();
    void on_btnNewPlaylist_clicked();
    void on_btnRemovePlaylist_clicked();
    void on_btnAddTrack_clicked();
    void on_btnRemoveTrack_clicked();

private:
    Ui::UiPlayer *ui;

    bool isPlaying;
    QString dataLocation;

    std::unique_ptr<Spotify>spotify;
    QVector< QPair<QString, Playlist*>>playlists;
    QVector<TrackData>dataSearch;

    Playlist* getCurrentPlaylist();
    void removePlaylist();
    void loadPlaylist(const QString &path, const QString &file);
    void addPlayList(const QString &playlistName);
    void addTrack(const TrackData &trackData);
    void checkDataPath();
    QString getPreviewUrl();
};

