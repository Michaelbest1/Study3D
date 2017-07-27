#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

#include "Terrain.h"
#include "Util.h"
#include "Automation.h"

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 text;
};

class Sprite
{
protected:
	UINT32 id;

	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;

	LPDIRECT3DTEXTURE9 texture = NULL;

	//All component of a sprite have a common world translation matrix.
	D3DXMATRIX worldTransM;
	D3DXMATRIX worldScaleM;

	D3DXVECTOR3 position;

public:
	Sprite(){}
	virtual ~Sprite() {}

	virtual void render(LPDIRECT3DDEVICE9 d3ddev) = 0;
	virtual void init(LPDIRECT3DDEVICE9 d3ddev, const D3DXVECTOR3 &center) = 0;
};

class StandingSnowman : public Sprite
{
protected:
	LPD3DXMESH headMesh;
	LPD3DXMESH lEyeMesh;
	LPD3DXMESH rEyeMesh;

	//LPDIRECT3DTEXTURE9 texture;
	LPDIRECT3DTEXTURE9 eyeText = NULL;
	LPDIRECT3DTEXTURE9 noseText = NULL;

	float headRadius = 2.5f;

	//void setHeadMaterial(LPDIRECT3DDEVICE9 d3ddev);
	void setBodyMaterial(LPDIRECT3DDEVICE9 d3ddev);
	void setBottomMaterial(LPDIRECT3DDEVICE9 d3ddev);

	void renderHead(LPDIRECT3DDEVICE9 d3ddev);
	void renderBody(LPDIRECT3DDEVICE9 d3ddev);

	void renderEye(LPDIRECT3DDEVICE9 d3ddev);
	void renderNose(LPDIRECT3DDEVICE9 d3ddev);

	virtual void setHeadTransform(LPDIRECT3DDEVICE9 d3ddev);
	virtual void setBodyTransform(LPDIRECT3DDEVICE9 d3ddev);
	virtual void setLeftEyeTransform(LPDIRECT3DDEVICE9 d3ddev);
	virtual void setRightEyeTransform(LPDIRECT3DDEVICE9 d3ddev);
	virtual void setNoseTransform(LPDIRECT3DDEVICE9 d3ddev);

public:
	static const UINT32 VERTEXSIZE;

	using Sprite::Sprite;
	virtual ~StandingSnowman()
	{
		headMesh->Release();
		lEyeMesh->Release();
		rEyeMesh->Release();

		texture->Release();
		eyeText->Release();
		noseText->Release();

		v_buffer->Release();
	}

	virtual void init(
		LPDIRECT3DDEVICE9 d3ddev,
		const D3DXVECTOR3 &center) override;

	virtual void render(LPDIRECT3DDEVICE9 d3ddev) override;

	virtual void initWorldTrans(const D3DXVECTOR3 &center);
};

class Snowman : public StandingSnowman, public Rotatable
{
public:
	using StandingSnowman::StandingSnowman;
	virtual ~Snowman() {
		StandingSnowman::~StandingSnowman();
	}

	virtual void initWorldTrans(const D3DXVECTOR3 &center) override ;
	virtual void setHeadTransform(LPDIRECT3DDEVICE9 d3ddev) override;
	virtual void setBodyTransform(LPDIRECT3DDEVICE9 d3ddev) override;
	virtual void setLeftEyeTransform(LPDIRECT3DDEVICE9 d3ddev) override;
	virtual void setRightEyeTransform(LPDIRECT3DDEVICE9 d3ddev) override;
	virtual void setNoseTransform(LPDIRECT3DDEVICE9 d3ddev) override;
};

class Box : public Sprite, public Rotatable
{
protected:
	UINT32 static const NUMTRIANGLE;

	float height;

	void setMaterial(LPDIRECT3DDEVICE9 d3ddev);

	virtual void initBuffer(LPDIRECT3DDEVICE9 d3ddev);
	virtual void setTransform(LPDIRECT3DDEVICE9 d3ddev);

public:
	using Sprite::Sprite;
	virtual ~Box() {
		texture->Release();
		v_buffer->Release();
	}

	virtual void init(
		LPDIRECT3DDEVICE9 d3ddev,
		const D3DXVECTOR3 &center) override;

	virtual void render(LPDIRECT3DDEVICE9 d3ddev) override;

	void getPosition(D3DXVECTOR3 *pos) { *pos = position; }
	float getHeight() const { return height; }
};
