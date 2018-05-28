#include "hitcheckMesh.h"

//マクロ
#define NUM_QUICKSORT (10)
#define ONEDEGREE_TO_RADIAN (0.0174533)

float MinAB(float A, float B)
{
	if(A > B) return B;
	else return A;
}

bool Rects2D(D3DXVECTOR2 const *fRectA_pos, D3DXVECTOR2 const *fRectA_HW, D3DXVECTOR2 const *fRectB_pos, D3DXVECTOR2 const *fRectB_HW)
{
	if(
		(fRectA_pos->x <= (fRectB_pos->x + fRectB_HW->x) ) &&
		(fRectA_pos->y <= (fRectB_pos->y + fRectB_HW->y) ) &&
		(fRectB_pos->x <= (fRectA_pos->x + fRectA_HW->x) ) &&
		(fRectB_pos->y <= (fRectA_pos->y + fRectA_HW->y) ) 
		)
	{
		return true;
	}

	else
	{
		return false;
	}
}

float DistancePointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect)
{
	float fDistance;             //ポイントとポリゴンの距離
	D3DXVECTOR3 Vec = *pPoint - pRect->VectexC[0].pos;             //頂点とポイントのベクトル
	fDistance = D3DXVec3Dot(&pRect->VectexC[0].normal,&Vec) / D3DXVec3Length(&pRect->VectexC[0].normal);      //法線とベクトルの内積 / 法線の長さ
	return fabs(fDistance);
}

D3DXVECTOR3 GetVecPlaneIntersection(const D3DXVECTOR3 *pStartPoint,const D3DXVECTOR3 *pEndPoint, const HITRECT *pRect)
{
	//
	//宣言部分
	///////////////////////////
	float dStart, dEnd;           //ベクトルの始点、終点と板ポリゴンの距離
	float s;      //内分比
	D3DXVECTOR3 VecStart, VecEnd;           //頂点と始点、終点成すベクトル
	D3DXVECTOR3 Intersection;        //貫通点
	D3DXVECTOR3 IntersectionVec;        //貫通点ベクトル

	//
	//計算部分
	///////////////////////////
	dStart = DistancePointPlane(pStartPoint, pRect);
	dEnd   = DistancePointPlane(pEndPoint, pRect);
	VecStart = *pStartPoint - pRect->VectexC[0].pos;
	VecEnd = *pEndPoint - pRect->VectexC[0].pos;
	s = dStart / (dStart + dEnd);    //内分比計算
	D3DXVec3Lerp(&IntersectionVec, &VecStart , &VecEnd, s);         //貫通点計算
	Intersection = IntersectionVec + pRect->VectexC[0].pos;

	return Intersection;
}

bool Rects3DPointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect)
{
	D3DXVECTOR3 Vec01, Vec13 , Vec32, Vec20;
	D3DXVECTOR3 Vec0P,Vec1P, Vec2P, Vec3P;
	D3DXVECTOR3 n[4],Normarlize;

	//
	//計算部分
	////////////////////////
	//板ポリゴンの右手回りベクトル
	Vec01 = pRect->VectexC[1].pos - pRect->VectexC[0].pos;
	Vec13 = pRect->VectexC[3].pos - pRect->VectexC[1].pos;
	Vec32 = pRect->VectexC[2].pos - pRect->VectexC[3].pos;
	Vec20 = pRect->VectexC[0].pos - pRect->VectexC[2].pos;

	//頂点とポイントのベクトル
	Vec1P = *pPoint - pRect->VectexC[1].pos;
	Vec2P = *pPoint - pRect->VectexC[2].pos;
	Vec3P = *pPoint - pRect->VectexC[3].pos;
	Vec0P = *pPoint - pRect->VectexC[0].pos;

	//外積を求め
	D3DXVec3Cross(&n[0], &Vec01, &Vec1P);
	D3DXVec3Cross(&n[1], &Vec13, &Vec3P);
	D3DXVec3Cross(&n[2], &Vec32, &Vec2P);
	D3DXVec3Cross(&n[3], &Vec20, &Vec0P);

	//精度調整
	for (int i = 0; i < 4; i++)
	{
		//単位ベクトル化
		D3DXVec3Normalize(&n[i], &n[i]);
	}

	Normarlize = pRect->VectexC[0].normal;

	float Dot[4];
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		Dot[nCnt] = D3DXVec3Dot(&n[nCnt],&Normarlize);             //結果法線とポリゴンの法線の内積を求め
	}

	//判定(4つ結果法線ベクトル、各とポリゴン法線の内積の結果、つまりcos()の値は無限に1に近いなら,判定成立)
	if(
		(Dot[0] > 0.99) &&
		(Dot[1] > 0.99) &&
		(Dot[2] > 0.99) &&
		(Dot[3] > 0.99)
		)
	{
		return true;
	}

	return false;
}

