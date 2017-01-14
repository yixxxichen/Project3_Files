#include "stdafx.h"
#include<Windows.h>
#include "mysql.h"

#include <iostream>

using namespace std;

MYSQL *conn;

int main(int argc, char *argv[])
{
	conn = mysql_init(NULL);

	mysql_real_connect(
		conn, "localhost", "user_name", "password", "test", 0, NULL, CLIENT_MULTI_RESULTS);

	mysql_close(conn);
}