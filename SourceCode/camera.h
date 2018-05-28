#ifndef _CAMERA_H_
#define _CAMERA_H_

//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//FPSカメラの座標
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 TargetPos;
	D3DXVECTOR3 VecUp;
	float fDistance;            //注視点までの距離(半径)
	float fAngleHorizon;        //水平の方向の角度
	float fAngleVertical;       //垂直の方向の角度
}CAMERA_FPS;

//FPSカメラの注視点
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 TargetPos;
}CAMERA_LOOKAT;

//プロトタイプ宣言
void InitCamera(void);
void UpdateCamera(void);
CAMERA_FPS *GetCameraFps(void);
CAMERA_LOOKAT *GetCameraLookat(void);
void CameraMove(void);             //FPSカメラ移動関数
void CameraRorate(void);           //FPSカメラ回転関数

#endif