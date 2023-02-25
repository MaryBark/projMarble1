#include "interaction2Database.h"



DBparametrs::DBparametrs():
    m_driver("QSQLITE"),
    m_host(),
    m_port(-1),
    m_name()
{}

DBparametrs::DBparametrs(const QString &_driver,
                         const QString &_host,
                         const int _port,
                         const QString &_name,
                         const QString &_user,
                         const QString &_password,
                         const QString &_options):
     m_driver(_driver),
      m_host(_host),
      m_port(_port),
      m_name(_name),
      m_user(_user),
      m_password(_password),
      m_options(_options)
{}

DBparametrs::DBparametrs(const DBparametrs &old):
      m_driver(old.m_driver),
      m_host(old.m_host),
      m_port(old.m_port),
      m_rawName(old.m_rawName),
      m_name(old.m_name),
      m_user(old.m_user),
      m_password(old.m_password),
      m_options(old.m_options)
{}


//interaction2Database::interaction2Database()/* : _isConnected(false)*/
//{}

//bool interaction2Database::_createConnection(const DBparametrs & params)
//{
//    if (!QSqlDatabase::isDriverAvailable(params.m_driver))
//    {
//        return false;
//    }

//    _db = QSqlDatabase::addDatabase(params.m_driver,
//                                     QS("qt_sql_connection"));

//    _db.setHostName(params.m_host);
//    _db.setDatabaseName(params.m_name);
//    _db.setUserName(params.m_user);
//    _db.setPassword(params.m_password);

//    if(!params.m_options.isEmpty())
//        _db.setConnectOptions(params.m_options);

//    if(params.m_port >= 0)
//        _db.setPort(params.m_port);

//    if (!_db.open())
//    {
//        return false;
//    }

//    _isConnected = true;

//    return true;
//}
