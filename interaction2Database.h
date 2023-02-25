#ifndef INTERACTION2DATABASE_H
#define INTERACTION2DATABASE_H

#include <iostream>
//---------------------------------------------------------------------------
#include <QVariant>
#include <QSet>
#include <QtSql/QSqlDatabase>
#include <definitions.h>


struct DBparametrs
{
    QString m_driver;
    QString m_host;
    int m_port;
    QString m_rawName;
    QString m_name;
    QString m_user;
    QString m_password;
    QString m_options;

    DBparametrs();

    DBparametrs(const QString &_driver,
            const QString &_host,
            const int _port,
            const QString &_name,
            const QString &_user,
            const QString &_password,
            const QString &_options = QString());

    DBparametrs(const DBparametrs& old);
};


//class interaction2Database
//{
//public:
//    interaction2Database();


////    /*virtual*/ bool _createConnection(const DBparametrs&);

//private:
//    QSqlDatabase _db;
//    bool _isConnected;
//};






#endif // INTERACTION2DATABASE_H
