#include "uiplayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UiPlayer w;
    w.show();
    return a.exec();
}
