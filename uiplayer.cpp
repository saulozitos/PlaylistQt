#include "uiplayer.h"
#include "ui_uiplayer.h"

namespace ButtonName
{
constexpr auto play = "Play";
constexpr auto pause = "Pause";
}

UiPlayer::UiPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiPlayer),
    isPlaying(false)
{
    ui->setupUi(this);

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

}

void UiPlayer::on_btnNewPlaylist_clicked()
{

}

void UiPlayer::on_btnRemovePlaylist_clicked()
{

}

void UiPlayer::on_btnAddTrack_clicked()
{

}

void UiPlayer::on_btnRemoveTrack_clicked()
{

}

void UiPlayer::removePlaylist()
{

}
