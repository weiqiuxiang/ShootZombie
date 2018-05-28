#pragma once
#ifndef _HITCIRCLE_H_
#define _HITCIRCLE_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "skydome.h"

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define HITCIRCLE_MAX (128)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define HITCIRCLE_TEXTURENAME "data/TEXTURE/hitRect.png"

typedef enum
{
	HBTYPE_NULL = 0,
	HBTYPE_HANDGUN,
	HBTYPE_AK47,
	HBTYPE_ENEMY,
}HITBULLETTYPE;

typedef enum
{
	TYPE_PLAYER,
	TYPE_ENEMY,
	TYPE_OBJ,
	TYPE_MAX
}HRTYPE;

typedef struct
{
	SKYDOME m_skydome;
	D3DXMATRIX m_MtxWorld;                                   // ワールドマトリクス
	WORLD_MATRIX m_Matrix;                           //ワールドマトリクスの変換行列
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_offset;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer = NULL;						//頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
	LPDIRECT3DINDEXBUFFER9 m_pIdxBufferField = NULL;					//インデックスバッファを操作するためのポインタ
	int m_nVertexNum;                    //頂点数
	int m_nIndexNum;                     //インデックス数
	int m_nPolygonNum;                   //ポリゴン数
	D3DCOLOR m_Color;
	int nID;
	int nEnemyID;
	int nCntExplosion;
	HRTYPE m_type;
	HITBULLETTYPE HitCheckType;
	bool bUse;
}HITCIRCLE;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitHitCircle(void);                                                     //ポリゴン(頂点)の初期化
void DrawHitCircle(void);                                                        //ポリゴンの描画
void UpdateHitCircle(void);                                                      //ポリゴンの更新
void UninitHitCircle(void);                                                      //ポリゴンの終了
void MakeVertexHitCircle(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //ポリゴン設定
HRESULT SetHitCirclePlayer(const D3DXVECTOR3 *pPos, float nRadius, const D3DXVECTOR3 *offset);    //判定ポリゴンセット
HRESULT SetHitCircleEnemy(const D3DXVECTOR3 *pPos, float nRadius, const D3DXVECTOR3 *offset,int EnemyID);    //判定ポリゴンセット

void SetCirclePos(const D3DXVECTOR3 *pos,int nID);                            //相応のID番号の円の位置をセットする
HITCIRCLE *Get_HitCircle(int nID);                                            //相応のID番号の円メッシュを返す

#endif