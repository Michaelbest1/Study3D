#include "Sky.h"

void Sky::init(LPDIRECT3DDEVICE9 d3ddev, const D3DXVECTOR3 &center, Terrain *pTerrain, Camera *camera)
{
	this->camera = camera;
	initBuffer(d3ddev);

	//Init texture
	HRESULT hr = D3DXCreateTextureFromFile(d3ddev, "sky.jpg", &texture);
	if (FAILED(hr))
		MessageBox(NULL, "Load texture for sky failed!!", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	height = 800;
	width = pTerrain->getWidth();
	depth = pTerrain->getDepth();
	thick = center.y + height;

	//D3DXMatrixTranslation(&worldTransM, 0, thick / 2, 0);
	D3DXMatrixTranslation(&worldTransM, 0, center.y + 500, 0);
	position = D3DXVECTOR3(0, 0, 0);
	D3DXVec3TransformCoord(&position, &position, &worldTransM);

	lastDir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

void Sky::initBuffer(LPDIRECT3DDEVICE9 d3ddev)
{
	CUSTOMVERTEX vertices[] = {
		// Quad 0. front
		{ D3DXVECTOR3(-1.0f, 1.0f,-1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0, 0) }, // 0 (unique)
		{ D3DXVECTOR3(1.0f, 1.0f,-1.0f),  D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1, 0) }, // 1 (unique)
		{ D3DXVECTOR3(-1.0f,-1.0f,-1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0, 1) }, // 2 (unique)
		{ D3DXVECTOR3(1.0f,-1.0f,-1.0f),  D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1, 1) }, // 3 (unique)

																							   // Quad 1. back
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f),  D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0, 0) }, // 4 (unique)
		{ D3DXVECTOR3(-1.0f,-1.0f, 1.0f),  D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0, 1) }, // 5 (unique)
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f),  D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1, 0) }, // 6 (unique)
		{ D3DXVECTOR3(1.0f,-1.0f, 1.0f),  D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1, 1) }, // 7 (unique)

																							  // Quad 2. top
		{ D3DXVECTOR3(-1.0f, 1.0f, 1.0f),  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0, 0) }, // 4 (start repeating here)
		{ D3DXVECTOR3(1.0f, 1.0f, 1.0f),  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1, 0) }, // 6 (repeat of vertex 6)
		{ D3DXVECTOR3(-1.0f, 1.0f,-1.0f),  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0, 0.7) }, // 0 (repeat of vertex 0... etc.)
		{ D3DXVECTOR3(1.0f, 1.0f,-1.0f),  D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(1, 0.7) }, // 1

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

void Sky::setTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, width, thick, depth);

	D3DXVECTOR3 cameraDir;
	camera->getDirection(&cameraDir);
	cameraDir.y = 0;
	D3DXVec3Normalize(&cameraDir, &cameraDir);
	float cosine = D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), &cameraDir);
	float theta = acosf(cosine);
	D3DXMATRIX matRotate;

	if (cameraDir.x < 0)
		D3DXMatrixRotationY(&matRotate, -theta);
	else
		D3DXMatrixRotationY(&matRotate, theta);

	//D3DXMatrixScaling(&matScale, 100, 100, 100);
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matRotate * worldTransM));
	lastDir = cameraDir;
}