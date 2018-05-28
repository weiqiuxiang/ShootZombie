

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
メッセージBOX表示処理[camera.cpp]

Author :
Data   :
----------------------------------------------------------------
■Update : 2016/04/18
カメラの操作
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/////////////////////////////////インクルードヘッド//////////////////////////////////
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

////////////////////////////////プロトタイプ宣言///////////////////////////////////
void Move_Camera(void);
void Move_LookAt(void);

/////////////////////////////////グロバール変数/////////////////////////////////
CAMERA_FPS g_CameraTps;
CAMERA_LOOKAT g_TpsCameraLookat;
TPS_SUBANGLE g_TpsBefore;                 //補正前
TPS_SUBANGLE g_TpsRepair;                 //Tpsのカメラ補正変数

/////////////////////////////////カメラ初期化関数//////////////////////////////////
void InitTpsCamera(void)
{
	//注視するオブジェの取得
	AIRCRAFT *pAircraft = Get_Aircraft();

	//カメラ視点の初期化
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

	//補正前
	g_TpsBefore.fDistance = 1.0f;
	g_TpsBefore.fHorizon = g_CameraTps.fAngleHorizon;
	g_TpsBefore.fVertical = g_CameraTps.fAngleVertical;

	//カメラ補正値初期化
	g_TpsRepair.fDistance = DISTANCE_NORMAL;
	g_TpsRepair.fHorizon = -0.5f;
	g_TpsRepair.fVertical = 0.5f;
	g_TpsRepair.fChange_RClick = false;

	//カメラ注視点の初期化
	g_TpsCameraLookat.pos.x = pAircraft->m_Matrix.pos.x;
	g_TpsCameraLookat.pos.y = pAircraft->m_Matrix.pos.y;
	g_TpsCameraLookat.pos.z = pAircraft->m_Matrix.pos.z;
	g_TpsCameraLookat.TargetPos = g_TpsCameraLookat.pos;
}

/////////////////////////////////カメラ更新関数//////////////////////////////////
void UpdateTpsCamera(void)
{
	TpsCameraMove();   //カメラ移動
	TpsCameraRorate(); //カメラ回転
	SetViewMatrix(&g_CameraTps.pos,&g_TpsCameraLookat.pos, &g_CameraTps.VecUp);
}

/////////////////////////////////tpsカメラ移動関数/////////////////////////////////
void TpsCameraMove(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();
	float fAngle = pAircraft->m_fTargetAngle;
	D3DXVECTOR3 MovePos;                //カメラベクトルをモデルの左の方向にずれるベクトル

	g_TpsCameraLookat.pos = pAircraft->m_Matrix.pos;         //モデル座標と一致する
	MovePos.x = g_CameraTps.fDistance*cos(g_CameraTps.fAngleHorizon - 0.5*PI)*cos(g_CameraTps.fAngleVertical);
	MovePos.z = g_CameraTps.fDistance*sin(g_CameraTps.fAngleHorizon - 0.5*PI)*cos(g_CameraTps.fAngleVertical);
	MovePos.y = g_CameraTps.fDistance*cos(g_CameraTps.fAngleVertical);

	D3DXVec3Normalize(&MovePos,&MovePos);             //ベクトル単位化

	g_TpsCameraLookat.pos += MovePos*5;

	//Move_Camera(); //カメラ移動
	//Move_LookAt(); //注視点移動
}

/////////////////////////////////tpsカメラ回転関数/////////////////////////////////
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

	//角度制限
	// -180～180度に丸める
	if (g_TpsBefore.fHorizon > PI) { g_TpsBefore.fHorizon -= 2 * PI; }
	if (g_TpsBefore.fHorizon < -PI) { g_TpsBefore.fHorizon += 2 * PI; }

	//垂直方向:可動範囲は-90～90度
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

	//角度制限
	// -180～180度に丸める
	if (g_CameraTps.fAngleHorizon > PI) { g_CameraTps.fAngleHorizon -= 2 * PI; }
	if (g_CameraTps.fAngleHorizon < -PI) { g_CameraTps.fAngleHorizon += 2 * PI; }

	//垂直方向:可動範囲は-90～90度
	if (g_CameraTps.fAngleVertical > 0.4*PI) { g_CameraTps.fAngleVertical = oldfAngleVertical; }
	if (g_CameraTps.fAngleVertical < -0.4*PI) { g_CameraTps.fAngleVertical = oldfAngleVertical; }

	g_CameraTps.pos.x = g_CameraTps.fDistance*cos(g_CameraTps.fAngleHorizon)*cos(g_CameraTps.fAngleVertical) + g_TpsCameraLookat.pos.x;
	g_CameraTps.pos.z = g_CameraTps.fDistance*sin(g_CameraTps.fAngleHorizon)*cos(g_CameraTps.fAngleVertical) + g_TpsCameraLookat.pos.z;
	g_CameraTps.pos.y = g_CameraTps.fDistance*sin(g_CameraTps.fAngleVertical) + g_TpsCameraLookat.pos.y;

	//カメラとメッシュの判定
	VectorAndRect_TpsCamera(&g_TpsCameraLookat.pos, &g_CameraTps.pos);
	VectorAndRect_TpsCamera(&g_CameraTps.pos, &g_TpsCameraLookat.pos);
}

/////////////////////////////////tpsカメラ視点をゲット/////////////////////////////////
CAMERA_FPS *GetCameraTps(void)
{
	return &g_CameraTps;
}

/////////////////////////////////tpsカメラ注視点をゲット/////////////////////////////////
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
	//カメラ(視点)ゆっくり動く
	///////////////////////////////
	//ターゲットの目標点
	//移動補正
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
	//移動距離丸め
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

//注視点移動
void Move_LookAt(void)
{
	//
	//注視点ゆっくり動く
	///////////////////////////////
	//ターゲットの目標点
	//移動補正
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

	//移動距離丸め
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