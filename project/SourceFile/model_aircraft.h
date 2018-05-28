#ifndef _AIRCRAFT_AIRCRAFT_H_
#define _AIRCRAFT_AIRCRAFT_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "motion.h"

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define NUM_AIRCRAFTVERTEX (4)                        //頂点数
#define NUM_AIRCRAFTPOLYGON (2)                       //ポリゴン数

#define STAMINA_MAX (2000)                             //体力ゲージ最大値
#define STAMINA_MIN (50)

#define LIFE_MAX (5)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define Aircraft_NAME "data/player.x"

//-----------------------------------------------------------------------------------------
//構造体変数
//-----------------------------------------------------------------------------------------
typedef enum
{
	GUNTYPE_HANDGUN = 0,
	GUNTYPE_AK47,
	GUNTYPE_GRENADE,
	GUNTYPE_MAX
}GUNTYPE;

typedef struct
{
	D3DXMATRIX m_MtxWorld;                                   // ワールドマトリクス
	WORLD_MATRIX m_Matrix;                           //ワールドマトリクスの変換行列
	D3DXVECTOR3 m_AirPosOld;                                //当り判定用古い座標
	D3DXVECTOR3 m_Speed;                                //速度
	D3DXVECTOR3 m_ModelVec;                            //モデルの向きベクトル
	D3DXVECTOR3 m_GunPos;
	D3DXVECTOR3 m_ShootPoint;
	float m_fTargetAngle;                                   //回転目標の角度
	float m_fTargetAngleV;                                   //垂直目標の角度
	int m_nActionFrame;                                   //アクションのフレーム
	bool m_bModeChange;                                   //モードチェンジフラグ
	bool bJump;                                           //ジャンプフラグ
	bool m_bmove;
	int m_nIDHR;                                           //使用しているメッシュ当り判定のID
	int m_nIDHC;                                           //使用している当り判定円のID
	int m_nLife;
	int m_NoDamageTime;
	int m_nGrenadeThrowTime;
	int m_nGrenadeCreateTime;
	bool m_bGrenadeCreate;
	float m_fStamina;                                        //スタミナの値
	bool m_StaminaOver;

	int m_ReloadTimeHandGun;
	int m_ReloadTimeAK47;
	int m_HandGunNum;
	int m_AK47Num;
	int m_AK47Store;
	int m_GrenadeNum;
	int m_GrenadeStore;
	bool m_bReloadHandGun;
	bool m_bReloadAK47;
	GUNTYPE m_GunType;
	GUNTYPE m_GunTypeOld;

	CModel::CHAR_MODE m_MotionNow;
	CModel::CHAR_MODE m_MotionBefore;
}AIRCRAFT;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言

//-----------------------------------------------------------------------------------------
HRESULT InitAircraft(void);                                                     //ポリゴン(頂点)の初期化
void DrawAircraft(void);                                                        //ポリゴンの描画
void UpdateAircraft(void);                                                      //ポリゴンの更新
void UninitAircraft(void);                                                      //ポリゴンの終了

AIRCRAFT *Get_Aircraft(void);    //戦闘機のデータを取得

#endif