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

	//�t�@�N�V�����֐�
	void SetTrack(const D3DXVECTOR3 *pos/*�J�n�ʒu*/, const D3DXVECTOR3 *speed/*�X�s�[�h*/,float gravity/*�O���l�[�h�̏d�͉e��*/, const D3DXVECTOR3 *color/*�O�Ղ̃J���[*/);
private:
	CObj3dBillboard m_ObjBill[BILLBOARD_NUM];
	CObj3dBillboard m_ObjBillEF[BILLBOARD_NUM];
	bool m_ObjBillUse[BILLBOARD_NUM];
	int m_nFrame;
};

#endif