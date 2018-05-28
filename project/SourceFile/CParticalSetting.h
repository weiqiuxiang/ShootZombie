#pragma once

#ifndef _CPSETTING_H_
#define _CPSETTING_H_

#include "main.h"

class CPSetting
{
public:
	CPSetting();
	~CPSetting();
	
	virtual void Init(const D3DXVECTOR3 *pos);
	virtual void Update(void) = 0;
	void SetUse(bool bUse);
	bool GetUse(void);
protected:
	D3DXVECTOR3 m_pos;
	int m_nFrame;
	bool m_bUse;
};

class CSpawn : public CPSetting
{
public:
	CSpawn();
	~CSpawn();

	void Init(const D3DXVECTOR3 *pos,float fRadius, float nSpeedY,int m_HorizonNum, const D3DXVECTOR3 *pColor);
	void Update(void);

	D3DXVECTOR3 GetPos(void);
private:
	float m_fRadius;
	float m_fSpeedY;
	int m_HorizonNum;
	D3DXVECTOR3 m_Color;
};

class CExplosion : public CPSetting
{
public:
	CExplosion();
	~CExplosion();

	static void Create(const D3DXVECTOR3 *pos, int nDistanceMax, const D3DXVECTOR3 *pColor);                    //”š”­ƒZƒbƒg
};

#endif