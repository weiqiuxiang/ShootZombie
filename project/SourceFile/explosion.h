#pragma once
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define EXPLOSION_TEXTURENAME "data/TEXTURE/explosion_Tex.png"

//構造体
typedef struct
{
	D3DXMATRIX m_ExplosionWorldMatrix;         //ポリゴンのワールドマトリクス
	WORLD_MATRIX m_ExplosionMatrix;  //ポリゴンのマトリクスの変換行列
	int nFrame;                   //進行フレーム数
	bool bUse;                    //使用フラグ
}EXPLOSION_EF;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitExplosion(void);                                                     //ポリゴン(頂点)の初期化
void DrawExplosion(void);                                                        //ポリゴンの描画
void UpdateExplosion(void);                                                      //ポリゴンの更新
void UninitExplosion(void);                                                      //ポリゴンの終了
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //ポリゴン設定
void SetExplosion(D3DXVECTOR3);       //爆発テクスチャオブジェセット

#endif