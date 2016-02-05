/*===========================================================================================
概　略：各ゲームユニットの親クラス
作成日：
更新日：
制作者：
=============================================================================================*/
#include "Fbx.h"


Fbx::Fbx()
{
	vertexCount = 0;
	polygonCount = 0;
	indexCount = 0;
	materialCount = 0;
	pVertexBuffer = NULL;
	pIndexBuffer = NULL;
	pMaterial = NULL;
	polygonCountOfMaterial = NULL;
	pTexture = NULL;
	D3DXMatrixIdentity(&lastWorldMatrix);
}


Fbx::~Fbx()
{
	for (int i = 0; i < materialCount; i++)
	{
		SAFE_RELEASE(pIndexBuffer[i]);
	}
	SAFE_DELETE_ARRAY(pIndexBuffer);
	SAFE_DELETE_ARRAY(polygonCountOfMaterial);

	SAFE_RELEASE(pVertexBuffer);
	SAFE_DELETE_ARRAY(pMaterial);
	SAFE_DELETE_ARRAY(pTexture);
	manager->Destroy();
}

HRESULT Fbx::Load(char* fileName)
{
	manager = FbxManager::Create();
	importer = FbxImporter::Create(manager, "");
	scene = FbxScene::Create(manager, "");

	importer->Initialize(fileName);
	importer->Import(scene);
	importer->Destroy();

	char  defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	char    dir[MAX_PATH];
	_splitpath_s(fileName, NULL, 0, dir, MAX_PATH, NULL, 0, NULL, 0);
	SetCurrentDirectory(dir);


	FbxNode* rootNode = scene->GetRootNode();
	int childCount = rootNode->GetChildCount();

	for (int i = 0; i < childCount; i++)
	{
		CheckNode(rootNode->GetChild(i));
	}

	SetCurrentDirectory(defaultCurrentDir);

	return S_OK;
}

HRESULT Fbx::CheckNode(FbxNode* pNode)
{
	FbxNodeAttribute* attr = pNode->GetNodeAttribute();
	if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		//メッシュノードだった
		materialCount = pNode->GetMaterialCount();
		pMaterial = new D3DMATERIAL9[materialCount];
		pTexture = new LPDIRECT3DTEXTURE9[materialCount];

		for (int i = 0; i < materialCount; i++)
		{
			FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)pNode->GetMaterial(i);
			FbxDouble3  diffuse = pLambert->Diffuse;
			FbxDouble3  ambient = pLambert->Ambient;

			ZeroMemory(&pMaterial[i], sizeof(D3DMATERIAL9));

			pMaterial[i].Diffuse.r = (float)diffuse[0];
			pMaterial[i].Diffuse.g = (float)diffuse[1];
			pMaterial[i].Diffuse.b = (float)diffuse[2];
			pMaterial[i].Diffuse.a = 1.0f;

			pMaterial[i].Ambient.r = (float)ambient[0];
			pMaterial[i].Ambient.g = (float)ambient[1];
			pMaterial[i].Ambient.b = (float)ambient[2];
			pMaterial[i].Ambient.a = 1.0f;

			FbxProperty  lProperty = pNode->GetMaterial(i)->FindProperty(FbxSurfaceMaterial::sDiffuse);
			FbxFileTexture* textureFile = lProperty.GetSrcObject<FbxFileTexture>(0);

			if (textureFile == NULL)
			{
				pTexture[i] = NULL;
			}
			else
			{
				const char* textureFileName = textureFile->GetFileName();

				char    name[_MAX_FNAME];
				char    ext[_MAX_EXT];
				_splitpath_s(textureFileName, NULL, 0, NULL, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, name, 0, 0, 0, 0, D3DFMT_UNKNOWN,
					D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, NULL, NULL, NULL, &pTexture[i])))
				{
					MessageBox(0, "テクスチャの作成に失敗しました", name, MB_OK);
					return E_FAIL;
				}

			}

		}


		CheckMesh(pNode->GetMesh());

	}
	else
	{
		int childCount = pNode->GetChildCount();
		for (int i = 0; i < childCount; i++)
		{
			CheckNode(pNode->GetChild(i));
		}
	}
	return S_OK;
}


