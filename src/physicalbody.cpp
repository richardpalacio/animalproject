#include "physicalbody.h"


PhysicalBody::PhysicalBody()
{
	//mesh vars
	m_pMesh = 0;
	m_pBoundingBoxMesh = 0;
	m_pAdjacencyBuffer = 0;
	m_pBoundingBoxAdjacencyBuffer = 0;
	m_pMaterialBuffer = 0;
	m_dwMaterialsCount = 0;
	m_pAnimationController = 0;

	m_vWorldSpaceUpVector = D3DXVECTOR3(0, 1, 0);
	m_vWorldSpaceLookVector = D3DXVECTOR3(0, 0, 1);
	m_vWorldSpaceRightVector = D3DXVECTOR3(1, 0, 0);

	//mesh texture
	m_pMeshTex = 0;
	
	D3DXMatrixIdentity(&m_matTransformMatrix);
	D3DXMatrixIdentity(&m_matTranslationMatrix);
	D3DXMatrixIdentity(&m_matRotationMatrix);
	D3DXMatrixIdentity(&m_matScalingMatrix);
	
	m_hMeshCol = g_pD3DGraphics->GetFXInterface()->GetParameterByName(0, "gMeshColor");
	if (!m_hMeshCol)
	{
		MessageBox(0, _TEXT("Invalid Parameter name gMeshColor"), 0, 0);
		exit(0);
	}
	
	m_hMeshTex = g_pD3DGraphics->GetFXInterface()->GetParameterByName(0, "gMeshTex");
	if (!m_hMeshTex)
	{
		MessageBox(0, _TEXT("Invalid Parameter name gMeshTex"), 0, 0);
		exit(0);
	}
}


PhysicalBody::~PhysicalBody()
{
	//release mesh variables
	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = 0;
	}
	
	if (m_pBoundingBoxMesh)
	{
		m_pBoundingBoxMesh->Release();
		m_pBoundingBoxMesh = 0;
	}

	if (m_pAdjacencyBuffer)
	{
		m_pAdjacencyBuffer->Release();
		m_pAdjacencyBuffer = 0;
	}

	if (m_pBoundingBoxAdjacencyBuffer)
	{
		m_pBoundingBoxAdjacencyBuffer->Release();
		m_pBoundingBoxAdjacencyBuffer = 0;
	}

	if (m_pMaterialBuffer)
	{
		m_pMaterialBuffer->Release();
		m_pMaterialBuffer = 0;
	}

	if (m_pMeshTex)
	{
		m_pMeshTex->Release();
		m_pMeshTex = 0;
	}

	if (m_pAnimationController)
	{
		m_pAnimationController->Release();
		m_pAnimationController = 0;
	}

	if (m_pRoot)
	{
		AllocationHierarchy alloc;
		D3DXFrameDestroy(m_pRoot, &alloc);
	}
}

