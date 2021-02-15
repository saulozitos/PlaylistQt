#pragma once

#include <QWidget>
#include <memory>

class Auth;

namespace Ui {
class Login;
}

class UiLogin : public QWidget
{
    Q_OBJECT

public:
    explicit UiLogin(QWidget *parent = nullptr);
    ~UiLogin();

private:
    Ui::Login *ui;
    std::unique_ptr<Auth>auth;

signals:
    void sucess();
    void loginErro();
private slots:
    void on_btnLogin_clicked();
};

