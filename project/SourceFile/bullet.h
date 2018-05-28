#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define NUM_VERTEX (4)                        //頂点数
#define NUM_BULLET (2)						  //ポリゴン数
#define BULLET_MAX (128)
#define BULLETEFFECT_MAX (4096)
#define BULLET_SPEED (10.0f)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define BULLET_TEXTURENAME "data/TEXTURE/bullet.png"
#define BULLETEFFECT_TEXTURENAME "data/TEXTURE/bulletEffect.jpg"

typedef enum
{
	BTYPE_HANDGUN = 0,
	BTYPE_AK47,
	BTYPE_MAX,
}BULLETTYPE;

//構造体
typedef struct
{
	D3DXVECTOR3 posOld;           //座標
	D3DXVECTOR3 speed;         //スピード
	D3DXMATRIX m_BulletWorldMatrix;         //ポリゴンのワールドマトリクス
	WORLD_MATRIX m_BulletMatrix;            //ポリゴンのマトリクスの変換行列
	BULLETTYPE m_Btype;
	bool bUse;                 //使用フラグ
}BULLET;

typedef struct
{
	D3DXMATRIX m_BulletWorldMatrix;         //ポリゴンのワールドマトリクス
	WORLD_MATRIX m_BulletMatrix;            //ポリゴンのマトリクスの変換行列
	float fRadius;           //半径
	float fRadiusValue;      //半径の変化量
	int nAlpha;              //α値
	float fColorAlphaValue;  //色(α)変化量
	int nLife;               //寿命
	BULLETTYPE m_Btype;
	bool bUse;                 //使用フラグ
}BULLETEFFECT;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitBullet(void);                                                     //ポリゴン(頂点)の初期化
void DrawBullet(void);                                                        //ポリゴンの描画
void UpdateBullet(void);                                                      //ポリゴンの更新
void UninitBullet(void);                                                      //ポリゴンの終了
void MakeVertexBulletBG(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //ポリゴン設定
void SetBullet(D3DXVECTOR3, D3DXVECTOR3, BULLETTYPE Btype);
void SetBulletEffect(D3DXVECTOR3 pos, int nLife, BULLETTYPE Btype);                              //

#endif