bool Rect2D_Vector(const D3DXVECTOR2 *pPoint, const RECT_2D *pRect)
{
	D3DXVECTOR2 Vec01, Vec13, Vec32, Vec20;
	D3DXVECTOR2 Vec0P, Vec1P, Vec2P, Vec3P;
	float Value[4];

	RECT_2D Rect2D = *pRect;

	//計算用ベクトル算出
	Vec01 = Rect2D.p[1] - Rect2D.p[0];
	Vec13 = Rect2D.p[3] - Rect2D.p[1];
	Vec32 = Rect2D.p[2] - Rect2D.p[3];
	Vec20 = Rect2D.p[0] - Rect2D.p[2];

	Vec0P = *pPoint - Rect2D.p[0];
	Vec1P = *pPoint - Rect2D.p[1];
	Vec2P = *pPoint - Rect2D.p[2];
	Vec3P = *pPoint - Rect2D.p[3];

	Value[0] = D3DXVec2CCW(&Vec01, &Vec0P);
	Value[1] = D3DXVec2CCW(&Vec13, &Vec1P);
	Value[2] = D3DXVec2CCW(&Vec32, &Vec3P);
	Value[3] = D3DXVec2CCW(&Vec20, &Vec2P);

	//判定
	if (
		((Value[0] <= 0) &&
		 (Value[1] <= 0) &&
		 (Value[2] <= 0) &&
		 (Value[3] <= 0))||

		((Value[0] >= 0) &&
		(Value[1] >= 0) &&
		(Value[2] >= 0) &&
		(Value[3] >= 0)) 
		)
	{
		return true;
	}

	return false;
}

