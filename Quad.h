#pragma once
#include "Global.h"

class Quad
{
	//テクスチャオブジェクト
	LPDIRECT3DTEXTURE9 pTexture;

public:
	Quad();
	~Quad();
	HRESULT Load(char* fileName);
	HRESULT Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex = NULL);
};

