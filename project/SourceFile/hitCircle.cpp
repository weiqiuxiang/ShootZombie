//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "matrix.h"
#include "hitCircle.h"
#include "input.h"
#include "model_aircraft.h"
#include "EnemyBall.h"

//-----------------------------------------------------------------------------------------
//プロトタイプ
//-----------------------------------------------------------------------------------------
void InitVertexIndex(int nID);

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
HITCIRCLE g_hitCircle[HITCIRCLE_MAX];       //スカイドーム
#ifdef _DEBUG
bool g_DrawCheckHR;
#endif // 

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitHitCircle(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;
	pD3DDevice = Get_Device();

	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		g_hitCircle[i].m_pVtxBuffer = NULL;
		g_hitCircle[i].m_pIdxBufferField = NULL;
		g_hitCircle[i].m_type = TYPE_OBJ;
		g_hitCircle[i].HitCheckType = HBTYPE_NULL;                           //当り判定チェック
		g_hitCircle[i].bUse = false;
		g_hitCircle[i].m_offset = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

#ifdef _DEBUG
	g_DrawCheckHR = false;
#endif //
	return S_OK;
}

//ポリゴンの更新
void UpdateHitCircle(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();

	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		if (g_hitCircle[i].bUse == true)
		{
			g_hitCircle[i].m_posOld = g_hitCircle[i].m_Matrix.pos;

			if(g_hitCircle[i].m_type == TYPE_PLAYER)
			{
				g_hitCircle[i].m_Matrix.pos = pAircraft->m_Matrix.pos + g_hitCircle[i].m_offset;
			}

			if(g_hitCircle[i].m_type == TYPE_ENEMY)
			{
				ENEMYBALL *pEB = Get_EB(g_hitCircle[i].nEnemyID);
				if(pEB->bUse == false)
				{
					g_hitCircle[i].bUse = false;
					continue;
				}
				else
				{
					g_hitCircle[i].m_Matrix.pos = pEB->m_Matrix.pos + g_hitCircle[i].m_offset;
				}
			}
		}
	}
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawHitCircle(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{
		if (g_DrawCheckHR == false) g_DrawCheckHR = true;
		else g_DrawCheckHR = false;
	}
	if (g_DrawCheckHR == true)
	{
		LPDIRECT3DDEVICE9 pD3DDevice;
		pD3DDevice = Get_Device();
		for (int i = 0; i < HITCIRCLE_MAX; i++)
		{
			pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //ライトをオフにする
			if (g_hitCircle[i].bUse == true)
			{
				SetMatrix(&g_hitCircle[i].m_MtxWorld, g_hitCircle[i].m_Matrix);      //マトリクス変換セット

				pD3DDevice->SetStreamSource(0, g_hitCircle[i].m_pVtxBuffer, 0, sizeof(VERTEX_3D));                     //VRAMからGPUにデータをストリーム
				pD3DDevice->SetIndices(g_hitCircle[i].m_pIdxBufferField);                                                    //インデックスバッファのセット
				pD3DDevice->SetFVF(FVF_VERTEX3D);
				pD3DDevice->SetTexture(0, NULL);												 //テクスチャの設定
				pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_hitCircle[i].m_nVertexNum, 0, g_hitCircle[i].m_nPolygonNum);              //描画
			}
			pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトをオンにする
		}
	}
#endif // DEBUG
}

////////////////////////////////////////////////
//ポリゴンの終了
////////////////////////////////////////////////
void UninitHitCircle(void)
{
	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		SAFE_RELEASE(g_hitCircle[i].m_pVtxBuffer);                       //頂点バッファインターフェイスの片付け
		SAFE_RELEASE(g_hitCircle[i].m_pIdxBufferField);                  //インデックスバッファを解放
	}
}

//ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexHitCircle(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************デバイス設定***********************************************/
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//円メッシュの頂点ロックとインデックスロック
void InitVertexIndex(int nID)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;									   //テクスチャ構造体のポインタ
	WORD *pIdx = NULL;
	int i, j;                                                  //カウント用変数

	float fHorizon_Angel;
	float fVertical_Angel;
	float posX;
	float posY;
	float posZ;
	
	/***************************仮想アドレスを取得し頂点バッファをロックする************************************/
	g_hitCircle[nID].m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	//頂点登録
	for (i = 0; i < (g_hitCircle[nID].m_skydome.nVertical_PieceNum + 1); i++)
	{
		for (j = 0; j < (g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1); j++)
		{
			int k = i * (g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1) + j;
			//posに関する計算
			fHorizon_Angel = g_hitCircle[nID].m_skydome.fHorizon_PerAngel*j;                                       //水平角度
			fVertical_Angel = g_hitCircle[nID].m_skydome.fStart_Angel_Vertical + g_hitCircle[nID].m_skydome.fVertical_PerAngel * i;   //垂直角度
			posX = g_hitCircle[nID].m_skydome.fRadius*sin(fVertical_Angel)*cos(fHorizon_Angel);            //頂点のX座標計算
			posY = g_hitCircle[nID].m_skydome.fRadius*cos(fVertical_Angel);            //頂点のY座標計算
			posZ = g_hitCircle[nID].m_skydome.fRadius*sin(fVertical_Angel)*sin(fHorizon_Angel);            //頂点のZ座標計算

			pVtx[0 + k].pos = D3DXVECTOR3(posX, posY, posZ);
			pVtx[0 + k].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			pVtx[0 + k].tex = D3DXVECTOR2(j*(1.0f / g_hitCircle[nID].m_skydome.nHorizon_PieceNum), i*(1.0f / g_hitCircle[nID].m_skydome.nVertical_PieceNum));
			pVtx[0 + k].color = g_hitCircle[nID].m_Color;
		}
	}

	g_hitCircle[nID].m_pVtxBuffer->Unlock();
	/***************************頂点バッファのアンロック************************************/

	int q;
	/***************************仮想アドレスを取得しインデックスバッファをロックする************************************/
	g_hitCircle[nID].m_pIdxBufferField->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファ登録
	//縮退
	for (i = 0; i < g_hitCircle[nID].m_skydome.nVertical_PieceNum; i++)
	{
		//普通部分
		for (j = 0; j < g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1; j++)
		{
			int k = j * 2 + i*(g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 2) * 2;

			pIdx[0 + k] = (i + 1)*(g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1) + j;
			pIdx[1 + k] = pIdx[0 + k] - (g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1);
		}

		q = (2 + 2 * g_hitCircle[nID].m_skydome.nHorizon_PieceNum)*(i + 1) + ((i + 1) - 1) * 2;

		//縮退部分
		if (q < g_hitCircle[nID].m_nIndexNum)
		{
			pIdx[q] = pIdx[q - 1];         //縮退1の頂点番号
			pIdx[q + 1] = pIdx[q] + (g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1) + 1;     //縮退2の頂点番号
		}
	}

	g_hitCircle[nID].m_pIdxBufferField->Unlock();
	/***************************インデックスバッファのアンロック************************************/
}

HRESULT SetHitCirclePlayer(const D3DXVECTOR3 *pPos, float nRadius,const D3DXVECTOR3 *offset)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;
	pD3DDevice = Get_Device();

	for(int i = 0; i < HITCIRCLE_MAX; i++)
	{
		if(g_hitCircle[i].bUse == false)
		{
			//
			//スカドーム部分
			//////////////////////////
			g_hitCircle[i].m_skydome.fRadius = nRadius;
			g_hitCircle[i].m_skydome.fStart_Angel_Vertical = 0;
			g_hitCircle[i].m_skydome.fEnd_Angel_Vertical = PI;
			g_hitCircle[i].m_skydome.nHorizon_PieceNum = 10;
			g_hitCircle[i].m_skydome.nVertical_PieceNum = 10;
			g_hitCircle[i].m_skydome.fHorizon_PerAngel = 2 * PI / g_hitCircle[i].m_skydome.nHorizon_PieceNum;
			g_hitCircle[i].m_skydome.fVertical_PerAngel = (g_hitCircle[i].m_skydome.fEnd_Angel_Vertical - g_hitCircle[i].m_skydome.fStart_Angel_Vertical) / g_hitCircle[i].m_skydome.nVertical_PieceNum;

			//頂点数,インデックス数,ポリゴン数計算
			g_hitCircle[i].m_nVertexNum = (g_hitCircle[i].m_skydome.nHorizon_PieceNum + 1)*(g_hitCircle[i].m_skydome.nVertical_PieceNum + 1);
			g_hitCircle[i].m_nIndexNum = (2 + 2 * g_hitCircle[i].m_skydome.nHorizon_PieceNum)*g_hitCircle[i].m_skydome.nVertical_PieceNum + (g_hitCircle[i].m_skydome.nVertical_PieceNum - 1) * 2;
			g_hitCircle[i].m_nPolygonNum = g_hitCircle[i].m_nIndexNum - 2;

			g_hitCircle[i].m_Matrix.pos = *pPos;
			g_hitCircle[i].m_posOld = g_hitCircle[i].m_Matrix.pos;
			g_hitCircle[i].m_offset = *offset;
			g_hitCircle[i].m_type = TYPE_PLAYER;
			g_hitCircle[i].m_Matrix.rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_hitCircle[i].m_Matrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_hitCircle[i].m_Color = D3DCOLOR_RGBA(200,10,10,100);
			g_hitCircle[i].nID = i;

			//頂点バッファの生成
			if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*g_hitCircle[i].m_nVertexNum, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_hitCircle[i].m_pVtxBuffer, NULL)))
			{
				return E_FAIL;
			}

			//インデックスバッファの生成
			HRESULT hr;
			hr = pD3DDevice->CreateIndexBuffer(
				sizeof(WORD)*g_hitCircle[i].m_nIndexNum,     //バッファ量(2バイト)
				D3DUSAGE_WRITEONLY,        //使用方法フラグ
				D3DFMT_INDEX16,              //インデックスフォーマット(重複しない頂点数は65536個は限界)
				D3DPOOL_MANAGED,           //メモリ管理方法
				&g_hitCircle[i].m_pIdxBufferField,        //インデックスバッファインターフェースへのポインタのアドレス
				NULL);

			if (FAILED(hr))
			{
				return E_FAIL;
			}

			InitVertexIndex(g_hitCircle[i].nID);                                      //頂点バッファのロックと頂点じゃないバッファのロック
			g_hitCircle[i].bUse = true;
			break;
		}
	}

	return S_OK;
}

