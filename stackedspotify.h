#pragma once

#include <QStackedWidget>
#include <memory>

class UiLogin;
class UiPlayer;

namespace Ui {
class Stack;
}

class StackedSpotify : public QStackedWidget
{
    Q_OBJECT

public:
    explicit StackedSpotify(QWidget *parent = nullptr);
    ~StackedSpotify();

private:
    Ui::Stack *ui;

    std::unique_ptr<UiLogin>uiLogin;
    std::unique_ptr<UiPlayer>uiPlayer;
};

