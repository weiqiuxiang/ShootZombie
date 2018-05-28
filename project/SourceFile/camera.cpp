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
#include "camera.h"

#define SPEED (0.05f)
#define RORATE_SPEED (0.01f)

/////////////////////////////////�O���o�[���ϐ�/////////////////////////////////
CAMERA_FPS g_CameraFps;
CAMERA_LOOKAT g_CameraLookat;

/////////////////////////////////�J�����������֐�//////////////////////////////////
void InitCamera(void)
{
	//�J�������_�̏�����
	g_CameraFps.pos.x = 5.0f;
	g_CameraFps.pos.y = 0.0f;
	g_CameraFps.pos.z = 1.0f;
	g_CameraFps.VecUp.x = 0.0f;
	g_CameraFps.VecUp.y = 1.0f;
	g_CameraFps.VecUp.z = 0.0f;
	g_CameraFps.fDistance = 5.0f;
	g_CameraFps.fAngleHorizon = 0;
	g_CameraFps.fAngleVertical = 0.0f;

	//�J���������_�̏�����
	g_CameraLookat.pos.x = 0.0f;
	g_CameraLookat.pos.y = 0.0f;
	g_CameraLookat.pos.z = 0.0f;
}

/////////////////////////////////�J�����X�V�֐�//////////////////////////////////
void UpdateCamera(void)
{
	CameraMove();   //�J�����ړ�
	CameraRorate(); //�J������]
}

/////////////////////////////////FPS�J�����ړ��֐�/////////////////////////////////
void CameraMove(void)
{
	//�J�������ֈڍs�ړ�
	if(GetKeyboardPress(DIK_J))
	{
		g_CameraFps.pos.x -= SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.x -= SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraFps.pos.z += SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.z += SPEED*cos(g_CameraFps.fAngleHorizon);
	}
	//�J�����E�ֈڍs�ړ�
	if(GetKeyboardPress(DIK_L))
	{
		g_CameraFps.pos.x += SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.x += SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraFps.pos.z -= SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.z -= SPEED*cos(g_CameraFps.fAngleHorizon);
	}
	//�J�����O�ֈڍs�ړ�
	if(GetKeyboardPress(DIK_I))
	{
		g_CameraFps.pos.x += SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.x += SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraFps.pos.z += SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.z += SPEED*sin(g_CameraFps.fAngleHorizon);
	}
	//�J�������ֈڍs�ړ�
	if(GetKeyboardPress(DIK_K))
	{
		g_CameraFps.pos.x -= SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.x -= SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraFps.pos.z -= SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.z -= SPEED*sin(g_CameraFps.fAngleHorizon);
	}
}

/////////////////////////////////FPS�J������]�֐�/////////////////////////////////
void CameraRorate(void)
{
	g_CameraLookat.pos.x = g_CameraFps.fDistance*cos(g_CameraFps.fAngleHorizon)*cos(g_CameraFps.fAngleVertical) + g_CameraFps.pos.x;
	g_CameraLookat.pos.z = g_CameraFps.fDistance*sin(g_CameraFps.fAngleHorizon)*cos(g_CameraFps.fAngleVertical) + g_CameraFps.pos.z;
	g_CameraLookat.pos.y = g_CameraFps.fDistance*sin(g_CameraFps.fAngleVertical) + g_CameraFps.pos.y;

	//�}�E�X��Ԏ擾
	DIMOUSESTATE *pMouseState = GetMouseStates();

	static float oldfAngleVertical;
	oldfAngleVertical = g_CameraFps.fAngleVertical;

	g_CameraFps.fAngleHorizon -= pMouseState->lX*RORATE_SPEED;
	g_CameraFps.fAngleVertical -= pMouseState->lY*RORATE_SPEED;

	//�p�x����
	// -180�`180�x�Ɋۂ߂�
	if (g_CameraFps.fAngleHorizon > PI) { g_CameraFps.fAngleHorizon -= 2*PI; }
	if (g_CameraFps.fAngleHorizon < -PI) { g_CameraFps.fAngleHorizon += 2*PI; }

	//��������:���͈͂�-90�`90�x
	if (g_CameraFps.fAngleVertical > 0.5*PI) { g_CameraFps.fAngleVertical = oldfAngleVertical; }
	if (g_CameraFps.fAngleVertical < -0.5*PI) { g_CameraFps.fAngleVertical = oldfAngleVertical; }
}

/////////////////////////////////FPS�J�������_���Q�b�g/////////////////////////////////
CAMERA_FPS *GetCameraFps(void)
{
	return &g_CameraFps;
}

/////////////////////////////////FPS�J���������_���Q�b�g/////////////////////////////////
CAMERA_LOOKAT *GetCameraLookat(void)
{
	return &g_CameraLookat;
}
