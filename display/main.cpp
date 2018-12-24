#include "Widget.h"
#include <QApplication>
#include <QTabWidget>

#include "displayconf.h"
#include "serverconf.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Widget w;
    //w.show();

    QTabWidget mainTab;
    mainTab.addTab(new DisplayConf, "Display Config");
    mainTab.addTab(new ServerConf, "Server Config");
    mainTab.show();

    return a.exec();
}
