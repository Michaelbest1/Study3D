#include "Sprite.h"

void Snowman::initWorldTrans(const D3DXVECTOR3 &center)
{
	D3DXMatrixScaling(&worldScaleM, 1.0f, 1.0f, 1.0f);
	D3DXMatrixTranslation(&worldTransM, 0, 20, 0);
}

void Snowman::setBodyTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matScale;
	D3DXMATRIX matRot;
	D3DXMatrixScaling(&matScale, 7.5f, 7.5f, 7.5f);
	rotater->getCurrTransM(&matRot);
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale * worldTransM * matRot));
}

void Snowman::setHeadTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matScale;
	D3DXMATRIX matTrans;
	D3DXMATRIX matRot;

	D3DXMatrixScaling(&matScale, 5.0f, 5.0f, 5.0f);
	D3DXMatrixTranslation(&matTrans, 0.0f, 15.0f, 0.0f);
	rotater->getCurrTransM(&matRot);
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matTrans * worldTransM * matRot));
}

void Snowman::setLeftEyeTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMATRIX matRot;
	float scale = 1.0f;
	D3DXMatrixScaling(&matScale, scale, scale, scale);
	D3DXMatrixTranslation(&matTrans, -headRadius * 0.6, 15.0f + 2.0f, -headRadius - 1.0f);
	rotater->getCurrTransM(&matRot);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTrans * matScale * worldTransM * matRot));
}

void Snowman::setRightEyeTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMATRIX matRot;
	float scale = 1.0f;
	D3DXMatrixScaling(&matScale, scale, scale, scale);
	D3DXMatrixTranslation(&matTrans, headRadius * 0.6, 15.0f + 2.0f, -headRadius - 1.0f);
	rotater->getCurrTransM(&matRot);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTrans * matScale * worldTransM * matRot));
}

void Snowman::setNoseTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMATRIX matRot;
	D3DXMatrixScaling(&matScale, 0.7f, 1.5f, 1.0f);
	D3DXMatrixTranslation(&matTrans, 0, 15.0f, -headRadius - 2.0f);
	rotater->getCurrTransM(&matRot);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTrans * worldTransM * matRot));
}