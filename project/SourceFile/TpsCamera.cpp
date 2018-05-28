

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
���b�Z�[�WBOX�\������[camera.cpp]

Author :
Data   :
----------------------------------------------------------------
��Update : 2016/04/18
�J�����̑���
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/////////////////////////////////�C���N���[�h�w�b�h//////////////////////////////////
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "inputMouse.h"
#include "TpsCamera.h"
#include "model.h"
#include "model_aircraft.h"
#include "polygon.h"
#include "hitcheckMesh.h"
#include "matrix.h"

#define SPEED (1.5f)
#define TMOVE_SPEED (SPEED * 0.8)
#define MOVE_DISTANCE (1.5f)
#define RORATE_SPEED (0.02f)

#define MOVE_CAMERASPEED (1.0f)

#define CAMERA_JUSTY (1.0f)

////////////////////////////////�v���g�^�C�v�錾///////////////////////////////////
void Move_Camera(void);
void Move_LookAt(void);

/////////////////////////////////�O���o�[���ϐ�/////////////////////////////////
CAMERA_FPS g_CameraTps;
CAMERA_LOOKAT g_TpsCameraLookat;
TPS_SUBANGLE g_TpsBefore;                 //�␳�O
TPS_SUBANGLE g_TpsRepair;                 //Tps�̃J�����␳�ϐ�

/////////////////////////////////�J�����������֐�//////////////////////////////////
void InitTpsCamera(void)
{
	//��������I�u�W�F�̎擾
	AIRCRAFT *pAircraft = Get_Aircraft();

	//�J�������_�̏�����
	g_CameraTps.pos.x = 0.0f;
	g_CameraTps.pos.y = 0.0f;
	g_CameraTps.pos.z = -5.0f;
	g_CameraTps.TargetPos = g_CameraTps.pos;
	g_CameraTps.VecUp.x = 0.0f;
	g_CameraTps.VecUp.y = 1.0f;
	g_CameraTps.VecUp.z = 0.0f;
	g_CameraTps.fDistance = DISTANCE_NORMAL;
	g_CameraTps.fAngleHorizon = PI*(0.5f);
	g_CameraTps.fAngleVertical = 0.0f;

	//�␳�O
	g_TpsBefore.fDistance = 1.0f;
	g_TpsBefore.fHorizon = g_CameraTps.fAngleHorizon;
	g_TpsBefore.fVertical = g_CameraTps.fAngleVertical;

	//�J�����␳�l������
	g_TpsRepair.fDistance = DISTANCE_NORMAL;
	g_TpsRepair.fHorizon = -0.5f;
	g_TpsRepair.fVertical = 0.5f;
	g_TpsRepair.fChange_RClick = false;

	//�J���������_�̏�����
	g_TpsCameraLookat.pos.x = pAircraft->m_Matrix.pos.x;
	g_TpsCameraLookat.pos.y = pAircraft->m_Matrix.pos.y;
	g_TpsCameraLookat.pos.z = pAircraft->m_Matrix.pos.z;
	g_TpsCameraLookat.TargetPos = g_TpsCameraLookat.pos;
}

/////////////////////////////////�J�����X�V�֐�//////////////////////////////////
void UpdateTpsCamera(void)
{
	TpsCameraMove();   //�J�����ړ�
	TpsCameraRorate(); //�J������]
	SetViewMatrix(&g_CameraTps.pos,&g_TpsCameraLookat.pos, &g_CameraTps.VecUp);
}

/////////////////////////////////tps�J�����ړ��֐�/////////////////////////////////
void TpsCameraMove(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();
	float fAngle = pAircraft->m_fTargetAngle;
	D3DXVECTOR3 MovePos;                //�J�����x�N�g�������f���̍��̕����ɂ����x�N�g��

	g_TpsCameraLookat.pos = pAircraft->m_Matrix.pos;         //���f�����W�ƈ�v����
	MovePos.x = g_CameraTps.fDistance*cos(g_CameraTps.fAngleHorizon - 0.5*PI)*cos(g_CameraTps.fAngleVertical);
	MovePos.z = g_CameraTps.fDistance*sin(g_CameraTps.fAngleHorizon - 0.5*PI)*cos(g_CameraTps.fAngleVertical);
	MovePos.y = g_CameraTps.fDistance*cos(g_CameraTps.fAngleVertical);

	D3DXVec3Normalize(&MovePos,&MovePos);             //�x�N�g���P�ʉ�

	g_TpsCameraLookat.pos += MovePos*5;

	//Move_Camera(); //�J�����ړ�
	//Move_LookAt(); //�����_�ړ�
}

/////////////////////////////////tps�J������]�֐�/////////////////////////////////
void TpsCameraRorate(void)
{
	DIMOUSESTATE *pMouseState = GetMouseStates();
	AIRCRAFT *pAircraft = Get_Aircraft();
	
	g_TpsBefore.fHorizon -= pMouseState->lX*RORATE_SPEED*0.2;

	static float oldfAngleBeforeV;
	static float oldfAngleVertical;

	oldfAngleBeforeV = g_TpsBefore.fVertical;
	oldfAngleVertical = g_CameraTps.fAngleVertical;

	g_TpsBefore.fVertical += pMouseState->lY*RORATE_SPEED*0.2;

	//�p�x����
	// -180�`180�x�Ɋۂ߂�
	if (g_TpsBefore.fHorizon > PI) { g_TpsBefore.fHorizon -= 2 * PI; }
	if (g_TpsBefore.fHorizon < -PI) { g_TpsBefore.fHorizon += 2 * PI; }

	//��������:���͈͂�-90�`90�x
	if (g_TpsBefore.fVertical > 0.4*PI) { g_TpsBefore.fVertical = oldfAngleBeforeV; }
	if (g_TpsBefore.fVertical < -0.4*PI) { g_TpsBefore.fVertical = oldfAngleBeforeV; }

	if ((pAircraft->m_MotionNow == CModel::MOTION_SHOOT) || (pAircraft->m_MotionNow == CModel::MOTION_GUNMOVE))
	{
		if (g_TpsRepair.fDistance >= DISTANCE_AIM)
		{
			g_TpsRepair.fDistance -= MOVE_CAMERASPEED;
			if (fabs(g_TpsRepair.fDistance - DISTANCE_AIM) < MOVE_CAMERASPEED * 1.2)
			{
				g_TpsRepair.fDistance = DISTANCE_AIM;
			}
		}
	}

	else
	{
		if (g_TpsRepair.fDistance <= DISTANCE_NORMAL)
		{
			g_TpsRepair.fDistance += MOVE_CAMERASPEED;
			if (fabs(g_TpsRepair.fDistance - DISTANCE_NORMAL) < MOVE_CAMERASPEED * 1.2)
			{
				g_TpsRepair.fDistance = DISTANCE_NORMAL;
			}
		}
	}

	g_CameraTps.fAngleHorizon = g_TpsBefore.fHorizon + g_TpsRepair.fHorizon;
	g_CameraTps.fAngleVertical = g_TpsBefore.fVertical + g_TpsRepair.fVertical;
	g_CameraTps.fDistance = g_TpsBefore.fDistance + g_TpsRepair.fDistance;

	//�p�x����
	// -180�`180�x�Ɋۂ߂�
	if (g_CameraTps.fAngleHorizon > PI) { g_CameraTps.fAngleHorizon -= 2 * PI; }
	if (g_CameraTps.fAngleHorizon < -PI) { g_CameraTps.fAngleHorizon += 2 * PI; }

	//��������:���͈͂�-90�`90�x
	if (g_CameraTps.fAngleVertical > 0.4*PI) { g_CameraTps.fAngleVertical = oldfAngleVertical; }
	if (g_CameraTps.fAngleVertical < -0.4*PI) { g_CameraTps.fAngleVertical = oldfAngleVertical; }

	g_CameraTps.pos.x = g_CameraTps.fDistance*cos(g_CameraTps.fAngleHorizon)*cos(g_CameraTps.fAngleVertical) + g_TpsCameraLookat.pos.x;
	g_CameraTps.pos.z = g_CameraTps.fDistance*sin(g_CameraTps.fAngleHorizon)*cos(g_CameraTps.fAngleVertical) + g_TpsCameraLookat.pos.z;
	g_CameraTps.pos.y = g_CameraTps.fDistance*sin(g_CameraTps.fAngleVertical) + g_TpsCameraLookat.pos.y;

	//�J�����ƃ��b�V���̔���
	VectorAndRect_TpsCamera(&g_TpsCameraLookat.pos, &g_CameraTps.pos);
	VectorAndRect_TpsCamera(&g_CameraTps.pos, &g_TpsCameraLookat.pos);
}

/////////////////////////////////tps�J�������_���Q�b�g/////////////////////////////////
CAMERA_FPS *GetCameraTps(void)
{
	return &g_CameraTps;
}

/////////////////////////////////tps�J���������_���Q�b�g/////////////////////////////////
CAMERA_LOOKAT *GetTpsCameraLookat(void)
{
	return &g_TpsCameraLookat;
}

TPS_SUBANGLE *GetBeforeAngle(void)
{
	return &g_TpsBefore;
}

void Move_Camera(void)
{
	//
	//�J����(���_)������蓮��
	///////////////////////////////
	//�^�[�Q�b�g�̖ڕW�_
	//�ړ��␳
	if (g_CameraTps.TargetPos.x > g_CameraTps.pos.x)
	{
		g_CameraTps.pos.x += TMOVE_SPEED;
	}

	else
	{
		g_CameraTps.pos.x -= TMOVE_SPEED;
	}

	if (g_CameraTps.TargetPos.y > g_CameraTps.pos.y)
	{
		g_CameraTps.pos.y += TMOVE_SPEED;
	}

	else
	{
		g_CameraTps.pos.y -= TMOVE_SPEED;
	}

	if (g_CameraTps.TargetPos.z > g_CameraTps.pos.z)
	{
		g_CameraTps.pos.z += TMOVE_SPEED;
	}

	else
	{
		g_CameraTps.pos.z -= TMOVE_SPEED;
	}
	/*
	//�ړ������ۂ�
	if (fabs(g_CameraTps.TargetPos.x - g_CameraTps.pos.x) < (1.05*TMOVE_SPEED))
	{
		g_CameraTps.pos.x = g_CameraTps.TargetPos.x;
	}
	if (fabs(g_CameraTps.TargetPos.y - g_CameraTps.pos.y) < (1.05*TMOVE_SPEED))
	{
		g_CameraTps.pos.y = g_CameraTps.TargetPos.y;
	}
	if (fabs(g_CameraTps.TargetPos.z - g_CameraTps.pos.z) < (1.05*TMOVE_SPEED))
	{
		g_CameraTps.pos.z = g_CameraTps.TargetPos.z;
	}
	*/
}

//�����_�ړ�
void Move_LookAt(void)
{
	//
	//�����_������蓮��
	///////////////////////////////
	//�^�[�Q�b�g�̖ڕW�_
	//�ړ��␳
	if (g_TpsCameraLookat.TargetPos.x > g_TpsCameraLookat.pos.x)
	{
		g_TpsCameraLookat.pos.x += TMOVE_SPEED;
	}

	else
	{
		g_TpsCameraLookat.pos.x -= TMOVE_SPEED;
	}

	if (g_TpsCameraLookat.TargetPos.y > g_TpsCameraLookat.pos.y)
	{
		g_TpsCameraLookat.pos.y += TMOVE_SPEED;
	}

	else
	{
		g_TpsCameraLookat.pos.y -= TMOVE_SPEED;
	}

	if (g_TpsCameraLookat.TargetPos.z > g_TpsCameraLookat.pos.z)
	{
		g_TpsCameraLookat.pos.z += TMOVE_SPEED;
	}

	else
	{
		g_TpsCameraLookat.pos.z -= TMOVE_SPEED;
	}

	//�ړ������ۂ�
	/*
	if (fabs(g_TpsCameraLookat.TargetPos.x - g_TpsCameraLookat.pos.x) < (1.05*TMOVE_SPEED))
	{
		g_TpsCameraLookat.pos.x = g_TpsCameraLookat.TargetPos.x;
	}
	if (fabs(g_TpsCameraLookat.TargetPos.y - g_TpsCameraLookat.pos.y) < (1.05*TMOVE_SPEED))
	{
		g_TpsCameraLookat.pos.y = g_TpsCameraLookat.TargetPos.y;
	}
	if (fabs(g_TpsCameraLookat.TargetPos.z - g_TpsCameraLookat.pos.z) < (1.05*TMOVE_SPEED))
	{
		g_TpsCameraLookat.pos.z = g_TpsCameraLookat.TargetPos.z;
	}
	*/
}