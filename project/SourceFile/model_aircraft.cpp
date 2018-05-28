//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "inputMouse.h"
#include "model_aircraft.h"
#include "camera.h"
#include "TpsCamera.h"
#include "textureManager.h"
#include "bullet.h"
#include "MeshField.h"
#include "vector.h"
#include "sound.h"
#include "hitcheckMesh.h"
#include "hitCircleMesh.h"
#include "tpsAim.h"
#include "matrix.h"
#include "hitCircle.h"
#include "fade.h"
#include "CGrenadeTrack.h"
#include "particalManager.h"
#include "CGrenade.h"

#define NUM_MATERIAL (0)
#define AIRCRAFT_SPEED (0.05f)                //戦闘機速度
#define ROT_SPEED (0.05f*PI)                    //戦闘機回転速度
#define AIR_FRICTION (0.1f)                    //空気摩擦係数(スピード減少用)

#define HANDGUNBULLET_MAX (15)
#define AK47BULLET_MAX (30)
#define GRENADE_MAX (8)
#define GRENADE_THROWTIME (50)
#define AK47_STORE (90)
#define AK47_RELOAD (120.0f)
#define HANDGUN_RELOAD (60.0f)

#define GRENADE_TO_EXPLOSION_TIME (120)

#define TRACK_FRAME (5)

#define POWER_TO_THROW_GRENADE (1.5f)

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
void AircraftScale(void);                              //モデルの拡大
void AircraftRotate(void);                             //モデルの回転
void AircraftMove(void);                               //モデルの移動
void DamageCheck(void);
void Shoot(void);              //バレット発射
void ReloadAction(void);
void PlayerInputAction(void);

//-----------------------------------------------------------------------------------------
//構造体変数
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
//int g_AircraftTextureNUM[NUM_MATERIAL];                       //テクスチャ発行番号
LPD3DXMESH g_pMeshAircraft;                               //メッシュインターフェースポインタ
LPD3DXBUFFER g_pBuffMatAircraft;                          //マテリアル情報バッファ(D3DX汎用バッファインターフェース)
DWORD g_nNumMatAircraft;                                  //マテリアルの数

AIRCRAFT g_Aircraft;                            //戦闘機の構造体

CModel g_CModel;

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitAircraft(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();
	CAMERA_FPS *pCameraFps = GetCameraTps();            //カメラ取得
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //カメラ注視点取得

	g_nNumMatAircraft = NUM_MATERIAL;                               //マテリアルの数

	//モデルワールド変換マトリクスの初期化
	g_Aircraft.m_Matrix.scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	g_Aircraft.m_Matrix.rot = D3DXVECTOR3(0.0f, -pCameraFps->fAngleHorizon + PI, 0.0f);
	g_Aircraft.m_Matrix.pos = D3DXVECTOR3(0.0f, -22.0f, 100.0f);
	g_Aircraft.m_AirPosOld = g_Aircraft.m_Matrix.pos;
	g_Aircraft.m_Speed = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Aircraft.m_ModelVec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);                          //モデルの向き初期ベクトル
	g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + PI;
	g_Aircraft.m_nActionFrame = 0;
	g_Aircraft.m_nIDHR = -1;
	g_Aircraft.m_nIDHC = -1;
	g_Aircraft.m_bModeChange = false;
	g_Aircraft.bJump = false;
	g_Aircraft.m_bmove = false;
	g_Aircraft.m_nLife = LIFE_MAX;
	g_Aircraft.m_NoDamageTime = 0;                                               //無敵時間
	g_Aircraft.m_fStamina = STAMINA_MAX;                                         //体力ゲージ最大値
	g_Aircraft.m_StaminaOver = false;

	g_Aircraft.m_GunType = GUNTYPE_HANDGUN;
	g_Aircraft.m_HandGunNum = 15;
	g_Aircraft.m_AK47Num = AK47BULLET_MAX;
	g_Aircraft.m_AK47Store = AK47_STORE;
	g_Aircraft.m_GrenadeNum = 1;
	g_Aircraft.m_GrenadeStore = 8;
	g_Aircraft.m_ReloadTimeHandGun = 0;
	g_Aircraft.m_nGrenadeThrowTime = 0;
	g_Aircraft.m_nGrenadeCreateTime = 0;
	g_Aircraft.m_bGrenadeCreate = false;
	g_Aircraft.m_ReloadTimeAK47 = 0;
	g_Aircraft.m_bReloadAK47 = false;
	g_Aircraft.m_bReloadHandGun = false;

	g_Aircraft.m_MotionNow = CModel::MOTION_NORMAL;
	g_Aircraft.m_MotionBefore = CModel::MOTION_NORMAL;

	SetHitCirclePlayer(&g_Aircraft.m_Matrix.pos,2.0f, &D3DXVECTOR3(0.0f, 2.0f,0.0f));                 //当り判定円の設置
	SetHitCirclePlayer(&g_Aircraft.m_Matrix.pos, 2.0f, &D3DXVECTOR3(0.0f, 0.0f, 0.0f));                 //当り判定円の設置
	SetHitCirclePlayer(&g_Aircraft.m_Matrix.pos, 2.0f, &D3DXVECTOR3(0.0f, -2.0f, 0.0f));                 //当り判定円の設置

	g_CModel.Init("data/MODEL/motion.txt");                                                    //モーションファイルの読み込み

	return S_OK;
}

