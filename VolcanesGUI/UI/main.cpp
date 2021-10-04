#include "parametrosUI.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParametrosUI w;
    w.show();
    return a.exec();
}
