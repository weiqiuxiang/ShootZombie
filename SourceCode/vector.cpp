#include <math.h>
#include "vector.h"

//
//�񎟌��x�N�g��:��X�E�̊O��
////////////////////////////
float Angle_TwoVector(D3DXVECTOR2 A, D3DXVECTOR2 B)
{
	float ABCross,ABDot;
	float fAngle;
	ABCross = A.x*B.y - A.y*B.x;   //�O��
	ABDot = A.x*B.x + A.y*B.y;     //����
	fAngle = atan2(ABCross, ABDot);

	return fAngle;
}

//
//���f���̊p�x���^�[�Q�b�g�p�x�ɒ���(��]���x�w��)
//////////////////////////////////////////////
void RotToTargetAngle(float *ModelAngle, float TargetAngle,float fRotSpeed)
{
	float fAngleAbs;                     //��Βl

	fAngleAbs = fabs(*ModelAngle - TargetAngle);

	//�p�x���ۂ߂�
	if (fAngleAbs > PI)//�ǐ�
	{
		fAngleAbs = 2 * PI - fAngleAbs;
	}

	//�O��:�ڕW�p�x�̃x�N�g���̓��f���x�N�g���̍��ɂ��邩�E�ɂ��邩���f
	float ABCross = cos(*ModelAngle)*sin(TargetAngle) - sin(*ModelAngle)*cos(TargetAngle);           //�O�ς�����

																																			   //�p�x�̍��������x�ȏ�Ȃ烂�f������
	if (fAngleAbs > (fRotSpeed*1.2))
	{
		//�E�ɂ���
		if (ABCross > 0)
		{
			*ModelAngle += fRotSpeed;
		}
		//���ɂ���
		else
		{
			*ModelAngle -= fRotSpeed;
		}
	}

	//�p�x�̍��������x�ȉ��Ȃ烂�f���̊p�x���^�[�Q�b�g�p�x�Ɋۂ߂�
	else
	{
		*ModelAngle = TargetAngle;
	}
}

//
//���f���̊p�x���^�[�Q�b�g�p�x�ɒ���(��]�|����t���[�����w��)
///////////////////////////////////
void RotToTargetAngleFrame(float *ModelAngle, float TargetAngle, int nFrame)
{
	float fAngleAbs;                     //��Βl

	fAngleAbs = fabs(*ModelAngle - TargetAngle);

	//�p�x���ۂ߂�
	if (fAngleAbs > PI)//�ǐ�
	{
		fAngleAbs = 2 * PI - fAngleAbs;
	}

	//�O��:�ڕW�p�x�̃x�N�g���̓��f���x�N�g���̍��ɂ��邩�E�ɂ��邩���f
	float ABCross = cos(*ModelAngle)*sin(TargetAngle) - sin(*ModelAngle)*cos(TargetAngle);           //�O�ς�����

	//�E�ɂ���
	if (ABCross > 0)
	{
		*ModelAngle += fAngleAbs / nFrame;
	}
	//���ɂ���
	else
	{
		*ModelAngle -= fAngleAbs / nFrame;
	}
}