//モデルの更新
void UpdateAircraft(void)
{
	//プレーヤー生きている時
	if (g_Aircraft.m_nLife > 0)
	{
		if (g_Aircraft.m_nGrenadeThrowTime <= 0)
		{
			AircraftRotate();          //モデル回転
			AircraftMove();            //モデル移動
		}
		DamageCheck();             //敵との当り判定チェック
		Shoot();                   //バレット発射
		ReloadAction();
		PlayerInputAction();       //入力によってモードが変わる

		g_CModel.Update();

		CGrenade::UpdateAll();
	}

	//プレーヤー死亡
	else
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

#ifdef _DEBUG
	if(GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}
#endif
}

//モデル拡大
void AircraftScale(void)
{
	
}
//モデル回転
void AircraftRotate(void) 
{
	CAMERA_FPS *pCameraFps = GetCameraTps();            //カメラ取得
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //カメラ注視点取得
	TPS_SUBANGLE *pBefore = GetBeforeAngle();

	// -180～180度に丸める
	if (g_Aircraft.m_Matrix.rot.y > PI) { g_Aircraft.m_Matrix.rot.y -= 2 * PI; }
	if (g_Aircraft.m_Matrix.rot.y < -PI) { g_Aircraft.m_Matrix.rot.y += 2 * PI; }
	
	if ( (g_Aircraft.m_MotionNow == CModel::MOTION_SHOOT) || (g_Aircraft.m_MotionNow == CModel::MOTION_GUNMOVE))
	{
		//
		//モデルの向き計算
		////////////////////////////////////
		//レイの交差点を求め
		RayCheck(&g_Aircraft.m_ShootPoint, &pCameraFpsLookAt->pos, &pCameraFps->pos, &g_Aircraft.m_Matrix.pos);              //レイとメッシュの交差点を求め
		D3DXVECTOR3 RayVec = g_Aircraft.m_ShootPoint - g_Aircraft.m_Matrix.pos;            //向きベクトルを求め
		D3DXVec3Normalize(&RayVec, &RayVec);                            //単位ベクトル化
																		//回転目標角度を求める
		D3DXVECTOR2 RayVec2 = D3DXVECTOR2(RayVec.x, RayVec.z);
		D3DXVECTOR2 ModelVec2 = D3DXVECTOR2(g_Aircraft.m_ModelVec.x, g_Aircraft.m_ModelVec.z);
		g_Aircraft.m_fTargetAngle = Angle_TwoVector(ModelVec2, RayVec2);
		g_Aircraft.m_fTargetAngleV = asin(RayVec.y);                               //Y軸の目標角度を求める

		RotToTargetAngle(&g_Aircraft.m_Matrix.rot.y, -g_Aircraft.m_fTargetAngle + PI, ROT_SPEED);
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + PI;
	}

	else if ((g_Aircraft.m_MotionNow == CModel::MOTION_GSHOOT) || (g_Aircraft.m_MotionNow == CModel::MOTION_GGUNMOVE))
	{
		RotToTargetAngle(&g_Aircraft.m_Matrix.rot.y, -pCameraFps->fAngleHorizon +0.5* PI, ROT_SPEED);
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + PI;
	}

	else
	{
		RotToTargetAngle(&g_Aircraft.m_Matrix.rot.y, g_Aircraft.m_fTargetAngle - 0.5*PI, ROT_SPEED);
	}
}

