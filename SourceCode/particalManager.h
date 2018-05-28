#pragma once

#ifndef _PARTICALMANAGER_H_
#define _PARTICALMANAGER_H_

#include "CPartical.h"

#define PARTICAL_MAX (1024)
#define PARTICAL_EFFECT (4096)

class CPartical;

class CParticalManager
{
public:
	CParticalManager();
	~CParticalManager();
	static void Init(void);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);
	static void SetPartical(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *speed, float fRadius, int nLife, int TexNum, CPartical::PARTICAL_TYPE type,const D3DXVECTOR3 *color, float fGravity);
	static void SetParticalEF(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *speed, float fRadius, int nLife, int TexNum, CPartical::PARTICAL_TYPE type,const D3DXVECTOR3 *color, float fGravity);
	static void UpdateAll();
	static void DrawAll();
	static void ReleaseAll();
private:
	static CPartical *m_aPartical[PARTICAL_MAX];                         //パーティクル
	static CPartical *m_aParticalEF[PARTICAL_EFFECT];                    //パーティクルエフェクト
};

#endif