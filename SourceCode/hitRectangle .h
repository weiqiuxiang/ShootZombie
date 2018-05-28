#ifndef _HITRECT_H_
#define _HITRECT_H_
//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "MeshField.h"

//-----------------------------------------------------------------------------------------
//頂点フォーマット
//-----------------------------------------------------------------------------------------
#define HITRECT_MAX (128)

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define HITRECT_TEXTURENAME "data/TEXTURE/hitRect.png"

typedef struct
{
	D3DXMATRIX m_HitRectWorldMatrix;                        //ポリゴンのワールドマトリクス
	WORLD_MATRIX m_HitRectMatrix;                           //ポリゴンのマトリクスの変換行列
	VERTEX_3D    Vectex[4];                            //四頂点の情報
	VERTEX_3D    VectexC[4];                           //四頂点の当り判定情報
	D3DXVECTOR3 VecXZ;                    //法線と垂直のXZ平面のベクトル
	int nID;                             //使用番号
	bool bUse;                           //使用状態フラグ
	MESHTYPE meshType;
}HITRECT;

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
HRESULT InitHitRect(void);                                                     //ポリゴン(頂点)の初期化
void DrawHitRect(void);                                                        //ポリゴンの描画
void UpdateHitRect(void);                                                      //ポリゴンの更新
void UninitHitRect(void);                                                      //ポリゴンの終了
void MakeVertexHitRect(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //ポリゴン設定
void SetHitRect(const WORLD_MATRIX *pResultMatrix, const D3DXVECTOR2 *HW, MESHTYPE meshType);    //判定ポリゴンセット
HITRECT *Get_HitRect(void);

#endif