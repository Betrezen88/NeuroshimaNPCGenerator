#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    const QString appName{ "Neuroshima NPC Generator" };

    QApplication a(argc, argv);
    a.setWindowIcon( QIcon(":/images/icons/Neuroshima_icon.png") );
    a.setApplicationName( appName );
    a.setApplicationDisplayName( appName );

    MainWindow w;
    w.show();

    return a.exec();
}
