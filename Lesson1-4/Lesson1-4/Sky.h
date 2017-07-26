#pragma once

#include "Sprite.h"
#include "Camera.h"

class Sky : public Box
{
	Camera *camera;
	float width, depth, thick;
	float height;
	D3DXVECTOR3 lastDir;

	virtual void setTransform(LPDIRECT3DDEVICE9 d3ddev) override;
	virtual void initBuffer(LPDIRECT3DDEVICE9 d3ddev) override;

public:
	using Box::~Box;

	virtual void init(
		LPDIRECT3DDEVICE9 d3ddev,
		const D3DXVECTOR3 &center, 
		Terrain *pTerrain,
		Camera *camera);
};