VOID PhysicalBody::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	DOUBLE PI_OVER_SIX = D3DX_PI / 6;
	DOUBLE PI_OVER_TWO = D3DX_PI / 2;

	D3DXMATRIX tempMatrix;

	if (m_pAnimationController)
	{
		m_pAnimationController->AdvanceTime(deltaTime, NULL);
	}

	if (currentCamera)
	{
		D3DXMatrixTranslation(&m_matTransformMatrix, m_vWorldSpacePositionVector.x, m_vWorldSpacePositionVector.y, m_vWorldSpacePositionVector.z);

		if ((g_pD3DGraphics->GetKeyboardState()[DIK_LEFTARROW] & 0x80)
			&& (g_pD3DGraphics->GetKeyboardState()[DIK_RCONTROL] & 0x80))
		{
			//move the camera along its negative x-axis
			m_vWorldSpacePositionVector += -0.01f * m_vWorldSpaceRightVector;
		}
		else if (g_pD3DGraphics->GetKeyboardState()[DIK_LEFTARROW] & 0x80)
		{
			//rotate the right, up and look camera axis vectors
			D3DXMatrixRotationY(&tempMatrix, -0.001f);
			m_matModelSpaceMatrix *= tempMatrix;
			
			D3DXVec3TransformNormal(&m_vWorldSpaceRightVector, &m_vWorldSpaceRightVector, &tempMatrix);
			D3DXVec3TransformNormal(&m_vWorldSpaceUpVector, &m_vWorldSpaceUpVector, &tempMatrix);
			D3DXVec3TransformNormal(&m_vWorldSpaceLookVector, &m_vWorldSpaceLookVector, &tempMatrix);
		}

		if ((g_pD3DGraphics->GetKeyboardState()[DIK_RIGHTARROW] & 0x80)
			&& (g_pD3DGraphics->GetKeyboardState()[DIK_RCONTROL] & 0x80))
		{
			//move the camera along its positive x-axis
			m_vWorldSpacePositionVector += 0.01f * m_vWorldSpaceRightVector;
		}
		else if (g_pD3DGraphics->GetKeyboardState()[DIK_RIGHTARROW] & 0x80)
		{
			//rotate the right, up and look camera axis vectors
			D3DXMatrixRotationY(&tempMatrix, 0.001f);
			m_matModelSpaceMatrix *= tempMatrix;
			
			D3DXVec3TransformNormal(&m_vWorldSpaceRightVector, &m_vWorldSpaceRightVector, &tempMatrix);
			D3DXVec3TransformNormal(&m_vWorldSpaceUpVector, &m_vWorldSpaceUpVector, &tempMatrix);
			D3DXVec3TransformNormal(&m_vWorldSpaceLookVector, &m_vWorldSpaceLookVector, &tempMatrix);
		}

		if (g_pD3DGraphics->GetKeyboardState()[DIK_UPARROW] & 0x80)
		{
			//400px/1s * 1s/1000ms
			m_vWorldSpacePositionVector += 0.05f * m_vWorldSpaceLookVector * static_cast<float>(deltaTime);
		}

		if (g_pD3DGraphics->GetKeyboardState()[DIK_DOWNARROW] & 0x80)
		{
			m_vWorldSpacePositionVector += -0.05f * m_vWorldSpaceLookVector * static_cast<float>(deltaTime);
		}

		//keep the eye at the same height above the ground
		m_vWorldSpacePositionVector.y = 0;

		D3DXVec3Normalize(&m_vWorldSpaceRightVector, &m_vWorldSpaceRightVector);
		D3DXVec3Normalize(&m_vWorldSpaceUpVector, &m_vWorldSpaceUpVector);
		D3DXVec3Normalize(&m_vWorldSpaceLookVector, &m_vWorldSpaceLookVector);
	}
}

/*
Position the model, set textures, draw
note drawsubset changes the FVF automatically
*/
VOID PhysicalBody::Draw(D3DXMATRIX m_matView, D3DXMATRIX m_matProjection)
{
	// old FX value holders
	BOOL isFloorOld = FALSE;
	BOOL isMeshOld = FALSE;
	BOOL isBoundingBoxOld = FALSE;
	D3DXMATRIX temp;
	
	// new FX file booleans
	BOOL isMesh = TRUE;

	if (m_pMesh != 0)
	{
		// save the old FX values
		HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetChooserHandle(), &isFloorOld, sizeof(BOOL)));
		HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMeshOld, sizeof(BOOL)));
		HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetIsBoundingBoxHandle(), &isBoundingBoxOld, sizeof(BOOL)));

		// set new FX variables
		HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMesh, sizeof(BOOL)));

		if (m_pMeshTex != 0)
		{
			HR(g_pD3DGraphics->GetFXInterface()->SetTexture(m_hMeshTex, m_pMeshTex));
		}
		else
		{
			HR(g_pD3DGraphics->GetFXInterface()->SetValue(m_hMeshCol, &m_d3dcvDiffuseMesh, sizeof(D3DCOLORVALUE)));
		}
		
		// draw the mesh
		HR(g_pD3DGraphics->GetFXInterface()->SetMatrix(
			g_pD3DGraphics->GetWorldViewProjectionHandle(),
			&(m_matModelSpaceMatrix * g_pD3DGraphics->GetWorldMatrix() * m_matTransformMatrix * m_matView * m_matProjection))
		);
		D3DXMATRIX worldInverseTranspose;
		D3DXMatrixInverse(&worldInverseTranspose, 0, &(m_matModelSpaceMatrix * g_pD3DGraphics->GetWorldMatrix() * m_matTransformMatrix * m_matView * m_matProjection));
		D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);
		HR(g_pD3DGraphics->GetFXInterface()->SetMatrix(
			g_pD3DGraphics->GetWorldInverseTransposeHandler(),
			&worldInverseTranspose)
		);
		HR(g_pD3DGraphics->GetFXInterface()->CommitChanges());
		HR(m_pMesh->DrawSubset(0));
		
		// draw the bounding box
		D3DXMatrixTranslation(&temp, m_vBoundingSphereCenterPoint.x, m_vBoundingSphereCenterPoint.y, m_vBoundingSphereCenterPoint.z);
		HR(g_pD3DGraphics->GetFXInterface()->SetMatrix(
			g_pD3DGraphics->GetWorldViewProjectionHandle(),
			&(temp * m_matModelSpaceMatrix * g_pD3DGraphics->GetWorldMatrix() * m_matTransformMatrix * m_matView * m_matProjection))
			);
		worldInverseTranspose;
		D3DXMatrixInverse(&worldInverseTranspose, 0, &(m_matModelSpaceMatrix * g_pD3DGraphics->GetWorldMatrix() * m_matTransformMatrix * m_matView * m_matProjection));
		D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);
		HR(g_pD3DGraphics->GetFXInterface()->SetMatrix(
			g_pD3DGraphics->GetWorldInverseTransposeHandler(),
			&worldInverseTranspose)
			);

		BOOL isMesh = FALSE;
		BOOL isBoundingBox = TRUE;
		g_pD3DGraphics->GetD3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pD3DGraphics->GetD3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pD3DGraphics->GetD3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMesh, sizeof(BOOL)));
		HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsBoundingBoxHandle(), &isBoundingBox, sizeof(BOOL)));
		HR(g_pD3DGraphics->GetFXInterface()->CommitChanges());
		HR(m_pBoundingBoxMesh->DrawSubset(0));

		// restore the old FX values
		g_pD3DGraphics->GetD3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetChooserHandle(), &isFloorOld, sizeof(BOOL)));
		HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMeshOld, sizeof(BOOL)));
		HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsBoundingBoxHandle(), &isBoundingBoxOld, sizeof(BOOL)));
		HR(g_pD3DGraphics->GetFXInterface()->CommitChanges());
	}
}

