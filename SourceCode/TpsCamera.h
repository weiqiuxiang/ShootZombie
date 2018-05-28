#ifndef _TPSCAMERA_H_
#define _TPSCAMERA_H_

//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "camera.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define PLAYER_NUMVERTEX (8)         //プレーヤー頂点数

#define PLAYER_HEADRATE (0.2f)
#define PLAYER_HEIGHT (2.365f*2)

#define DISTANCE_NORMAL (10.0f)
#define DISTANCE_AIM (5.0f)

//TPSカメラ補正構造体
typedef struct
{
	float fHorizon;                //水平補正角度
	float fVertical;               //垂直補正角度
	float fDistance;               //距離補正
	bool fChange_RClick;           //右クリック時のカメラ移動フラグ
}TPS_SUBANGLE;

//プロトタイプ宣言
void InitTpsCamera(void);
void UpdateTpsCamera(void);
CAMERA_FPS *GetCameraTps(void);
CAMERA_LOOKAT *GetTpsCameraLookat(void);
TPS_SUBANGLE *GetBeforeAngle(void);

void TpsCameraMove(void);             //TPSカメラ移動関数
void TpsCameraRorate(void);           //TPSカメラ回転関数

#endif