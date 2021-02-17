#include "uiplayer.h"
#include "ui_uiplayer.h"
#include "searchengine.h"
#include "playlist.h"
#include "musictrack.h"
#include <QDir>
#include <QStandardPaths>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <algorithm>
#include "settings.h"

namespace ButtonName
{
constexpr auto play = "Play";
constexpr auto pause = "Pause";
}

UiPlayer::UiPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiPlayer),
    isPlaying(false),
    isPaused(false),
    dataLocation(QStandardPaths::standardLocations(QStandardPaths::CacheLocation).constFirst()),
    spotify(std::make_unique<Spotify>(this)),
    playlist(nullptr),
    player(new QMediaPlayer(this))
{
    ui->setupUi(this);

    checkDataPath();

    ui->btnPlay->setText(ButtonName::play);

    ui->lnSearch->setText(QStringLiteral("Children of the sea"));
    ui->btnAddTrack->setEnabled(ui->comboBox->currentIndex() > -1);
    ui->btnRemoveTrack->setEnabled(ui->comboBox->currentIndex() > -1);
    ui->btnRemovePlaylist->setEnabled(ui->comboBox->currentIndex() > -1);
    ui->btnSearch->setEnabled(!ui->lnSearch->text().isEmpty());

    connect(ui->lnSearch, &QLineEdit::editingFinished, [this](){
        ui->btnSearch->setEnabled(!ui->lnSearch->text().isEmpty());
    });

    connect(ui->comboBox, qOverload<int>(&QComboBox::currentIndexChanged), [this] (int index){
        ui->btnAddTrack->setEnabled(index > -1);
        ui->btnRemoveTrack->setEnabled(index > -1);
        ui->btnRemovePlaylist->setEnabled(index > -1);
        player->stop();
        isPlaying = false;
        isPaused = false;
        ui->btnPlay->setText(ButtonName::play);
        ui->lbMusic->clear();
        removePlaylist();
    });
}

UiPlayer::~UiPlayer()
{
    if(!dataPlaylists.isEmpty())
        for(const auto &it : qAsConst(dataPlaylists))
            delete it.second;

    delete playlist;
    delete player;
    delete ui;
}

void UiPlayer::on_btnBack_clicked()
{
    if(ui->playlistTracks->count() < 1) return;

    if(playlist != nullptr)
        playlist->previous();

    updateButtonPlay();
}

void UiPlayer::on_btnPlay_clicked()
{
    if(ui->playlistTracks->count() < 1) return;

    if(!isPlaying)
    {
        if(!isPaused) getPlaylist();

        player->play();
        isPaused = false;
        isPlaying = true;
        ui->btnPlay->setText(ButtonName::pause);
    }
    else
    {
        isPaused = true;
        isPlaying = false;
        player->pause();
        ui->btnPlay->setText(ButtonName::play);
    }

}

void UiPlayer::on_btnStop_clicked()
{
    if(ui->playlistTracks->count() < 1) return;

    if(isPlaying)
    {
        isPlaying = false;
        player->stop();
        ui->btnPlay->setText(ButtonName::play);
        ui->lbMusic->clear();
    }
}

void UiPlayer::on_btnForward_clicked()
{
    if(ui->playlistTracks->count() < 1) return;

    if(playlist != nullptr)
        playlist->next();

    updateButtonPlay();
}

void UiPlayer::on_btnSearch_clicked()
{
    if(ui->listWidget->count() > -1)
        ui->listWidget->clear();

    if(!searchResult.isEmpty())
        searchResult.clear();

    const auto textToSearch = ui->lnSearch->text();
    if(!textToSearch.isEmpty())
    {
        SearchEngine se;
        searchResult = se.getDataTrackFromSpotifyJson(spotify->search(textToSearch));

        for(const auto &musicTrack : qAsConst(searchResult))
            ui->listWidget->addItem(QStringLiteral("%1 - %2").arg(musicTrack.musicaName, musicTrack.artistName));
    }
}

void UiPlayer::on_btnNewPlaylist_clicked()
{
    const QString playlistName = QInputDialog::getText(this, QStringLiteral("New Playlist"),
                                                       QStringLiteral("Playlist name:"), QLineEdit::Normal);
    addPlayList(playlistName);
}

void UiPlayer::on_btnRemovePlaylist_clicked()
{
    const auto selectedPlaylistName = ui->comboBox->currentText();
    if(!selectedPlaylistName.isEmpty())
    {
        auto *currentPlaylist = getAddrFromCurrentPlaylist();
        if(currentPlaylist != nullptr)
        {
            currentPlaylist->removeAll();

            for(auto it = 0; it < dataPlaylists.size(); ++it)
                if(dataPlaylists.at(it).first == selectedPlaylistName)
                    dataPlaylists.remove(it);

            ui->playlistTracks->clear();
            ui->comboBox->removeItem(ui->comboBox->currentIndex());
        }
    }
}

void UiPlayer::on_btnAddTrack_clicked()
{
    if(!ui->listWidget->currentIndex().data().toString().isEmpty())
    {
        const auto musicTrack = searchResult.at(ui->listWidget->currentIndex().row());
        if(musicTrack.urlPreview.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText(QStringLiteral("The music %1 does not have a sample in Spotify API to play and will not be added to your playlist").arg(
                               musicTrack.musicaName));
            msgBox.exec();
        }
        else
            addTrack(musicTrack);
    }
}

