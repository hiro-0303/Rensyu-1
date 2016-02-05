/*=======================================================================================================
概　略：ゲーム全体の流れを扱う処理
作成日：
更新日：
制作者：
=======================================================================================================*/

//---------------インクルード-----------------------
#include "Game.h"
#include "PlayScene.h"
#include "Camera.h"

//----------------グローバル変数----------------
GAME_SCENE			g_gameScene;//現在のゲームシーン
LPDIRECT3DDEVICE9	g_pDevice;  //Direct3Dデバイスオブジェクト
Input*          g_pInput;    //入力処理オブジェクト
Camera*			g_pCamera;

//――――――――――――――――――――――
//	コンストラクタ
//――――――――――――――――――――――
Game::Game()
{
	//各シーンオブジェクトの生成
	scene[SC_PLAY] =	new PlayScene;		//プレイシーン

	//最初はプレイルシーン
	g_gameScene = SC_PLAY;

	//カメラ
	g_pCamera = new Camera;
}

//――――――――――――――――――――――
//	デストラクタ
//――――――――――――――――――――――
Game::~Game()
{
	SAFE_DELETE(g_pCamera);

	for (int i = 0; i < SC_MAX; i++)
	{
		delete scene[i];
	}

	//Direct3D解放
	SAFE_RELEASE(g_pDevice);
	SAFE_RELEASE(pD3d);
}

//――――――――――――――――――――――
// Direct3D初期化
//――――――――――――――――――――――
HRESULT Game::InitD3d(HWND hWnd)
{
   // 「Direct3D」オブジェクトの作成
   if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
   {
      MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
      return E_FAIL;
   }

   // 「DIRECT3Dデバイス」オブジェクトの作成
   D3DPRESENT_PARAMETERS d3dpp;
   ZeroMemory(&d3dpp, sizeof(d3dpp));
   d3dpp.BackBufferFormat =			D3DFMT_UNKNOWN;
   d3dpp.BackBufferCount =			1;
   d3dpp.SwapEffect =				D3DSWAPEFFECT_DISCARD;
   d3dpp.Windowed =					TRUE;
   d3dpp.EnableAutoDepthStencil =	TRUE;
   d3dpp.AutoDepthStencilFormat =	D3DFMT_D16;
 
   if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
	   D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice)))
   {
	   MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);

	   if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
		   D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice)))
	   {
		   MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
		   return E_FAIL;
	   }
   }

   //if (FAILED(g_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
   //{
	  // MessageBox(0, "ライトの設定に失敗しました", "エラー", MB_OK);
	  // return E_FAIL;
   //}

   if (FAILED(g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
   {
	   MessageBox(0, "カリングモードの設定に失敗しました", "エラー", MB_OK);
	   return E_FAIL;
   }

   if (FAILED(g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE) ||
	   g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA) ||
	   g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))
   {
	   MessageBox(0, "アルファブレンドの設定に失敗しました", "エラー", MB_OK);
	   return E_FAIL;
   }

   if (FAILED(g_pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2)))
   {
	   MessageBox(0, "テクスチャステートの設定に失敗しました", "エラー", MB_OK);
	   return E_FAIL;
   }

   if (FAILED(InitLight()))
   {
	   return E_FAIL;
   }

   return S_OK;
}

//――――――――――――――――――――――
//	衝突判定
//――――――――――――――――――――――
HRESULT Game::Hit()
{
	if (FAILED(scene[g_gameScene]->Hit()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//――――――――――――――――――――――
// 読み込み処理
//――――――――――――――――――――――
HRESULT Game::Load()
{
	for (int i = 0; i < SC_MAX; i++)
	{
		if (FAILED(scene[i]->Load()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//――――――――――――――――――――――
//	更新処理
//――――――――――――――――――――――
HRESULT Game::Update()
{
	if (FAILED(g_pCamera->Update()))
	{
		return E_FAIL;
	}

	g_pInput->Update();
	
	if (FAILED(scene[g_gameScene]->Update()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//――――――――――――――――――――――
//	描画処理
//――――――――――――――――――――――
HRESULT Game::Render()
{
	//画面をクリア
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
									D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pDevice->BeginScene()))
	{
		//ゲーム画面の描画
		if (FAILED(scene[g_gameScene]->Render()))
		{
			return E_FAIL;
		}

		//描画終了
		g_pDevice->EndScene();
	}

	//フリップ
	g_pDevice->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}


HRESULT Game::InitLight()
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DXVECTOR3(0.5f, -0.5f, 1.0f);

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;

	light.Ambient.r = 0.4f;
	light.Ambient.g = 0.4f;
	light.Ambient.b = 0.4f;

	if (FAILED(g_pDevice->SetLight(0, &light)))
	{
		MessageBox(0, "ライトをセットできませんでした", "Game", MB_OK);
		return E_FAIL;
	}

	if (FAILED(g_pDevice->LightEnable(0, TRUE)))
	{
		MessageBox(0, "ライトを有効にできませんでした", "Game", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}