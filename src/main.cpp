#include "qtodolist.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QToDoList w;
    w.show();
    return a.exec();
}
