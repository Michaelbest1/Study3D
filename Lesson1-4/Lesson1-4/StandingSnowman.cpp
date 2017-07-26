#include "Sprite.h"

const UINT32 StandingSnowman::VERTEXSIZE = 64;

void StandingSnowman::initWorldTrans(const D3DXVECTOR3 &center)
{
	D3DXMatrixScaling(&worldScaleM, 1.0f, 1.0f, 1.0f);
	D3DXMatrixTranslation(&worldTransM, center.x, center.y + 5, center.z);
}

void StandingSnowman::init( LPDIRECT3DDEVICE9 d3ddev, const D3DXVECTOR3 &center)
{
	initWorldTrans(center);

	CUSTOMVERTEX vertices[VERTEXSIZE];

	d3ddev->CreateVertexBuffer(VERTEXSIZE * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	//Init body
	vertices[0].position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	vertices[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[0].text = D3DXVECTOR2(0.0f, 0.5f);

	for (int i = 1; i < 32; ++i) {
		float theta = D3DXToRadian((i - 1) * 12);
		vertices[i].position = D3DXVECTOR3(sin(theta), 0.0f, cos(theta));
		vertices[i].normal = D3DXVECTOR3(sin(theta), 0.0f, cos(theta));
		float m = (float)sqrt(5);
		float beta = (float)((0.5 - 1.0 / m + (i - 1.0) / 15 / m) * D3DX_PI);
		vertices[i].text = D3DXVECTOR2(0.5 * sin(beta), 0.5 - 0.5*cos(beta));
	}

	vertices[32].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[32].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int i = 33; i < 64; ++i) {
		vertices[i].position = vertices[64 - i].position;
		vertices[i].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}

	VOID* pVoid;
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	//Init head.
	D3DXCreateSphere(d3ddev, 1.0f, 20, 20, &headMesh, NULL);
	D3DXCreateSphere(d3ddev, 1.0f, 20, 20, &lEyeMesh, NULL);
	D3DXCreateSphere(d3ddev, 1.0f, 20, 20, &rEyeMesh, NULL);

	//Init texture
	HRESULT hr = D3DXCreateTextureFromFile(d3ddev, "snow2.jpg", &texture);
	if (FAILED(hr))
		MessageBox(NULL, "Load texture failed!!", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	hr = D3DXCreateTextureFromFile(d3ddev, "black.jpg", &eyeText);
	if (FAILED(hr))
		MessageBox(NULL, "Load texture failed!!", "ERROR", MB_OK | MB_ICONEXCLAMATION);

	hr = D3DXCreateTextureFromFile(d3ddev, "carrot.jpg", &noseText);
	if (FAILED(hr))
		MessageBox(NULL, "Load texture failed!!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
}



void StandingSnowman::setBottomMaterial(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	d3ddev->SetMaterial(&mtrl);
}

void StandingSnowman::setBodyMaterial(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 2.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 2.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 2.0f;
	d3ddev->SetMaterial(&mtrl);
}

void StandingSnowman::setBodyTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 7.5f, 7.5f, 7.5f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale * worldTransM));
}

void StandingSnowman::setHeadTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matScale;
	D3DXMATRIX matTrans;
	float scale = headRadius * 2;

	D3DXMatrixScaling(&matScale, scale, scale, scale);
	D3DXMatrixTranslation(&matTrans, 0.0f, 15.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matTrans * worldTransM));
}

void StandingSnowman::setLeftEyeTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	float scale = 1.0f;
	D3DXMatrixScaling(&matScale, scale, scale, scale);
	/*float radx = D3DXToRadian(-20);
	float rady = D3DXToRadian(45);
	float radz = D3DXToRadian(-135);
	D3DXMatrixTranslation(&matTrans, sinf(radx) * headRadius, 15.0f + cosf(rady) * headRadius, sinf(radz) * headRadius);*/
	D3DXMatrixTranslation(&matTrans, -headRadius * 0.6, 15.0f + 2.0f, -headRadius - 1.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTrans * matScale * worldTransM));
}

void StandingSnowman::setRightEyeTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	float scale = 1.0f;
	D3DXMatrixScaling(&matScale, scale, scale, scale);
	D3DXMatrixTranslation(&matTrans, headRadius * 0.6, 15.0f + 2.0f, -headRadius - 1.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTrans * matScale * worldTransM));
}

void StandingSnowman::setNoseTransform(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.7f, 1.5f, 1.0f);
	D3DXMatrixTranslation(&matTrans, 0, 15.0f, -headRadius - 2.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTrans * worldTransM));
}

void StandingSnowman::renderHead(LPDIRECT3DDEVICE9 d3ddev)
{
	setHeadTransform(d3ddev);
	headMesh->DrawSubset(0);
}

void StandingSnowman::renderEye(LPDIRECT3DDEVICE9 d3ddev)
{
	d3ddev->SetTexture(0, eyeText);
	setLeftEyeTransform(d3ddev);
	lEyeMesh->DrawSubset(0);

	setRightEyeTransform(d3ddev);
	rEyeMesh->DrawSubset(0);
}

void StandingSnowman::renderNose(LPDIRECT3DDEVICE9 d3ddev)
{
	setBodyMaterial(d3ddev);
	setNoseTransform(d3ddev);
	d3ddev->SetTexture(0, noseText);

	d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 30);

}

void StandingSnowman::renderBody(LPDIRECT3DDEVICE9 d3ddev)
{
	setBodyTransform(d3ddev);
	setBodyMaterial(d3ddev);

	d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 30);

	setBottomMaterial(d3ddev);
	d3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN, 30, 30);

}
void StandingSnowman::render(LPDIRECT3DDEVICE9 d3ddev)
{
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(25, 25, 25, 0));
	d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	DWORD oldTexCoordIndex;
	d3ddev->GetTextureStageState(0, D3DTSS_TEXCOORDINDEX, &oldTexCoordIndex);
	d3ddev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
		D3DTSS_TCI_CAMERASPACENORMAL | 1);

	d3ddev->SetTexture(0, texture);
	//Render body
	renderBody(d3ddev);

	//Render head
	renderHead(d3ddev);

	renderEye(d3ddev);

	renderNose(d3ddev);

	//Do some restore
	d3ddev->SetTexture(0, NULL);
	d3ddev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, oldTexCoordIndex);

}
