#pragma once
#include "UnitBase.h"
#include "Fbx.h"


class Player :	public UnitBase
{
	Fbx model;
	float axisX, axisY;
	POINT prevMousePos;


public:
	Player();
	~Player();
	HRESULT Load();
	HRESULT Update();
	HRESULT Hit(UnitBase* pTarget);
	HRESULT Render();

};

