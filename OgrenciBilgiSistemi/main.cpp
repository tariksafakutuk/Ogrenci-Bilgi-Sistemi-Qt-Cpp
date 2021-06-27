#include "anamenu.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnaMenu w;
    w.show();
    return a.exec();
}
