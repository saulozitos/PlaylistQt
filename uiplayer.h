#pragma once

#include <QWidget>
#include <QString>

namespace Ui {
class UiPlayer;
}

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
    QString cacheLocation;

    void removePlaylist();
};

