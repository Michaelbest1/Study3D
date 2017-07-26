#pragma once

#include <d3d9.h>
#include <d3dx9math.h>

#include "Terrain.h"
#include "Util.h"

/*
Control the rotation of the snowman and box.
*/
class Rotater
{
	Terrain *pTerrain;
	D3DXVECTOR3 currDirection;
	float radius;
	float speed;
	//D3DXMATRIX M;
public:
	Rotater(Terrain *_pt) :pTerrain(_pt)
	{
		radius = 150;
		speed = 0.01;
		currDirection = D3DXVECTOR3(1, 0, 0);
	}

	void getCurrTransM(D3DXMATRIX *m);
};

class Rotatable
{
protected:
	Rotater *rotater;

public:
	void setRotater(Rotater *pr)
	{
		rotater = pr;
	}
};