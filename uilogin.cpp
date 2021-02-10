#include "uilogin.h"
#include "ui_uilogin.h"
#include "settings.h"
#include "auth.h"

UiLogin::UiLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    connect(ui->btnCancel, &QPushButton::clicked, [this](){
        ui->lnUser->clear();
        ui->lnPass->clear();
    });
}

UiLogin::~UiLogin()
{
    delete ui;
}

void UiLogin::on_btnLogin_clicked()
{
    Settings::getInstance().setClientId(ui->lnUser->text());
    Settings::getInstance().setClientSecret(ui->lnPass->text());

    auth = std::make_unique<Auth>(this);
    connect(auth.get(), &Auth::connectionOk, [this](){
        emit sucess();
    });

    connect(auth.get(), &Auth::connectionError, [this](const QString &msn){
        emit loginErro();
        ui->lbInfo->setText(msn);
    });
}