//モデル移動
void AircraftMove(void)
{
	//カメラ取得
	CAMERA_FPS *pCameraFps = GetCameraTps();            //カメラ取得
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //カメラ注視点取得
	int nID_Air = -1;                     //当り判定が存在する四角ポリゴンの番号を保存
	bool bMoveFlag = false;

	//メッシュフィールド取得
	MESHWALL *pMeshwall = GetMesh();

	g_Aircraft.m_AirPosOld = g_Aircraft.m_Matrix.pos;       //現在座標保存

	//
	//移動処理
	////////////////////////////////////
	if (GetKeyboardPress(DIK_A))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + 0.5*PI;
		bMoveFlag = true;
	}
	//カメラ右へ移行移動
	if (GetKeyboardPress(DIK_D))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon - 0.5*PI;
		bMoveFlag = true;
	}
	//カメラ前へ移行移動
	if (GetKeyboardPress(DIK_W))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + PI;
		bMoveFlag = true;
	}
	//カメラ後ろへ移行移動
	if (GetKeyboardPress(DIK_S))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon;
		bMoveFlag = true;
	}

	//左上
	if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_A))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + 0.75*PI;
		bMoveFlag = true;
	}

	//右上
	if(GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_D))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon - 0.75*PI;
		bMoveFlag = true;
	}

	//左下
	if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_A))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + 0.25*PI;
		bMoveFlag = true;
	}

	//右下
	if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_D))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon - 0.25*PI;
		bMoveFlag = true;
	}

#ifdef _DEBUG
	//test用
	if(GetKeyboardPress(DIK_Q))
	{
		g_Aircraft.m_Matrix.pos.y += 0.3f;
	}

	if (GetKeyboardPress(DIK_E))
	{
		g_Aircraft.m_Matrix.pos.y -= 0.3f;
	}
