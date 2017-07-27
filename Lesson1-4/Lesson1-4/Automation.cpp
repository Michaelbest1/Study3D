#include "Automation.h"

void Rotater::getCurrTransM(D3DXMATRIX *m)
{
	float deltaTm = GameTime::getTimeElapsed();
	D3DXMATRIX rotM;
	D3DXMatrixRotationY(&rotM, D3DXToRadian(speed * deltaTm));
	D3DXVec3TransformCoord(&currDirection, &currDirection, &rotM);
	D3DXVec3Normalize(&currDirection, &currDirection);
	float x = currDirection.x * radius;
	float z = currDirection.z * radius;
	float y = pTerrain->getHeight(x, z);
	D3DXMatrixTranslation(m, x, y, z);
}