
#include "DataBase.h"

using namespace sql;

#define DB_CONNECTION	"tcp://localhost:3306"
#define DB_USER_NAME	"spaceman"
#define DB_PASSWORD		"fyutkjxtr27"
#define DB_SCHEMA		"space"

static DataBase* pDataBase = DataBase::getInstance();

DataBase* const DataBase::getInstance()
{
	if(!pDataBase)
		pDataBase = new DataBase();
	return pDataBase;
}

void DataBase::init()
{
	printf("Initialization database..\n");

	pDriver = get_driver_instance();
	printf("Found database driver: %s\n", pDriver->getName().c_str());

	if(!ping())
		printf("WARNING! Couldn't connect to database!");
}

void DataBase::free()
{
	if(pDataBase)
		delete pDataBase;
}

bool DataBase::ping()
{
	try {
		Connection* connection = pDriver->connect(DB_CONNECTION, DB_USER_NAME, DB_PASSWORD);
		connection->setSchema(DB_SCHEMA);
		printf("Connected to database: %s@%s\n", DB_CONNECTION, DB_SCHEMA);

		/*Statement* st = connection->createStatement();
		ResultSet* rs = st->executeQuery("SELECT 'Hello World!' AS _message");

		while(rs->next()) 
		{
			SQLString result = rs->getString(1);
			printf("Answer from MySQL: %s\n", result.c_str());
		}

		delete rs;
		delete st;*/
		delete connection;
		return true;

	} catch (SQLException &e) {
		printf("SQLException :: %s", e.what());
	}

	return false;
}