#ifndef _TPSCAMERA_H_
#define _TPSCAMERA_H_

//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "camera.h"

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define PLAYER_NUMVERTEX (8)         //�v���[���[���_��

#define PLAYER_HEADRATE (0.2f)
#define PLAYER_HEIGHT (2.365f*2)

#define DISTANCE_NORMAL (10.0f)
#define DISTANCE_AIM (5.0f)

//TPS�J�����␳�\����
typedef struct
{
	float fHorizon;                //�����␳�p�x
	float fVertical;               //�����␳�p�x
	float fDistance;               //�����␳
	bool fChange_RClick;           //�E�N���b�N���̃J�����ړ��t���O
}TPS_SUBANGLE;

//�v���g�^�C�v�錾
void InitTpsCamera(void);
void UpdateTpsCamera(void);
CAMERA_FPS *GetCameraTps(void);
CAMERA_LOOKAT *GetTpsCameraLookat(void);
TPS_SUBANGLE *GetBeforeAngle(void);

void TpsCameraMove(void);             //TPS�J�����ړ��֐�
void TpsCameraRorate(void);           //TPS�J������]�֐�

#endif