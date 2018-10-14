#include "Widget.h"
#include <QApplication>

#include "displayconf.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();

    DisplayConf conf;
    conf.show();

    return a.exec();
}