HRESULT  Fbx::CheckMesh(FbxMesh* pMesh)
{
	FbxVector4* pVertexPos = pMesh->GetControlPoints();

	vertexCount = pMesh->GetControlPointsCount();
	Vertex* pVertexList = new Vertex[vertexCount];

	for (int i = 0; vertexCount > i; i++)
	{
		pVertexList[i].pos.x = (float)pVertexPos[i][0];
		pVertexList[i].pos.y = (float)pVertexPos[i][1];
		pVertexList[i].pos.z = (float)pVertexPos[i][2];
	}

	FbxStringList uvSetName;
	pMesh->GetUVSetNames(uvSetName);
	int uvCount = uvSetName.GetCount();
	bool unMap = false;
	FbxVector2  uv;

	polygonCount = pMesh->GetPolygonCount();
	for (int i = 0; i < polygonCount; i++)
	{
		int   startIndex = pMesh->GetPolygonVertexIndex(i);
		for (int j = 0; j < 3; j++)
		{
			int index = pMesh->GetPolygonVertices()[startIndex + j];

			FbxVector4  Normal;
			pMesh->GetPolygonVertexNormal(i, j, Normal);
			pVertexList[index].normal = D3DXVECTOR3((float)Normal[0], (float)Normal[1], (float)Normal[2]);

			for (int k = 0; k < uvCount; k++)
			{
				FbxString name = uvSetName.GetStringAt(k);
				pMesh->GetPolygonVertexUV(i, j, name, uv, unMap);
				pVertexList[index].uv = D3DXVECTOR2((float)uv.mData[0], (float)(1.0 - uv.mData[1]));
			}
		}
	}

	if (FAILED(g_pDevice->CreateVertexBuffer(sizeof(Vertex)* vertexCount, 0, D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
		D3DPOOL_MANAGED, &pVertexBuffer, 0)))
	{
		MessageBox(0, "頂点バッファの作成に失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	Vertex *pVertexCopy;
	if (FAILED(pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0)))
	{
		MessageBox(0, "頂点バッファのロックに失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	memcpy(pVertexCopy, pVertexList, sizeof(Vertex)*vertexCount);

	if (FAILED(pVertexBuffer->Unlock()))
	{
		MessageBox(0, "頂点バッファのアンロックに失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	SAFE_DELETE_ARRAY(pVertexList);







	indexCount = pMesh->GetPolygonVertexCount();
	pIndexBuffer = new IDirect3DIndexBuffer9*[materialCount];
	polygonCountOfMaterial = new int[materialCount];


	for (int i = 0; i < materialCount; i++)
	{

		int* pIndexList = new int[indexCount];

		int count = 0;
		for (int polygon = 0; polygon < polygonCount; polygon++)
		{
			int materialID = pMesh->GetLayer(0)->GetMaterials()->GetIndexArray().GetAt(polygon);
			if (materialID == i)
			{
				for (int vertex = 0; vertex < 3; vertex++)
				{
					pIndexList[count++] = pMesh->GetPolygonVertex(polygon, vertex);
				}
			}
		}
		polygonCountOfMaterial[i] = count / 3;


		if (FAILED(g_pDevice->CreateIndexBuffer(sizeof(int)* indexCount, 0, D3DFMT_INDEX32,
			D3DPOOL_MANAGED, &pIndexBuffer[i], 0)))
		{
			MessageBox(0, "インデックスバッファの作成に失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		DWORD *pIndexCopy;
		if (FAILED(pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0)))
		{
			MessageBox(0, "インデックスバッファのロックに失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		memcpy(pIndexCopy, pIndexList, sizeof(int)* indexCount);

		if (FAILED(pIndexBuffer[i]->Unlock()))
		{
			MessageBox(0, "インデックスバッファのアンロックに失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		SAFE_DELETE_ARRAY(pIndexList);
	}

	return S_OK;
}


HRESULT Fbx::Draw(D3DXMATRIX* matWorld)
{
	if (FAILED(g_pDevice->SetTransform(D3DTS_WORLD, matWorld)))
	{
		MessageBox(0, "ワールド行列の設定に失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)))
	{
		MessageBox(0, "頂点ストリームの設定に失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(Vertex))))
	{
		MessageBox(0, "頂点バッファの設定に失敗しました", "Fbx", MB_OK);
		return E_FAIL;
	}

	for (int i = 0; i < materialCount; i++)
	{
		if (FAILED(g_pDevice->SetIndices(pIndexBuffer[i])))
		{
			MessageBox(0, "インデックスバッファの設定に失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		if (FAILED(g_pDevice->SetMaterial(&pMaterial[i])))
		{
			MessageBox(0, "マテリアルの設定に失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		if (FAILED(g_pDevice->SetTexture(0, pTexture[i])))
		{
			MessageBox(0, "テクスチャのセットに失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}

		if (FAILED(g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, indexCount, 0, polygonCountOfMaterial[i])))
		{
			MessageBox(0, "メッシュの描画に失敗しました", "Fbx", MB_OK);
			return E_FAIL;
		}
	}

	lastWorldMatrix = *matWorld;

	return S_OK;
}

BOOL Fbx::RayCast(RayCastData *data)
{
	data->dist = 99999.0f;
	data->hit = false;


	//頂点バッファにアクセスできるように
	Vertex *pVertexCopy;
	pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0);


	//マテリアル毎
	for (DWORD i = 0; i < materialCount; i++)
	{
		//インデックスバッファにアクセスできるように
		DWORD *pIndexCopy;
		pIndexBuffer[i]->Lock(0, 0, (void**)&pIndexCopy, 0);

		//そのマテリアルのポリゴン毎
		for (DWORD j = 0; j < polygonCountOfMaterial[i]; j++)
		{
			//3頂点分
			D3DXVECTOR3 ver[3];
			ver[0] = pVertexCopy[pIndexCopy[j * 3 + 0]].pos;
			ver[1] = pVertexCopy[pIndexCopy[j * 3 + 1]].pos;
			ver[2] = pVertexCopy[pIndexCopy[j * 3 + 2]].pos;

			D3DXVec3TransformCoord(&ver[0], &ver[0], &lastWorldMatrix);	
			D3DXVec3TransformCoord(&ver[1], &ver[1], &lastWorldMatrix);
			D3DXVec3TransformCoord(&ver[2], &ver[2], &lastWorldMatrix);

			//ぶつかってるかチェック
			float dist, u, v;
			
			D3DXVec3Normalize(&data->dir, &data->dir);///////////////////////※

			BOOL hit = D3DXIntersectTri(&ver[0], &ver[1], &ver[2], &data->start, &data->dir, &u, &v, &dist);

			//今まで一番近い？
			if (hit && dist < data->dist)
			{
				data->hit = true;
				data->dist = dist;

				D3DXVECTOR3 v1 = ver[1] - ver[0];
				D3DXVECTOR3 v2 = ver[2] - ver[0];
				D3DXVec3Cross(&data->normal, &v1, &v2);
				//正規化
				D3DXVec3Normalize(&data->normal, &data->normal);


				data->pos = ver[0] + u * (ver[1] - ver[0]) + v * (ver[2] - ver[0]);
			}
		}
		//インデックスバッファ使用終了
		pIndexBuffer[i]->Unlock();
	}

	//頂点バッファ使用終了
	pVertexBuffer->Unlock();

	return data->hit;
}