#include "ui/mainWindow.h"
#include <QApplication>


int main(int argc, char* argv[])
{
    //InitializeTSF();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int ret = a.exec();
    //CleanupTSF();
    return ret;
}
