#pragma once
#ifndef _CGRENADETRACK_H_
#define _CGRENADETRACK_H_

#include "main.h"
#include "obj3dBillboard.h"
#include "CNumber.h"

#define BILLBOARD_NUM (100)

class CObj3dBillboard;

class CGrenadeTrack
{
public:
	CGrenadeTrack();
	~CGrenadeTrack();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//ファクション関数
	void SetTrack(const D3DXVECTOR3 *pos/*開始位置*/, const D3DXVECTOR3 *speed/*スピード*/,float gravity/*グレネードの重力影響*/, const D3DXVECTOR3 *color/*軌跡のカラー*/);
private:
	CObj3dBillboard m_ObjBill[BILLBOARD_NUM];
	CObj3dBillboard m_ObjBillEF[BILLBOARD_NUM];
	bool m_ObjBillUse[BILLBOARD_NUM];
	int m_nFrame;
};

#endif