bool Rect3D_Vector(const D3DXVECTOR3 *pPoint, const RECT_3D *pRect)
{
	int nCntTure = 0;
	//
	//小数点以下をある程度丸めする
	////////////////////////////////////////////////
	D3DXVECTOR3 Point = *pPoint;
	RECT_3D Rect = *pRect;

	//
	//XY平面の判定
	///////////////////////////////////////////////////
	//pointとRect2Dを作る
	RECT_2D Rect2DXY;
	D3DXVECTOR2 PointXY = D3DXVECTOR2(Point.x, Point.y);
	Rect2DXY.p[0] = D3DXVECTOR2(Rect.p0.x, Rect.p0.y);
	Rect2DXY.p[1] = D3DXVECTOR2(Rect.p1.x, Rect.p1.y);
	Rect2DXY.p[2] = D3DXVECTOR2(Rect.p2.x, Rect.p2.y);
	Rect2DXY.p[3] = D3DXVECTOR2(Rect.p3.x, Rect.p3.y);

	//2d四辺形とポイントの当り判定を行う
	if (Rect2D_Vector(&PointXY, &Rect2DXY) == true)
	{
		nCntTure++;
	}

	//
	//YZ平面の判定
	///////////////////////////////////////////////////
	//pointとRect2Dを作る
	RECT_2D Rect2DYZ;
	D3DXVECTOR2 PointYZ = D3DXVECTOR2(Point.y, Point.z);
	Rect2DYZ.p[0] = D3DXVECTOR2(Rect.p0.y, Rect.p0.z);
	Rect2DYZ.p[1] = D3DXVECTOR2(Rect.p1.y, Rect.p1.z);
	Rect2DYZ.p[2] = D3DXVECTOR2(Rect.p2.y, Rect.p2.z);
	Rect2DYZ.p[3] = D3DXVECTOR2(Rect.p3.y, Rect.p3.z);
	//2d四辺形とポイントの当り判定を行う
	if (Rect2D_Vector(&PointYZ, &Rect2DYZ) == true)
	{
		nCntTure++;
	}

	//
	//XY平面の判定
	///////////////////////////////////////////////////
	//pointとRect2Dを作る
	RECT_2D Rect2DXZ;
	D3DXVECTOR2 PointXZ = D3DXVECTOR2(Point.x, Point.z);
	Rect2DXZ.p[0] = D3DXVECTOR2(Rect.p0.x, Rect.p0.z);
	Rect2DXZ.p[1] = D3DXVECTOR2(Rect.p1.x, Rect.p1.z);
	Rect2DXZ.p[2] = D3DXVECTOR2(Rect.p2.x, Rect.p2.z);
	Rect2DXZ.p[3] = D3DXVECTOR2(Rect.p3.x, Rect.p3.z);
	//2d四辺形とポイントの当り判定を行う
	if (Rect2D_Vector(&PointXZ, &Rect2DXZ) == true)
	{
		nCntTure++;
	}

	if (nCntTure == 3) return true;
	return false;
}

bool Wall3DPointPlane(const D3DXVECTOR3 *pPoint, const HITRECT *pRect)
{
	RECT_2D RectXY, RectYZ;           //ポリゴンのXY平面の投影とYZ平面の投影
	D3DXVECTOR2 PointXY,PointYZ;                //判定点のXY平面の投影とYZ平面の投影
	//フラグ
	bool bXYCheck = false;
	bool bYZCheck = false;

	for (int i = 0; i < 4; i++) 
	{
		RectXY.p[i] = D3DXVECTOR2(pRect->VectexC[i].pos.x, pRect->VectexC[i].pos.y);
		RectYZ.p[i] = D3DXVECTOR2(pRect->VectexC[i].pos.y, pRect->VectexC[i].pos.z);
	}

	PointXY = D3DXVECTOR2(pPoint->x, pPoint->y);
	PointYZ = D3DXVECTOR2(pPoint->y, pPoint->z);

	bXYCheck = Rect2D_Vector(&PointXY,&RectXY);
	bYZCheck = Rect2D_Vector(&PointYZ, &RectYZ);

	if(bXYCheck && bYZCheck)
	{
		return true;
	}

	return false;
}

