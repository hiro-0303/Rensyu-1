/*===========================================================================================
概　略：各ゲームユニットの親クラス
作成日：
更新日：
制作者：
=============================================================================================*/
#pragma once
#include "Global.h"
#include <fbxsdk.h>

#pragma comment(lib,"libfbxsdk-mt.lib")

//レイキャスト用構造体
//レイキャストとは見えないビームのようなもの
struct RayCastData
{
	D3DXVECTOR3 start;	//レイ発射位置
	D3DXVECTOR3 dir;	//レイの向きベクトル
	float		dist;	//衝突点までの距離
	D3DXVECTOR3 normal;	//衝突点の法線
	D3DXVECTOR3 pos;	//衝突位置
	BOOL		hit;	//レイが当たったか
};

class Fbx
{
	struct Vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2  uv;
	};

	FbxManager*  manager;
	FbxImporter* importer;
	FbxScene*    scene;

	int			vertexCount;
	int			polygonCount;
	int			indexCount;
	int			materialCount;
	int*		polygonCountOfMaterial;

	IDirect3DVertexBuffer9   *pVertexBuffer;
	IDirect3DIndexBuffer9    **pIndexBuffer;
	D3DMATERIAL9			*pMaterial;
	LPDIRECT3DTEXTURE9       *pTexture;

	D3DXMATRIX lastWorldMatrix;

	HRESULT      CheckNode(FbxNode* pNode);
	HRESULT      CheckMesh(FbxMesh* pMesh);

public:
	Fbx();
	~Fbx();
	HRESULT Load(char* fileName);
	HRESULT Draw(D3DXMATRIX* matWorld);
	BOOL RayCast(RayCastData *data);
};

