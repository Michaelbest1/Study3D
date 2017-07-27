#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9math.h>

#include "SpriteManager.h"
#include "Camera.h"
#include "Terrain.h"
#include "Console.h"

class MyDirect3D
{
	//GLobal declarations
	HWND hWnd;
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;

	//Game component.
	SpriteManager *spriteMgr;
	Camera *camera;
	Terrain *terrain;
	Console *console;

	bool mousing;
	D3DXVECTOR3 lightDirection;

public:
	MyDirect3D(HWND hWnd);
	~MyDirect3D();

	//void setCamera();
	//void setProjection();
	void setLight();

	void renderFrame();

	void ShowCursor(bool);

	void setMousing(bool b) { mousing = b; }
	bool getMousing() const { return mousing; }
};
