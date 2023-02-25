#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    ConnectDB::Ptr db = ConnectDB::instance(DRIVER_SQLITE, &a);

//    if(IS_NULL(db))
//    {
//        QMessageBox::critical(nullptr, MBCW_TITLE,
//                              QS("Драйвер БД %1 не найден в системе!")
//                              .arg(DRIVER_SQLITE));

//        return -1;
//    }


    MainWindow w;
    w.show();
    return a.exec();
}
