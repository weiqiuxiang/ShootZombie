#pragma once
#ifndef _CGAMEUI_H_
#define _CGAMEUI_H_

#include "main.h"
#include "obj2d.h"
#include "CNumber.h"

#define UIOBJ_NUM (20)

class CObj2D;
class CNumber;

class CGameUI
{
public:
	CGameUI();
	~CGameUI();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
private:
	CObj2D m_UIObj[UIOBJ_NUM];
	CBullet m_BulletNum[2];
	CBullet m_BulletStore[2];
	int m_nFrame;
};

#endif