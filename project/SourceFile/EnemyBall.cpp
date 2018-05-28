//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "Enemyball.h"
#include "camera.h"
#include "TpsCamera.h"
#include "textureManager.h"
#include "matrix.h"
#include "hitCircle.h"
#include "hitCircleMesh.h"
#include "model_aircraft.h"
#include "vector.h"
#include "CScore.h"
#include "CParticalSetting.h"

#define ENEMYBALL_PASS "data/zombie.x"
#define ENEMY_SPEED (0.1f)
#define ENEMY_FICTION (0.1f)
#define ROT_SPEED (0.05f*PI)                    //回転速度
//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
ENEMYBALL g_EB[ENEMYBALL_MAX];                    //敵の配列

LPD3DXMESH g_pMeshEB;                               //メッシュインターフェースポインタ
LPD3DXBUFFER g_pBuffMatEB;                          //マテリアル情報バッファ(D3DX汎用バッファインターフェース)
DWORD g_nNumMatEB;                                  //マテリアルの数
//int g_TexNUMEB[NUM_EB_MATERIAL];                    //テクスチャ発行番号

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitEB(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	g_nNumMatEB = NUM_EB_MATERIAL;                               //マテリアルの数

	//モデルワールド変換マトリクスの初期化
	for(int nCnt = 0; nCnt < ENEMYBALL_MAX; nCnt++)
	{
		g_EB[nCnt].m_Matrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EB[nCnt].m_Matrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EB[nCnt].m_Matrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_EB[nCnt].m_DirectVec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		g_EB[nCnt].m_fSpeedY = 0.0f;
		g_EB[nCnt].m_nLife = 10;
		g_EB[nCnt].m_nID = -1;
		g_EB[nCnt].bUse = false;
		g_EB[nCnt].DamageEfTime = 0;
	}

	//モデルデータの読み込み
	if(FAILED(D3DXLoadMeshFromX(ENEMYBALL_PASS, D3DXMESH_MANAGED/*指数メモリ*/, pD3DDevice/*デバイス*/, NULL/*隣接バッファ*/, &g_pBuffMatEB/*マテリアル情報を格納する*/, NULL, &g_nNumMatEB/*マテリアル数*/, &g_pMeshEB/*メッシュ*/)))
	{
		MessageBox(NULL, "モデルMODEL_NAMEの読み込みが失敗した", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatEB->GetBufferPointer();   //マテリアル情報のポインタとしてバッファのアドレスを取得

	//テクスチャ読み込む
	for(int nCnt = 0; nCnt < g_nNumMatEB; nCnt++)
	{
		//SetTexture(&g_TexNUMEB[nCnt], pMat[nCnt].pTextureFilename);
	}

	return S_OK;
}

//モデルの更新
void UpdateEB(void)
{
	CScore *pCScore = GetScore();

	for (int i = 0; i < ENEMYBALL_MAX; i++)
	{
		if (g_EB[i].bUse == true)
		{
			if (g_EB[i].m_nLife > 0)
			{
				EBRotate(g_EB[i].m_nID);          //モデル回転
				EBMove(g_EB[i].m_nID);            //モデル移動

				g_EB[i].DamageEfTime--;
				if(g_EB[i].DamageEfTime < 0)
				{
					g_EB[i].DamageEfTime = 0;
				}
			}

			else
			{
				for(int nCnt = 0;nCnt < HITCIRCLE_MAX;nCnt++)
				{
					HITCIRCLE *pHitCircle = Get_HitCircle(nCnt);

					if (pHitCircle->bUse == true)
					{
						if (pHitCircle->m_type == TYPE_ENEMY)
						{
							if (pHitCircle->nEnemyID == g_EB[i].m_nID)
							{
								pHitCircle->bUse = false;
							}
						}
					}
				}

				CExplosion::Create(&g_EB[i].m_Matrix.pos,25,&D3DXVECTOR3(170,4,3));
				g_EB[i].bUse = false;
				pCScore->AddScore(300);
			}
		}
	}
}

//モデル回転
void EBRotate(int EnemyID) 
{
	AIRCRAFT *pAircraft = Get_Aircraft();
	D3DXVECTOR2 DirectVec;
	DirectVec.x = pAircraft->m_Matrix.pos.x - g_EB[EnemyID].m_Matrix.pos.x;
	DirectVec.y = pAircraft->m_Matrix.pos.z - g_EB[EnemyID].m_Matrix.pos.z;

	D3DXVECTOR2 DefaultVec;
	DefaultVec.x = g_EB[EnemyID].m_DirectVec.x;
	DefaultVec.y = g_EB[EnemyID].m_DirectVec.z;

	g_EB[EnemyID].m_fTargetAngle = Angle_TwoVector(DefaultVec, DirectVec);

	if (g_EB[EnemyID].m_Matrix.rot.y > PI) { g_EB[EnemyID].m_Matrix.rot.y -= 2 * PI; }
	if (g_EB[EnemyID].m_Matrix.rot.y < -PI) { g_EB[EnemyID].m_Matrix.rot.y += 2 * PI; }

	RotToTargetAngle(&g_EB[EnemyID].m_Matrix.rot.y, -g_EB[EnemyID].m_fTargetAngle + PI, ROT_SPEED);
}
//モデル移動
void EBMove(int EnemyID)
{
	AIRCRAFT *pAircraft = Get_Aircraft();
	
	g_EB[EnemyID].m_SpeedVec = pAircraft->m_Matrix.pos - g_EB[EnemyID].m_Matrix.pos;
	D3DXVECTOR2 MoveSpeed = D3DXVECTOR2(g_EB[EnemyID].m_SpeedVec.x, g_EB[EnemyID].m_SpeedVec.z);
	g_EB[EnemyID].m_fSpeedY -= GRAVITY;
	D3DXVec2Normalize(&MoveSpeed, &MoveSpeed);

	//座標加算
	g_EB[EnemyID].m_posOld = g_EB[EnemyID].m_Matrix.pos;
	g_EB[EnemyID].m_Matrix.pos.x += g_EB[EnemyID].m_fSpeed*MoveSpeed.x;
	g_EB[EnemyID].m_Matrix.pos.y += g_EB[EnemyID].m_fSpeedY;
	g_EB[EnemyID].m_Matrix.pos.z += g_EB[EnemyID].m_fSpeed*MoveSpeed.y;

	//当り判定計算
	//
	//当り判定(仮)
	////////////////////////////////////////////////////
	bool Check = false;
	bool Damage = false;
	int nCntHit = 0;
	D3DXVECTOR3 Start, End;                             //当り判定用
	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		HITCIRCLE *pHitCircle = Get_HitCircle(i);
		if (pHitCircle->bUse == true)
		{
			if (pHitCircle->m_type == TYPE_ENEMY)
			{
				if (pHitCircle->nEnemyID == EnemyID)
				{
					//メッシュ判定
					Start = g_EB[EnemyID].m_posOld + pHitCircle->m_offset;                              //当り判定用
					End = g_EB[EnemyID].m_Matrix.pos + pHitCircle->m_offset;                               //当り判定用
					Check = CircleAndRect(&Start, &End, pHitCircle->m_skydome.fRadius);
					g_EB[EnemyID].m_posOld = Start - pHitCircle->m_offset;                             //修正後のold座標算出
					g_EB[EnemyID].m_Matrix.pos = End - pHitCircle->m_offset;                              //修正後の座標算出

					//プレーヤー判定
					for (int j = 0; j < HITCIRCLE_MAX; j++)
					{
						HITCIRCLE *pHitCirclePlayer = Get_HitCircle(j);
						if (pHitCirclePlayer->bUse == true)
						{
							if (pHitCirclePlayer->m_type == TYPE_PLAYER)
							{
								D3DXVECTOR3 fDistanceVec = pHitCirclePlayer->m_Matrix.pos - pHitCircle->m_Matrix.pos;
								float fDistance = D3DXVec3Length(&fDistanceVec);
								if (fDistance < (pHitCirclePlayer->m_skydome.fRadius + pHitCircle->m_skydome.fRadius)) pHitCirclePlayer->HitCheckType = HBTYPE_ENEMY;
							}
						}
					}

					//プレーヤーの弾から受けたダーメジ判定
					if (Damage == false)
					{
						if (pHitCircle->HitCheckType == HBTYPE_HANDGUN)
						{
							Damage = true;
							g_EB[EnemyID].m_nLife -= 1;
							pHitCircle->HitCheckType = HBTYPE_NULL;
							g_EB[EnemyID].DamageEfTime = 10;
						}

						if (pHitCircle->HitCheckType == HBTYPE_AK47)
						{
							Damage = true;
							g_EB[EnemyID].m_nLife -= 2;
							pHitCircle->HitCheckType = HBTYPE_NULL;
							g_EB[EnemyID].DamageEfTime = 10;
						}
					}
				}
			}
		}
	}

	if(Check == true)
	{
		g_EB[EnemyID].m_fSpeedY = 0.0f;
	}
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawEB(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	D3DMATERIAL9 matDef;
	pD3DDevice->GetMaterial(&matDef);                                                 //デバイスのマテリアルを取得し、保存する

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatEB->GetBufferPointer();

	TEXTUREMANAGER *pTextureM = GetTextureManager();                                  //テクスチャマネージャー取得

	D3DMATERIAL9 DamageMat;
	DamageMat.Diffuse.r = 255;
	DamageMat.Diffuse.g = 0;
	DamageMat.Diffuse.b = 0;
	DamageMat.Diffuse.a = 50;
	DamageMat.Ambient = DamageMat.Diffuse;
	DamageMat.Specular.r = 0;
	DamageMat.Specular.g = 0;
	DamageMat.Specular.b = 0;
	DamageMat.Specular.a = 0;
	DamageMat.Emissive = DamageMat.Specular;
	DamageMat.Power = 1.0f;

	for (int i = 0; i < ENEMYBALL_MAX; i++)
	{
		if (g_EB[i].bUse == true)
		{
			SetMatrix(&g_EB[i].m_Mtxworld, g_EB[i].m_Matrix);                                            //マトリクスの計算

			for (int nCnt = 0; nCnt < (int)g_nNumMatEB; nCnt++)
			{
				if (g_EB[i].DamageEfTime > 0)
				{
					pD3DDevice->SetMaterial(&DamageMat);
				}
				else 
				{
					pD3DDevice->SetMaterial(&pMat[nCnt].MatD3D);
				}
				pD3DDevice->SetTexture(0, NULL);
				g_pMeshEB->DrawSubset(nCnt);
			}
		}
	}
	pD3DDevice->SetMaterial(&matDef);                                                //マテリアルを戻す
}

void UninitEB(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE(g_pMeshEB);                      //メッシュインターフェースポインタを解放

	//頂点バッファを解放する
	SAFE_RELEASE(g_pBuffMatEB);                       //マテリアル情報バッファを解放
}

ENEMYBALL *Get_EB(int nID)
{
	return &g_EB[nID];
}

void SetEB(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pRot, float fSpeed)
{
	for (int i = 0; i < ENEMYBALL_MAX; i++)
	{
		if (g_EB[i].bUse == false)
		{
			g_EB[i].m_Matrix.pos = *pPos;
			g_EB[i].m_Matrix.rot = *pRot;
			g_EB[i].m_Matrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_EB[i].m_posOld = g_EB[i].m_Matrix.pos;
			g_EB[i].m_fSpeed = fSpeed;
			g_EB[i].m_fSpeedY = 0.0f;
			g_EB[i].m_SpeedVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_EB[i].DamageEfTime = 0;
			g_EB[i].m_nID = i;
			g_EB[i].m_nLife = 10;
			SetHitCircleEnemy(&g_EB[i].m_Matrix.pos,2.0f, &D3DXVECTOR3(0.0f,2.0f,0.0f),g_EB[i].m_nID);                                //当り判定円のセット
			SetHitCircleEnemy(&g_EB[i].m_Matrix.pos, 2.0f, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_EB[i].m_nID);                                //当り判定円のセット
			SetHitCircleEnemy(&g_EB[i].m_Matrix.pos, 2.0f, &D3DXVECTOR3(0.0f, -2.0f, 0.0f), g_EB[i].m_nID);                                //当り判定円のセット
			g_EB[i].bUse = true;
			break;
		}
	}
}