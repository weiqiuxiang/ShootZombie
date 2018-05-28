#pragma once
#ifndef _SHADOW_H_
#define _SHADOW_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define SHADOW_TEXTURENAME "data/TEXTURE/shadow.jpg"

//構造体
typedef struct
{
	D3DXMATRIX m_ShadowWorldMatrix; //ポリゴンのワールドマトリクス
	WORLD_MATRIX m_ShadowMatrix;    //ポリゴンのマトリクスの変換行列
	float fStandRadius;                 //地面にぴったり立つ時の影(離れていない)
	float fRadiusValue;                 //半径の変化量(単位距離の変化量)
	int nAlpha;                         //地面にぴったり立つ時のアルファ値(離れていない)
	int nAlphaValue;                    //アルファ値変化量(単位距離の変化量)
}SHADOW;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitShadow(void);                                                     //ポリゴン(頂点)の初期化
void DrawShadow(void);                                                        //ポリゴンの描画
void UpdateShadow(void);                                                      //ポリゴンの更新
void UninitShadow(void);                                                      //ポリゴンの終了
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //ポリゴン設定

#endif