#include "blackbear.h"

BlackBear::BlackBear()
{
	sql::Driver *pDriver;
	sql::Connection *pDBConnection;
	sql::Statement *pStatement;
	sql::ResultSet *pResultSet;

	wstring modelFileName;
	string tempModelFileName;
	string textureFileName;
	D3DXMATRIX translation;
	D3DXMATRIX rotation;
	D3DXMATRIX scale;
	D3DXMatrixIdentity(&translation);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&scale);

	D3DXMatrixRotationY(&rotation, D3DXToRadian(180));

	try
	{
		pDriver = sql::mysql::get_mysql_driver_instance();
		pDBConnection = pDriver->connect(g_sqlStringDatabaseLocation, "root", "worldeye");
		pDBConnection->setSchema("elements");

		pStatement = pDBConnection->createStatement();
		pResultSet = pStatement->executeQuery("SELECT * FROM beastiary WHERE idbeastiary = 1");

		while(pResultSet->next())
		{
			pResultSet->getInt("level");
			tempModelFileName = string(pResultSet->getString("modelFileName"));
			modelFileName = wstring(tempModelFileName.begin(), tempModelFileName.end());
			textureFileName = string(pResultSet->getString("textureFileName"));
		}
	}
	catch (sql::SQLException &e)
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	if (pResultSet)
	{
		delete pResultSet;
	}

	if (pStatement)
	{
		delete pStatement;
	}

	if (pDBConnection)
	{
		delete pDBConnection;
	}

	m_pPhysicalBody.SetModelSpaceTransform(translation, rotation, scale);
	m_pPhysicalBody.SetMesh(modelFileName.c_str(), textureFileName.c_str());
}

BlackBear::~BlackBear()
{
	cout << "black bear destructor called" << endl;
}