#include "eye.h"

Eye::Eye()
{
	m_nCameraFieldOfViewAngle = D3DX_PI / 4;
	m_nCameraLensMagnitude = 100;
	m_pCameraRightVectorU = D3DXVECTOR3(1,0,0);
	m_pCameraUpVectorV = D3DXVECTOR3(0,1,0);
	m_pCameraLookVectorN = D3DXVECTOR3(0,0,1);
	m_pCameraLookAtPosition = D3DXVECTOR3(0,0,0);
	m_nAspectRatio = 0;
	m_nWindowHeight = 0;
	m_nWindowWidth = 0;
	m_pSelfMesh = 0;

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProjection);

	m_nZNear = 1.0f;
	m_nZFar = 1000.0f;
}

Eye::~Eye()
{
}

/*
this is the one of the main features of
this class, this is where the 2 most
important pieces of generating the scene come
from,  view and projection matrices
setup view, and projection matrices
*/
VOID Eye::SetupViewProjection()
{
	//Set the view matrix with the camera parameters
	D3DXMatrixLookAtLH(&m_matView, &m_pCameraOriginVector, &m_pCameraLookAtPosition, &m_pCameraUpVectorV);

	//Set the projection matrix
	m_nWindowWidth = g_pD3DGraphics->GetPresentationParams().BackBufferWidth;
	m_nWindowHeight = g_pD3DGraphics->GetPresentationParams().BackBufferHeight;
	m_nAspectRatio = static_cast<FLOAT>(m_nWindowWidth) / static_cast<FLOAT>(m_nWindowHeight);
	D3DXMatrixPerspectiveFovLH(&m_matProjection, m_nCameraFieldOfViewAngle, m_nAspectRatio, m_nZNear, m_nZFar);
}

