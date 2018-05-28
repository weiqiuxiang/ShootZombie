#pragma once
#ifndef _TPSAIM_H_
#define _TPSAIM_H_

//�w�b�_�t�@�C��
#include "main.h"

//�v���g�^�C�v
void RayMesh(D3DXVECTOR3 *pOut,const D3DXVECTOR3 *pVectorLookat/*�x�N�g���n�_�|�C���^*/, const D3DXVECTOR3 *pVectorCamera/*�x�N�g���I�_�|�C���^*/);
void RayMeshRect(D3DXVECTOR3 *pOut, float *fDistanceOut,const D3DXVECTOR3 *pVectorLookat/*�x�N�g���n�_�|�C���^*/, const D3DXVECTOR3 *pVectorCamera/*�x�N�g���I�_�|�C���^*/);
void RayCircle(D3DXVECTOR3 *pOut, float *fDistanceOut,const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera, const D3DXVECTOR3 *pModelPos);
void RayCheck(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera, const D3DXVECTOR3 *pModelPos);                   //���C�̓��蔻��`�F�b�N
float GetRayDistance(void);
D3DXVECTOR3 GetRayInt(void);
#endif