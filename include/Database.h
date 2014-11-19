#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <mysql.h>

class Database
{
public:
	Database();
	Database( std::string Host, std::string Username, std::string Password, std::string Db );
	virtual ~Database();

	MYSQL* GetMySQL();

	bool Valid();
	void Close();

	MYSQL_RES* Query( std::string Query );
	MYSQL_ROW FetchRow( MYSQL_RES* Result );
	int Next();

	int NumRows( MYSQL_RES* Result );
	void Free( MYSQL_RES* Result );

	std::string Format( const char* fmt, ... );

private:
	std::string _host, _username, _password, _db;
	MYSQL* _mysql;
};

extern Database* Db;

#endif
