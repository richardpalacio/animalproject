#pragma once

#include <windows.h>
#include <iostream>
#include <sstream>
#include "graphicdevice.h"
#include "visualsystem.h"
#include "physicalbody.h"

#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

extern D3DGraphics *g_pD3DGraphics;
extern sql::SQLString g_sqlStringDatabaseLocation;

using namespace std;

class Entity
{
public:
	Entity(int id = 0);
	virtual ~Entity();

	/*
	update objects according to keyboard input
	*/
	virtual VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
	VOID Draw(VisualSystem* camera = 0);
	
	VOID SetPosition(FLOAT x, FLOAT y, FLOAT z){
		m_pPhysicalBody.SetPosition(x, y, z);
		m_pVisualSystem.SetPosition(x, y, z);
	}

	D3DXVECTOR3 GetPosition(){
		return m_pPhysicalBody.GetPosition();
	}

	D3DXVECTOR3 GetCentroid(){
		return m_pPhysicalBody.GetCentroid();
	}
	FLOAT GetRadius(){
		return m_pPhysicalBody.GetRadius();
	}

	VisualSystem* GetVisualSystem(){
		return &m_pVisualSystem;
	}
protected:
	bool cursed;
	
	enum Gender {male, female};
	
	int m_nLevel;
	int m_nHealth;
	int m_nHealthRegeneration;
	int m_nMana;
	int m_nManaRegeneration;
	int m_nEnergy;
	int m_nEnergyRegeneration;

	VisualSystem m_pVisualSystem;
	PhysicalBody m_pPhysicalBody;
};
