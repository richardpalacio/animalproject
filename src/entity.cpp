#include "entity.h"

Entity::Entity(int id)
{
	sql::mysql::MySQL_Driver *pDriver;
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
		
		string query = "SELECT * FROM beastiary WHERE idbeastiary = ";
		stringstream queryStream;
		queryStream << id;
		query += queryStream.str();
		pResultSet = pStatement->executeQuery(query);

		while (pResultSet->next())
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

Entity::~Entity()
{
}

VOID Entity::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	m_pPhysicalBody.Update(deltaTime, currentCamera);
	m_pVisualSystem.Update(deltaTime, currentCamera);
}

VOID Entity::Draw(VisualSystem *camera)
{
	// old FX value holders
	BOOL isFloorOld = FALSE;
	BOOL isMeshOld = FALSE;
	BOOL isBoundingBoxOld = FALSE;

	unsigned int numpasses = 0;

	// save the old FX values
	HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetChooserHandle(), &isFloorOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMeshOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetIsBoundingBoxHandle(), &isBoundingBoxOld, sizeof(BOOL)));

	// set the vertex declaration
	HR(g_pD3DGraphics->GetD3DDevice()->SetVertexDeclaration(g_pD3DGraphics->GetD3DVertexDecl()));

	/*
	start programmable shader pipeline
	because of row vector use, right multiply matrices
	*/
	//set technique / parameters
	HR(g_pD3DGraphics->GetFXInterface()->SetTechnique(g_pD3DGraphics->GetTechniqueHandle()));

	HR(g_pD3DGraphics->GetFXInterface()->Begin(&numpasses, 0));
	for (int i = 0; i < (int)numpasses; i++)
	{
		// begin pass
		HR(g_pD3DGraphics->GetFXInterface()->BeginPass(i));

		/*
		if a camera object was passed in then that means
		that this entity is not the one being controlled
		so do not use it's view/projection matrix
		*/
		if (camera != 0)
		{
			// update FX mesh texture variable
			m_pPhysicalBody.Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix());
		}
		else
		{
			// update FX mesh texture variable
			m_pVisualSystem.Draw();

			// update FX mesh texture variable
			m_pPhysicalBody.Draw(m_pVisualSystem.GetViewMatrix(), m_pVisualSystem.GetProjectionMatrix());
		}

		//done with this pass
		HR(g_pD3DGraphics->GetFXInterface()->EndPass());
	}

	// restore the old FX values
	HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetChooserHandle(), &isFloorOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMeshOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsBoundingBoxHandle(), &isBoundingBoxOld, sizeof(BOOL)));

	//make sure begin actually got called
	if (numpasses != 0)
	{
		HR(g_pD3DGraphics->GetFXInterface()->End());
	}
}