HRESULT SetHitCircleEnemy(const D3DXVECTOR3 *pPos, float nRadius,const D3DXVECTOR3 *offset,int nEnemyID)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;
	pD3DDevice = Get_Device();

	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		if (g_hitCircle[i].bUse == false)
		{
			//
			//スカドーム部分
			//////////////////////////
			g_hitCircle[i].m_skydome.fRadius = nRadius;
			g_hitCircle[i].m_skydome.fStart_Angel_Vertical = 0;
			g_hitCircle[i].m_skydome.fEnd_Angel_Vertical = PI;
			g_hitCircle[i].m_skydome.nHorizon_PieceNum = 10;
			g_hitCircle[i].m_skydome.nVertical_PieceNum = 10;
			g_hitCircle[i].m_skydome.fHorizon_PerAngel = 2 * PI / g_hitCircle[i].m_skydome.nHorizon_PieceNum;
			g_hitCircle[i].m_skydome.fVertical_PerAngel = (g_hitCircle[i].m_skydome.fEnd_Angel_Vertical - g_hitCircle[i].m_skydome.fStart_Angel_Vertical) / g_hitCircle[i].m_skydome.nVertical_PieceNum;

			//頂点数,インデックス数,ポリゴン数計算
			g_hitCircle[i].m_nVertexNum = (g_hitCircle[i].m_skydome.nHorizon_PieceNum + 1)*(g_hitCircle[i].m_skydome.nVertical_PieceNum + 1);
			g_hitCircle[i].m_nIndexNum = (2 + 2 * g_hitCircle[i].m_skydome.nHorizon_PieceNum)*g_hitCircle[i].m_skydome.nVertical_PieceNum + (g_hitCircle[i].m_skydome.nVertical_PieceNum - 1) * 2;
			g_hitCircle[i].m_nPolygonNum = g_hitCircle[i].m_nIndexNum - 2;

			g_hitCircle[i].m_Matrix.pos = *pPos;
			g_hitCircle[i].m_offset = *offset;
			g_hitCircle[i].m_type = TYPE_ENEMY;
			g_hitCircle[i].m_Matrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_hitCircle[i].m_Matrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_hitCircle[i].m_Color = D3DCOLOR_RGBA(200, 10, 10, 100);
			g_hitCircle[i].nID = i;

			//頂点バッファの生成
			if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*g_hitCircle[i].m_nVertexNum, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_hitCircle[i].m_pVtxBuffer, NULL)))
			{
				return E_FAIL;
			}

			//インデックスバッファの生成
			HRESULT hr;
			hr = pD3DDevice->CreateIndexBuffer(
				sizeof(WORD)*g_hitCircle[i].m_nIndexNum,     //バッファ量(2バイト)
				D3DUSAGE_WRITEONLY,        //使用方法フラグ
				D3DFMT_INDEX16,              //インデックスフォーマット(重複しない頂点数は65536個は限界)
				D3DPOOL_MANAGED,           //メモリ管理方法
				&g_hitCircle[i].m_pIdxBufferField,        //インデックスバッファインターフェースへのポインタのアドレス
				NULL);

			if (FAILED(hr))
			{
				return E_FAIL;
			}

			InitVertexIndex(g_hitCircle[i].nID);                                      //頂点バッファのロックと頂点じゃないバッファのロック
			g_hitCircle[i].nEnemyID = nEnemyID;                                             //使用先にIDを渡す
			g_hitCircle[i].bUse = true;
			break;
		}
	}

	return S_OK;
}

void SetCirclePos(const D3DXVECTOR3 *pos, int nID)
{
	g_hitCircle[nID].m_Matrix.pos = *pos;
}

HITCIRCLE *Get_HitCircle(int nID)
{
	return &g_hitCircle[nID];
}