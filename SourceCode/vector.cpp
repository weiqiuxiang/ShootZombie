#include <math.h>
#include "vector.h"

//
//二次元ベクトル:左X右の外積
////////////////////////////
float Angle_TwoVector(D3DXVECTOR2 A, D3DXVECTOR2 B)
{
	float ABCross,ABDot;
	float fAngle;
	ABCross = A.x*B.y - A.y*B.x;   //外積
	ABDot = A.x*B.x + A.y*B.y;     //内積
	fAngle = atan2(ABCross, ABDot);

	return fAngle;
}

//
//モデルの角度をターゲット角度に直す(回転速度指定)
//////////////////////////////////////////////
void RotToTargetAngle(float *ModelAngle, float TargetAngle,float fRotSpeed)
{
	float fAngleAbs;                     //絶対値

	fAngleAbs = fabs(*ModelAngle - TargetAngle);

	//角度を丸める
	if (fAngleAbs > PI)//追跡
	{
		fAngleAbs = 2 * PI - fAngleAbs;
	}

	//外積:目標角度のベクトルはモデルベクトルの左にあるか右にあるか判断
	float ABCross = cos(*ModelAngle)*sin(TargetAngle) - sin(*ModelAngle)*cos(TargetAngle);           //外積を求む

																																			   //角度の差が一定程度以上ならモデルを回す
	if (fAngleAbs > (fRotSpeed*1.2))
	{
		//右にある
		if (ABCross > 0)
		{
			*ModelAngle += fRotSpeed;
		}
		//左にある
		else
		{
			*ModelAngle -= fRotSpeed;
		}
	}

	//角度の差が一定程度以下ならモデルの角度をターゲット角度に丸める
	else
	{
		*ModelAngle = TargetAngle;
	}
}

//
//モデルの角度をターゲット角度に直す(回転掛かるフレーム数指定)
///////////////////////////////////
void RotToTargetAngleFrame(float *ModelAngle, float TargetAngle, int nFrame)
{
	float fAngleAbs;                     //絶対値

	fAngleAbs = fabs(*ModelAngle - TargetAngle);

	//角度を丸める
	if (fAngleAbs > PI)//追跡
	{
		fAngleAbs = 2 * PI - fAngleAbs;
	}

	//外積:目標角度のベクトルはモデルベクトルの左にあるか右にあるか判断
	float ABCross = cos(*ModelAngle)*sin(TargetAngle) - sin(*ModelAngle)*cos(TargetAngle);           //外積を求む

	//右にある
	if (ABCross > 0)
	{
		*ModelAngle += fAngleAbs / nFrame;
	}
	//左にある
	else
	{
		*ModelAngle -= fAngleAbs / nFrame;
	}
}