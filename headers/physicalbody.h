#pragma once

#include <string>
#include "graphicdevice.h"

extern D3DGraphics *g_pD3DGraphics;

using namespace std;

class PhysicalBody
{
	friend class AllocationHierarchy;

public:
	PhysicalBody();
	virtual ~PhysicalBody();
	
	VOID Update(DOUBLE deltaTime, BOOL currentCamera = FALSE);
	VOID Draw(D3DXMATRIX m_matView, D3DXMATRIX m_matProjection);
	VOID SetMesh(const TCHAR* meshFileName, const char* textureFileName);
	VOID SetModelSpaceTransform(const D3DXMATRIX &translation, const D3DXMATRIX &rotation, const D3DXMATRIX &scale);
	
	/*
		set the position of the model in world space
		however make the position vector reference the bounding sphere
		center position vector
	*/
	VOID SetPosition(FLOAT x, FLOAT y, FLOAT z){
		m_vWorldSpacePositionVector.x = x;
		m_vWorldSpacePositionVector.y = y;
		m_vWorldSpacePositionVector.z = z;

		D3DXMatrixTranslation(&m_matTranslationMatrix, m_vWorldSpacePositionVector.x, m_vWorldSpacePositionVector.y, m_vWorldSpacePositionVector.z);
		m_matTransformMatrix = m_matTranslationMatrix * m_matRotationMatrix * m_matScalingMatrix;
	}

	D3DXVECTOR3 GetPosition(){
		return m_vWorldSpacePositionVector;
	}

	D3DXVECTOR3 GetCentroid(){
		return m_vBoundingSphereCenterPoint;
	}

	FLOAT GetRadius(){
		if (m_pMesh != 0){
			return m_nBoundingSphereRadius;
		}
		else{
			return 0;
		}
	}
protected:
	D3DXMATRIX m_matModelSpaceMatrix;
	D3DXMATRIX m_matTranslationMatrix;
	D3DXMATRIX m_matScalingMatrix;
	D3DXMATRIX m_matRotationMatrix;
	D3DXMATRIX m_matTransformMatrix;
	
	D3DXVECTOR3 m_vWorldSpacePositionVector;
	D3DXVECTOR3 m_vWorldSpaceUpVector;
	D3DXVECTOR3 m_vWorldSpaceLookVector;
	D3DXVECTOR3 m_vWorldSpaceRightVector;

	DWORD m_dwMaterialsCount;
	
	ID3DXMesh *m_pMesh;
	ID3DXMesh *m_pBoundingBoxMesh;

	D3DXFRAME *m_pRoot;
	ID3DXAnimationController *m_pAnimationController;

	ID3DXBuffer *m_pMaterialBuffer;
	ID3DXBuffer *m_pAdjacencyBuffer;
	ID3DXBuffer *m_pBoundingBoxAdjacencyBuffer;

	D3DMATERIAL9 m_pBoundingBoxMaterial;

	D3DXHANDLE m_hEyeVecW;
	D3DXHANDLE m_hWorld;
	D3DXHANDLE m_hMeshCol;
	D3DXHANDLE m_hMeshTex;
	
	D3DCOLORVALUE m_d3dcvDiffuseMesh;
	
	IDirect3DTexture9 *m_pMeshTex;

	D3DXVECTOR3 *m_pMeshVertexData;
	D3DXVECTOR3 m_vBoundingSphereCenterPoint;
	FLOAT m_nBoundingSphereRadius;
};

class AllocationHierarchy : public ID3DXAllocateHierarchy
{
	HRESULT STDMETHODCALLTYPE CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame);

	HRESULT STDMETHODCALLTYPE CreateMeshContainer(LPCSTR Name,
		const D3DXMESHDATA *pMeshData, const D3DXMATERIAL *pMaterials,
		const D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials,
		const DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer
		);

	HRESULT STDMETHODCALLTYPE DestroyFrame(LPD3DXFRAME pFrameToFree);

	HRESULT STDMETHODCALLTYPE DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree);
};