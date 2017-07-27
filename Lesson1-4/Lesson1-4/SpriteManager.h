#pragma once

#include <d3d9.h>
#include <d3dx9math.h>
#include <vector>

#include "Sprite.h"
#include "Camera.h"

using namespace std;

class SpriteManager
{
	LPDIRECT3DDEVICE9 d3ddev;

	vector<Sprite *> spriteVec;

	Rotater *rotater;

public:
	SpriteManager(LPDIRECT3DDEVICE9 _d3ddev);
	~SpriteManager();

	void initSprites(const D3DXVECTOR3 &center, Terrain *pTerrain, Camera *camera);
	void renderAll();
	//void drawHypercraft();
};