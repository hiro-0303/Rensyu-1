#pragma once
#include "Global.h"

enum
{
	BMESH_BOX,
	BMESH_SPHERE,
	BMESH_CYLINDER,
	BMESH_TORUS,
	BMESH_TEAPOT
};

class BasicMesh
{
	LPD3DXMESH   pMesh;
	D3DMATERIAL9 material;

	HRESULT InitMaterial();
public:
	BasicMesh();
	~BasicMesh();
	HRESULT Create(DWORD type);
	HRESULT Draw(D3DXMATRIX* matWorld);

};