#endif
	
	//スピード増加
	if (bMoveFlag == true)
	{
		g_Aircraft.m_Speed.x += AIRCRAFT_SPEED*cos(g_Aircraft.m_fTargetAngle);
		g_Aircraft.m_Speed.z -= AIRCRAFT_SPEED*sin(g_Aircraft.m_fTargetAngle);

		if( (g_Aircraft.m_MotionNow == CModel::MOTION_RUN) || (g_Aircraft.m_MotionNow == CModel::MOTION_GRUN))
		{
			g_Aircraft.m_Speed.x += AIRCRAFT_SPEED*cos(g_Aircraft.m_fTargetAngle);
			g_Aircraft.m_Speed.z -= AIRCRAFT_SPEED*sin(g_Aircraft.m_fTargetAngle);
			g_Aircraft.m_fStamina -= 30;
		}
	}

	g_Aircraft.m_bmove = bMoveFlag;

	//重力によるY軸スピードの増加
	g_Aircraft.m_Speed.y -= GRAVITY;

	//空気摩擦によってスピード減少
	g_Aircraft.m_Speed.x += (0 - g_Aircraft.m_Speed.x)*AIR_FRICTION;
	g_Aircraft.m_Speed.z += (0 - g_Aircraft.m_Speed.z)*AIR_FRICTION;

	// -180～180度に丸める
	if (g_Aircraft.m_fTargetAngle > PI) { g_Aircraft.m_fTargetAngle -= 2 * PI; }
	if (g_Aircraft.m_fTargetAngle < -PI) { g_Aircraft.m_fTargetAngle += 2 * PI; }

	//位置変化
	g_Aircraft.m_Matrix.pos.x += g_Aircraft.m_Speed.x;
	g_Aircraft.m_Matrix.pos.y += g_Aircraft.m_Speed.y;
	g_Aircraft.m_Matrix.pos.z += g_Aircraft.m_Speed.z;

	g_Aircraft.m_GunPos = g_Aircraft.m_Matrix.pos;
	g_Aircraft.m_GunPos.y += 2.0f;

	//
	//当り判定(仮)
	////////////////////////////////////////////////////
	bool Check = false;
	bool Damage = false;
	D3DXVECTOR3 Start,End;                             //当り判定用
	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		HITCIRCLE *pHitCircle = Get_HitCircle(i);
		if (pHitCircle->bUse == true)
		{
			if (pHitCircle->m_type == TYPE_PLAYER)
			{
				Start = g_Aircraft.m_AirPosOld + pHitCircle->m_offset;                              //当り判定用
				End = g_Aircraft.m_Matrix.pos + pHitCircle->m_offset;                               //当り判定用
				Check = CircleAndRect(&Start, &End, pHitCircle->m_skydome.fRadius);
				g_Aircraft.m_AirPosOld = Start - pHitCircle->m_offset;                             //修正後のold座標算出
				g_Aircraft.m_Matrix.pos = End - pHitCircle->m_offset;                              //修正後の座標算出

				//無敵時間外
				if (g_Aircraft.m_NoDamageTime <= 0)
				{
					if (Damage == false)
					{
						if (pHitCircle->HitCheckType == HBTYPE_ENEMY)
						{
							Damage = true;
							pHitCircle->HitCheckType = HBTYPE_NULL;
						}
					}

					else
					{
						pHitCircle->HitCheckType = HBTYPE_NULL;
					}
				}
				//無敵時間中
				else
				{
					pHitCircle->HitCheckType = HBTYPE_NULL;
				}
			}
		}
	}

	if (Check == true)
	{
		g_Aircraft.bJump = false;
		g_Aircraft.m_Speed.y = 0;
	}

	if(Damage == true)
	{
		g_Aircraft.m_nLife--;
		g_Aircraft.m_NoDamageTime = 120;
	}

	if ((g_Aircraft.m_MotionNow != CModel::MOTION_RUN) && (g_Aircraft.m_MotionNow != CModel::MOTION_GRUN) )
	{
		g_Aircraft.m_fStamina += 15;
		if (g_Aircraft.m_fStamina > STAMINA_MAX)
		{
			g_Aircraft.m_fStamina = STAMINA_MAX;
		}
	}
	if(g_Aircraft.m_fStamina < 0)
	{
		g_Aircraft.m_fStamina = 0;
	}
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawAircraft(void)
{
	SetMatrix(&g_Aircraft.m_MtxWorld, g_Aircraft.m_Matrix);    //マトリクスの計算

	g_CModel.Draw();                                       //モデル描画

	CGrenade::DrawAll();
}

void UninitAircraft(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE (g_pMeshAircraft);                      //メッシュインターフェースポインタを解放

	//頂点バッファを解放する
	SAFE_RELEASE (g_pBuffMatAircraft);                       //マテリアル情報バッファを解放

	g_CModel.Uninit();

	CGrenade::UninitAll();                                //全てのグレネードオブジェ削除
}

