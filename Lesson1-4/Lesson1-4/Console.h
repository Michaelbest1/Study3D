#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <tchar.h>

class Console
{
	//MyFont *font;

	LPD3DXFONT d3dfont = NULL;
	double fps = 0;
	int frameCnt = 0;
	DWORD tm = 0;

	void getFpsStr(char *buf);

public:
	Console() {}
	~Console() {
		d3dfont->Release();
	}

	void init(
		LPDIRECT3DDEVICE9 d3ddev);

	void render(
		LPDIRECT3DDEVICE9 d3ddev);
};

/*(
// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004

// Font rendering flags
#define D3DFONT_CENTERED    0x0001
#define D3DFONT_TWOSIDED    0x0002
#define D3DFONT_FILTERED    0x0004

#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct FONT2DVERTEX
{ 
	D3DXVECTOR4 p;
	DWORD color;
	FLOAT tu, tv;
};

/*2D font
class MyFont
{
	TCHAR   m_strFontName[80];            // Font properties
	DWORD   m_dwFontHeight;
	DWORD   m_dwFontFlags;

	LPDIRECT3DDEVICE9 d3ddev;
	LPDIRECT3DSTATEBLOCK9 m_pStateBlockSaved;
	LPDIRECT3DSTATEBLOCK9 m_pStateBlockDrawText;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9      m_pTexture;

	DWORD   m_dwTexWidth;                 // Texture dimensions
	DWORD   m_dwTexHeight;
	FLOAT   m_fTextScale;
	FLOAT   m_fTexCoords[128 - 32][4];

	void restoreFontRs();

	void restoreOtherRs();

public:
	MyFont(const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags);
	~MyFont();

	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);

	HRESULT RestoreDeviceObjects();

	HRESULT DrawText(FLOAT sx, FLOAT sy, DWORD dwColor,
		const TCHAR* strText, DWORD dwFlags = 0L);
}; */
