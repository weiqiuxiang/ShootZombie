#ifndef _CAMERA_H_
#define _CAMERA_H_

//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//FPS�J�����̍��W
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 TargetPos;
	D3DXVECTOR3 VecUp;
	float fDistance;            //�����_�܂ł̋���(���a)
	float fAngleHorizon;        //�����̕����̊p�x
	float fAngleVertical;       //�����̕����̊p�x
}CAMERA_FPS;

//FPS�J�����̒����_
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 TargetPos;
}CAMERA_LOOKAT;

//�v���g�^�C�v�錾
void InitCamera(void);
void UpdateCamera(void);
CAMERA_FPS *GetCameraFps(void);
CAMERA_LOOKAT *GetCameraLookat(void);
void CameraMove(void);             //FPS�J�����ړ��֐�
void CameraRorate(void);           //FPS�J������]�֐�

#endif