/*
update objects according to keyboard input
remember we are in left-handed camera space at this point
DirectX uses a left-handed coordinate system
so the positive z-axis goes into the page
*/
VOID Eye::Update(DOUBLE deltaTime)
{
	DOUBLE tempAngleInRadians;
	DOUBLE PI_OVER_SIX = D3DX_PI / 6;
	DOUBLE PI_OVER_TWO = D3DX_PI / 2;
	FLOAT cameraRightRotationSpeed = 0.05f * static_cast<float>(deltaTime);
	FLOAT scalarValue;
	
	D3DXMATRIX tempMatrix;
	
	if (g_pD3DGraphics->GetMouseState().lZ < 0)
	{
		//decrease the frustrum size
		m_nCameraFieldOfViewAngle -= g_pD3DGraphics->GetMouseState().lZ * (FLOAT)1/1200 * static_cast<float>(deltaTime);
		
		//clamp at 60°
		if (m_nCameraFieldOfViewAngle > (D3DX_PI / 3)){
			m_nCameraFieldOfViewAngle = D3DX_PI / 3;
		}
	}
	else if (g_pD3DGraphics->GetMouseState().lZ > 0)
	{
		//increase the frustrum size
		m_nCameraFieldOfViewAngle -= g_pD3DGraphics->GetMouseState().lZ * (FLOAT)1/1200 * static_cast<float>(deltaTime);
		
		//clamp at 0°
		if (m_nCameraFieldOfViewAngle < 0){
			m_nCameraFieldOfViewAngle = 0;
		}
	}
	
	//negative rotation about the camera x-axis
	//rotation should be smooth, if we rotate too much, set the rotation to the max or min
	if (g_pD3DGraphics->GetKeyboardState()[DIK_PGDN] & 0x80)
	{
		//get the scalar projection of the camera up vector onto the world y vector
		scalarValue = D3DXVec3Dot(&m_pCameraUpVectorV, &D3DXVECTOR3(0,1,0));
		//get the angle between vec1 and vec2
		tempAngleInRadians = acos(scalarValue);
		
		// is the y component of the N axis greater then 0, m_pCameraLookVectorN.y > 0 || 
		//limit the camera rotation
		if (m_pCameraLookVectorN.y > 0 || tempAngleInRadians + cameraRightRotationSpeed < PI_OVER_SIX)
		{
			//just rotate the up and look camera axis vectors because the right axis is the rotation axis
			D3DXMatrixRotationAxis(&tempMatrix, &m_pCameraRightVectorU, cameraRightRotationSpeed);
			D3DXVec3TransformNormal(&m_pCameraUpVectorV, &m_pCameraUpVectorV, &tempMatrix);
			D3DXVec3TransformNormal(&m_pCameraLookVectorN, &m_pCameraLookVectorN, &tempMatrix);
		}
		else //handle the case where we increment by less then the increment amount
		{
			cameraRightRotationSpeed = static_cast<FLOAT>(PI_OVER_SIX - tempAngleInRadians);
			
			if (fabs(PI_OVER_SIX - tempAngleInRadians) < DBL_EPSILON && cameraRightRotationSpeed > 0)
			{
				//just rotate the up and look camera axis vectors because the right axis is the rotation axis
				D3DXMatrixRotationAxis(&tempMatrix, &m_pCameraRightVectorU, cameraRightRotationSpeed);
				D3DXVec3TransformNormal(&m_pCameraUpVectorV, &m_pCameraUpVectorV, &tempMatrix);
				D3DXVec3TransformNormal(&m_pCameraLookVectorN, &m_pCameraLookVectorN, &tempMatrix);
			}
		}
	}

	//positive rotation about the camera x-axis
	if (g_pD3DGraphics->GetKeyboardState()[DIK_PGUP] & 0x80)
	{
		//get the scalar projection of the camera up vector onto the world y vector
		scalarValue = D3DXVec3Dot(&m_pCameraUpVectorV, &D3DXVECTOR3(0,1,0));
		//get the angle between vec1 and vec2
		tempAngleInRadians = acos(scalarValue);
		
		//is the y component of the N axis less then 0, m_pCameraLookVectorN.y < 0
		//limit the camera rotation
		if (m_pCameraLookVectorN.y < 0 || tempAngleInRadians + cameraRightRotationSpeed < PI_OVER_SIX)
		{
			//just rotate the up and look camera axis vectors because the right axis is the rotation axis
			D3DXMatrixRotationAxis(&tempMatrix, &m_pCameraRightVectorU, -cameraRightRotationSpeed);
			D3DXVec3TransformNormal(&m_pCameraUpVectorV, &m_pCameraUpVectorV, &tempMatrix);
			D3DXVec3TransformNormal(&m_pCameraLookVectorN, &m_pCameraLookVectorN, &tempMatrix);
		}
		else //handle the case where we increment by less then the increment amount
		{
			//implement epsilon checking of floats
			cameraRightRotationSpeed = static_cast<FLOAT>(PI_OVER_SIX - tempAngleInRadians);
			
			if (fabs(PI_OVER_SIX - tempAngleInRadians) < DBL_EPSILON && cameraRightRotationSpeed > 0)
			{
				//just rotate the up and look camera axis vectors because the right axis is the rotation axis
				D3DXMatrixRotationAxis(&tempMatrix, &m_pCameraRightVectorU, -cameraRightRotationSpeed);
				D3DXVec3TransformNormal(&m_pCameraUpVectorV, &m_pCameraUpVectorV, &tempMatrix);
				D3DXVec3TransformNormal(&m_pCameraLookVectorN, &m_pCameraLookVectorN, &tempMatrix);
			}
		}
	}

	if ((g_pD3DGraphics->GetKeyboardState()[DIK_LEFTARROW] & 0x80)
		&& (g_pD3DGraphics->GetKeyboardState()[DIK_RCONTROL] & 0x80))
	{
		//move the camera along its negative x-axis
		m_pCameraOriginVector += -0.01f * m_pCameraRightVectorU;
	}
	else if (g_pD3DGraphics->GetKeyboardState()[DIK_LEFTARROW] & 0x80)
	{
		//rotate the right, up and look camera axis vectors
		D3DXMatrixRotationY(&tempMatrix, -0.001f);
		D3DXVec3TransformNormal(&m_pCameraRightVectorU, &m_pCameraRightVectorU, &tempMatrix);
		D3DXVec3TransformNormal(&m_pCameraUpVectorV, &m_pCameraUpVectorV, &tempMatrix);
		D3DXVec3TransformNormal(&m_pCameraLookVectorN, &m_pCameraLookVectorN, &tempMatrix);
	}

	if ((g_pD3DGraphics->GetKeyboardState()[DIK_RIGHTARROW] & 0x80)
		&& (g_pD3DGraphics->GetKeyboardState()[DIK_RCONTROL] & 0x80))
	{
		//move the camera along its positive x-axis
		m_pCameraOriginVector += 0.01f * m_pCameraRightVectorU;
	}
	else if (g_pD3DGraphics->GetKeyboardState()[DIK_RIGHTARROW] & 0x80)
	{
		//rotate the right, up and look camera axis vectors
		D3DXMatrixRotationY(&tempMatrix, 0.001f);
		D3DXVec3TransformNormal(&m_pCameraRightVectorU, &m_pCameraRightVectorU, &tempMatrix);
		D3DXVec3TransformNormal(&m_pCameraUpVectorV, &m_pCameraUpVectorV, &tempMatrix);
		D3DXVec3TransformNormal(&m_pCameraLookVectorN, &m_pCameraLookVectorN, &tempMatrix);
	}

	if (g_pD3DGraphics->GetKeyboardState()[DIK_UPARROW] & 0x80)
	{
		//400px/1s * 1s/1000ms
		m_pCameraOriginVector += 0.05f * m_pCameraLookVectorN * static_cast<float>(deltaTime);
	}

	if (g_pD3DGraphics->GetKeyboardState()[DIK_DOWNARROW] & 0x80)
	{
		m_pCameraOriginVector += -0.05f * m_pCameraLookVectorN * static_cast<float>(deltaTime);
	}

	//keep the eye at the same height above the ground
	m_pCameraOriginVector.y = 2;

	D3DXVec3Normalize(&m_pCameraRightVectorU, &m_pCameraRightVectorU);
	D3DXVec3Normalize(&m_pCameraUpVectorV, &m_pCameraUpVectorV);
	D3DXVec3Normalize(&m_pCameraLookVectorN, &m_pCameraLookVectorN);

	//camera look at vector is 100 units along the n-axis of view space
	m_pCameraLookAtPosition = m_pCameraOriginVector + m_pCameraLookVectorN * 3;

	// setup the view/projection matrix
	SetupViewProjection();
}

