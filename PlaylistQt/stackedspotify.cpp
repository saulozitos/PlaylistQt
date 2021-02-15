#include "stackedspotify.h"
#include "ui_stackedspotify.h"
#include "uiplayer.h"
#include "uilogin.h"

StackedSpotify::StackedSpotify(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::Stack),
    uiLogin(std::make_unique<UiLogin>(this))
{
    ui->setupUi(this);
    ui->stack->addWidget(uiLogin.get());


    connect(uiLogin.get(), &UiLogin::sucess, [this](){
        uiPlayer = std::make_unique<UiPlayer>(this);
        ui->stack->addWidget(uiPlayer.get());
        ui->stack->setCurrentWidget(uiPlayer.get());
    });

    ui->stack->setCurrentWidget(uiLogin.get());
}

StackedSpotify::~StackedSpotify()
{
    delete ui;
}
