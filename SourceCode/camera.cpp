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
#include "camera.h"

#define SPEED (0.05f)
#define RORATE_SPEED (0.01f)

/////////////////////////////////グロバール変数/////////////////////////////////
CAMERA_FPS g_CameraFps;
CAMERA_LOOKAT g_CameraLookat;

/////////////////////////////////カメラ初期化関数//////////////////////////////////
void InitCamera(void)
{
	//カメラ視点の初期化
	g_CameraFps.pos.x = 5.0f;
	g_CameraFps.pos.y = 0.0f;
	g_CameraFps.pos.z = 1.0f;
	g_CameraFps.VecUp.x = 0.0f;
	g_CameraFps.VecUp.y = 1.0f;
	g_CameraFps.VecUp.z = 0.0f;
	g_CameraFps.fDistance = 5.0f;
	g_CameraFps.fAngleHorizon = 0;
	g_CameraFps.fAngleVertical = 0.0f;

	//カメラ注視点の初期化
	g_CameraLookat.pos.x = 0.0f;
	g_CameraLookat.pos.y = 0.0f;
	g_CameraLookat.pos.z = 0.0f;
}

/////////////////////////////////カメラ更新関数//////////////////////////////////
void UpdateCamera(void)
{
	CameraMove();   //カメラ移動
	CameraRorate(); //カメラ回転
}

/////////////////////////////////FPSカメラ移動関数/////////////////////////////////
void CameraMove(void)
{
	//カメラ左へ移行移動
	if(GetKeyboardPress(DIK_J))
	{
		g_CameraFps.pos.x -= SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.x -= SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraFps.pos.z += SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.z += SPEED*cos(g_CameraFps.fAngleHorizon);
	}
	//カメラ右へ移行移動
	if(GetKeyboardPress(DIK_L))
	{
		g_CameraFps.pos.x += SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.x += SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraFps.pos.z -= SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.z -= SPEED*cos(g_CameraFps.fAngleHorizon);
	}
	//カメラ前へ移行移動
	if(GetKeyboardPress(DIK_I))
	{
		g_CameraFps.pos.x += SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.x += SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraFps.pos.z += SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.z += SPEED*sin(g_CameraFps.fAngleHorizon);
	}
	//カメラ後ろへ移行移動
	if(GetKeyboardPress(DIK_K))
	{
		g_CameraFps.pos.x -= SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.x -= SPEED*cos(g_CameraFps.fAngleHorizon);
		g_CameraFps.pos.z -= SPEED*sin(g_CameraFps.fAngleHorizon);
		g_CameraLookat.pos.z -= SPEED*sin(g_CameraFps.fAngleHorizon);
	}
}

/////////////////////////////////FPSカメラ回転関数/////////////////////////////////
void CameraRorate(void)
{
	g_CameraLookat.pos.x = g_CameraFps.fDistance*cos(g_CameraFps.fAngleHorizon)*cos(g_CameraFps.fAngleVertical) + g_CameraFps.pos.x;
	g_CameraLookat.pos.z = g_CameraFps.fDistance*sin(g_CameraFps.fAngleHorizon)*cos(g_CameraFps.fAngleVertical) + g_CameraFps.pos.z;
	g_CameraLookat.pos.y = g_CameraFps.fDistance*sin(g_CameraFps.fAngleVertical) + g_CameraFps.pos.y;

	//マウス状態取得
	DIMOUSESTATE *pMouseState = GetMouseStates();

	static float oldfAngleVertical;
	oldfAngleVertical = g_CameraFps.fAngleVertical;

	g_CameraFps.fAngleHorizon -= pMouseState->lX*RORATE_SPEED;
	g_CameraFps.fAngleVertical -= pMouseState->lY*RORATE_SPEED;

	//角度制限
	// -180～180度に丸める
	if (g_CameraFps.fAngleHorizon > PI) { g_CameraFps.fAngleHorizon -= 2*PI; }
	if (g_CameraFps.fAngleHorizon < -PI) { g_CameraFps.fAngleHorizon += 2*PI; }

	//垂直方向:可動範囲は-90～90度
	if (g_CameraFps.fAngleVertical > 0.5*PI) { g_CameraFps.fAngleVertical = oldfAngleVertical; }
	if (g_CameraFps.fAngleVertical < -0.5*PI) { g_CameraFps.fAngleVertical = oldfAngleVertical; }
}

/////////////////////////////////FPSカメラ視点をゲット/////////////////////////////////
CAMERA_FPS *GetCameraFps(void)
{
	return &g_CameraFps;
}

/////////////////////////////////FPSカメラ注視点をゲット/////////////////////////////////
CAMERA_LOOKAT *GetCameraLookat(void)
{
	return &g_CameraLookat;
}
