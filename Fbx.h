/*===========================================================================================
�T�@���F�e�Q�[�����j�b�g�̐e�N���X
�쐬���F
�X�V���F
����ҁF
=============================================================================================*/
#pragma once
#include "Global.h"
#include <fbxsdk.h>

#pragma comment(lib,"libfbxsdk-mt.lib")

//���C�L���X�g�p�\����
//���C�L���X�g�Ƃ͌����Ȃ��r�[���̂悤�Ȃ���
struct RayCastData
{
	D3DXVECTOR3 start;	//���C���ˈʒu
	D3DXVECTOR3 dir;	//���C�̌����x�N�g��
	float		dist;	//�Փ˓_�܂ł̋���
	D3DXVECTOR3 normal;	//�Փ˓_�̖@��
	D3DXVECTOR3 pos;	//�Փˈʒu
	BOOL		hit;	//���C������������
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

