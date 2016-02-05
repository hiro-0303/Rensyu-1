#include "Player.h"
#include "Camera.h"
Player::Player()
{
	axisX = 0;
	axisY = 0;
	
}


Player::~Player()
{
	//�R�����g�I�I�I�I

}

HRESULT Player::Load()
{
	model.Load("Assets\\wall.fbx");


	g_pInput->SetCursorCenter();		//�J�[�\���𒆉��ցi�R���X�g���N�^�ŌĂԂƁA�܂��E�B���h�E���ł��ĂȂ��̂ŃG���[�ɂȂ�j
	GetCursorPos(&prevMousePos);	//�J�[�\���̈ʒu���o���Ă���
	ShowCursor(FALSE);				//�}�E�X�J�[�\����\�����Ȃ�
	return S_OK;
}


HRESULT Player::Update()
{
	//�}�E�X�̌��ݒn�i�E�B���h�E��̈ʒu�ł͂Ȃ��A�f�B�X�v���C��̈ʒu�j
	POINT mousePos;
	GetCursorPos(&mousePos);


	//�J��������]������p�x
	axisY += (float)(mousePos.x - prevMousePos.x) / 10;
	axisX += (float)(mousePos.y - prevMousePos.y) / 10;
	

	//�����i���������́}30���j
	if (axisX < -30)	axisX = -30;
	if (axisX > 30)		axisX = 30;

	//�J�����̈ʒu�ύX
	D3DXVECTOR3 camVec(0, 5, -5);
	D3DXMATRIX matRotateX, matRotateY;
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(axisX));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(axisY));
	D3DXVec3TransformCoord(&camVec, &camVec, &(matRotateX * matRotateY));
	g_pCamera->SetPos(camVec);


	//�}�E�X�J�[�\���𒆐S��
	g_pInput->SetCursorCenter();

	//���̈ʒu���L��
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

