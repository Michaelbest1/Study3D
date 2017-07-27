#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

class Camera
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 up;
	D3DXVECTOR3 right;

	float moveSpeed, rotateSpeed;

	D3DXVECTOR3 lastMousePos;
	D3DXVECTOR3 currMousePos;

	void getUserInput(HWND hWnd, bool mousing);
	void getViewMatrix(D3DXMATRIX &V);

	void pitch(float angle);
	void yaw(float angle);
	void walk(float units);
	void strafe(float units);
	void fly(float units);

public:
	Camera();
	~Camera();

	void init(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 &lookat);

	void update(HWND _hWnd, LPDIRECT3DDEVICE9 d3ddev, bool mousing);
	void setPosition(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 &pos);
	void getDirection(D3DXVECTOR3 *d) { *d = direction; }
};