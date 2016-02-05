/*===========================================================================================
概　略：プロジェクト全体で必要なものをまとめたファイル
作成日：
更新日：
制作者：
=============================================================================================*/
#pragma once

//---------------インクルード-----------------------
#include <windows.h>
#include <d3dx9.h>
#include "Input.h"

//----------ライブラリファイルのロード--------------
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


//-------------------定数宣言------------------
//ウィンドウサイズ
#define WINDOW_WIDTH	800	//ウィンドウの幅
#define WINDOW_HEIGHT	480		//ウィンドウ高さ

#define ACTOR_SIZE		60

//-------------------マクロ------------------
#define SAFE_DELETE(p)     {delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p) {delete[] (p);  (p) = NULL;}
#define SAFE_RELEASE(p) { if(p != NULL) { (p)->Release(); (p) = NULL; } }

//-------------------グローバル変数------------------
//シーン
enum GAME_SCENE
{
	SC_PLAY,	//プレイシーン
	SC_MAX
};
extern GAME_SCENE	g_gameScene;		//実態はGame.cppで宣言

//Direct3Dデバイスオブジェクト
extern LPDIRECT3DDEVICE9 g_pDevice;

//入力処理オブジェクト
extern Input* g_pInput;

//カメラオブジェクト
class Camera;
extern Camera* g_pCamera;
