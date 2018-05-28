#pragma once
#ifndef _HITCIRCLEMESH_H_
#define _HITCIRCLEMESH_H_

#include "main.h"
#include "hitRectangle .h"
#include "bullet.h"

bool CircleAndRect(D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, float fRadius/*����͈͔��a*/);            //�~�Ɣ���l�p�|���S���̓��蔻��̗L��
bool CircleAndRectRepel(D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, float fRadius/*����͈͔��a*/, float elasticity, D3DXVECTOR3 *pspeedOut);
void CircleAndRay(D3DXVECTOR3 *pOut,float *pfOutDistance,const D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, const D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, const D3DXVECTOR3 *pCirclePos/*�~�̈ʒu*/, float fRadius/*����͈͔��a*/, const D3DXVECTOR3 *pModelPos);           //���C�Ɖ~�̌����_�Ƌ���������
bool BulletCircleAndCircle(const D3DXVECTOR3 *pModelpos,float fModelRadius, BULLETTYPE Btype);
bool BulletCircleAndCircleSSV(D3DXVECTOR3 *pOutModelpos, const D3DXVECTOR3 *pModelposOld, const D3DXVECTOR3 *pModelpos, float fModelRadius, BULLETTYPE Btype);
float CircleAndPlaneHitTime(const D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/,const D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, const D3DXVECTOR3 *pPlanePoint,const D3DXVECTOR3 *pPlaneNormal/*���ʖ@��*/,float fRadius/*����͈͔��a*/);                    //�Ԃ�l:�~�p�[�e�B�N���Ɩ����啽�ʂ̏Փ˃^�C��
bool CircleAndRectSSV(D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/, float fRadius/*����͈͔��a*/);
#endif