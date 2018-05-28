//-----------------------------------------------------------------------------------------
//パーティクルヘッドファイル[ParticalCenter.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _PARTICALCENTER_H_
#define _PARTICALCENTER_H_

#include "main.h"

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_PARTICALCENTERVERTEX (4)                            //頂点数
#define NUM_PARTICALCENTERPOLYGON (128)						    //矩形数
#define NUM_PARTICALCENTERTEXTURE (2)                           //テクスチャ数

//パーティクルの幅高さ
#define PARTICALCENTER_WIDTH (20.0f)
#define PARTICALCENTER_HEIGHT (20.0f)

//パーティクルのエフェクトの半径
#define MAIN_RADIUS (20.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define PARTICALCENTERPOLYGON01_TEXTURENAME "data/TEXTURE/partical.png"
#define PARTICALCENTERPOLYGON02_TEXTURENAME "data/TEXTURE/particalEffect.jpg"

//-----------------------------------------------------------------------------------------
//パーティクルの構造体
//-----------------------------------------------------------------------------------------
//パーティクルのエフェクト選択
typedef enum
{
	EFFECTMODE_BASE = 0,
	EFFECTMODE_EXPLOSION,
	EFFECTMODE_FIRE,
	EFFECTMODE_MAX,
}EFFECTMODE;

typedef struct
{
	D3DXVECTOR3 pos;                               //パーティクルのポジション
	D3DXVECTOR2 speed;                             //パーティクルのスピード
	D3DXVECTOR2 posOld;                            //パーティクルのポジション
	float fRangeAngel;                             //パーティクルの拡散角度の大きさ
	float fShootAngel;                             //パーティクルの発射角度
	float fRadius;                                 //パーティクルの半径
	float fDistance;                               //パーティクルの飛び基準距離
	bool bUse;                                     //パーティクルを使うかどうか
	EFFECTMODE EffectMode;                         //エフェクトモード

	//パーティクル周囲環境
	float fGravity;                //重力
	float fWindPower;              //風力
	float fWindAngel;              //風が吹く角度
}PARTICALCENTER;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitParticalCenter(void);
void UninitParticalCenter(void);
void UpdateParticalCenter(void);
void MakeVertexPolygonParticalCenter(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);
PARTICALCENTER *GetParticalCenter(void);

void SetParticalCenter( float fposX, float fposY, float fposZ, float fRadius, float fDistance, EFFECTMODE effectMode);  //パーティクルセット

#endif
