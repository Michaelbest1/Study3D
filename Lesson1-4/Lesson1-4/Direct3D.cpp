#include "Direct3D.h"
#include "Constants.h"

MyDirect3D::MyDirect3D(HWND _hWnd):hWnd(_hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	setLight();

	D3DXVECTOR3 terrainLight(0.0f, 1.0f, 0.0f);
	terrain = new Terrain(256, 256, 10, 5.0f);
	terrain->init(d3ddev, &terrainLight, "terrain.raw");

	D3DXVECTOR3 lookat(0, terrain->getHeight(0, 0), 0);
	camera = new Camera();
	camera->init(d3ddev, lookat);

	spriteMgr = new SpriteManager(d3ddev);
	spriteMgr->initSprites(lookat, terrain, camera);

	console = new Console();
	console->init(d3ddev);

	mousing = false;
}

MyDirect3D::~MyDirect3D()
{
	d3ddev->Release();
	d3d->Release();
	delete camera;
	delete terrain;
	delete console;
}

void MyDirect3D::renderFrame(void)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	camera->update(hWnd, d3ddev, mousing);

	d3ddev->BeginScene();
	terrain->render(d3ddev);
	spriteMgr->renderAll();
	console->render(d3ddev);
	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void MyDirect3D::setLight()
{
	D3DLIGHT9 light;
	lightDirection = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&lightDirection, &lightDirection);

	::ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 10.0f;
	light.Diffuse.g = 10.0f;
	light.Diffuse.b = 10.0f;
	light.Direction = lightDirection;
	light.Range = 1000.0f;
	d3ddev->SetLight(0, &light);
	d3ddev->LightEnable(0, TRUE);

}

void MyDirect3D::ShowCursor(bool b)
{
	d3ddev->ShowCursor(b);
}