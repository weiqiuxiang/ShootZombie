#include "tpsAim.h"
#include "hitRectangle .h"
#include "hitcheckMesh.h"
#include "hitCircle.h"
#include "hitCircleMesh.h"
#include "TpsCamera.h"

//グローバル変数
float g_fDistance;
D3DXVECTOR3 g_Intersection;

//方法1
void RayMesh(D3DXVECTOR3 *pOut,const D3DXVECTOR3 *pVectorLookat/*ベクトル始点ポインタ*/, const D3DXVECTOR3 *pVectorCamera/*ベクトル終点ポインタ*/)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //結果ベクトルを求め
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector*100000;
	const HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bCheck = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター
	int nWallCollision = 0;  //MESH_WALLとの衝突数
	float fDistance = 0;                                //カメラと衝突平面の距離
	float fMinDistance = 1000000;                           //カメラと多平面の衝突に対してカメラと平面の最短距離

	//カメラ-注視点ベクトルを単位化
	D3DXVec3Normalize(&resultVector, &resultVector);

	//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			D3DXVECTOR3 InverNor = -(pHitRect + nCnt)->VectexC[0].normal;      //法線の逆法線

			//レイと法線の逆ベクトルの内積は0大きいな場合
			if (D3DXVec3Dot(&resultVector, &InverNor) > 0)
			{
				float fAngle = D3DXVec3Dot(&resultVector, &InverNor) / (D3DXVec3Length(&resultVector) * D3DXVec3Length(&InverNor));  //成す角度
				fDistance = DistancePointPlane(pVectorCamera, (pHitRect + nCnt));          //カメラと衝突メッシュの距離
				fDistance = fDistance / cos(fAngle);                   //レイとメッシュの距離
				D3DXVECTOR3 resultVectorCal = (*pVectorLookat - *pVectorCamera);
				if (fDistance > D3DXVec3Length(&resultVectorCal) )
				{

					D3DXVECTOR3 EndVec = (fDistance+1.0f)*resultVector + *pVectorCamera;    //貫通点を求める
					D3DXVECTOR3 Intersection = GetVecPlaneIntersection(pVectorCamera, &EndVec, (pHitRect + nCnt));    //貫通点を求める

					//斜め
					if ((pHitRect + nCnt)->meshType == MESH_SLOPE)
					{
						//貫通点が平面上にあるか判定する
						if (Rects3DPointPlane(&Intersection, (pHitRect + nCnt)))
						{
							if (fDistance < fMinDistance)
							{
								fMinDistance = fDistance;                              //最短距離保存
								RayIntersection = Intersection;     //修正
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
						RectA_Pos.x = MinAB(EndVec.x, pVectorCamera->x);
						RectA_Pos.y = MinAB(EndVec.z, pVectorCamera->z);
						RectA_HW.x = fabs(pVectorCamera->x - EndVec.x);
						RectA_HW.y = fabs(pVectorCamera->z - EndVec.z);

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
									RayIntersection = Intersection;     //修正
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
									RayIntersection = Intersection;     //修正
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

	*pOut = RayIntersection;
}

//方法2
void RayMeshRect(D3DXVECTOR3 *pOut,float *fDistanceOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //結果ベクトルを求め
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector * 100000;
	const HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bCheck = false;  //地面衝突チェックフラグ
	float fMinDistance = 1000000;                           //カメラと多平面の衝突に対してカメラと平面の最短距離

	//カメラ-注視点ベクトルを単位化
	D3DXVec3Normalize(&resultVector, &resultVector);

	//判定チェック
	for (int nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			D3DXVECTOR3 InverNor = -(pHitRect + nCnt)->VectexC[0].normal;      //法線の逆法線
			//nCnt == 4
			//レイと法線の逆ベクトルの内積は0大きいな場合
			if (D3DXVec3Dot(&resultVector, &InverNor) > 0)
			{
				D3DXVECTOR3 VecP0_S = (pHitRect + nCnt)->VectexC[0].pos - *pVectorCamera;
				float fDot1 = DistancePointPlane(pVectorCamera, (pHitRect + nCnt));
				float fDot2 = D3DXVec3Dot(&resultVector,&InverNor);
				g_fDistance = fDot1 / fDot2;
				D3DXVECTOR3 resultVectorCal = (*pVectorLookat - *pVectorCamera);

				if (g_fDistance > D3DXVec3Length(&resultVectorCal))
				{
					g_Intersection = *pVectorCamera + g_fDistance*resultVector;    //貫通点を求める

					/*
					RECT_3D MeshRect;
					MeshRect.p0 = (pHitRect + nCnt)->VectexC[0].pos;
					MeshRect.p1 = (pHitRect + nCnt)->VectexC[1].pos;
					MeshRect.p2 = (pHitRect + nCnt)->VectexC[2].pos;
					MeshRect.p3 = (pHitRect + nCnt)->VectexC[3].pos;
					*/
					bCheck = Rects3DPointPlane(&g_Intersection, (pHitRect + nCnt) );                //3D当り判定を行う

					if(bCheck == true)
					{
						if (g_fDistance < fMinDistance)
						{
							fMinDistance = g_fDistance;                              //最短距離保存
							RayIntersection = g_Intersection;     //修正
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
	g_fDistance = fMinDistance;               //最短距離保存
	g_Intersection = RayIntersection;         //交差点保存
	*fDistanceOut = g_fDistance;
	*pOut = RayIntersection;
}

void RayCircle(D3DXVECTOR3 *pOut, float *fDistanceOut,const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera,const D3DXVECTOR3 *pModelPos)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //結果ベクトルを求め
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector * 100000;
	const HITCIRCLE *pHitCircle = Get_HitCircle(0);                                //判定当り判定円データ取得
	float fMinDistance = 1000000;                           //カメラと多平面の衝突に対してカメラと平面の最短距離

	for (int nCnt = 0; nCnt < HITCIRCLE_MAX; nCnt++)
	{
		if ( (pHitCircle + nCnt)->bUse == true)
		{
			if ((pHitCircle + nCnt)->m_type != TYPE_PLAYER)
			{
				CircleAndRay(&g_Intersection, &g_fDistance, pVectorCamera, pVectorLookat, &(pHitCircle + nCnt)->m_Matrix.pos, (pHitCircle + nCnt)->m_skydome.fRadius, pModelPos);

				if (g_fDistance < fMinDistance)
				{
					fMinDistance = g_fDistance;                              //最短距離保存
					RayIntersection = g_Intersection;     //修正
				}
			}
		}
	}

	g_fDistance = fMinDistance;               //最短距離保存
	g_Intersection = RayIntersection;         //交差点保存
	*fDistanceOut = g_fDistance;
	*pOut = RayIntersection;
}

void RayCheck(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera, const D3DXVECTOR3 *pModelPos)
{
	float fDistance01 = 0;
	float fDistance02 = 0;
	D3DXVECTOR3 pOut01;
	D3DXVECTOR3 pOut02;
	g_fDistance = 1000000;
	RayMeshRect(&pOut01, &fDistance01,pVectorLookat, pVectorCamera);
	RayCircle(&pOut02, &fDistance02, pVectorLookat, pVectorCamera, pModelPos);

	if(fDistance01 < fDistance02)
	{
		*pOut = pOut01;
	}

	else
	{
		*pOut = pOut02;
	}
}

/*
void RayMeshRect(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //結果ベクトルを求め
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector * 100000;
	const HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bCheck = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター
	int nWallCollision = 0;  //MESH_WALLとの衝突数
	float fMinDistance = 1000000;                           //カメラと多平面の衝突に対してカメラと平面の最短距離

															//カメラ-注視点ベクトルを単位化
	D3DXVec3Normalize(&resultVector, &resultVector);

	//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			D3DXVECTOR3 InverNor = -(pHitRect + nCnt)->VectexC[0].normal;      //法線の逆法線

																			   //レイと法線の逆ベクトルの内積は0大きいな場合
			if (D3DXVec3Dot(&resultVector, &InverNor) > 0)
			{
				float fAngle = D3DXVec3Dot(&resultVector, &InverNor) / (D3DXVec3Length(&resultVector) * D3DXVec3Length(&InverNor));  //成す角度
				g_fDistance = DistancePointPlane(pVectorCamera, (pHitRect + nCnt));          //カメラと衝突メッシュの距離
				g_fDistance = g_fDistance / fAngle;                   //レイとメッシュの距離
				D3DXVECTOR3 resultVectorCal = (*pVectorLookat - *pVectorCamera);

				if (g_fDistance > D3DXVec3Length(&resultVectorCal))
				{

					D3DXVECTOR3 EndVec = (g_fDistance)*resultVector + *pVectorCamera;    //貫通点を求める
					g_Intersection = GetVecPlaneIntersection(pVectorCamera, &EndVec, (pHitRect + nCnt));    //貫通点を求める

					RECT_3D MeshRect;
					MeshRect.p0 = (pHitRect + nCnt)->VectexC[0].pos;
					MeshRect.p1 = (pHitRect + nCnt)->VectexC[1].pos;
					MeshRect.p2 = (pHitRect + nCnt)->VectexC[2].pos;
					MeshRect.p3 = (pHitRect + nCnt)->VectexC[3].pos;

					bCheck = Rect3D_Vector(&g_Intersection, &MeshRect);                //3D当り判定を行う

					if (bCheck == true)
					{
						if (g_fDistance < fMinDistance)
						{
							fMinDistance = g_fDistance;                              //最短距離保存
							RayIntersection = g_Intersection;     //修正
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
	*pOut = RayIntersection;
}
*/
/*
void RayMeshRect(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pVectorLookat, const D3DXVECTOR3 *pVectorCamera)
{
	D3DXVECTOR3 resultVector = *pVectorLookat - *pVectorCamera;           //結果ベクトルを求め
	D3DXVECTOR3 RayIntersection = *pVectorCamera + resultVector * 100000;
	const HITRECT *pHitRect = Get_HitRect();                                //判定四角ポリゴンデータ取得
	bool bCheck = false;  //地面衝突チェックフラグ
	int nCnt;              //カウンター
	int nWallCollision = 0;  //MESH_WALLとの衝突数
	float fMinDistance = 1000000;                           //カメラと多平面の衝突に対してカメラと平面の最短距離

															//カメラ-注視点ベクトルを単位化
	D3DXVec3Normalize(&resultVector, &resultVector);

	//判定チェック
	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定四角ポリゴン使用中なら
		if ((pHitRect + nCnt)->bUse == true)
		{
			float dotRN = D3DXVec3Dot(&resultVector, &(pHitRect + nCnt)->VectexC[0].normal);
			//レイと法線の逆ベクトルの内積は0大きいな場合
			if (dotRN != 0)
			{
				float t;
				float D3DXVec3Dot();

				if ()
				{
					float fAngle = D3DXVec3Dot(&resultVector, &InverNor) / (D3DXVec3Length(&resultVector) * D3DXVec3Length(&InverNor));  //成す角度
					g_fDistance = DistancePointPlane(pVectorCamera, (pHitRect + nCnt));          //カメラと衝突メッシュの距離
					g_fDistance = g_fDistance / fAngle;                   //レイとメッシュの距離
					D3DXVECTOR3 resultVectorCal = (*pVectorLookat - *pVectorCamera);

					if (g_fDistance > D3DXVec3Length(&resultVectorCal))
					{

						D3DXVECTOR3 EndVec = (g_fDistance)*resultVector + *pVectorCamera;    //貫通点を求める
						g_Intersection = GetVecPlaneIntersection(pVectorCamera, &EndVec, (pHitRect + nCnt));    //貫通点を求める

						RECT_3D MeshRect;
						MeshRect.p0 = (pHitRect + nCnt)->VectexC[0].pos;
						MeshRect.p1 = (pHitRect + nCnt)->VectexC[1].pos;
						MeshRect.p2 = (pHitRect + nCnt)->VectexC[2].pos;
						MeshRect.p3 = (pHitRect + nCnt)->VectexC[3].pos;

						bCheck = Rect3D_Vector(&g_Intersection, &MeshRect);                //3D当り判定を行う

						if (bCheck == true)
						{
							if (g_fDistance < fMinDistance)
							{
								fMinDistance = g_fDistance;                              //最短距離保存
								RayIntersection = g_Intersection;     //修正
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
	}
	*pOut = RayIntersection;
}
*/
float GetRayDistance(void)
{
	return g_fDistance;
}

D3DXVECTOR3 GetRayInt(void)
{
	return g_Intersection;
}