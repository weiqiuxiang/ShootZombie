#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "main.h"

//�v���g�^�C�v�錾
float Angle_TwoVector(D3DXVECTOR2, D3DXVECTOR2);       //�񎟌��x�N�g��:AXB�̊O��
void RotToTargetAngle(float *ModelAngle/*���f���p�x�|�C���^*/,float TargetAngle/*�ڕW�p�x*/, float fRotSpeed/*��]���x*/);     //���f���̊p�x���^�[�Q�b�g�p�x�ɒ���(��]���x�w��ł���)
void RotToTargetAngleFrame(float *ModelAngle/*���f���p�x�|�C���^*/, float TargetAngle/*�ڕW�p�x*/, int nFrame/*��]�|����t���[��*/);     //���f���̊p�x���^�[�Q�b�g�p�x�Ƀt���[�����̈��]���钼��
#endif // !_VECTOR_H_
