#include "Quad.h"


Quad::Quad()
{
	pTexture = NULL;
}


Quad::~Quad()
{
	SAFE_RELEASE(pTexture);
}

HRESULT Quad::Load(char* fileName)
{
	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, fileName, 0, 0, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture)))
	{
		MessageBox(0, "テクスチャの作成に失敗しました", fileName, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Quad::Draw(D3DXMATRIX* matWorld, D3DXMATRIX* matTex)
{
	struct Vertex
	{
		D3DXVECTOR3	pos;
		DWORD		color;
		D3DXVECTOR2	uv;
	};

	Vertex point[] =
	{
		D3DXVECTOR3(-0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0,0),      //左上
		D3DXVECTOR3(0.5, 0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1,0),       //右上
		D3DXVECTOR3(-0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(0, 1),    //左下
		D3DXVECTOR3(0.5, -0.5, 0), D3DCOLOR_ARGB(255, 255, 255, 255), D3DXVECTOR2(1, 1)      //右下
	};

	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)))
	{
		MessageBox(0, "頂点ストリームの設定に失敗しました", "Line", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "ワールド行列の設定に失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetTexture(0, pTexture)))
	{
		MessageBox(0, "テクスチャのセットに失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}

	if (matTex != NULL)
	{
		if (FAILED(g_pDevice->SetTransform(D3DTS_TEXTURE0, matTex)))
		{
			MessageBox(0, "テクスチャ行列の設定に失敗しました", "Quad", MB_OK);
			return E_FAIL;
		}
	}

	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
	{
		MessageBox(0, "ライトの設定に失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, point, sizeof(Vertex))))
	{
		MessageBox(0, "ポリゴンの描画に失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		MessageBox(0, "ライトの設定に失敗しました", "Quad", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}
