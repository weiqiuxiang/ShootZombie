#pragma once
#ifndef _HITCHECKMESH_H_
#define _HITCHECKMESH_H_

#include "main.h"
#include "hitRectangle .h"

//�v���g�^�C�v�錾
float MinAB(float A , float B);          //��r��,�������̕���Ԃ�
bool Rects2D(D3DXVECTOR2 const *fRectA_pos, D3DXVECTOR2 const *fRectA_HW, D3DXVECTOR2 const *fRectB_pos, D3DXVECTOR2 const *fRectB_HW);                //2D�̎l�p�|���S���Ǝl�p�|���S���̓��蔻��
bool Rects3DPointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect);             //�_�͕��ʏ�ɂ��邩�ǂ���
bool Wall3DPointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect);              //�_�̕��ʂɑ΂��铊�e�͕��ʏ�̃|���S���ɂ��邩�ǂ���
bool Rect2D_Vector(const D3DXVECTOR2 *pPoint, const RECT_2D *pRect);
bool Rect3D_Vector(const D3DXVECTOR3 *pPoint, const RECT_3D *pRect);            //3D�ɂ����Ẵx�N�g�����蔻��
bool VectorAndRect( D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/);            //�x�N�g���Ɣ���l�p�|���S���̓��蔻��̗L��
bool VectorAndRect_GroundCheck(D3DXVECTOR3 *pVectorStart/*�x�N�g���n�_�|�C���^*/,D3DXVECTOR3 *pVectorEnd/*�x�N�g���I�_�|�C���^*/);
void VectorAndRect_TpsCamera(D3DXVECTOR3 *pVectorLookat/*�x�N�g���n�_�|�C���^*/, D3DXVECTOR3 *pVectorCamera/*�x�N�g���I�_�|�C���^*/);
D3DXVECTOR3 GetVecPlaneIntersection(const D3DXVECTOR3 *pStartPoint, const D3DXVECTOR3 *pEndPoint, const HITRECT *pRect);     //�׃N�g���ƃ|���S���̊ђʓ_�����߂�
float DistancePointPlane(const D3DXVECTOR3 *pPoint,const HITRECT *pRect);               //�_�ƕ��ʂ̋������擾
float GetElevation(int nID, const D3DXVECTOR3 *pModelPos);      //���������MESH_FLOOR�̕W�����擾
void Quick_Sort(int nLeftID, int nRightID, float *a);    //�N�C�b�N�\�[�g
#endif // !_HITCHECK_H_
