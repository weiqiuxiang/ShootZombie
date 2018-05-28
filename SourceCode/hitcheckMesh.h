#pragma once
#ifndef _HITCHECKMESH_H_
#define _HITCHECKMESH_H_

#include "main.h"
#include "hitRectangle .h"

//プロトタイプ宣言
float MinAB(float A , float B);          //比較し,小さいの方を返す
bool Rects2D(D3DXVECTOR2 const *fRectA_pos, D3DXVECTOR2 const *fRectA_HW, D3DXVECTOR2 const *fRectB_pos, D3DXVECTOR2 const *fRectB_HW);                //2Dの四角ポリゴンと四角ポリゴンの当り判定
bool Rects3DPointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect);             //点は平面上にあるかどうか
bool Wall3DPointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect);              //点の平面に対する投影は平面上のポリゴンにあるかどうか
bool Rect2D_Vector(const D3DXVECTOR2 *pPoint, const RECT_2D *pRect);
bool Rect3D_Vector(const D3DXVECTOR3 *pPoint, const RECT_3D *pRect);            //3Dにおいてのベクトル当り判定
bool VectorAndRect( D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/);            //ベクトルと判定四角ポリゴンの当り判定の有無
bool VectorAndRect_GroundCheck(D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/,D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/);
void VectorAndRect_TpsCamera(D3DXVECTOR3 *pVectorLookat/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorCamera/*ベクトル終点ポインタ*/);
D3DXVECTOR3 GetVecPlaneIntersection(const D3DXVECTOR3 *pStartPoint, const D3DXVECTOR3 *pEndPoint, const HITRECT *pRect);     //べクトルとポリゴンの貫通点を求める
float DistancePointPlane(const D3DXVECTOR3 *pPoint,const HITRECT *pRect);               //点と平面の距離を取得
float GetElevation(int nID, const D3DXVECTOR3 *pModelPos);      //発生判定のMESH_FLOORの標高を取得
void Quick_Sort(int nLeftID, int nRightID, float *a);    //クイックソート
#endif // !_HITCHECK_H_