bool VectorAndRect(D3DXVECTOR3 *pVectorStart, D3DXVECTOR3 *pVectorEnd)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //結果ベクトルを求め
	D3DXVECTOR3 MoveSpeedVector;             //移動量ベクトル
	float WallMoveSpeed;                         //壁に沿う移動量
	HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bCheck = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター

	//移動量ベクトル計算
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y方向の値を0にする

	//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			float a, b;
			D3DXVECTOR3 VecA = *pVectorStart - (pHitRect + nCnt)->VectexC[0].pos;
			D3DXVECTOR3 VecB = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].pos;
			a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
			b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積

																					  //法線判定成立
			if ((a*b) <= 0)
			{
				D3DXVECTOR3 Intersection = GetVecPlaneIntersection(pVectorStart, pVectorEnd, (pHitRect + nCnt));    //貫通点を求める
				//斜め
				if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
				{
					//貫通点が平面上にあるか判定する
					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
					{
						pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001;
						bCheck = true;
					}
				}

				//平面ORウォール
				if (
					((pHitRect + nCnt)->meshType == MESH_WALL) ||
					((pHitRect + nCnt)->meshType == MESH_FLOOR)
					)
				{
					//
					//XZ平面で正方形と正方形判定
					//////////////////////////////////////
					D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //変数宣言

																			 //四角Aの計算
					RectA_Pos.x = MinAB(pVectorStart->x, pVectorEnd->x);
					RectA_Pos.y = MinAB(pVectorStart->z, pVectorEnd->z);
					RectA_HW.x = fabs(pVectorEnd->x - pVectorStart->x);
					RectA_HW.y = fabs(pVectorEnd->z - pVectorStart->z);

					//四角Bの計算
					RectB_Pos.x = MinAB((pHitRect + nCnt)->VectexC[0].pos.x, (pHitRect + nCnt)->VectexC[3].pos.x);
					RectB_Pos.y = MinAB((pHitRect + nCnt)->VectexC[0].pos.z, (pHitRect + nCnt)->VectexC[3].pos.z);
					RectB_HW.x = fabs((pHitRect + nCnt)->VectexC[0].pos.x - (pHitRect + nCnt)->VectexC[3].pos.x);
					RectB_HW.y = fabs((pHitRect + nCnt)->VectexC[0].pos.z - (pHitRect + nCnt)->VectexC[3].pos.z);

					//メッシュのタイプはFLOORの時
					if ((pHitRect + nCnt)->meshType == MESH_FLOOR)
					{
						if (Rects2D(&RectA_Pos, &RectA_HW, &RectB_Pos, &RectB_HW))
						{
							pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001;
							bCheck = true;
						}
					}

					//メッシュのタイプはWALLの時
					else
					{
						//頂点のY座標をソートする
						float aSort[4];
						bool bYCheck = false;
						for (int j = 0; j < 4; j++)
						{
							aSort[j] = (pHitRect + nCnt)->VectexC[j].pos.y;
						}

						Quick_Sort(0, 3, aSort);

						if ((Intersection.y >= aSort[0]) && (Intersection.y <= aSort[3])) bYCheck = true;

						//四角当り判定
						if (Rects2D(&RectA_Pos, &RectA_HW, &RectB_Pos, &RectB_HW) && bYCheck)
						{
							pVectorEnd->x = pVectorStart->x;
							pVectorEnd->z = pVectorStart->z;
							bCheck = true;
						}
					}
				}
			}

			//法線判定成立しない
			else
			{
				continue;
			}
		}
	}

	return bCheck;
}

