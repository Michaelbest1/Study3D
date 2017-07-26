#include <fstream>
#include <d3d9.h>
#include <cstdio>

#include "Terrain.h"
#include "Util.h"

const DWORD Terrain::TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

void Terrain::init(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 *pLight, const std::string &filename)
{
	if (!readRawFile(filename)) return;

	if (!computeVertices(d3ddev)) {
		::MessageBox(0, "computeVertices failed!", 0, 0);
		PostQuitMessage(0);
		return;
	}

	if (!computeIndices(d3ddev)) {
		::MessageBox(0, "computeIndices failed!", 0, 0);
		PostQuitMessage(0);
		return;
	}

	if (!genTexture(d3ddev, pLight)) {
		::MessageBox(0, "genTexture failed!", 0, 0);
		PostQuitMessage(0);
		return;
	}
}

void Terrain::render(LPDIRECT3DDEVICE9 d3ddev)
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	d3ddev->SetTransform(D3DTS_WORLD, &world);

	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(TerrainVertex));
	d3ddev->SetFVF(TerrainVertex::FVF);
	d3ddev->SetIndices(i_buffer);
	d3ddev->SetTexture(0, texture);

	d3ddev->SetRenderState(D3DRS_LIGHTING, false);
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numTriangles);
	d3ddev->SetRenderState(D3DRS_LIGHTING, true);

	d3ddev->SetTexture(0, NULL);
}

bool Terrain::readRawFile(const std::string &filename)
{
	std::vector<BYTE> in(numVertices);
	std::ifstream inFile(filename.c_str(), std::ios_base::binary);

	if (! inFile.is_open()) {
		::MessageBox(0, "Open raw file failed!", 0, 0);
		PostQuitMessage(0);
		return false;
	}

	inFile.read((char*)&in[0], in.size());
	inFile.close();

	heightMap.resize(numVertices);

	for (int i = 0; i < in.size(); ++i)
		heightMap[i] = in[i];

	return true;
}

int Terrain::getHeightMapEntry(int row, int col)
{
	return heightMap[row * numVertsPerRow + col];
}


void Terrain::setHeightMapEntry(int row, int col, int value)
{
	heightMap[row * numVertsPerRow + col] = value;
}

bool Terrain::computeVertices(LPDIRECT3DDEVICE9 d3ddev)
{
	HRESULT hr = d3ddev->CreateVertexBuffer(
		numVertices * sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		0);

	if (FAILED(hr)) return false;

	INT32 startX = -(INT32)(width) / 2;
	INT32 startZ = depth / 2;

	INT32 endX = width / 2;
	INT32 endZ = - (INT32)(depth) / 2;

	float uCoordIncSize = 1.0f / (float)numCellsPerRow;
	float vCoordIncSize = 1.0f / (float)numCellsPerCol;

	TerrainVertex *pv = NULL;
	v_buffer->Lock(0, 0, (void**)&pv, 0);

	int i = 0;
	for (int z = startZ; z >= endZ; z -= cellSpacing) {
		int j = 0;
		for (int x = startX; x <= endX; x += cellSpacing) {
			int index = i * numVertsPerRow + j;
			pv[index] = TerrainVertex(
				(float)x,
				(float)heightMap[index],
				(float)z,
				(float)j * uCoordIncSize,
				(float)i * vCoordIncSize
				);
			++j;
		}
		++i;
	}

	v_buffer->Unlock();
	return true;
}

