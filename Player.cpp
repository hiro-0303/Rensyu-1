#include "Player.h"
#include "Camera.h"
Player::Player()
{
	axisX = 0;
	axisY = 0;
	
}


Player::~Player()
{
	//コメント！！！！

}

HRESULT Player::Load()
{
	model.Load("Assets\\wall.fbx");


	g_pInput->SetCursorCenter();		//カーソルを中央へ（コンストラクタで呼ぶと、まだウィンドウができてないのでエラーになる）
	GetCursorPos(&prevMousePos);	//カーソルの位置を覚えておく
	ShowCursor(FALSE);				//マウスカーソルを表示しない
	return S_OK;
}


HRESULT Player::Update()
{
	//マウスの現在地（ウィンドウ上の位置ではなく、ディスプレイ上の位置）
	POINT mousePos;
	GetCursorPos(&mousePos);


	//カメラを回転させる角度
	axisY += (float)(mousePos.x - prevMousePos.x) / 10;
	axisX += (float)(mousePos.y - prevMousePos.y) / 10;
	

	//制限（高さ方向は±30°）
	if (axisX < -30)	axisX = -30;
	if (axisX > 30)		axisX = 30;

	//カメラの位置変更
	D3DXVECTOR3 camVec(0, 5, -5);
	D3DXMATRIX matRotateX, matRotateY;
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(axisX));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(axisY));
	D3DXVec3TransformCoord(&camVec, &camVec, &(matRotateX * matRotateY));
	g_pCamera->SetPos(camVec);


	//マウスカーソルを中心へ
	g_pInput->SetCursorCenter();

	//その位置を記憶
	GetCursorPos(&prevMousePos);

	return S_OK;
}

HRESULT Player::Hit(UnitBase* pTarget)
{

	return S_OK;
}


HRESULT Player::Render()
{
	D3DXMATRIX matW;
	D3DXMatrixIdentity(&matW);
	model.Draw(&matW);
	return S_OK;
}