bool VectorAndRect_GroundCheck(D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //結果ベクトルを求め
	D3DXVECTOR3 MoveSpeedVector;             //移動量ベクトル
	float WallMoveSpeed;                         //壁に沿う移動量
	HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bGround = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター
	int nWallCollision = 0;  //MESH_WALLとの衝突数

	//移動量ベクトル計算
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y方向の値を0にする

	//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			float a, b;
			D3DXVECTOR3 VecA = *pVectorStart - (pHitRect + nCnt)->VectexC[0].pos;
			D3DXVECTOR3 VecB = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].pos;
			a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
			b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積

																					  //法線判定成立
			if ((a*b) <= 0)
			{
				D3DXVECTOR3 Intersection = GetVecPlaneIntersection(pVectorStart, pVectorEnd, (pHitRect + nCnt));    //貫通点を求める
				//斜め
				if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
				{
					//貫通点が平面上にあるか判定する
					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
					{
						pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001;
						bGround = true;
					}
				}

				//平面ORウォール
				if (
					((pHitRect + nCnt)->meshType == MESH_WALL) ||
					((pHitRect + nCnt)->meshType == MESH_FLOOR)
					)
				{
					//
					//XZ平面で正方形と正方形判定
					//////////////////////////////////////
					D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //変数宣言

																			 //四角Aの計算
					RectA_Pos.x = MinAB(pVectorStart->x, pVectorEnd->x);
					RectA_Pos.y = MinAB(pVectorStart->z, pVectorEnd->z);
					RectA_HW.x = fabs(pVectorEnd->x - pVectorStart->x);
					RectA_HW.y = fabs(pVectorEnd->z - pVectorStart->z);

					//四角Bの計算
					RectB_Pos.x = MinAB((pHitRect + nCnt)->VectexC[0].pos.x, (pHitRect + nCnt)->VectexC[3].pos.x);
					RectB_Pos.y = MinAB((pHitRect + nCnt)->VectexC[0].pos.z, (pHitRect + nCnt)->VectexC[3].pos.z);
					RectB_HW.x = fabs((pHitRect + nCnt)->VectexC[0].pos.x - (pHitRect + nCnt)->VectexC[3].pos.x);
					RectB_HW.y = fabs((pHitRect + nCnt)->VectexC[0].pos.z - (pHitRect + nCnt)->VectexC[3].pos.z);

					//メッシュのタイプはFLOORの時
					if ((pHitRect + nCnt)->meshType == MESH_FLOOR)
					{
						if (Rects2D(&RectA_Pos, &RectA_HW, &RectB_Pos, &RectB_HW))
						{
							pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001;
							bGround = true;
						}
					}

					//メッシュのタイプはWALLの時
					else
					{
						//頂点のY座標をソートする
						float aSort[4];
						bool bYCheck = false;
						for (int j = 0; j < 4; j++)
						{
							aSort[j] = (pHitRect + nCnt)->VectexC[j].pos.y;
						}

						Quick_Sort(0, 3, aSort);

						if ((Intersection.y >= aSort[0]) && (Intersection.y <= aSort[3])) bYCheck = true;

						//四角当り判定
						if (Rects2D(&RectA_Pos, &RectA_HW, &RectB_Pos, &RectB_HW) && bYCheck)
						{
							pVectorEnd->x = pVectorStart->x;
							pVectorEnd->z = pVectorStart->z;

							//XZ平面での移動速度を壁沿い移動ベクトルと移動量ベクトルの内積で算出
							WallMoveSpeed = D3DXVec3Dot(&MoveSpeedVector, &(pHitRect + nCnt)->VecXZ);

							*pVectorEnd += (pHitRect + nCnt)->VecXZ * WallMoveSpeed;                    //壁沿い移動

							nWallCollision++;
						}
					}
				}
			}

			//法線判定成立しない
			else
			{
				continue;
			}
		}

		//複数の壁と判定がある場合,前の座標に戻す
		if(nWallCollision > 1)
		{
			*pVectorEnd = *pVectorStart;
		}

	}

	return bGround;
}

