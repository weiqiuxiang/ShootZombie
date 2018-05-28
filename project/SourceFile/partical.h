//-----------------------------------------------------------------------------------------
//プレーヤーヘッドファイル[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#ifndef _PARTICAL_H_
#define _PARTICAL_H_

#include <d3dx9.h>
#include "main.h"
#include "particalCenter.h"
//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_PARTICAL (256)
#define NUM_PARTICALVERTEX (4)                          //頂点数
#define NUM_PARTICALTEXTURE (3)                         //テクスチャ数

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define PARTICALPOLYGON01_TEXTURENAME "data/TEXTURE/partical.png"
#define PARTICALPOLYGONEFFECT_TEXTURENAME "data/TEXTURE/water.jpg"
#define FIREEFFECT_TEXTURENAME "data/TEXTURE/fire.jpg"

//-----------------------------------------------------------------------------------------
//プレーヤーの構造体
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX MtxWorld;                                   // ワールドマトリクス
	WORLD_MATRIX MtxSRT;                           //ワールドマトリクスの変換行列
	D3DXVECTOR3 speed;       //移動量(速度)
	D3DXCOLOR color;         //カラー
	float fRadius;           //半径
	int nAlpha;              //α値
	float fColorAlphaValue;  //色(α)変化量
	int nLife;               //寿命
	bool bUse;               //使用フラグ
	int nID;                 //持っているID番号
}PARTICAL;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitPartical(void);
void UninitPartical(void);
void UpdatePartical(void);
void DrawPartical(void);
void UpdatePolygonPartical(void);
void MakeVertexPolygonPartical(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);
void SetPartical(float fSpeedX,float fSpeedY,float fSpeedZ,float fRadius,float nLife);
void SetFirePartical(PARTICALCENTER *pParticalCenter, float fFirePosX, float fSpeedX, float fSpeedY, float fSpeedZ, float nLife);

#endif
