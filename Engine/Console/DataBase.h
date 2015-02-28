
#pragma once

#pragma warning(disable:4251)
#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class DataBase
{
	public:
		static DataBase* const getInstance();
		void init();
		void free();

		bool ping();



	protected:
		virtual ~DataBase() {}

	private:
		sql::Driver* pDriver;
};