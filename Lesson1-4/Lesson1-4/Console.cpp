#include <cstdio>
#include "Console.h"
#include "Util.h"

void Console::init(LPDIRECT3DDEVICE9 d3ddev)
{
	HRESULT hr;
	HDC hDC;
	int nHeight;
	int nPointSize = 10;

	hDC = GetDC(NULL);
	nHeight = -(MulDiv(nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72));
	ReleaseDC(NULL, hDC);

	hr = D3DXCreateFont(d3ddev, nHeight, 0, FW_BOLD, 0, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"),
		&d3dfont);

	if (FAILED(hr))
		MessageBox(NULL, "Call to D3DXCreateFont failed!", "ERROR", MB_OK | MB_ICONEXCLAMATION);
}

void Console::render(LPDIRECT3DDEVICE9 d3ddev)
{
	RECT rect;
	SetRect(&rect, 5, 5, 0, 0);

	char buf[1024] = "0.0";
	getFpsStr(buf);
	buf[1023] = '\0';
	d3dfont->DrawText(NULL, buf, -1, &rect, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	SetRect(&rect, 5, 20, 0, 0);
	d3dfont->DrawText(NULL, "Use W/S/A/D to move camera",
		-1, &rect, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	SetRect(&rect, 5, 35, 0, 0);
	d3dfont->DrawText(NULL, "Use UP/DOWN/LEFT/RIGHT to yaw and pitch",
		-1, &rect, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	SetRect(&rect, 5, 50, 0, 0);
	d3dfont->DrawText(NULL, "Or left click mouse to start using mouse",
		-1, &rect, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	SetRect(&rect, 5, 65, 0, 0);
	d3dfont->DrawText(NULL, "Use Esc to exit",
		-1, &rect, DT_NOCLIP, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

void Console::getFpsStr(char *buf)
{
	sprintf(buf, "FPS: %.4f", fps);

	frameCnt++;
	tm += GameTime::getTimeElapsed();

	if (tm >= 100) {
		fps = (double)frameCnt / (double)tm * 1000;
		tm = 0;
		frameCnt = 0;
	}
}

/*
#define MAX_NUM_VERTICES 50*6

inline FONT2DVERTEX InitFont2DVertex(const D3DXVECTOR4& p, D3DCOLOR color,
	FLOAT tu, FLOAT tv)
{
	FONT2DVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
	return v;
}

MyFont::MyFont(const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags)
{
	_tcsncpy(m_strFontName, strFontName, sizeof(m_strFontName) / sizeof(TCHAR));
	m_strFontName[sizeof(m_strFontName) / sizeof(TCHAR) - 1] = _T('\0');
	m_dwFontHeight = dwHeight;
	m_dwFontFlags = dwFlags;

	d3ddev = NULL;
	m_pTexture = NULL;
	m_pVB = NULL;

	m_pStateBlockSaved = NULL;
	m_pStateBlockDrawText = NULL;
}

HRESULT MyFont::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	// Keep a local copy of the device
	d3ddev = pd3dDevice;

	// Establish the font and texture size
	m_fTextScale = 1.0f; // Draw fonts into texture without scaling

						 // Large fonts need larger textures
	if (m_dwFontHeight > 40)
		m_dwTexWidth = m_dwTexHeight = 1024;
	else if (m_dwFontHeight > 20)
		m_dwTexWidth = m_dwTexHeight = 512;
	else
		m_dwTexWidth = m_dwTexHeight = 256;

	// If requested texture is too big, use a smaller texture and smaller font,
	// and scale up when rendering.
	D3DCAPS9 d3dCaps;
	d3ddev->GetDeviceCaps(&d3dCaps);

	if (m_dwTexWidth > d3dCaps.MaxTextureWidth)
	{
		m_fTextScale = (FLOAT)d3dCaps.MaxTextureWidth / (FLOAT)m_dwTexWidth;
		m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
	}

	// Create a new texture for the font
	hr = d3ddev->CreateTexture(m_dwTexWidth, m_dwTexHeight, 1,
		0, D3DFMT_A4R4G4B4,
		D3DPOOL_MANAGED, &m_pTexture, 0);
	if (FAILED(hr))
		return hr;

	// Prepare to create a bitmap
	DWORD*      pBitmapBits;
	BITMAPINFO bmi;
	ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = (int)m_dwTexWidth;
	bmi.bmiHeader.biHeight = -(int)m_dwTexHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount = 32;

	// Create a DC and a bitmap for the font
	HDC     hDC = CreateCompatibleDC(NULL);
	HBITMAP hbmBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS,
		(void**)&pBitmapBits, NULL, 0);
	SetMapMode(hDC, MM_TEXT);

	// Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
	// antialiased font, but this is not guaranteed.
	INT nHeight = -MulDiv(m_dwFontHeight,
		(INT)(GetDeviceCaps(hDC, LOGPIXELSY) * m_fTextScale), 72);
	DWORD dwBold = (m_dwFontFlags&D3DFONT_BOLD) ? FW_BOLD : FW_NORMAL;
	DWORD dwItalic = (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE : FALSE;
	HFONT hFont = CreateFont(nHeight, 0, 0, 0, dwBold, dwItalic,
		FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		VARIABLE_PITCH, m_strFontName);
	if (NULL == hFont)
		return E_FAIL;

	SelectObject(hDC, hbmBitmap);
	SelectObject(hDC, hFont);

	// Set text properties
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, 0x00000000);
	SetTextAlign(hDC, TA_TOP);

	// Loop through all printable character and output them to the bitmap..
	// Meanwhile, keep track of the corresponding tex coords for each character.
	DWORD x = 0;
	DWORD y = 0;
	TCHAR str[2] = _T("x");
	SIZE size;

	for (TCHAR c = 32; c<127; c++)
	{
		str[0] = c;
		GetTextExtentPoint32(hDC, str, 1, &size);

		if ((DWORD)(x + size.cx + 1) > m_dwTexWidth)
		{
			x = 0;
			y += size.cy + 1;
		}

		ExtTextOut(hDC, x + 0, y + 0, ETO_OPAQUE, NULL, str, 1, NULL);

		m_fTexCoords[c - 32][0] = ((FLOAT)(x + 0)) / m_dwTexWidth;
		m_fTexCoords[c - 32][1] = ((FLOAT)(y + 0)) / m_dwTexHeight;
		m_fTexCoords[c - 32][2] = ((FLOAT)(x + 0 + size.cx)) / m_dwTexWidth;
		m_fTexCoords[c - 32][3] = ((FLOAT)(y + 0 + size.cy)) / m_dwTexHeight;

		x += size.cx + 1;
	}

	// Lock the surface and write the alpha values for the set pixels
	D3DLOCKED_RECT d3dlr;
	m_pTexture->LockRect(0, &d3dlr, 0, 0);
	BYTE* pDstRow = (BYTE*)d3dlr.pBits;
	WORD* pDst16;
	BYTE bAlpha; // 4-bit measure of pixel intensity

	for (y = 0; y < m_dwTexHeight; y++)
	{
		pDst16 = (WORD*)pDstRow;
		for (x = 0; x < m_dwTexWidth; x++)
		{
			bAlpha = (BYTE)((pBitmapBits[m_dwTexWidth*y + x] & 0xff) >> 4);
			if (bAlpha > 0)
			{
				*pDst16++ = (bAlpha << 12) | 0x0fff;
			}
			else
			{
				*pDst16++ = 0x0000;
			}
		}
		pDstRow += d3dlr.Pitch;
	}

	// Done updating texture, so clean up used objects
	m_pTexture->UnlockRect(0);
	DeleteObject(hbmBitmap);
	DeleteDC(hDC);
	DeleteObject(hFont);

	//HRESULT hr;

	// Create vertex buffer for the letters
	int vertexSize = sizeof(FONT2DVERTEX);
	hr = d3ddev->CreateVertexBuffer(MAX_NUM_VERTICES * vertexSize,
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
		D3DPOOL_DEFAULT, &m_pVB, 0);

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

MyFont::~MyFont()
{
	m_pTexture->Release();
	m_pVB->Release();
	m_pStateBlockSaved->Release();
	m_pStateBlockDrawText->Release();
	d3ddev = NULL;
}


HRESULT MyFont::RestoreDeviceObjects()
{
	//void restoreFontRs();
	//void restoreOtherRs();
	for (UINT which = 1; which < 2; ++which) {

		d3ddev->BeginStateBlock();
		d3ddev->SetTexture(0, m_pTexture);

		if (D3DFONT_ZENABLE & m_dwFontFlags)
			d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
		else
			d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);

		d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		d3ddev->SetRenderState(D3DRS_ALPHAREF, 0x08);
		d3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		d3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		d3ddev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_CLIPPING, TRUE);
		d3ddev->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		d3ddev->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		d3ddev->SetRenderState(D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
		d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		d3ddev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3ddev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		d3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		d3ddev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		d3ddev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		d3ddev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		d3ddev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		d3ddev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		if ( which == 0 )
			d3ddev->EndStateBlock(&m_pStateBlockSaved);
		else
			d3ddev->EndStateBlock(&m_pStateBlockDrawText);
	}

	d3ddev->BeginStateBlock();

	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3ddev->EndStateBlock(&m_pStateBlockSaved);

	return S_OK;
}

void MyFont::restoreFontRs()
{
	
}

void MyFont::restoreOtherRs()
{
	d3ddev->BeginStateBlock();

	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3ddev->EndStateBlock(&m_pStateBlockSaved);
}

HRESULT MyFont::DrawText(FLOAT sx, FLOAT sy, DWORD dwColor,
	const TCHAR* strText, DWORD dwFlags)
{
	if (d3ddev == NULL)
		return E_FAIL;

	// Setup renderstate
	m_pStateBlockSaved->Capture();
	m_pStateBlockDrawText->Apply();
	d3ddev->SetFVF(D3DFVF_FONT2DVERTEX);
	d3ddev->SetPixelShader(NULL);
	d3ddev->SetStreamSource(0, m_pVB, 0, sizeof(FONT2DVERTEX));

	// Set filter states
	if (dwFlags & D3DFONT_FILTERED)
	{
		d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	FLOAT fStartX = sx;

	// Fill vertex buffer
	FONT2DVERTEX* pVertices = NULL;
	DWORD         dwNumTriangles = 0;
	m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	while (*strText)
	{
		TCHAR c = *strText++;

		if (c == _T('\n'))
		{
			sx = fStartX;
			sy += (m_fTexCoords[0][3] - m_fTexCoords[0][1])*m_dwTexHeight;
		}

		if ((c - 32) < 0 || (c - 32) >= 128 - 32)
			continue;

		FLOAT tx1 = m_fTexCoords[c - 32][0];
		FLOAT ty1 = m_fTexCoords[c - 32][1];
		FLOAT tx2 = m_fTexCoords[c - 32][2];
		FLOAT ty2 = m_fTexCoords[c - 32][3];

		FLOAT w = (tx2 - tx1) *  m_dwTexWidth / m_fTextScale;
		FLOAT h = (ty2 - ty1) * m_dwTexHeight / m_fTextScale;

		if (c != _T(' '))
		{
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + h - 0.5f, 0.9f, 1.0f), dwColor, tx1, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + 0 - 0.5f, 0.9f, 1.0f), dwColor, tx1, ty1);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + h - 0.5f, 0.9f, 1.0f), dwColor, tx2, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + 0 - 0.5f, 0.9f, 1.0f), dwColor, tx2, ty1);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + h - 0.5f, 0.9f, 1.0f), dwColor, tx2, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + 0 - 0.5f, 0.9f, 1.0f), dwColor, tx1, ty1);
			dwNumTriangles += 2;

			if (dwNumTriangles * 3 > (MAX_NUM_VERTICES - 6))
			{
				// Unlock, render, and relock the vertex buffer
				m_pVB->Unlock();
				d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);
				pVertices = NULL;
				m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);
				dwNumTriangles = 0L;
			}
		}

		sx += w;
	}

	// Unlock and render the vertex buffer
	m_pVB->Unlock();
	if (dwNumTriangles > 0)
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);

	// Restore the modified renderstates
	m_pStateBlockSaved->Apply();

	return S_OK;
}*/