//カメラとメッシュの判定
void VectorAndRect_TpsCamera(D3DXVECTOR3 *pVectorLookat/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorCamera/*ベクトル終点ポインタ*/)
{
	D3DXVECTOR3 resultVector = *pVectorCamera - *pVectorLookat;           //結果ベクトルを求め
	
	HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bCheck = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター
	int nWallCollision = 0;  //MESH_WALLとの衝突数
	float fDistance = 0;                                //カメラと衝突平面の距離
	float fMinDistance = 100000;                           //カメラと多平面の衝突に対してカメラと平面の最短距離

	//カメラ-注視点ベクトルを単位化
	D3DXVec3Normalize(&resultVector,&resultVector);

	//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			float a, b;
			D3DXVECTOR3 VecA = *pVectorLookat - (pHitRect + nCnt)->VectexC[0].pos;
			D3DXVECTOR3 VecB = *pVectorCamera - (pHitRect + nCnt)->VectexC[0].pos;
			a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
			b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積

			//法線判定成立
			if ((a*b) <= 0)
			{
				D3DXVECTOR3 Intersection = GetVecPlaneIntersection(pVectorLookat, pVectorCamera, (pHitRect + nCnt));    //貫通点を求める

				//最短注視点--衝突メッシュ距離計算
				fDistance = DistancePointPlane(pVectorLookat,(pHitRect + nCnt));          //注視点と衝突メッシュの距離
				
				//斜め
				if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
				{
					//貫通点が平面上にあるか判定する
					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
					{
						if (fDistance < fMinDistance)
						{
							fMinDistance = fDistance;                              //最短距離保存
							*pVectorCamera = Intersection - resultVector*0.1;     //微修正
						}
						bCheck = true;
					}
				}

				//平面ORウォール
				if (
					((pHitRect + nCnt)->meshType == MESH_WALL) ||
					((pHitRect + nCnt)->meshType == MESH_FLOOR)
					)
				{
					//
					//XZ平面で正方形と正方形判定
					//////////////////////////////////////
					D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //変数宣言

																			 //四角Aの計算
					RectA_Pos.x = MinAB(pVectorLookat->x, pVectorCamera->x);
					RectA_Pos.y = MinAB(pVectorLookat->z, pVectorCamera->z);
					RectA_HW.x = fabs(pVectorCamera->x - pVectorLookat->x);
					RectA_HW.y = fabs(pVectorCamera->z - pVectorLookat->z);

					//四角Bの計算
					RectB_Pos.x = MinAB((pHitRect + nCnt)->VectexC[0].pos.x, (pHitRect + nCnt)->VectexC[3].pos.x);
					RectB_Pos.y = MinAB((pHitRect + nCnt)->VectexC[0].pos.z, (pHitRect + nCnt)->VectexC[3].pos.z);
					RectB_HW.x = fabs((pHitRect + nCnt)->VectexC[0].pos.x - (pHitRect + nCnt)->VectexC[3].pos.x);
					RectB_HW.y = fabs((pHitRect + nCnt)->VectexC[0].pos.z - (pHitRect + nCnt)->VectexC[3].pos.z);

					//メッシュのタイプはFLOORの時
					if ((pHitRect + nCnt)->meshType == MESH_FLOOR)
					{
						if (Rects2D(&RectA_Pos, &RectA_HW, &RectB_Pos, &RectB_HW))
						{
							//
							if (fDistance < fMinDistance)
							{
								fMinDistance = fDistance;                              //最短距離保存
								*pVectorCamera = Intersection - resultVector*1.0;     //微修正
							}
							bCheck = true;
						}
					}

					//メッシュのタイプはWALLの時
					else
					{
						//頂点のY座標をソートする
						float aSort[4];
						bool bYCheck = false;
						for (int j = 0; j < 4; j++)
						{
							aSort[j] = (pHitRect + nCnt)->VectexC[j].pos.y;
						}

						Quick_Sort(0, 3, aSort);

						if ((Intersection.y >= aSort[0]) && (Intersection.y <= aSort[3])) bYCheck = true;

						//四角当り判定
						if (Rects2D(&RectA_Pos, &RectA_HW, &RectB_Pos, &RectB_HW) && bYCheck)
						{
							if (fDistance < fMinDistance)
							{
								fMinDistance = fDistance;                              //最短距離保存
								*pVectorCamera = Intersection - resultVector*1.0;     //微修正
							}
							bCheck = true;
						}
					}
				}
			}

			//法線判定成立しない
			else
			{
				continue;
			}
		}
	}
}

float GetElevation(int nID,const D3DXVECTOR3 *pModelPos)
{
	HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	D3DXVECTOR3 Vec = (pHitRect + nID)->VectexC[0].pos - *pModelPos;   //計算用ベクトル
	float fElevation;   //標高

	//発生判定の地面が存在する
	if( (pHitRect + nID)->bUse == true )
	{
		//標高計算
		fElevation = (pHitRect + nID)->VectexC[0].pos.y + ((pHitRect + nID)->VectexC[0].normal.x*Vec.x + (pHitRect + nID)->VectexC[0].normal.z*Vec.z) / (pHitRect + nID)->VectexC[0].normal.y;
	}

	return fElevation;
}


