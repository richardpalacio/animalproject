#include "visualsystem.h"

VisualSystem::VisualSystem()
{
}

VisualSystem::~VisualSystem()
{
}

VOID VisualSystem::Update(DOUBLE deltaTime, BOOL currentCamera)
{
	if (currentCamera)
	{
		m_pEye.Update(deltaTime);
	}
}

VOID VisualSystem::Draw()
{
	m_pEye.Draw();
}