VOID PhysicalBody::SetModelSpaceTransform(const D3DXMATRIX &translation, const D3DXMATRIX &rotation, const D3DXMATRIX &scale)
{
	// position the the model in local space
	m_matModelSpaceMatrix = translation * rotation * scale;
}

/*
create the mesh object for
an entity
param: mesh filename
*/
VOID PhysicalBody::SetMesh(const TCHAR* meshFileName, const char* textureFileName)
{
	string tempSTDString = string(textureFileName);
	wstring tempTextureFileName = wstring(tempSTDString.begin(), tempSTDString.end());
	D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];

	m_pMesh = 0;
	m_pMaterialBuffer = 0;
	
	if (meshFileName && _tcslen(meshFileName) != 0){
		// load meshes from file
		HR(D3DXLoadMeshFromX(meshFileName, D3DXMESH_MANAGED, g_pD3DGraphics->GetD3DDevice(), &m_pAdjacencyBuffer,
			&m_pMaterialBuffer, 0, &m_dwMaterialsCount, &m_pMesh));
		
		// obtain the mesh file data: textures, lighting, animation
		if (m_pMaterialBuffer != 0 && m_dwMaterialsCount != 0)
		{
			D3DXMATERIAL *d3dMaterials = (D3DXMATERIAL*)m_pMaterialBuffer->GetBufferPointer();
			for (DWORD i = 0; i < m_dwMaterialsCount; i++)
			{
				if (textureFileName)
				{
					d3dMaterials[i].pTextureFilename = const_cast<char*>(textureFileName);
				}
				
				if (d3dMaterials[i].pTextureFilename != 0)
				{
					D3DXCreateTextureFromFile(g_pD3DGraphics->GetD3DDevice(), tempTextureFileName.c_str(), &m_pMeshTex);
				}
				else
				{
					m_d3dcvDiffuseMesh.a = d3dMaterials[i].MatD3D.Diffuse.a;
					m_d3dcvDiffuseMesh.r = d3dMaterials[i].MatD3D.Diffuse.r;
					m_d3dcvDiffuseMesh.g = d3dMaterials[i].MatD3D.Diffuse.g;
					m_d3dcvDiffuseMesh.b = d3dMaterials[i].MatD3D.Diffuse.b;
				}
			}
		}

		// lock the vertex buffer
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, reinterpret_cast<LPVOID*>(&m_pMeshVertexData));
		// set the vertex data
		D3DXComputeBoundingSphere(m_pMeshVertexData, m_pMesh->GetNumVertices(), m_pMesh->GetNumBytesPerVertex(), &m_vBoundingSphereCenterPoint, &m_nBoundingSphereRadius);
		// unlock the vertex buffer
		m_pMesh->UnlockVertexBuffer();

		// create the bounding box
		// directx create functions do not generate texture coordinates
		D3DXCreateSphere(g_pD3DGraphics->GetD3DDevice(), m_nBoundingSphereRadius, 10, 10, &m_pBoundingBoxMesh, &m_pBoundingBoxAdjacencyBuffer);

		// optimize the mesh then clone the mesh and bounding sphere
		m_pMesh->GetDeclaration(elements);
		m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT
			| D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)m_pAdjacencyBuffer->GetBufferPointer(),
			(DWORD*)m_pAdjacencyBuffer->GetBufferPointer(), 0, 0);

		m_pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
			elements, g_pD3DGraphics->GetD3DDevice(), &m_pMesh);

		AllocationHierarchy alloc;
		D3DXLoadMeshHierarchyFromX(meshFileName,
			D3DXMESH_MANAGED,
			g_pD3DGraphics->GetD3DDevice(),
			&alloc,
			0,
			&m_pRoot,
			&m_pAnimationController
		);
	}
}

