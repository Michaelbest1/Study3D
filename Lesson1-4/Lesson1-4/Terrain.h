#pragma once
#include <vector>
#include <string>
#include <windows.h>
#include <windowsx.h>
#include <d3dx9math.h>

class Terrain
{
	class TerrainVertex
	{
	public:
		static const DWORD FVF;
		float x, y, z;
		float u, v;
		TerrainVertex(float _x, float _y, float _z, float _u, float _v):
			x(_x), y(_y), z(_z), u(_u), v(_v)
		{}
	};

	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
	LPDIRECT3DINDEXBUFFER9  i_buffer = NULL;
	LPDIRECT3DTEXTURE9 texture;

	std::vector<int> heightMap;
	INT32 numVertsPerRow, numVertsPerCol;
	INT32 numCellsPerRow, numCellsPerCol;
	INT32 cellSpacing;
	float heightScale;

	INT32 numVertices;
	INT32 numTriangles;
	INT32 width, depth;

	bool readRawFile(const std::string &filename);
	int getHeightMapEntry(int row, int col);
	void setHeightMapEntry(int row, int col, int value);

	bool computeVertices(LPDIRECT3DDEVICE9 d3ddev);
	bool computeIndices(LPDIRECT3DDEVICE9 d3ddev);
	bool genTexture(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 *pLight);
	float computeShade(int cellRow, int cellCol, D3DXVECTOR3* pLight);
	bool lightTerrain(D3DXVECTOR3* pLight);

public:
	Terrain(INT32 _numVertsPerRow, INT32 _numVertsPerCol, INT32 _cellSpacing, float _heightScale)
		:numVertsPerRow(_numVertsPerRow), numVertsPerCol(_numVertsPerCol), 
		numVertices(_numVertsPerRow * _numVertsPerCol),
		numCellsPerRow(_numVertsPerRow - 1), numCellsPerCol(_numVertsPerCol - 1),
		cellSpacing(_cellSpacing), heightScale(_heightScale),
		width(numCellsPerRow * cellSpacing), depth(numCellsPerCol * cellSpacing),
		numTriangles(numCellsPerRow * numCellsPerCol * 2)
		{}

	~Terrain() {
		v_buffer->Release();
		i_buffer->Release();
		texture->Release();
	}

	void init(LPDIRECT3DDEVICE9 d3ddev, D3DXVECTOR3 *pLight, const std::string &filename);
	void render(LPDIRECT3DDEVICE9 d3ddev);
	float getHeight(float x, float z);

	float getWidth() { return width; }
	float getDepth() { return depth; }
};