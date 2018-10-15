#include <QCoreApplication>

#include "scoreboard.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Scoreboard sb;

    return a.exec();
}
