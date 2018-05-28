#pragma once
#ifndef _CGRENADE_H_
#define _CGRENADE_H_

#include "obj3d.h"

#define GRENADE_MAX (20)

class CGrenade : public CObj3D
{
public:
	CGrenade();
	~CGrenade();

	//定番関数
	HRESULT Init(const WORLD_MATRIX *pPosRotScl, char *ModelPass, DWORD nNumMatEB, bool bUse);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	//ファクション関数
	static void Create(WORLD_MATRIX *pPosRotScl, D3DXVECTOR3 *pSpeed, float gravity,int nCntExplosion);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void UninitAll(void);
	void InitGrenade(const int nID, D3DXVECTOR3 *pSpeed , const float gravity,const int nCntExplosion);

private:
	void HitEnemy(void);
	static CGrenade *m_aGrenade[GRENADE_MAX];
	D3DXVECTOR3 m_posOld;
	int m_nID;
	D3DXVECTOR3 m_Speed;
	float m_gravity;
	int m_nCntExplosion;
	bool m_bCollsion;
};

#endif