void UiPlayer::on_btnRemoveTrack_clicked()
{
    const auto selectedPlaylistName = ui->comboBox->currentText();
    if(!selectedPlaylistName.isEmpty())
    {
        auto *currentPlaylist = getAddrFromCurrentPlaylist();
        if(currentPlaylist != nullptr)
        {
            currentPlaylist->removeTrack(ui->playlistTracks->currentIndex().row());

            if(ui->playlistTracks->count() > 0)
                ui->playlistTracks->clear();

            for(auto &musicTrack : currentPlaylist->getPlaylistData())
            {
                const auto dataTrack = QStringLiteral("%1 - %2").arg(musicTrack.musicaName, musicTrack.artistName);
                ui->playlistTracks->addItem(dataTrack);
            }
        }
    }
}

Playlist *UiPlayer::getAddrFromCurrentPlaylist()
{
    const auto selectedPlaylistName = ui->comboBox->currentText();
    return std::find_if(dataPlaylists.begin(), dataPlaylists.end(), [selectedPlaylistName] (const QPair<QString, Playlist*>&pair) {
        return pair.first == selectedPlaylistName;
    })->second;
}

void UiPlayer::removePlaylist()
{
    if(!ui->comboBox->currentText().isEmpty())
    {
        auto *currentPlaylist = getAddrFromCurrentPlaylist();
        if(currentPlaylist != nullptr)
        {
            if(ui->playlistTracks->count() > 0)
                ui->playlistTracks->clear();

            for(auto &musicTrack : currentPlaylist->getPlaylistData())
            {
                const auto name = QStringLiteral("%1 - %2").arg(musicTrack.musicaName, musicTrack.artistName);
                ui->playlistTracks->addItem(name);
            }
        }
    }
}

void UiPlayer::loadPlaylist(const QString &path, const QString &file)
{
    SearchEngine se;
    const auto playlistData = se.loadPlaylistFromJsonFile(path, file);
    const auto playlistName = playlistData.first;
    addPlayList(playlistName);

    ui->playlistTracks->clear();
    for(const auto &trackData : playlistData.second)
        addTrack(trackData);
}

void UiPlayer::addPlayList(const QString &playlistName)
{
    if(!playlistName.isEmpty())
    {
        dataPlaylists.push_back({playlistName, new Playlist(playlistName)});
        ui->comboBox->addItem(playlistName);
        ui->comboBox->setCurrentText(playlistName);
    }
}

void UiPlayer::addTrack(const MusicTrack &musicTrack)
{
    auto *currentPlaylist = getAddrFromCurrentPlaylist();
    if(currentPlaylist != nullptr)
    {
        currentPlaylist->addtrack(musicTrack);

        auto lastTrack = currentPlaylist->getPlaylistData().constLast();
        const auto dataTrack = QStringLiteral("%1 - %2").arg(lastTrack.musicaName, lastTrack.artistName);
        ui->playlistTracks->addItem(dataTrack);
    }
}

void UiPlayer::checkDataPath()
{
    Settings::getInstance().setCacheLocation(dataLocation);
    QDir cacheDir(dataLocation);

    const auto userDir = QStringLiteral("playlist/%1").arg(Settings::getInstance().getClientId());
    cacheDir.mkpath(userDir);

    const auto playlisttPath = QStringLiteral("%1/%2").arg(dataLocation, userDir);
    QDir directory(playlisttPath);
    QStringList jsonFiles = directory.entryList(QStringList() << QStringLiteral("*.json") << QStringLiteral("*.JSON"), QDir::Files);
    if(!jsonFiles.empty())
        for(const auto &it : jsonFiles)
            loadPlaylist(playlisttPath, it);
}

void UiPlayer::getPlaylist()
{
    auto *currentPlaylist = getAddrFromCurrentPlaylist();
    if(currentPlaylist != nullptr)
    {
        uint index = 0;
        if(ui->playlistTracks->currentIndex().data().toString().isEmpty())
        {
            const auto playlistSize = ui->playlistTracks->count();
            index = QRandomGenerator::global()->bounded(playlistSize);
        }
        else
            index = ui->playlistTracks->currentIndex().row();

        playlist = currentPlaylist->getPlaylist();

        connect(playlist, &QMediaPlaylist::currentIndexChanged, [this] (int index)
        {
            if(index < 0)
                playlist->setCurrentIndex(0);
            else
            {
                auto *currentPlaylist = getAddrFromCurrentPlaylist();
                const auto track = currentPlaylist->getPlaylistData().at(index);
                const auto atualPlayback = QStringLiteral("Playing now: %1 - %2").arg(track.musicaName, track.artistName);
                ui->lbMusic->setText(atualPlayback);
            }

        });

        playlist->setCurrentIndex(index);
        player->setPlaylist(playlist);
    }
}

void UiPlayer::updateButtonPlay()
{
    if(player->mediaStatus() == QMediaPlayer::NoMedia)
        ui->btnPlay->setText(ButtonName::play);
}
