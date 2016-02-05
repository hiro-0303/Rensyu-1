#pragma once

#include "Global.h"


//画像の表示に使うデータ
struct SpriteData
{
	D3DXVECTOR3 pos;      //位置
	D3DXVECTOR2 size;     //サイズ
	D3DXVECTOR2 cut;      //切り抜き位置
	D3DXVECTOR2 center;   //基準点
	D3DXVECTOR2 scale;    //拡大率
	float		angle;	  //角度

	//色
	struct
	{
		int a, r, g, b;
	}color;

	//コンストラクタ
	SpriteData()
	{
		pos =	D3DXVECTOR3(0, 0, 0);
		size =	D3DXVECTOR2(-999, -999);
		cut =	D3DXVECTOR2(0, 0);
		center = D3DXVECTOR2(-999, -999);
		color = { 255, 255, 255, 255 };
		scale = D3DXVECTOR2(1, 1);
		angle = 0;
	}
};


class Sprite
{
	//スプライトオブジェクト
	LPD3DXSPRITE pSprite;

	//テクスチャオブジェクト
	LPDIRECT3DTEXTURE9 pTexture;

	//テクスチャのサイズ
	D3DXVECTOR2  texSize;

public:
	Sprite();
	~Sprite();

	HRESULT Load(char* fileName);
	void Draw(SpriteData* data);
};