/*
draw what is seen by the eye
*/
VOID Eye::Draw()
{
	// old FX value holders
	BOOL isFloorOld = FALSE;
	BOOL isMeshOld = FALSE;
	BOOL isBoundingBoxOld = FALSE;
	
	// new FX file booleans
	BOOL isMesh = TRUE;

	// save the old FX values
	HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetChooserHandle(), &isFloorOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMeshOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->GetValue(g_pD3DGraphics->GetIsBoundingBoxHandle(), &isBoundingBoxOld, sizeof(BOOL)));

	// set new FX variables
	HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMesh, sizeof(BOOL)));

	//draw the uv-axis of the camera to act as a cross hair
	Vertex vertices[4];
	vertices[0] = m_pCameraLookAtPosition;
	vertices[1] = m_pCameraLookAtPosition;
	vertices[2] = m_pCameraLookAtPosition;
	vertices[3] = m_pCameraLookAtPosition;
		
	vertices[0] += (FLOAT)-1/8 * m_pCameraRightVectorU;
	vertices[1] += (FLOAT)1/8 * m_pCameraRightVectorU;
	vertices[2] += (FLOAT)-1/8 * m_pCameraUpVectorV;
	vertices[3] += (FLOAT)1/8 * m_pCameraUpVectorV;
		
	// position the the model in local space
	HR(g_pD3DGraphics->GetFXInterface()->SetMatrix(
		g_pD3DGraphics->GetWorldViewProjectionHandle(),
		&(g_pD3DGraphics->GetWorldMatrix() * m_matView * m_matProjection))
	);
	D3DXMATRIX worldInverseTranspose;
	D3DXMatrixInverse(&worldInverseTranspose, 0, &g_pD3DGraphics->GetWorldMatrix());
	D3DXMatrixTranspose(&worldInverseTranspose, &worldInverseTranspose);
	HR(g_pD3DGraphics->GetFXInterface()->SetMatrix(
		g_pD3DGraphics->GetWorldInverseTransposeHandler(),
		&worldInverseTranspose)
	);
	HR(g_pD3DGraphics->GetFXInterface()->CommitChanges());
	HR(g_pD3DGraphics->GetD3DDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 2, vertices, sizeof(Vertex)));

	// restore the old FX values
	HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetChooserHandle(), &isFloorOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsMeshHandle(), &isMeshOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->SetValue(g_pD3DGraphics->GetIsBoundingBoxHandle(), &isBoundingBoxOld, sizeof(BOOL)));
	HR(g_pD3DGraphics->GetFXInterface()->CommitChanges());
}