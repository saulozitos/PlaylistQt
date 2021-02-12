#include "uiplayer.h"
#include "ui_uiplayer.h"
#include "searchformatter.h"
#include "playlist.h"
#include "structs.h"
#include <QDir>
#include <QStandardPaths>
#include <QInputDialog>
#include <QRandomGenerator>
#include <algorithm>
#include "settings.h"

#include <QDebug>

namespace ButtonName
{
constexpr auto play = "Play";
constexpr auto pause = "Pause";
}

UiPlayer::UiPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiPlayer),
    isPlaying(false),
    dataLocation(QStandardPaths::standardLocations(QStandardPaths::CacheLocation).constFirst()),
    spotify(std::make_unique<Spotify>(this))
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
        removePlaylist();
    });
}

UiPlayer::~UiPlayer()
{
    if(!playlists.isEmpty())
        for(const auto &it : qAsConst(playlists))
            delete it.second;

    delete ui;
}

void UiPlayer::on_btnBack_clicked()
{

}

void UiPlayer::on_btnPlay_clicked()
{
    if(!isPlaying)
    {
        isPlaying = true;
        ui->btnPlay->setText(ButtonName::pause);
    }
    else
    {
        isPlaying = false;
        ui->btnPlay->setText(ButtonName::play);
    }
}

void UiPlayer::on_btnStop_clicked()
{
    if(isPlaying)
    {
        isPlaying = false;
        ui->btnPlay->setText(ButtonName::play);
    }
}

void UiPlayer::on_btnForward_clicked()
{

}

void UiPlayer::on_btnSearch_clicked()
{
    if(ui->listWidget->count() > -1)
        ui->listWidget->clear();

    if(!dataSearch.isEmpty())
        dataSearch.clear();

    const auto trackName = ui->lnSearch->text();
    if(!trackName.isEmpty())
    {
        SearchFormatter sf;
        dataSearch = sf.track(spotify->search(trackName));

        for(const auto &it : qAsConst(dataSearch))
        {
            QString dado = QStringLiteral("%1 - %2").arg(it.track.name, it.artist.name);
            ui->listWidget->addItem(dado);
        }
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
    const auto playlistName = ui->comboBox->currentText();
    if(!playlistName.isEmpty())
    {
        playlists.erase(
                    std::remove_if(playlists.begin(), playlists.end(), [playlistName](const QPair<QString, Playlist*>&p){
            return p.first == playlistName;
        }), playlists.end());

        auto *currentPlaylist = getCurrentPlaylist();
        currentPlaylist->removeAll();

        ui->comboBox->removeItem(ui->comboBox->currentIndex());
    }

    if(ui->comboBox->currentText().isEmpty() && ui->listViewPlaylist->count() > 0)
        ui->listViewPlaylist->clear();
}

void UiPlayer::on_btnAddTrack_clicked()
{
    if(!ui->listWidget->currentIndex().data().toString().isEmpty())
    {
        const auto trackData = dataSearch.at(ui->listWidget->currentIndex().row());
        addTrack(trackData);
    }
}

void UiPlayer::on_btnRemoveTrack_clicked()
{
    const auto playlistName = ui->comboBox->currentText();
    if(!playlistName.isEmpty())
    {
        auto *currentPlaylist = getCurrentPlaylist();
        currentPlaylist->removeTrack(ui->listViewPlaylist->currentIndex().row());

        if(ui->listViewPlaylist->count() > 0)
            ui->listViewPlaylist->clear();

        for(auto &it : currentPlaylist->getPlaylist())
        {
            const auto name = QStringLiteral("%1 - %2").arg(it.track.name, it.artist.name);
            ui->listViewPlaylist->addItem(name);
        }
    }
}

Playlist *UiPlayer::getCurrentPlaylist()
{
    const auto playlistName = ui->comboBox->currentText();
    return std::find_if(playlists.begin(), playlists.end(), [playlistName] (const QPair<QString, Playlist*>&pair) {
        return pair.first == playlistName;
    })->second;
}

void UiPlayer::removePlaylist()
{
    if(!ui->comboBox->currentText().isEmpty())
    {
        auto *currentPlaylist = getCurrentPlaylist();

        if(ui->listViewPlaylist->count() > 0)
            ui->listViewPlaylist->clear();

        for(auto &it : currentPlaylist->getPlaylist())
        {
            const auto name = QStringLiteral("%1 - %2").arg(it.track.name, it.artist.name);
            ui->listViewPlaylist->addItem(name);
        }
    }
}

void UiPlayer::loadPlaylist(const QString &path, const QString &file)
{
    SearchFormatter s;
    const auto playlistData = s.getPlaylistData(path, file);
    const auto playlistName = playlistData.first;
    addPlayList(playlistName);

    ui->listViewPlaylist->clear();
    for(const auto &trackData : playlistData.second)
    {
        addTrack(trackData);
    }
}

void UiPlayer::addPlayList(const QString &playlistName)
{
    if(!playlistName.isEmpty())
    {
        playlists.push_back({playlistName, new Playlist(playlistName)});
        ui->comboBox->addItem(playlistName);
        ui->comboBox->setCurrentText(playlistName);
    }
}

void UiPlayer::addTrack(const TrackData &trackData)
{
    auto *currentPlaylist = getCurrentPlaylist();
    currentPlaylist->addtrack(trackData);

    auto last = currentPlaylist->getPlaylist().constLast();
    const auto name = QStringLiteral("%1 - %2").arg(last.track.name, last.artist.name);
    ui->listViewPlaylist->addItem(name);
}

void UiPlayer::checkDataPath()
{
    Settings::getInstance().setCacheLocation(dataLocation);
    QDir cacheDir(dataLocation);
    qDebug() << "Cache" << cacheDir;

    const auto userDir = QStringLiteral("playlist/%1").arg(Settings::getInstance().getClientId());
    cacheDir.mkpath(userDir);

    const auto playlisttPath = QStringLiteral("%1/%2").arg(dataLocation, userDir);
    QDir directory(playlisttPath);
    QStringList jsonFiles = directory.entryList(QStringList() << QStringLiteral("*.json") << QStringLiteral("*.JSON"), QDir::Files);
    if(!jsonFiles.empty())
    {
        for(const auto &it : jsonFiles)
            loadPlaylist(playlisttPath, it);
    }
}

QString UiPlayer::getPreviewUrl()
{
    auto * currentPlaylist = getCurrentPlaylist();

    //não selecionou nenhum música específica da playlista para ser tocada
    if(ui->listWidget->currentIndex().data().toString().isEmpty())
    {
        const auto playlistSize = ui->listViewPlaylist->count();
        const uint rIndex = QRandomGenerator::global()->bounded(playlistSize + 1);
        return currentPlaylist->getPreviewUrl(rIndex);
    }

    return currentPlaylist->getPreviewUrl(ui->listWidget->currentIndex().row());
}