//バレットの発射
void Shoot(void)
{
	D3DXVECTOR3 speedVec = D3DXVECTOR3(0.0f,0.0f,0.0f);    //弾スピード
	D3DXMATRIX rotMatrix;     //回転ベクトル
	CAMERA_FPS *pCameraFps = GetCameraTps();            //カメラ取得
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //カメラ注視点取得

	//計算
	speedVec = g_Aircraft.m_ShootPoint - g_Aircraft.m_GunPos;
	D3DXVec3Normalize(&speedVec, &speedVec);

	if ((g_Aircraft.m_MotionNow == CModel::MOTION_SHOOT) || (g_Aircraft.m_MotionNow == CModel::MOTION_GUNMOVE))
	{
		if (g_Aircraft.m_GunType == GUNTYPE_HANDGUN)
		{
			//左クリックで射撃
			if (GetMouseTrigger(MOUSE_LEFT))
			{
				if ((g_Aircraft.m_HandGunNum > 0) && (g_Aircraft.m_ReloadTimeHandGun <= 0))
				{
					//バレットセット
					SetBullet(g_Aircraft.m_GunPos, BULLET_SPEED*speedVec,BTYPE_HANDGUN);
					g_Aircraft.m_HandGunNum--;
					PlaySound(SOUND_LABEL_SE_SHOOT);
				}
			}
		}

		if (g_Aircraft.m_GunType == GUNTYPE_AK47)
		{
			//左クリックで射撃
			if (GetMouseRepeat(MOUSE_LEFT))
			{
				if ((g_Aircraft.m_AK47Num > 0) && (g_Aircraft.m_ReloadTimeAK47 <= 0))
				{
					//バレットセット
					SetBullet(g_Aircraft.m_GunPos, BULLET_SPEED*speedVec, BTYPE_AK47);
					g_Aircraft.m_AK47Num--;
					PlaySound(SOUND_LABEL_SE_SHOOT);
				}
			}
		}
	}

	static D3DXVECTOR3 speed;
	static D3DXVECTOR3 posResult;

	if ((g_Aircraft.m_MotionNow == CModel::MOTION_GSHOOT) || (g_Aircraft.m_MotionNow == CModel::MOTION_GGUNMOVE) || (g_Aircraft.m_MotionNow == CModel::MOTION_GTHROW))
	{
		if (g_Aircraft.m_GunType == GUNTYPE_GRENADE)
		{
			if ( (g_Aircraft.m_nGrenadeThrowTime <= 0) && (g_Aircraft.m_GrenadeNum > 0))
			{
				CAMERA_FPS *pCamera = GetCameraTps();
				CAMERA_LOOKAT *pLookAt = GetTpsCameraLookat();

				speed.x = POWER_TO_THROW_GRENADE*cos(pCamera->fAngleHorizon)*cos(pCamera->fAngleVertical - PI*0.2);
				speed.z = POWER_TO_THROW_GRENADE*sin(pCamera->fAngleHorizon)*cos(pCamera->fAngleVertical - PI*0.2);
				speed.y = POWER_TO_THROW_GRENADE*sin(pCamera->fAngleVertical - PI*0.2);

				speed = -speed;

				for (int nCnt = 1; nCnt < BILLBOARD_NUM; nCnt++)
				{
					//第nCnt目の点の位置の計算(点と点のframe数はTRACK_FRAME)
					posResult.x = g_Aircraft.m_Matrix.pos.x + speed.x * TRACK_FRAME * nCnt;
					posResult.y = speed.y * TRACK_FRAME * nCnt - 0.5 * GRAVITY * (TRACK_FRAME * nCnt) * (TRACK_FRAME * nCnt) + g_Aircraft.m_Matrix.pos.y;          //S = vt - 0.5*g*t^2;
					posResult.z = g_Aircraft.m_Matrix.pos.z + speed.z * TRACK_FRAME * nCnt;
					CParticalManager::SetPartical(&posResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.5f, 1, 0, CPartical::PARTICAL_NORMAL, &D3DXVECTOR3(150, 0, 0), 0);
					CParticalManager::SetParticalEF(&posResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.5f, 1, 0, CPartical::PARTICAL_NORMAL, &D3DXVECTOR3(150, 0, 0), 0);

					if (posResult.y < (g_Aircraft.m_Matrix.pos.y - 50.0f)) break;
				}

				if (GetMouseTrigger(MOUSE_LEFT))
				{
					g_Aircraft.m_nGrenadeThrowTime = GRENADE_THROWTIME;
					g_Aircraft.m_nGrenadeCreateTime = GRENADE_THROWTIME - 25;
					g_Aircraft.m_bGrenadeCreate = true;
				}
			}

			if (g_Aircraft.m_bGrenadeCreate == true)
			{
				if (g_Aircraft.m_nGrenadeCreateTime <= 0)
				{
					if (g_Aircraft.m_GrenadeStore > 0)
					{
						CGrenade::Create(&g_Aircraft.m_Matrix, &speed, GRAVITY, GRENADE_TO_EXPLOSION_TIME);      //グレネード投擲！
						g_Aircraft.m_GrenadeNum--;
						if (g_Aircraft.m_GrenadeNum <= 0)
						{
							g_Aircraft.m_GrenadeNum++;
							g_Aircraft.m_GrenadeStore--;
						}
					}

					else
					{
						if(g_Aircraft.m_GrenadeNum  > 0)
						{
							CGrenade::Create(&g_Aircraft.m_Matrix, &speed, GRAVITY, GRENADE_TO_EXPLOSION_TIME);      //グレネード投擲！
							g_Aircraft.m_GrenadeNum--;
						}
					}

					g_Aircraft.m_bGrenadeCreate = false;
				}
			}
		}
	}

	//弾撃ち切ると,自動リロードモードに入る
	if( (g_Aircraft.m_HandGunNum <= 0) && (g_Aircraft.m_bReloadHandGun == false) )
	{
		g_Aircraft.m_ReloadTimeHandGun = HANDGUN_RELOAD;
		g_Aircraft.m_bReloadHandGun = true;
	}

	if ((g_Aircraft.m_AK47Num <= 0) && (g_Aircraft.m_bReloadAK47 == false))
	{
		g_Aircraft.m_ReloadTimeAK47 = AK47_RELOAD;
		g_Aircraft.m_bReloadAK47 = true;
	}

	g_Aircraft.m_nGrenadeThrowTime--;                                //グレネード投擲硬直時間減少
	if(g_Aircraft.m_nGrenadeThrowTime < 0)
	{
		g_Aircraft.m_nGrenadeThrowTime = 0;
	}
	g_Aircraft.m_nGrenadeCreateTime--;
	if(g_Aircraft.m_nGrenadeCreateTime < 0)
	{
		g_Aircraft.m_nGrenadeCreateTime = 0;
	}
}

