#pragma once

#include <QDialog>
#include <QString>
#include <QNetworkAccessManager>
#include <QTcpServer>
#include <memory>
#include "settings.h"

class Auth : public QDialog
{
    Q_OBJECT
public:
    explicit Auth(QWidget *parent = nullptr);
    ~Auth() = default;

private:
    std::unique_ptr<QNetworkAccessManager>networkManager;
    std::unique_ptr<QTcpServer>server;
    Settings &settings;

    QString authUrl();
    QString auth();

signals:
    void connectionOk();
    void connectionError(const QString &msn);
};

