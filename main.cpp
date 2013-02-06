#include <QtGui/QApplication>
#include "pomodoro.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Pomodoro w;
    w.show();

    return a.exec();
}
