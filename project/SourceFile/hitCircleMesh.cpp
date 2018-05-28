#include "hitCircleMesh.h"
#include "hitcheckMesh.h"
#include "hitCircle.h"

bool CircleAndRect(D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, float fRadius/*判定範囲半径*/)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //結果ベクトルを求め
	D3DXVECTOR3 MoveSpeedVector;             //移動量ベクトル
	float WallMoveSpeed;                         //壁に沿う移動量
	HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bGround = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター
	int nWallCollision = 0;  //MESH_WALLとの衝突数
	bool bSlope = false;
	bool bWall = false;

	//移動量ベクトル計算
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y方向の値を0にする

											//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			//点と平面の距離を求め
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//点と平面の距離は判定半径以下なら
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //円の貫通点
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //円の貫通点の古い座標
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積


				//法線判定成立
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //貫通点を求める

					//平面ORウォール
					if (((pHitRect + nCnt)->meshType == MESH_FLOOR))
					{
						//
						//XZ平面で正方形と正方形判定
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //変数宣言

						//四角Aの計算
						RectA_Pos.x = MinAB(IntersectionCircleOld.x, IntersectionCircle.x);
						RectA_Pos.y = MinAB(IntersectionCircleOld.z, IntersectionCircle.z);
						RectA_HW.x = fabs(IntersectionCircle.x - IntersectionCircleOld.x);
						RectA_HW.y = fabs(IntersectionCircle.z - IntersectionCircleOld.z);

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
								pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001 + fRadius;

								bGround = true;
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

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			//点と平面の距離を求め
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//点と平面の距離は判定半径以下なら
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //円の貫通点
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //円の貫通点の古い座標
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積


				//法線判定成立
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //貫通点を求める

					//斜め
					if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
					{
						//貫通点が平面上にあるか判定する
						if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
						{
							WallMoveSpeed = D3DXVec3Length(&MoveSpeedVector);                     //スピードの大きさを計算する
							D3DXVec3Normalize(&MoveSpeedVector, &MoveSpeedVector);                           //ベクトル単位化

							Intersection += WallMoveSpeed*cos((pHitRect + nCnt)->m_HitRectMatrix.rot.x)*MoveSpeedVector;                //移動後の貫通点計算

							Intersection.y = GetElevation((pHitRect + nCnt)->nID, &Intersection) + 0.001; //貫通点の高さ計算

							*pVectorEnd = Intersection + fRadius*(pHitRect + nCnt)->VectexC[0].normal;             //終点ベクトル位置算出

							bSlope = true;
							bGround = true;
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
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			//点と平面の距離を求め
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//点と平面の距離は判定半径以下なら
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //円の貫通点
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //円の貫通点の古い座標
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積


				//法線判定成立
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //貫通点を求める

					if ((pHitRect + nCnt)->meshType == MESH_WALL)
					{
						//
						//XZ平面で正方形と正方形判定
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //変数宣言

						//四角Aの計算
						RectA_Pos.x = MinAB(IntersectionCircleOld.x, IntersectionCircle.x);
						RectA_Pos.y = MinAB(IntersectionCircleOld.z, IntersectionCircle.z);
						RectA_HW.x = fabs(IntersectionCircle.x - IntersectionCircleOld.x);
						RectA_HW.y = fabs(IntersectionCircle.z - IntersectionCircleOld.z);

						//四角Bの計算
						RectB_Pos.x = MinAB((pHitRect + nCnt)->VectexC[0].pos.x, (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_Pos.y = MinAB((pHitRect + nCnt)->VectexC[0].pos.z, (pHitRect + nCnt)->VectexC[3].pos.z);
						RectB_HW.x = fabs((pHitRect + nCnt)->VectexC[0].pos.x - (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_HW.y = fabs((pHitRect + nCnt)->VectexC[0].pos.z - (pHitRect + nCnt)->VectexC[3].pos.z);

						//メッシュのタイプはWALLの時

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
							bWall = true;
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
	//複数の壁と判定がある場合,前の座標に戻す
	if ( (nWallCollision > 1) || (bSlope && bWall))
	{
		*pVectorEnd = *pVectorStart;
	}

	return bGround;
}

bool CircleAndRectSSV(D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, float fRadius/*判定範囲半径*/)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //結果ベクトルを求め
	D3DXVECTOR3 MoveSpeedVector;             //移動量ベクトル
	float WallMoveSpeed;                         //壁に沿う移動量
	HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bGround = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター
	int nWallCollision = 0;  //MESH_WALLとの衝突数
	bool bSlope = false;
	bool bWall = false;

	//移動量ベクトル計算
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y方向の値を0にする

											//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			//メッシュのタイプはFLOORの時
			if ((pHitRect + nCnt)->meshType == MESH_FLOOR)
			{
				float timePlane = CircleAndPlaneHitTime(pVectorStart, pVectorEnd, &(pHitRect + nCnt)->VectexC[0].pos, &(pHitRect + nCnt)->VectexC[0].normal, fRadius);

				if (timePlane >= 0)
				{
					D3DXVECTOR3 Intersection = *pVectorStart + timePlane*(*pVectorEnd - *pVectorStart);

					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))                                 //点が平面上にあるか
					{
						pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, &Intersection) + 0.001 + fRadius;
						bGround = true;
					}
				}
			}
		}
	}

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
			{
				float timePlane = CircleAndPlaneHitTime(pVectorStart, pVectorEnd, &(pHitRect + nCnt)->VectexC[0].pos, &(pHitRect + nCnt)->VectexC[0].normal, fRadius);

				if (timePlane >= 0)
				{
					D3DXVECTOR3 Intersection = *pVectorStart + timePlane*(*pVectorEnd - *pVectorStart);                                   //貫通点を求める

					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))                                 //点が平面上にあるか
					{
						WallMoveSpeed = D3DXVec3Length(&MoveSpeedVector);                     //スピードの大きさを計算する
						D3DXVec3Normalize(&MoveSpeedVector, &MoveSpeedVector);                           //ベクトル単位化

						Intersection += WallMoveSpeed*cos((pHitRect + nCnt)->m_HitRectMatrix.rot.x)*MoveSpeedVector;                //移動後の貫通点計算

						Intersection.y = GetElevation((pHitRect + nCnt)->nID, &Intersection) + 0.001; //貫通点の高さ計算

						*pVectorEnd = Intersection + fRadius*(pHitRect + nCnt)->VectexC[0].normal;             //終点ベクトル位置算出

						bSlope = true;
						bGround = true;
					}
				}
			}
		}
	}

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			if ((pHitRect + nCnt)->meshType == MESH_WALL)
			{
				float timePlane = CircleAndPlaneHitTime(pVectorStart, pVectorEnd, &(pHitRect + nCnt)->VectexC[0].pos, &(pHitRect + nCnt)->VectexC[0].normal, fRadius);

				if (timePlane >= 0)
				{
					D3DXVECTOR3 Intersection = *pVectorStart + timePlane*(*pVectorEnd - *pVectorStart);                                   //貫通点を求める

					if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))                                 //点が平面上にあるか
					{
						pVectorEnd->x = pVectorStart->x;
						pVectorEnd->z = pVectorStart->z;

						//XZ平面での移動速度を壁沿い移動ベクトルと移動量ベクトルの内積で算出
						WallMoveSpeed = D3DXVec3Dot(&MoveSpeedVector, &(pHitRect + nCnt)->VecXZ);

						*pVectorEnd += (pHitRect + nCnt)->VecXZ * WallMoveSpeed;                    //壁沿い移動
						nWallCollision++;
						bWall = true;
					}
				}
			}
		}
	}
	//複数の壁と判定がある場合,前の座標に戻す
	if ((nWallCollision > 1) || (bSlope && bWall))
	{
		*pVectorEnd = *pVectorStart;
	}

	return bGround;
}

