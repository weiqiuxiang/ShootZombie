#pragma once
#ifndef _TPSAIM_H_
#define _TPSAIM_H_

//ヘッダファイル
#include "main.h"

//プロトタイプ
void RayMesh(D3DXVECTOR3 *pOut,const D3DXVECTOR3 *pVectorLookat/*ベクトル始点ポインタ*/, const D3DXVECTOR3 *pVectorCamera/*ベクトル終点ポインタ*/);
void RayMeshRect(D3DXVECTOR3 *pOut, float *fDistanceOut,const D3DXVECTOR3 *pVectorLookat/*ベクトル始点ポインタ*/, const D3DXVECTOR3 *pVectorCamera/*ベクトル終点ポインタ*/);
void RayCircle(D3DXVECTOR3 *pOut, float *fDistanceOut,const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera, const D3DXVECTOR3 *pModelPos);
void RayCheck(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera, const D3DXVECTOR3 *pModelPos);                   //レイの当り判定チェック
float GetRayDistance(void);
D3DXVECTOR3 GetRayInt(void);
#endif