bool Terrain::computeIndices(LPDIRECT3DDEVICE9 d3ddev)
{
	HRESULT hr = d3ddev->CreateIndexBuffer(
		numTriangles * 3 * sizeof(DWORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer,
		0
		);
	if (FAILED(hr)) return false;

	WORD* pi = NULL;
	i_buffer->Lock(0, 0, (void**)&pi, 0);

	int baseIndex = 0;

	for (int i = 0; i < numCellsPerCol; ++i) {
		for (int j = 0; j < numCellsPerRow; ++j) {
			pi[baseIndex] = i * numVertsPerRow + j;
			pi[baseIndex + 1] = i * numVertsPerRow + j + 1;
			pi[baseIndex + 2] = (i+1) * numVertsPerRow + j;

			pi[baseIndex + 3] = (i+1) * numVertsPerRow + j;
			pi[baseIndex + 4] = i * numVertsPerRow + j + 1;
			pi[baseIndex + 5] = (i+1) * numVertsPerRow + j + 1;

			baseIndex += 6;
		}
	}

	i_buffer->Unlock();
	return true;
}

bool Terrain::genTexture(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 *pLight)
{
	INT32 texWidth = numCellsPerRow;
	INT32 texHeight = numCellsPerCol;

	HRESULT hr = D3DXCreateTexture(
		d3ddev,
		texWidth, texHeight,
		0, 0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,
		&texture
		);

	if (FAILED(hr)) return false;

	D3DSURFACE_DESC textureDesc;
	texture->GetLevelDesc(0, &textureDesc);

	if (textureDesc.Format != D3DFMT_X8R8G8B8) return false;

	D3DLOCKED_RECT lockedRect;
	texture->LockRect(0, &lockedRect, 0, 0);

	DWORD *imgData = (DWORD*)lockedRect.pBits;
	for (int i = 0; i < texHeight; ++i) {
		for (int j = 0; j < texWidth; ++j) {
			//float height = (float)getHeightMapEntry(i, j) / heightScale;
			D3DXCOLOR c( D3DCOLOR_XRGB(255, 255, 255) );
			imgData[i * lockedRect.Pitch / 4 + j] = (D3DCOLOR)c;
		}
	}

	texture->UnlockRect(0);

	if (!lightTerrain(pLight)) {
		::MessageBox(0, "lightTerrain() failed!", 0, 0);
		return false;
	}

	hr = D3DXFilterTexture(texture, 0, 0, D3DX_DEFAULT);

	if (FAILED(hr)) {
		::MessageBox(0, "D3DXFilterTexture failed!", 0, 0);
		return false;
	}

	return true;
}

bool Terrain::lightTerrain(D3DXVECTOR3* pLight)
{
	D3DSURFACE_DESC textureDesc;
	texture->GetLevelDesc(0, &textureDesc);

	if (textureDesc.Format != D3DFMT_X8R8G8B8) return false;

	D3DLOCKED_RECT lockedRect;
	texture->LockRect(0, &lockedRect, 0, 0);

	DWORD *imgData = (DWORD*)lockedRect.pBits;
	for (int i = 0; i < textureDesc.Height; ++i) {
		for (int j = 0; j < textureDesc.Width; ++j) {
			int index = i * lockedRect.Pitch / 4 + j;
			D3DXCOLOR c( imgData[index] );
			float shade = computeShade(i, j, pLight);
			c *= shade;
			imgData[index] = (D3DCOLOR)c;
		}
	}

	texture->UnlockRect(0);
	return true;
}

/*
light must be a normal.
*/
float Terrain::computeShade(int cellRow, int cellCol, D3DXVECTOR3* pLight)
{
	float ha = getHeightMapEntry(cellRow, cellCol);
	float hb = getHeightMapEntry(cellRow, cellCol + 1);
	float hc = getHeightMapEntry(cellRow + 1, cellCol);

	D3DXVECTOR3 u(cellSpacing, hb - ha, 0.0f);
	D3DXVECTOR3 v(0.0f, hc - ha, -cellSpacing);

	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	float cosine = D3DXVec3Dot(&n, pLight);

	if (cosine < 0.0f) cosine = 0.0f;

	return cosine;
}


float Terrain::getHeight(float x, float z)
{
	x = ((float)width / 2.0f) + x;
	z = ((float)depth / 2.0f) - z;

	x /= (float)cellSpacing;
	z /= (float)cellSpacing;

	float col = ::floorf(x);
	float row = ::floorf(z);

	float A = getHeightMapEntry(row, col);
	float B = getHeightMapEntry(row, col + 1);
	float C = getHeightMapEntry(row + 1, col);
	float D = getHeightMapEntry(row + 1, col + 1);

	float dx = x - col;
	float dz = z - row;

	float height = 0.0f;
	if (dz < 1.0f - dx) {
		//upper triangle ABC
		float uy = B - A;
		float vy = C - A;
		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else {
		float uy = C - D;
		float vy = B - D;
		height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}
	return height;
}