bool CircleAndRectRepel(D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, float fRadius/*判定範囲半径*/,float elasticity , D3DXVECTOR3 *pspeedOut)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //結果ベクトルを求め
	D3DXVECTOR3 MoveSpeedVector;             //移動量ベクトル
	D3DXVECTOR3 Normalize = D3DXVECTOR3(0.0f,0.0f,0.0f);
	float WallMoveSpeed;                         //壁に沿う移動量
	HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bGround = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター
	int nWallCollision = 0;  //MESH_WALLとの衝突数
	bool bSlope = false;
	bool bWall = false;

	//移動量ベクトル計算
	MoveSpeedVector = resultVector;
	MoveSpeedVector.y = 0.0f;               //Y方向の値を0にする

											//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			//点と平面の距離を求め
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//点と平面の距離は判定半径以下なら
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //円の貫通点
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //円の貫通点の古い座標
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積


																						  //法線判定成立
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //貫通点を求める

																																		   //平面ORウォール
					if (((pHitRect + nCnt)->meshType == MESH_FLOOR))
					{
						//
						//XZ平面で正方形と正方形判定
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //変数宣言

																				 //四角Aの計算
						RectA_Pos.x = MinAB(IntersectionCircleOld.x, IntersectionCircle.x);
						RectA_Pos.y = MinAB(IntersectionCircleOld.z, IntersectionCircle.z);
						RectA_HW.x = fabs(IntersectionCircle.x - IntersectionCircleOld.x);
						RectA_HW.y = fabs(IntersectionCircle.z - IntersectionCircleOld.z);

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
								pVectorEnd->y = GetElevation((pHitRect + nCnt)->nID, pVectorEnd) + 0.001 + fRadius;
								Normalize = (pHitRect + nCnt)->VectexC[0].normal;
								bGround = true;
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

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			//点と平面の距離を求め
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//点と平面の距離は判定半径以下なら
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //円の貫通点
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //円の貫通点の古い座標
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積


																						  //法線判定成立
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //貫通点を求める

																																		   //斜め
					if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
					{
						//貫通点が平面上にあるか判定する
						if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
						{
							WallMoveSpeed = D3DXVec3Length(&MoveSpeedVector);                     //スピードの大きさを計算する
							D3DXVec3Normalize(&MoveSpeedVector, &MoveSpeedVector);                           //ベクトル単位化

							Intersection += WallMoveSpeed*cos((pHitRect + nCnt)->m_HitRectMatrix.rot.x)*MoveSpeedVector;                //移動後の貫通点計算

							Intersection.y = GetElevation((pHitRect + nCnt)->nID, &Intersection) + 0.001; //貫通点の高さ計算

							*pVectorEnd = Intersection + fRadius*(pHitRect + nCnt)->VectexC[0].normal;             //終点ベクトル位置算出
							Normalize = (pHitRect + nCnt)->VectexC[0].normal;
							bSlope = true;
							bGround = true;
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
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		if ((pHitRect + nCnt)->bUse == true)
		{
			//点と平面の距離を求め
			float fDistancePointPlane = DistancePointPlane(pVectorEnd, (pHitRect + nCnt));

			//点と平面の距離は判定半径以下なら
			if (fDistancePointPlane < fRadius)
			{
				float a, b;
				D3DXVECTOR3 IntersectionCircle = *pVectorEnd - (pHitRect + nCnt)->VectexC[0].normal * fRadius;                //円の貫通点
				D3DXVECTOR3 IntersectionCircleOld = *pVectorStart - (pHitRect + nCnt)->VectexC[0].normal * fRadius;             //円の貫通点の古い座標
				D3DXVECTOR3 VecA = IntersectionCircleOld - (pHitRect + nCnt)->VectexC[0].pos;
				D3DXVECTOR3 VecB = IntersectionCircle - (pHitRect + nCnt)->VectexC[0].pos;

				a = D3DXVec3Dot(&VecA, &(pHitRect + nCnt)->VectexC[0].normal);            //始点と法線内積
				b = D3DXVec3Dot(&VecB, &(pHitRect + nCnt)->VectexC[0].normal);            //終点と法線内積


																						  //法線判定成立
				if ((a*b) <= 0)
				{
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(&IntersectionCircleOld, &IntersectionCircle, (pHitRect + nCnt));    //貫通点を求める

					if ((pHitRect + nCnt)->meshType == MESH_WALL)
					{
						//
						//XZ平面で正方形と正方形判定
						//////////////////////////////////////
						D3DXVECTOR2 RectA_Pos, RectA_HW, RectB_Pos, RectB_HW;    //変数宣言

																				 //四角Aの計算
						RectA_Pos.x = MinAB(IntersectionCircleOld.x, IntersectionCircle.x);
						RectA_Pos.y = MinAB(IntersectionCircleOld.z, IntersectionCircle.z);
						RectA_HW.x = fabs(IntersectionCircle.x - IntersectionCircleOld.x);
						RectA_HW.y = fabs(IntersectionCircle.z - IntersectionCircleOld.z);

						//四角Bの計算
						RectB_Pos.x = MinAB((pHitRect + nCnt)->VectexC[0].pos.x, (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_Pos.y = MinAB((pHitRect + nCnt)->VectexC[0].pos.z, (pHitRect + nCnt)->VectexC[3].pos.z);
						RectB_HW.x = fabs((pHitRect + nCnt)->VectexC[0].pos.x - (pHitRect + nCnt)->VectexC[3].pos.x);
						RectB_HW.y = fabs((pHitRect + nCnt)->VectexC[0].pos.z - (pHitRect + nCnt)->VectexC[3].pos.z);

						//メッシュのタイプはWALLの時

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
							Normalize = (pHitRect + nCnt)->VectexC[0].normal;
							nWallCollision++;
							bWall = true;
							bGround = true;
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
	//複数の壁と判定がある場合,前の座標に戻す
	if ((nWallCollision > 1) || (bSlope && bWall))
	{
		*pVectorEnd = *pVectorStart;
	}

	if(bGround == true)
	{
		if(elasticity < 0)
		{
			elasticity = 0;
		}

		if (elasticity > 1)
		{
			elasticity = 1;
		}

		*pspeedOut += Normalize*D3DXVec3Length(pspeedOut)*elasticity;                               //跳ね返す速度計算
	}

	return bGround;
}

void CircleAndRay(D3DXVECTOR3 *pOut, float *pfOutDistance, const D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, const D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/, const D3DXVECTOR3 *pCirclePos/*円の位置*/, float fRadius/*判定範囲半径*/, const D3DXVECTOR3 *pModelPos)
{
	D3DXVECTOR3 resultVector = *pVectorEnd - *pVectorStart;           //方向ベクトルを求め
	D3DXVECTOR3 VectorK = *pVectorStart - *pCirclePos;                //レイ始点と円中心座標のベクトル
	D3DXVECTOR3 VecModelCircle = *pModelPos - *pCirclePos;            //円の中心とモデル座標のベクトル
	D3DXVec3Normalize(&resultVector, &resultVector);                      //方向ベクトル単位化
	
	if (D3DXVec3Length(&VecModelCircle) > fRadius)
	{
		//0 = fDistance^2*A + 2*fDistance*B + C の要素A,B,Cを求め
		float fDistance = 100000;                                            //レイ始点と表交差点の距離
		float A = D3DXVec3Dot(&resultVector, &resultVector);
		float B = D3DXVec3Dot(&VectorK, &resultVector);
		float C = D3DXVec3Dot(&VectorK, &VectorK) - fRadius*fRadius;

		float B2AC = B*B - A*C;           //B^2 - A*Cを求める

		//もし交差点が存在すれば
		if (B2AC > 0)
		{
			float fDistance1 = (-B - sqrt(B2AC)) / A;
			float fDistance2 = (-B + sqrt(B2AC)) / A;

			fDistance = fabs(fDistance1);
			*pfOutDistance = fDistance;                      //距離代入
			*pOut = fDistance*resultVector + *pVectorStart;  //交差点座標を求める
		}
	}
}

//モデル円と当り判定円の距離を求め,モデルが当り判定円の範囲内に入っていたらtrueを返す
bool BulletCircleAndCircle(const D3DXVECTOR3 *pModelpos, float fModelRadius,BULLETTYPE Btype)
{
	bool bCheck = false;
	HITCIRCLE *pHitCircle = Get_HitCircle(0);
	for (int nCnt = 0; nCnt < HITCIRCLE_MAX; nCnt++)
	{
		if ((pHitCircle + nCnt)->bUse == true)
		{
			if ((pHitCircle + nCnt)->m_type == TYPE_ENEMY)
			{
				D3DXVECTOR3 VecModelCircle = *pModelpos - (pHitCircle + nCnt)->m_Matrix.pos;
				float fDistanceModelCircle = D3DXVec3Length(&VecModelCircle);                           //モデルと当り判定円の距離

				if (fDistanceModelCircle < (fModelRadius + (pHitCircle + nCnt)->m_skydome.fRadius))
				{
					if(Btype == BTYPE_HANDGUN) (pHitCircle + nCnt)->HitCheckType = HBTYPE_HANDGUN;                      //当り判定円の当り判定フラグの有効
					if (Btype == BTYPE_AK47) (pHitCircle + nCnt)->HitCheckType = HBTYPE_AK47;
					bCheck = true;
					break;
				}
			}
		}
	}

	return bCheck;
}

bool BulletCircleAndCircleSSV(D3DXVECTOR3 *pOutModelpos,const D3DXVECTOR3 *pModelposOld,const D3DXVECTOR3 *pModelpos, float fModelRadius, BULLETTYPE Btype)
{
	bool bCheck = false;
	HITCIRCLE *pHitCircle = Get_HitCircle(0);

	for (int nCnt = 0; nCnt < HITCIRCLE_MAX; nCnt++)
	{
		if ((pHitCircle + nCnt)->bUse == true)
		{
			if ((pHitCircle + nCnt)->m_type == TYPE_ENEMY)
			{
				D3DXVECTOR3 C1 = *pModelpos - (pHitCircle + nCnt)->m_Matrix.pos;
				D3DXVECTOR3 C0 = *pModelposOld - (pHitCircle + nCnt)->m_posOld;
				D3DXVECTOR3 D = C1 - C0;
				float P = D3DXVec3LengthSq(&D);
				float Q = D3DXVec3Dot(&C0, &D);
				float R = D3DXVec3LengthSq(&C0);
				float Radius_SumSq = (fModelRadius + (pHitCircle + nCnt)->m_skydome.fRadius) * (fModelRadius + (pHitCircle + nCnt)->m_skydome.fRadius);

				float K = Q*Q - P*(R - Radius_SumSq);

				if (K >= 0)
				{
					float t1 = (-Q - sqrt(K) ) / P;
					float t2 = (-Q + sqrt(K) ) / P;

					bool bt1Check = false;
					bool bt2Check = false;

					if ((t1 >= 0.0f) && (t1 <= 1.0f)) bt1Check = true;
					if ((t2 >= 0.0f) && (t2 <= 1.0f)) bt2Check = true;
						
					if( (bt1Check == true) || (bt2Check == true) )
					{
						if ((bt1Check == true) && (bt2Check == true))
						{
							*pOutModelpos = *pModelposOld + t1*(*pModelpos - *pModelposOld);                                     //位置調整
							if (Btype == BTYPE_HANDGUN) (pHitCircle + nCnt)->HitCheckType = HBTYPE_HANDGUN;                      //当り判定円の当り判定フラグの有効
							if (Btype == BTYPE_AK47) (pHitCircle + nCnt)->HitCheckType = HBTYPE_AK47;
							bCheck = true;
							break;
						}

						else
						{
							if(bt1Check == true)
							{
								*pOutModelpos = *pModelposOld + t1*(*pModelpos - *pModelposOld);                                     //位置調整
								if (Btype == BTYPE_HANDGUN) (pHitCircle + nCnt)->HitCheckType = HBTYPE_HANDGUN;                      //当り判定円の当り判定フラグの有効
								if (Btype == BTYPE_AK47) (pHitCircle + nCnt)->HitCheckType = HBTYPE_AK47;
								bCheck = true;
								break;
							}

							else
							{
								*pOutModelpos = *pModelposOld + t2*(*pModelpos - *pModelposOld);                                     //位置調整
								if (Btype == BTYPE_HANDGUN) (pHitCircle + nCnt)->HitCheckType = HBTYPE_HANDGUN;                      //当り判定円の当り判定フラグの有効
								if (Btype == BTYPE_AK47) (pHitCircle + nCnt)->HitCheckType = HBTYPE_AK47;
								bCheck = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	return bCheck;
}

float CircleAndPlaneHitTime(const D3DXVECTOR3 *pVectorStart/*ベクトル始点ポインタ*/, const D3DXVECTOR3 *pVectorEnd/*ベクトル終点ポインタ*/,const D3DXVECTOR3 *pPlanePoint, const D3DXVECTOR3 *pPlaneNormal/*平面法線*/, float fRadius/*判定範囲半径*/)
{
	D3DXVECTOR3 C0 = *pVectorStart - *pPlanePoint;
	D3DXVECTOR3 d = *pVectorEnd - *pVectorStart;
	float DotCN = D3DXVec3Dot(&C0,pPlaneNormal);
	float DotdN = D3DXVec3Dot(&d, pPlaneNormal);
	float t = -1;
	//衝突時刻tを求める
	if (DotdN < 0)
	{
		t = (fRadius - DotCN) / DotdN;

		if( (t >= 0.0f) && (t <= 1.0f) )
		{
			return t;
		}
	}

	return -1;
}