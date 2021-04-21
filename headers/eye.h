#pragma once

//include DirectX libraries
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <tchar.h>
#include <stdio.h>

#include "graphicdevice.h"

extern D3DGraphics *g_pD3DGraphics;

class Eye
{
public:
	Eye();
	virtual ~Eye();
	
	//setup view/projection matrix
	VOID SetupViewProjection();
	VOID SetBodyObject(ID3DXMesh *mesh){ m_pSelfMesh = mesh; }
	VOID Update(DOUBLE deltaTime);
	VOID Draw();
	
	//getters and setters
	D3DXMATRIX GetViewMatrix(){
		return m_matView;
	}

	D3DXMATRIX GetProjectionMatrix(){
		return m_matProjection;
	}

	VOID SetPosition(FLOAT x, FLOAT y, FLOAT z){
		m_pCameraOriginVector = D3DXVECTOR3(x, y, z);
	}

	FLOAT GetNearPlane(){
		return m_nZNear;
	}

	FLOAT GetFarPlane(){
		return m_nZFar;
	}

protected:
	// get a reference to my own body
	ID3DXMesh *m_pSelfMesh;

	// camera variables
	//camera field of view angle in radians
	FLOAT m_nCameraFieldOfViewAngle;
	//camera lens magnitude along the n-axis
	FLOAT m_nCameraLensMagnitude;
	D3DXVECTOR3 m_pCameraOriginVector;
	//camera look at vector in world space
	D3DXVECTOR3 m_pCameraLookAtPosition;
	D3DXVECTOR3 m_pCameraRightVectorU;
	D3DXVECTOR3 m_pCameraUpVectorV;
	D3DXVECTOR3 m_pCameraLookVectorN;
	FLOAT m_nZNear;
	FLOAT m_nZFar;
	FLOAT m_nAspectRatio;
	UINT m_nWindowWidth;
	UINT m_nWindowHeight;

	// transform matrices
	D3DXMATRIX m_matTranslationMatrix;

	// model, world, view, projection matrices
	D3DXMATRIX m_matWorldMatrix;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProjection;

	// fx handles
	D3DXHANDLE m_hWorld; // world handle
	D3DXHANDLE m_hLightDirection; // light direction
};