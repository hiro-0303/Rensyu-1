#pragma once
#include "Global.h"

class Line
{
public:
	HRESULT Draw(D3DXVECTOR3 start, D3DXVECTOR3 end);
	HRESULT Draw(D3DXVECTOR3 start, D3DXVECTOR3 end, DWORD color);
};

