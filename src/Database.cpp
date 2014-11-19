#include "Database.h"

Database* Db;

Database::Database() :
	_mysql( 0 ), _host( "" ), _username( "" ), _password( "" ), _db( "" )
{
}

Database::Database( std::string Host, std::string Username, std::string Password, std::string Db ) :
	_host( Host ), _username( Username ), _password( Password ), _db( Db )
{
	_mysql = mysql_init( 0 );

	if( !_mysql )
	{
		std::cout << "Error initializing MySQL! (" << mysql_error( _mysql ) << ")" << std::endl;
		return;
	}

	if( !mysql_real_connect( _mysql, Host.c_str(), Username.c_str(), Password.c_str(), Db.c_str(), 0, 0, 0 ) )
	{
		std::cout << "Error connecting to MySQL! (" << mysql_error( _mysql ) << ")" << std::endl;
		Close();

		return;
	}
}

Database::~Database()
{
	_host.clear();
	_username.clear();
	_password.clear();
	_db.clear();

	delete _mysql;
}

MYSQL* Database::GetMySQL()
{
	return _mysql;
}

bool Database::Valid()
{
	return ( _mysql != 0 );
}

void Database::Close()
{
	mysql_close( _mysql );
}

MYSQL_RES* Database::Query( std::string Query )
{
	if( !Valid() )
		return 0;

	if( mysql_query( _mysql, Query.c_str() ) )
	{
		std::cout << "mysql_query error" << std::endl;
		return 0;
	}

	MYSQL_RES* result = mysql_use_result( _mysql );

	if( !result )
	{
		std::cout << "mysql_use_result error" << std::endl;
		return 0;
	}

	return result;
}

MYSQL_ROW Database::FetchRow( MYSQL_RES* Result )
{
	return mysql_fetch_row( Result );
}

int Database::Next()
{
	return mysql_next_result( _mysql );
}

int Database::NumRows( MYSQL_RES* Result )
{
	return mysql_num_rows( Result );
}

void Database::Free( MYSQL_RES* Result )
{
	mysql_free_result( Result );
}

std::string Database::Format( const char* fmt, ... )
{
	char strOut[512];
		
	va_list s;
	va_start( s, fmt );
		vsnprintf( strOut, sizeof( strOut ) / sizeof( char ), fmt, s );
	va_end( s );

	return std::string( strOut );
}
