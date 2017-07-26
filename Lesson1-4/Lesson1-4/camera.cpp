//#include <DirectXMath.h>
#include <iostream>
#include "Camera.h"
#include "Constants.h"
#include "Direct3D.h"


using namespace std;

Camera::Camera()
{
	lastMousePos.x = lastMousePos.y = 0;
	currMousePos.x = currMousePos.y = 0;
}

Camera::~Camera()
{

}

void Camera::init(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 &lookat)
{
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	direction = D3DXVECTOR3(0.0f, -1.0f, 3.0f);
	D3DXVec3Normalize(&direction, &direction);
	D3DXVec3Cross(&up, &direction, &right);
	D3DXVec3Normalize(&up, &up);

	//up = D3DXVECTOR3(0.0f, 1.0f, 0);
	position = lookat - direction * 500;
	moveSpeed = 0.1f;
	rotateSpeed = 0.001f;

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&position,	//camera position
		&lookat,		//look-at position
		&up		//up direction
		);
	d3ddev->SetTransform(D3DTS_VIEW, & (matView));

	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT,
		1.0f,
		5000.0f);
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);
}

void Camera::getUserInput(HWND hWnd, bool mousing)
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(hWnd, &mousePos);
	
	/*If reaching bound, we do not change. */
	if (mousePos.x > 0 && mousePos.x < SCREEN_WIDTH 
		&& mousePos.y > 0 && mousePos.y < SCREEN_HEIGHT) {
		lastMousePos.x = currMousePos.x;
		lastMousePos.y = currMousePos.y;

		currMousePos.x = mousePos.x;
		currMousePos.y = mousePos.y;
	}

}

void Camera::update(HWND hWnd, LPDIRECT3DDEVICE9 d3ddev, bool mousing)
{
	getUserInput(hWnd, mousing);
	double deltaTm = GameTime::getTimeElapsed();

	if (mousing) {
		float xdif = currMousePos.x - lastMousePos.x;
		float ydif = currMousePos.y - lastMousePos.y;

		if (ydif != 0) {
			pitch(D3DXToRadian(ydif) * 10 * rotateSpeed * deltaTm);
		}
		if (xdif != 0) {
			yaw(D3DXToRadian(xdif) * 10 * rotateSpeed * deltaTm);
		}
	}

	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		pitch(-rotateSpeed * deltaTm);

	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		pitch(rotateSpeed * deltaTm);

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		yaw(-rotateSpeed * deltaTm);

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		yaw(rotateSpeed * deltaTm);

	if (::GetAsyncKeyState('W') & 0x8000f)
		walk(moveSpeed * deltaTm);

	if (::GetAsyncKeyState('S') & 0x8000f)
		walk(-moveSpeed * deltaTm);

	if (::GetAsyncKeyState('A') & 0x8000f)
		strafe(-moveSpeed * deltaTm);

	if (::GetAsyncKeyState('D') & 0x8000f)
		strafe(moveSpeed * deltaTm);

	if (::GetAsyncKeyState('R') & 0x8000f)
		fly(moveSpeed * deltaTm);

	if (::GetAsyncKeyState('F') & 0x8000f)
		fly(-moveSpeed * deltaTm);

	//View transformation
	D3DXMATRIX matView;
	getViewMatrix(matView);
	d3ddev->SetTransform(D3DTS_VIEW, &(matView));
	//matView = newView;
}

void Camera::pitch(float radian)
{
	D3DXMATRIX matRotation;
	D3DXMatrixRotationAxis(&matRotation, &right, radian);
	//D3DXVec3TransformCoord(&up, &up, &matRotation);
	D3DXVec3TransformCoord(&direction, &direction, &matRotation);

	D3DXVec3Normalize(&direction, &direction);
	D3DXVec3Cross(&up, &right, &direction);
	up = -up;
	D3DXVec3Normalize(&up, &up);
}

void Camera::yaw(float radian)
{
	D3DXMATRIX matRotation;

	D3DXMatrixRotationY(&matRotation, radian);
	D3DXVec3TransformCoord(&up, &up, &matRotation);
	D3DXVec3TransformCoord(&direction, &direction, &matRotation);

	D3DXVec3Normalize(&direction, &direction);
	D3DXVec3Normalize(&up, &up);
	D3DXVec3Cross(&right, &up, &direction);
	D3DXVec3Normalize(&right, &right);
}

void Camera::walk(float units)
{
	position += direction * units;
}

void Camera::strafe(float units)
{
	position += right * units;
}

void Camera::fly(float units)
{
	position += up * units;
}

void Camera::getViewMatrix(D3DXMATRIX &view)
{
	/*D3DXVec3Normalize(&direction, &direction);
	D3DXVec3Cross(&right, &up, &direction);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Cross(&up, &direction, &right);
	D3DXVec3Normalize(&up, &up);*/

	view._11 = right.x;
	view._12 = up.x;
	view._13 = direction.x;
	view._14 = 0.0f;

	view._21 = right.y;
	view._22 = up.y;
	view._23 = direction.y;
	view._24 = 0.0f;

	view._31 = right.z;
	view._32 = up.z;
	view._33 = direction.z;
	view._34 = 0.0f;

	view._41 = -D3DXVec3Dot(&right, &position);
	view._42 = -D3DXVec3Dot(&up, &position);
	view._43 = -D3DXVec3Dot(&direction, &position);
	view._44 = 1.0f;
}

void Camera::setPosition(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 &pos)
{
	D3DXMATRIX matView;
	position = pos;
	D3DXMatrixLookAtLH(&matView,
		&position,	//camera position
		&(position + direction),		//look-at position
		&up		//up direction
		);
	d3ddev->SetTransform(D3DTS_VIEW, &(matView));
}