void DamageCheck(void)
{
	g_Aircraft.m_NoDamageTime--;

	if(g_Aircraft.m_NoDamageTime < 0)
	{
		g_Aircraft.m_NoDamageTime = 0;
	}
}

void ReloadAction(void)
{
	//リロードアクション
	if(GetKeyboardTrigger(DIK_R))
	{
		if(g_Aircraft.m_GunType == GUNTYPE_HANDGUN)
		{
			if((g_Aircraft.m_HandGunNum != HANDGUNBULLET_MAX) && (g_Aircraft.m_bReloadHandGun == false))
			{
				g_Aircraft.m_ReloadTimeHandGun = HANDGUN_RELOAD;
				g_Aircraft.m_bReloadHandGun = true;
			}
		}

		if (g_Aircraft.m_GunType == GUNTYPE_AK47)
		{
			if ((g_Aircraft.m_AK47Num != AK47BULLET_MAX) && (g_Aircraft.m_bReloadAK47 == false) && (g_Aircraft.m_AK47Store != 0) )
			{
				g_Aircraft.m_ReloadTimeAK47 = AK47_RELOAD;
				g_Aircraft.m_bReloadAK47 = true;
			}
		}
	}

	//リロード過程
	if(g_Aircraft.m_GunType == GUNTYPE_HANDGUN)
	{
		if(g_Aircraft.m_bReloadHandGun == true)
		{
			g_Aircraft.m_ReloadTimeHandGun--;

			if(g_Aircraft.m_ReloadTimeHandGun <= 0)
			{
				g_Aircraft.m_ReloadTimeHandGun = 0;
				g_Aircraft.m_HandGunNum = HANDGUNBULLET_MAX;
				g_Aircraft.m_bReloadHandGun = false;
			}
		}
	}

	if (g_Aircraft.m_GunType == GUNTYPE_AK47)
	{
		if (g_Aircraft.m_bReloadAK47 == true)
		{
			g_Aircraft.m_ReloadTimeAK47--;

			if (g_Aircraft.m_ReloadTimeAK47 <= 0)
			{
				g_Aircraft.m_ReloadTimeAK47 = 0;
				g_Aircraft.m_bReloadAK47 = false;

				if ( (g_Aircraft.m_AK47Store+ g_Aircraft.m_AK47Num) >= AK47BULLET_MAX)
				{
					int NowBulletNum = g_Aircraft.m_AK47Num;
					g_Aircraft.m_AK47Num = AK47BULLET_MAX;
					g_Aircraft.m_AK47Store -= AK47BULLET_MAX - NowBulletNum;
				}

				else
				{
					g_Aircraft.m_AK47Num = g_Aircraft.m_AK47Store + g_Aircraft.m_AK47Num;
					g_Aircraft.m_AK47Store = 0;
				}
			}
		}
	}
}

