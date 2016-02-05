#include "BasicMesh.h"

BasicMesh::BasicMesh()
{
	pMesh = NULL;
}

BasicMesh::~BasicMesh()
{
	SAFE_RELEASE(pMesh);
}

HRESULT BasicMesh::Create(DWORD type)
{
	HRESULT hr;
	switch (type)
	{
	case BMESH_BOX:
		hr = D3DXCreateBox(g_pDevice, 1, 1, 1, &pMesh, NULL);
		break;

	case BMESH_SPHERE:
		hr = D3DXCreateSphere(g_pDevice, 1, 24, 24, &pMesh, NULL);
		break;

	case BMESH_CYLINDER:
		hr = D3DXCreateCylinder(g_pDevice, 1, 1, 3, 24, 2, &pMesh, NULL);
		break;

	case BMESH_TORUS:
		hr = D3DXCreateTorus(g_pDevice, 0.5, 1, 12, 24, &pMesh, NULL);
		break;

	case BMESH_TEAPOT:
		hr = D3DXCreateTeapot(g_pDevice, &pMesh, NULL);
		break;
	}

	if (FAILED(hr))
	{
		MessageBox(0, "メッシュの作成に失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	InitMaterial();

	return S_OK;
}

HRESULT BasicMesh::InitMaterial()
{
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;

	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;

	return S_OK;
}


HRESULT BasicMesh::Draw(D3DXMATRIX* matWorld)
{
	if (FAILED(g_pDevice->SetTexture(0, NULL)))
	{
		MessageBox(0, "テクスチャのリセットに失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "ワールド行列の設定に失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetMaterial(&material)))
	{
		MessageBox(0, "マテリアルの設定に失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}

	if (FAILED(pMesh->DrawSubset(0)))
	{
		MessageBox(0, "メッシュの描画に失敗しました", "BasicMesh", MB_OK);
		return E_FAIL;
	}


	return S_OK;
}