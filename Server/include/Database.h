#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include "mysql_connection.h"

#include "../include/cppconn/driver.h"
#include "../include/cppconn/exception.h"
#include "../include/cppconn/resultset.h"
#include "../include/cppconn/statement.h"

class Database;

class Database{

    public:
        Database();
        ~Database();

};


#endif // DATABASE_H_INCLUDED
