#pragma once
#ifndef _HITCIRCLEMESH_H_
#define _HITCIRCLEMESH_H_

#include "main.h"
#include "hitRectangle .h"
#include "bullet.h"

bool CircleAndRect(D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, float fRadius/*判定範囲半径*/);            //円と判定四角ポリゴンの当り判定の有無
bool CircleAndRectRepel(D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, float fRadius/*判定範囲半径*/, float elasticity, D3DXVECTOR3 *pspeedOut);
void CircleAndRay(D3DXVECTOR3 *pOut,float *pfOutDistance,const D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, const D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, const D3DXVECTOR3 *pCirclePos/*円の位置*/, float fRadius/*判定範囲半径*/, const D3DXVECTOR3 *pModelPos);           //レイと円の交差点と距離を求め
bool BulletCircleAndCircle(const D3DXVECTOR3 *pModelpos,float fModelRadius, BULLETTYPE Btype);
bool BulletCircleAndCircleSSV(D3DXVECTOR3 *pOutModelpos, const D3DXVECTOR3 *pModelposOld, const D3DXVECTOR3 *pModelpos, float fModelRadius, BULLETTYPE Btype);
float CircleAndPlaneHitTime(const D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/,const D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, const D3DXVECTOR3 *pPlanePoint,const D3DXVECTOR3 *pPlaneNormal/*平面法線*/,float fRadius/*判定範囲半径*/);                    //返り値:円パーティクルと無限大平面の衝突タイム
bool CircleAndRectSSV(D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, float fRadius/*判定範囲半径*/);
#endif