//モーションフラグ管理
void PlayerInputAction(void)
{
	bool bShootModeFlag = false;
	bool bRunModeFlag = false;

	g_Aircraft.m_MotionBefore = g_Aircraft.m_MotionNow;                                  //モーション保存

	//
	if (g_Aircraft.m_GunType != GUNTYPE_GRENADE)
	{
		g_Aircraft.m_MotionNow = CModel::MOTION_NORMAL;                                      //何もしないならMOTION_NORMAlにする

		if (GetMousePress(MOUSE_RIGHT))
		{
			bShootModeFlag = true;
		}

		if (bShootModeFlag == true)
		{
			g_Aircraft.m_MotionNow = CModel::MOTION_SHOOT;                                   //シュットモーションになる
		}

		if (GetKeyboardTrigger(DIK_LSHIFT))
		{
			g_Aircraft.m_StaminaOver = true;
		}

		if (g_Aircraft.m_fStamina < STAMINA_MIN)
		{
			g_Aircraft.m_StaminaOver = false;
		}

		if (g_Aircraft.m_bmove == true)
		{
			if (g_Aircraft.m_MotionNow == CModel::MOTION_NORMAL) g_Aircraft.m_MotionNow = CModel::MOTION_NORMALMOVE;
			if (g_Aircraft.m_MotionNow == CModel::MOTION_SHOOT) g_Aircraft.m_MotionNow = CModel::MOTION_GUNMOVE;

			if (GetKeyboardPress(DIK_LSHIFT))
			{
				if (g_Aircraft.m_fStamina >= STAMINA_MIN)
				{
					bRunModeFlag = true;
				}
			}

			if ((bRunModeFlag == true) && (g_Aircraft.m_StaminaOver == true))
			{
				g_Aircraft.m_MotionNow = CModel::MOTION_RUN;                                   //ダッシュモーションになる
			}
		}
	}

	//
	else
	{
		if (g_Aircraft.m_nGrenadeThrowTime <= 0)
		{
			g_Aircraft.m_MotionNow = CModel::MOTION_GNORMAL;                                      //何もしないならMOTION_NORMAlにする

			if (GetMousePress(MOUSE_RIGHT))
			{
				bShootModeFlag = true;
			}

			if (bShootModeFlag == true)
			{
				g_Aircraft.m_MotionNow = CModel::MOTION_GSHOOT;                                   //シュットモーションになる
			}

			if (GetKeyboardTrigger(DIK_LSHIFT))
			{
				g_Aircraft.m_StaminaOver = true;
			}

			if (g_Aircraft.m_fStamina < STAMINA_MIN)
			{
				g_Aircraft.m_StaminaOver = false;
			}

			if (g_Aircraft.m_bmove == true)
			{
				if (g_Aircraft.m_MotionNow == CModel::MOTION_GNORMAL) g_Aircraft.m_MotionNow = CModel::MOITON_GNORMALMOVE;
				if (g_Aircraft.m_MotionNow == CModel::MOTION_GSHOOT) g_Aircraft.m_MotionNow = CModel::MOTION_GGUNMOVE;

				if (GetKeyboardPress(DIK_LSHIFT))
				{
					if (g_Aircraft.m_fStamina >= STAMINA_MIN)
					{
						bRunModeFlag = true;
					}
				}

				if ((bRunModeFlag == true) && (g_Aircraft.m_StaminaOver == true))
				{
					g_Aircraft.m_MotionNow = CModel::MOTION_GRUN;                                   //ダッシュモーションになる
				}
			}
		}

		else
		{
			g_Aircraft.m_MotionNow = CModel::MOTION_GTHROW;
		}
	}

	if (g_Aircraft.m_MotionNow != CModel::MOTION_GTHROW)
	{
		g_Aircraft.m_GunTypeOld = g_Aircraft.m_GunType;

		if (GetKeyboardTrigger(DIK_1))
		{
			g_Aircraft.m_GunType = GUNTYPE_HANDGUN;
		}

		if (GetKeyboardTrigger(DIK_2))
		{
			g_Aircraft.m_GunType = GUNTYPE_AK47;
		}

		if (GetKeyboardTrigger(DIK_3))
		{
			g_Aircraft.m_GunType = GUNTYPE_GRENADE;
		}
	}
}

AIRCRAFT *Get_Aircraft(void)
{
	return &g_Aircraft;
}