HRESULT AllocationHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	LPSTR meshName = new CHAR[strlen(Name) + 1];
	memcpy(meshName, Name, strlen(Name) + 1);
	
	(*ppNewFrame) = new D3DXFRAME();
	(*ppNewFrame)->Name = meshName;
	(*ppNewFrame)->pMeshContainer = NULL;
	(*ppNewFrame)->pFrameSibling = NULL;
	(*ppNewFrame)->pFrameFirstChild = NULL;

	return D3D_OK;
}

HRESULT AllocationHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	delete[] pFrameToFree->Name;
	delete pFrameToFree;

	return D3D_OK;
}

HRESULT AllocationHierarchy::CreateMeshContainer(LPCSTR Name,
	const D3DXMESHDATA *pMeshData, const D3DXMATERIAL *pMaterials,
	const D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials,
	const DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER *meshContainer = new D3DXMESHCONTAINER();
	memset(meshContainer, 0, sizeof(D3DXMESHCONTAINER));

	meshContainer->Name = new CHAR[strlen(Name) + 1];
	memcpy(meshContainer->Name, Name, strlen(Name) + 1);

	meshContainer->MeshData.pMesh = pMeshData->pMesh;
	meshContainer->MeshData.Type = pMeshData->Type;
	pMeshData->pMesh->AddRef();

	meshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];
	for (DWORD i = 0; i < NumMaterials; i++)
	{
		D3DXMATERIAL *materials = meshContainer->pMaterials;
		materials[i].MatD3D = pMaterials[i].MatD3D;
		materials[i].MatD3D.Ambient = pMaterials[i].MatD3D.Diffuse;

		//materials[i].pTextureFilename = new CHAR[strlen(pMaterials[i].pTextureFilename) + 1];
		//memcpy(materials[i].pTextureFilename, pMaterials[i].pTextureFilename, strlen(pMaterials[i].pTextureFilename) + 1);
	}

	// copy effects
	meshContainer->pEffects = new D3DXEFFECTINSTANCE(*pEffectInstances);
	// copy adjacency
	meshContainer->pAdjacency = new DWORD(*pAdjacency);

	meshContainer->NumMaterials = NumMaterials;

	if (pSkinInfo) {
		meshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
	}

	(*ppNewMeshContainer) = meshContainer;

	return D3D_OK;
}

HRESULT AllocationHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	delete[] pMeshContainerToFree->Name;

	for (DWORD i = 0; i < pMeshContainerToFree->NumMaterials; ++i)
	{
		//delete[] pMeshContainerToFree->pMaterials[i].pTextureFilename;
	}

	delete[] pMeshContainerToFree->pMaterials;
	delete[] pMeshContainerToFree->pEffects;
	delete[] pMeshContainerToFree->pAdjacency;

	pMeshContainerToFree->MeshData.pMesh->Release();
	pMeshContainerToFree->MeshData.pMesh = 0;

	if (pMeshContainerToFree->pSkinInfo){
		pMeshContainerToFree->pSkinInfo->Release();
		pMeshContainerToFree->pSkinInfo = 0;
	}

	delete pMeshContainerToFree;

	return D3D_OK;
}