void Quick_Sort(int nLeftID, int nRightID,float *a)
{
	//---------------------------
	//変数宣言
	//---------------------------
	int nCnt = 0;           //カウント
	int nTotalNum = nRightID - nLeftID + 1;              //総数
	int nMiddleID = (nLeftID + nRightID) / 2;
	int nLeftNum_BE = nMiddleID - nLeftID;           //分割前の中間値の左側の配列メンバ数
	int nRightNum_BE = nRightID - nMiddleID;         //分割前の中間値の右側の配列メンバ数
	int nLeftNum_AF = 0; //分割後の中間値の左側の配列メンバ数
	int nRightNum_AF = 0; //分割後の中間値の右側の配列メンバ数

	float aDistanceL[NUM_QUICKSORT];                    //パーティクルからカメラまでの距離左配列
	float aDistanceR[NUM_QUICKSORT];                    //パーティクルからカメラまでの距離右配列

	//---------------------------
	//左と右分割
	//---------------------------
	for (int i = 1; i <= nLeftNum_BE; i++)
	{
		//---------------------------
		//中間値前のメンバ値チェック
		//---------------------------
		//左配列に入れる
		if (a[nMiddleID - i] < a[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = a[nMiddleID - i];          //距離配列に入れる
			nLeftNum_AF++;                                         //メンバ数カウントアップ
		}
		//右配列に入れる
		else
		{
			aDistanceR[nRightNum_AF] = a[nMiddleID - i];  //配列に入れる
			nRightNum_AF++;                                         //メンバ数カウントアップ
		}

		//---------------------------
		//中間値後ろのメンバ値チェック
		//---------------------------
		//左配列に入れる
		if (a[nMiddleID + i] < a[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = a[nMiddleID + i];          //距離配列に入れる
			nLeftNum_AF++;                                         //メンバ数カウントアップ
		}
		//右配列に入れる
		else
		{
			aDistanceR[nRightNum_AF] = a[nMiddleID + i];  //配列に入れる
			nRightNum_AF++;                                         //メンバ数カウントアップ
		}
	}

	//総数が偶数の場合
	if (nRightNum_BE > nLeftNum_BE)
	{
		//左配列に入れる
		if (a[nRightID] < a[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = a[nRightID];          //距離配列に入れる
			nLeftNum_AF++;                                         //メンバ数カウントアップ
		}
		//右配列に入れる
		else
		{
			aDistanceR[nRightNum_AF] = a[nRightID];  //配列に入れる
			nRightNum_AF++;                                         //メンバ数カウントアップ
		}
	}

	//----------------------------
	//分割の後の配列を配列に反映する
	//----------------------------
	nCnt = nLeftID;              //カウント初期化
	float MIDistance = a[nMiddleID];      //中間値保存

											   //左部分
	for (int j = 0; j < nLeftNum_AF; j++)
	{
		a[nCnt] = aDistanceL[j];        //距離代入
		nCnt++;            //カウントアップ
	}

	//中間値
	a[nCnt] = MIDistance;        //距離代入
	nCnt++;       //カウントアップ

				  //右部分
	for (int j = 0; j < nRightNum_AF; j++)
	{
		a[nCnt] = aDistanceR[j];        //距離代入
		nCnt++;            //カウントアップ
	}

	//------------------
	//再帰部分
	//------------------
	//左部分
	if (nLeftNum_AF > 1)
	{
		Quick_Sort(nLeftID, nLeftID + nLeftNum_AF - 1,a);
	}

	//右部分
	if (nRightNum_AF > 1)
	{
		Quick_Sort(nRightID - nRightNum_AF + 1, nRightID,a);
	}
}