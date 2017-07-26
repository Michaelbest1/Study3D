#include <cmath>
#include <D3dx9shape.h>
#include <iostream>

#include "Sprite.h"

UINT32 const Box::NUMTRIANGLE = 24;

void Box::init(LPDIRECT3DDEVICE9 d3ddev, const D3DXVECTOR3 &center)
{
	initBuffer(d3ddev);

	//Init texture
	HRESULT hr = D3DXCreateTextureFromFile(d3ddev, "box.jpg", &texture);
	if (FAILED(hr))
		MessageBox(NULL, "Load texture for box failed!!", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	height = 10;
	D3DXMatrixTranslation(&worldTransM, 0, height / 2 + 5, 0);
	position = D3DXVECTOR3(0, 0, 0);
	D3DXVec3TransformCoord(&position, &position, &worldTransM);
}

void Box::initBuffer(LPDIRECT3DDEVICE9 d3ddev)
{
	CUSTOMVERTEX vertices[] = {
		// Quad 0. front
		{ D3DXVECTOR3(-1.0f, 1.0f,-1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0, 0) }, // 0 (unique)
		{ D3DXVECTOR3(1.0f, 1.0f,-1.0f),  D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1, 0) }, // 1 (unique)
		{ D3DXVECTOR3(-1.0f,-1.0f,-1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0, 1) }, // 2 (unique)
		{ D3DXVECTOR3(1.0f,-1.0f,-1.0f),  D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1, 1) }, // 3 (unique)

																							   // Quad 1. back
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f),  D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1, 0) }, // 4 (unique)
		{ D3DXVECTOR3(-1.0f,-1.0f, 1.0f),  D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1, 1) }, // 5 (unique)
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f),  D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0, 0) }, // 6 (unique)
		{ D3DXVECTOR3(1.0f,-1.0f, 1.0f),  D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0, 1) }, // 7 (unique)

																							  // Quad 2. top
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f),  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0, 0) }, // 4 (start repeating here)
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f),  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1, 0) }, // 6 (repeat of vertex 6)
		{ D3DXVECTOR3(-1.0f, 1.0f,-1.0f),  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0, 1) }, // 0 (repeat of vertex 0... etc.)
		{ D3DXVECTOR3(1.0f, 1.0f,-1.0f),  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1, 1) }, // 1

																							  // Quad 3. bottom
		{ D3DXVECTOR3(-1.0f,-1.0f, 1.0f),  D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0, 0) }, // 5
		{ D3DXVECTOR3(-1.0f,-1.0f,-1.0f),  D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(1, 0) }, // 2
		{ D3DXVECTOR3(1.0f,-1.0f, 1.0f),  D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0, 1) }, // 7
		{ D3DXVECTOR3(1.0f,-1.0f,-1.0f),  D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(1, 1) }, // 3

																							   // Quad 4. right
		{ D3DXVECTOR3(1.0f, 1.0f,-1.0f),  D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0, 0) }, // 1
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f),  D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(1, 0) }, // 6
		{ D3DXVECTOR3(1.0f,-1.0f,-1.0f),  D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0, 1) }, // 3
		{ D3DXVECTOR3(1.0f,-1.0f, 1.0f),  D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(1, 1) }, // 7

																							  // Quad 5, left
		{ D3DXVECTOR3(-1.0f, 1.0f,-1.0f),  D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1, 0) }, // 0
		{ D3DXVECTOR3(-1.0f,-1.0f,-1.0f),  D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1, 1) }, // 2
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f),  D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0, 0) }, // 4
		{ D3DXVECTOR3(-1.0f,-1.0f, 1.0f),  D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0, 1) }  // 5
	};


	d3ddev->CreateVertexBuffer(NUMTRIANGLE * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();
}

void Box::render(LPDIRECT3DDEVICE9 d3ddev)
{
	//D3DXMatrixScaling(&matScale, 5.0f, 5.0f, 5.0f);
	//D3DXMatrixTranslation(&worldScaleM, 0.0f, 15.0f, 0.0f);

	/*d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(25, 25, 25, 0));
	d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	DWORD oldTexCoordIndex;
	d3ddev->GetTextureStageState(0, D3DTSS_TEXCOORDINDEX, &oldTexCoordIndex);
	d3ddev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
		D3DTSS_TCI_CAMERASPACENORMAL | 1);*/
    d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

	d3ddev->SetTexture(0, texture);
	setMaterial(d3ddev);
	setTransform(d3ddev);
	d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);

	d3ddev->SetTexture(0, NULL);
	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//d3ddev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, oldTexCoordIndex);
}

void Box::setMaterial(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	d3ddev->SetMaterial(&mtrl);
}

void Box::setTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matScale, matRotate;
	D3DXMatrixScaling(&matScale, height, height, height);
	rotater->getCurrTransM(&matRotate);
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale * worldTransM * matRotate));
}
