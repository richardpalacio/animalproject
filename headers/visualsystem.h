#pragma once

#include "eye.h"

class VisualSystem
{
public:
	VisualSystem();
	virtual ~VisualSystem();
	
	VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
	VOID Draw();

	D3DXMATRIX GetViewMatrix(){
		return m_pEye.GetViewMatrix();
	}
	
	D3DXMATRIX GetProjectionMatrix(){
		return m_pEye.GetProjectionMatrix();
	}

	VOID SetPosition(FLOAT x, FLOAT y, FLOAT z){
		m_pEye.SetPosition(x, y, z);
	}

	FLOAT GetNearPlane(){
		return m_pEye.GetNearPlane();
	}

	FLOAT GetFarPlane(){
		return m_pEye.GetFarPlane();
	}
protected:
	bool blind;

	Eye m_pEye;
};
