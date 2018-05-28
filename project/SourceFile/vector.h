#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "main.h"

//プロトタイプ宣言
float Angle_TwoVector(D3DXVECTOR2, D3DXVECTOR2);       //二次元ベクトル:AXBの外積
void RotToTargetAngle(float *ModelAngle/*モデル角度ポインタ*/,float TargetAngle/*目標角度*/, float fRotSpeed/*回転速度*/);     //モデルの角度をターゲット角度に直す(回転速度指定できる)
void RotToTargetAngleFrame(float *ModelAngle/*モデル角度ポインタ*/, float TargetAngle/*目標角度*/, int nFrame/*回転掛かるフレーム*/);     //モデルの角度をターゲット角度にフレーム分の一回転する直す
#endif // !_VECTOR_H_
