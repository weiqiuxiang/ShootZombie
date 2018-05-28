//-----------------------------------------------------------------------------------------
//プレーヤー,道具などの処理[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "particalCenter.h"
#include "partical.h"
#include "camera.h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------------------
#define PARTICALRADIUS (10)
#define PLUS_EFFECT_NUM (3)
//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
void UpdateParticalBase(void);
void UpdateParticalFire(void);
void QuickSort_Draw(void);                   //クイックソート法を使ってパーティクルの描画順を決める
void QuickSort(int nLeftID, int nRightID);                       //再帰的なクリックソート関数
void DrawBase(void);
void DrawFire(void);

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureParticalPolygon[NUM_PARTICALTEXTURE] = {};           //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticalPolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFirePartical = NULL;      //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

PARTICAL g_Partical[MAX_PARTICAL][PLUS_EFFECT_NUM];
PARTICAL g_ParticalFire[MAX_PARTICAL][PLUS_EFFECT_NUM];                     //炎のパーティクル

//クイックソート用変数
int g_aQuickSortID[MAX_PARTICAL];                 //整列の後のクイックソート
float g_aDistance[MAX_PARTICAL];                      //パーティクルからカメラまでの距離配列
int g_nUseParticalNum;                             //使用中のパーティクル総数

//-----------------------------------------------------------------------------------------
//重要定番関数達
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//各変数の初期化
//-----------------------------------------------------------------------------------------
HRESULT InitPartical(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();
	 
	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_PARTICALVERTEX * MAX_PARTICAL * PLUS_EFFECT_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferParticalPolygon, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_PARTICALVERTEX * MAX_PARTICAL * PLUS_EFFECT_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferFirePartical, NULL)))
	{
		return E_FAIL;
	}

	//パーティクルのテクスチャ取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALPOLYGON01_TEXTURENAME, &g_pTextureParticalPolygon[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}
	//水エフェクトのテクスチャ取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALPOLYGONEFFECT_TEXTURENAME, &g_pTextureParticalPolygon[1])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}
	//炎エフェクトのテクスチャ取得
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, FIREEFFECT_TEXTURENAME, &g_pTextureParticalPolygon[2])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//プレーヤーのポリゴン
	g_pVtxBufferParticalPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPartical(pD3DDevice, pVtx);

	g_nUseParticalNum = 0;
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
		{
			//パーティクルの諸データの初期値
			D3DXMatrixIdentity(&g_Partical[nCnt][nCnt2].MtxWorld);  //ワールドマトリクス初期化

			g_Partical[nCnt][nCnt2].MtxSRT.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Partical[nCnt][nCnt2].MtxSRT.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Partical[nCnt][nCnt2].MtxSRT.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Partical[nCnt][nCnt2].speed.x = 0;
			g_Partical[nCnt][nCnt2].speed.y = 0;
			g_Partical[nCnt][nCnt2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_Partical[nCnt][nCnt2].nAlpha = 255;
			g_Partical[nCnt][nCnt2].nLife = 20;
			g_Partical[nCnt][nCnt2].bUse = false;
			g_Partical[nCnt][nCnt2].fRadius = 0;
			g_Partical[nCnt][nCnt2].fColorAlphaValue = 255 / g_Partical[nCnt][nCnt2].nLife;

			pVtx[0].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);

			pVtx += 4;
		}
	}

	g_pVtxBufferParticalPolygon->Unlock();                     //バッファのアンロック

	//fireパーティクルをロック
	g_pVtxBufferFirePartical->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

	//fireパーティクル初期化
	for(int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
		{
			D3DXMatrixIdentity(&g_ParticalFire[nCnt][nCnt2].MtxWorld);  //ワールドマトリクス初期化
			g_ParticalFire[nCnt][nCnt2].MtxSRT.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_ParticalFire[nCnt][nCnt2].MtxSRT.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_ParticalFire[nCnt][nCnt2].MtxSRT.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_ParticalFire[nCnt][nCnt2].speed.x = 0;
			g_ParticalFire[nCnt][nCnt2].speed.y = 0;
			g_ParticalFire[nCnt][nCnt2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_ParticalFire[nCnt][nCnt2].nAlpha = 255;
			g_ParticalFire[nCnt][nCnt2].nLife = 20;
			g_ParticalFire[nCnt][nCnt2].bUse = false;
			g_ParticalFire[nCnt][nCnt2].fRadius = 0;
			g_ParticalFire[nCnt][nCnt2].fColorAlphaValue = 255 / g_ParticalFire[nCnt][nCnt2].nLife;

			pVtx[0].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);

			pVtx += 4;
		}
	}

	g_pVtxBufferFirePartical->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	
	return S_OK;
}

//-----------------------------------------------------------------------------------------
//終了処理
//-----------------------------------------------------------------------------------------
void UninitPartical(void)
{
	// テクスチャインターフェイスの片付け
	for (int nCnt = 0; nCnt < NUM_PARTICALTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureParticalPolygon[nCnt]);
	}

	//頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferParticalPolygon);
	SAFE_RELEASE(g_pVtxBufferFirePartical);
}


//-----------------------------------------------------------------------------------------
//パーティクル更新処理
//-----------------------------------------------------------------------------------------
void UpdatePartical(void)
{
	UpdateParticalBase();
	UpdateParticalFire();
	UpdatePolygonPartical();
}
//-----------------------------------------------------------------------------------------
//BASE
//-----------------------------------------------------------------------------------------
void UpdateParticalBase(void)
{
	//パーティクル発射中心データ取得
	PARTICALCENTER *pParticalCenter;
	pParticalCenter = GetParticalCenter();

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//もしパーティクルは使用可の状態なら
		if (g_Partical[nCnt][0].bUse == true)
		{
			//風力計算
			g_Partical[nCnt][0].speed.x += pParticalCenter->fWindPower * cos(pParticalCenter->fWindAngel);
			g_Partical[nCnt][0].speed.y += pParticalCenter->fWindPower * sin(pParticalCenter->fWindAngel);

			//重力計算
			g_Partical[nCnt][0].speed.y -= pParticalCenter->fGravity;

			//パーティクル移動
			g_Partical[nCnt][0].MtxSRT.pos.x += g_Partical[nCnt][0].speed.x;
			g_Partical[nCnt][0].MtxSRT.pos.y += g_Partical[nCnt][0].speed.y;
			g_Partical[nCnt][0].MtxSRT.pos.z += g_Partical[nCnt][0].speed.z;

			//パーティクル終了処理
			g_Partical[nCnt][0].nLife--;                                   //Lifeが減る
			g_Partical[nCnt][0].nAlpha -= g_Partical[nCnt][0].fColorAlphaValue; //α値が減る

			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				//エフェクト部分
				g_Partical[nCnt][nCnt2].nAlpha -= g_Partical[nCnt][nCnt2].fColorAlphaValue;    //エフェクトα減少

																							   //必要な値一旦保存
				D3DCOLOR color = g_Partical[nCnt][nCnt2].color;
				float nAlpha = g_Partical[nCnt][nCnt2].nAlpha;
				float fColorAlphaValue = g_Partical[nCnt][nCnt2].fColorAlphaValue;

				g_Partical[nCnt][nCnt2] = g_Partical[nCnt][0];                    //情報をエフェクトに与える

				g_Partical[nCnt][nCnt2].color = color;
				g_Partical[nCnt][nCnt2].nAlpha = nAlpha;
				g_Partical[nCnt][nCnt2].fColorAlphaValue = fColorAlphaValue;
				g_Partical[nCnt][nCnt2].fRadius = g_Partical[nCnt][1].fRadius * 2;
			}

			//lifeが0以下になったら未使用にする
			if (g_Partical[nCnt][0].nLife <= 0)
			{
				g_Partical[nCnt][0].bUse = false;
				for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
				{
					g_Partical[nCnt][nCnt2].bUse = false;
				}
			}
		}
	}

	QuickSort_Draw();                   //クイックソートで描画順番作成
}
//-----------------------------------------------------------------------------------------
//Fire
//-----------------------------------------------------------------------------------------
void UpdateParticalFire(void)
{
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//パーティクル移動
		g_ParticalFire[nCnt][0].MtxSRT.pos.x += g_ParticalFire[nCnt][0].speed.x;
		g_ParticalFire[nCnt][0].MtxSRT.pos.y += g_ParticalFire[nCnt][0].speed.y;
		g_ParticalFire[nCnt][0].MtxSRT.pos.z += g_ParticalFire[nCnt][0].speed.z;

		//パーティクル終了処理
		g_ParticalFire[nCnt][0].nLife--;                                   //Lifeが減る

		for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
		{
			//エフェクト部分
			g_ParticalFire[nCnt][nCnt2].nAlpha -= g_ParticalFire[nCnt][nCnt2].fColorAlphaValue;    //エフェクトα減少

																						   //必要な値一旦保存
			D3DCOLOR color = g_ParticalFire[nCnt][nCnt2].color;
			float nAlpha = g_ParticalFire[nCnt][nCnt2].nAlpha;
			float fColorAlphaValue = g_ParticalFire[nCnt][nCnt2].fColorAlphaValue;

			g_ParticalFire[nCnt][nCnt2] = g_ParticalFire[nCnt][0];                    //情報をエフェクトに与える

			g_ParticalFire[nCnt][nCnt2].color = color;
			g_ParticalFire[nCnt][nCnt2].nAlpha = nAlpha;
			g_ParticalFire[nCnt][nCnt2].fColorAlphaValue = fColorAlphaValue;
			g_ParticalFire[nCnt][nCnt2].fRadius = g_ParticalFire[nCnt][1].fRadius * 2;

			//lifeが0以下になったら未使用にする
			if (g_ParticalFire[nCnt][0].nLife <= 0)
			{
				g_ParticalFire[nCnt][0].bUse = false;
				for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
				{
					g_ParticalFire[nCnt][nCnt2].bUse = false;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
//プレーヤーポリゴンのデータ更新
//-----------------------------------------------------------------------------------------
void UpdatePolygonPartical(void)
{
	VERTEX_3D *pVtx = NULL;
	VERTEX_3D *pVtxFire = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	g_pVtxBufferParticalPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //仮想アドレスを取得しバッファをロックする
	MakeVertexPolygonPartical(pD3DDevice, pVtx);

	//パーティクルロックアンロック
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if(g_Partical[nCnt][0].bUse == true)
		{
			int k = nCnt * 4 * 2;
			pVtx[0 + k].pos = D3DXVECTOR3(-g_Partical[nCnt][0].fRadius,g_Partical[nCnt][0].fRadius, 0.0f);
			pVtx[1 + k].pos = D3DXVECTOR3(g_Partical[nCnt][0].fRadius,g_Partical[nCnt][0].fRadius, 0.0f);
			pVtx[2 + k].pos = D3DXVECTOR3(-g_Partical[nCnt][0].fRadius,-g_Partical[nCnt][0].fRadius, 0.0f);
			pVtx[3 + k].pos = D3DXVECTOR3(g_Partical[nCnt][0].fRadius,-g_Partical[nCnt][0].fRadius, 0.0f);

			g_Partical[nCnt][0].color = D3DCOLOR_RGBA(70,70,0,g_Partical[nCnt][0].nAlpha);

			pVtx[0 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[0+k].color = g_Partical[nCnt][0].color;
			pVtx[1+k].color = g_Partical[nCnt][0].color;
			pVtx[2+k].color = g_Partical[nCnt][0].color;
			pVtx[3+k].color = g_Partical[nCnt][0].color;

			pVtx[0+k].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+k].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+k].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+k].tex = D3DXVECTOR2(1.0f, 1.0f);

			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				int p = k + 4 * nCnt2;

				//エフェクト部分
				pVtx[0 + p].pos = D3DXVECTOR3(-g_Partical[nCnt][nCnt2].fRadius, g_Partical[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[1 + p].pos = D3DXVECTOR3(g_Partical[nCnt][nCnt2].fRadius, g_Partical[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[2 + p].pos = D3DXVECTOR3(-g_Partical[nCnt][nCnt2].fRadius, -g_Partical[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[3 + p].pos = D3DXVECTOR3(g_Partical[nCnt][nCnt2].fRadius, -g_Partical[nCnt][nCnt2].fRadius, 0.0f);

				g_Partical[nCnt][nCnt2].color = D3DCOLOR_RGBA(5, 102, 184, 255);   //水のRGB

				pVtx[0 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[1 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[2 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[3 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				pVtx[0 + p].color = g_Partical[nCnt][nCnt2].color;
				pVtx[1 + p].color = g_Partical[nCnt][nCnt2].color;
				pVtx[2 + p].color = g_Partical[nCnt][nCnt2].color;
				pVtx[3 + p].color = g_Partical[nCnt][nCnt2].color;

				pVtx[0 + p].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1 + p].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2 + p].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3 + p].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}

	g_pVtxBufferParticalPolygon -> Unlock();                                     //バッファのアンロック

	//炎のパーティクル頂点ロック
	g_pVtxBufferFirePartical->Lock(0, 0, (void**)&pVtxFire, 0);          //仮想アドレスを取得しバッファをロックする

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if (g_ParticalFire[nCnt][0].bUse == true)
		{
			int k = nCnt * 4 * 2;
			pVtx[0 + k].pos = D3DXVECTOR3(-g_ParticalFire[nCnt][0].fRadius, g_ParticalFire[nCnt][0].fRadius, 0.0f);
			pVtx[1 + k].pos = D3DXVECTOR3(g_ParticalFire[nCnt][0].fRadius, g_ParticalFire[nCnt][0].fRadius, 0.0f);
			pVtx[2 + k].pos = D3DXVECTOR3(-g_ParticalFire[nCnt][0].fRadius, -g_ParticalFire[nCnt][0].fRadius, 0.0f);
			pVtx[3 + k].pos = D3DXVECTOR3(g_ParticalFire[nCnt][0].fRadius, -g_ParticalFire[nCnt][0].fRadius, 0.0f);

			g_ParticalFire[nCnt][0].color = D3DCOLOR_RGBA(70, 70, 0, g_ParticalFire[nCnt][0].nAlpha);

			pVtx[0 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[0 + k].color = g_ParticalFire[nCnt][0].color;
			pVtx[1 + k].color = g_ParticalFire[nCnt][0].color;
			pVtx[2 + k].color = g_ParticalFire[nCnt][0].color;
			pVtx[3 + k].color = g_ParticalFire[nCnt][0].color;

			pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				int p = k + 4 * nCnt2;

				//エフェクト部分
				pVtx[0 + p].pos = D3DXVECTOR3(-g_ParticalFire[nCnt][nCnt2].fRadius, g_ParticalFire[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[1 + p].pos = D3DXVECTOR3(g_ParticalFire[nCnt][nCnt2].fRadius, g_ParticalFire[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[2 + p].pos = D3DXVECTOR3(-g_ParticalFire[nCnt][nCnt2].fRadius, -g_ParticalFire[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[3 + p].pos = D3DXVECTOR3(g_ParticalFire[nCnt][nCnt2].fRadius, -g_ParticalFire[nCnt][nCnt2].fRadius, 0.0f);

				g_ParticalFire[nCnt][nCnt2].color = D3DCOLOR_RGBA(5, 102, 184, 255);   //水のRGB

				pVtx[0 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[1 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[2 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[3 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				pVtx[0 + p].color = g_ParticalFire[nCnt][nCnt2].color;
				pVtx[1 + p].color = g_ParticalFire[nCnt][nCnt2].color;
				pVtx[2 + p].color = g_ParticalFire[nCnt][nCnt2].color;
				pVtx[3 + p].color = g_ParticalFire[nCnt][nCnt2].color;

				pVtx[0 + p].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1 + p].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2 + p].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3 + p].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}
	g_pVtxBufferFirePartical->Unlock();                                     //バッファのアンロック
}

//-----------------------------------------------------------------------------------------
//パーティクル描画
//-----------------------------------------------------------------------------------------
void DrawPartical(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //ライトをオフにする

	pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);                               //zバッファを参照
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);                               //z情報残さない

	DrawBase();//水パーティクル描画

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //ブレンドモードをアルファブレンドに戻す

	DrawFire();//炎パーティクル描画

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //ブレンドモードをアルファブレンドに戻す
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      //z情報残す
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトをオンにする
}

//ベース
void DrawBase(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	for (int nCnt = g_nUseParticalNum - 1; nCnt >= 0; nCnt--)
	{
		//パーティクルが使われている
		if (g_Partical[g_aQuickSortID[nCnt]][0].bUse == true)
		{
			int k = g_aQuickSortID[nCnt] * 4 * 2;

			SetBillboardMatrix(&g_Partical[g_aQuickSortID[nCnt]][0].MtxWorld, g_Partical[g_aQuickSortID[nCnt]][0].MtxSRT);

			//パーティクル
			pD3DDevice->SetStreamSource(0, g_pVtxBufferParticalPolygon, 0, sizeof(VERTEX_3D));              //VRAMからGPUにデータをストリーム
			pD3DDevice->SetFVF(FVF_VERTEX3D);
			pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[0]);				 //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, k, 2);                                            //スプライト描画

																											 //エフェクト
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                             //加算合成を行う!
			pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[1]);				 //テクスチャの設定
			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				int p = k + 4 * nCnt2;
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, p, 2);                                            //エフェクト描画
			}
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //ブレンドモードをアルファブレンドに戻す
		}
	}
}

//炎
void DrawFire(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//炎パーティクル
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//パーティクルが使われている
		if (g_ParticalFire[nCnt][0].bUse == true)
		{
			int k = nCnt * 4 * 2;

			SetBillboardMatrix(&g_ParticalFire[nCnt][0].MtxWorld, g_ParticalFire[nCnt][0].MtxSRT);

			//パーティクル
			pD3DDevice->SetStreamSource(0, g_pVtxBufferFirePartical, 0, sizeof(VERTEX_3D));              //VRAMからGPUにデータをストリーム
			pD3DDevice->SetFVF(FVF_VERTEX3D);
			pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[0]);				 //テクスチャの設定
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, k, 2);                                            //スプライト描画

																											 //エフェクト
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                       //加算合成を行う!
			pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[2]);				 //テクスチャの設定
			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				int p = k + 4 * nCnt2;
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, p, 2);                                            //エフェクト描画
			}
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //ブレンドモードをアルファブレンドに戻す
		}
	}
}


//プレーヤーポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonPartical(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//----------------------------------
//パーティクルを設置
//----------------------------------
void SetPartical(float fSpeedX,float fSpeedY, float fSpeedZ,float fRadius,float nLife)
{
	for(int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if(g_Partical[nCnt][0].bUse == false)
		{
			D3DXMatrixIdentity(&g_Partical[nCnt][0].MtxWorld);  //ワールドマトリクス初期化
			g_Partical[nCnt][0].MtxSRT.pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_Partical[nCnt][0].MtxSRT.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Partical[nCnt][0].MtxSRT.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Partical[nCnt][0].speed.x = fSpeedX;
			g_Partical[nCnt][0].speed.y = fSpeedY;
			g_Partical[nCnt][0].speed.z = fSpeedZ;
			g_Partical[nCnt][0].color = D3DCOLOR_RGBA(255,255,255,255);
			g_Partical[nCnt][0].fRadius = fRadius;
			g_Partical[nCnt][0].nLife = nLife;
			g_Partical[nCnt][0].nAlpha = 20;
			g_Partical[nCnt][0].fColorAlphaValue = g_Partical[nCnt][0].nAlpha / g_Partical[nCnt][0].nLife;
			g_Partical[nCnt][0].bUse = true;
			g_Partical[nCnt][0].nID = nCnt;

			//エフェクトの方もtrueにする
			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				g_Partical[nCnt][nCnt2] = g_Partical[nCnt][0];
				g_Partical[nCnt][nCnt2].nAlpha = 255;
				g_Partical[nCnt][nCnt2].fColorAlphaValue = g_Partical[nCnt][nCnt2].nAlpha / g_Partical[nCnt][nCnt2].nLife;
				g_Partical[nCnt][nCnt2].bUse = true; 
			}
			break;
		}
	}
}

//----------------------------------
//炎パーティクル
//----------------------------------
void SetFirePartical(PARTICALCENTER *pParticalCenter, float fFirePosX ,float fSpeedX, float fSpeedY, float fSpeedZ, float nLife)
{
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if (g_ParticalFire[nCnt][0].bUse == false)
		{
			D3DXMatrixIdentity(&g_ParticalFire[nCnt][0].MtxWorld);  //ワールドマトリクス初期化
			g_ParticalFire[nCnt][0].MtxSRT.pos = D3DXVECTOR3(pParticalCenter->pos.x + fFirePosX, pParticalCenter->pos.y, pParticalCenter->pos.z);
			g_ParticalFire[nCnt][0].MtxSRT.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_ParticalFire[nCnt][0].MtxSRT.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_ParticalFire[nCnt][0].speed.x = fSpeedX;
			g_ParticalFire[nCnt][0].speed.y = fSpeedY;
			g_ParticalFire[nCnt][0].speed.z = fSpeedZ;
			g_ParticalFire[nCnt][0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_ParticalFire[nCnt][0].fRadius = pParticalCenter->fRadius;
			g_ParticalFire[nCnt][0].nLife = nLife;
			g_ParticalFire[nCnt][0].nAlpha = 20;
			g_ParticalFire[nCnt][0].fColorAlphaValue = g_ParticalFire[nCnt][0].nAlpha / g_ParticalFire[nCnt][0].nLife;
			g_ParticalFire[nCnt][0].bUse = true;
			g_ParticalFire[nCnt][0].nID = nCnt;

			//エフェクトの方もtrueにする
			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				g_ParticalFire[nCnt][nCnt2] = g_ParticalFire[nCnt][0];
				g_ParticalFire[nCnt][nCnt2].nAlpha = 255;
				g_ParticalFire[nCnt][nCnt2].fColorAlphaValue = g_ParticalFire[nCnt][nCnt2].nAlpha / g_ParticalFire[nCnt][nCnt2].nLife;
				g_ParticalFire[nCnt][nCnt2].bUse = true;
			}

			break;
		}
	}
}

//----------------------------------
//クイックソートで描画順番を決める
//----------------------------------
void QuickSort_Draw(void)
{
	CAMERA_FPS *pCameraFps = GetCameraFps();            //カメラ取得
	int nUseParticalNum = 0;                          //使用中のパーティクル数

	for (int nCnt = 0; nCnt < MAX_PARTICAL;nCnt++)
	{
		//距離とIDの登録
		if (g_Partical[nCnt][0].bUse == true)
		{
			//距離計算
			float x = g_Partical[nCnt][0].MtxSRT.pos.x - pCameraFps->pos.x;
			float y = g_Partical[nCnt][0].MtxSRT.pos.y - pCameraFps->pos.y;
			float z = g_Partical[nCnt][0].MtxSRT.pos.z - pCameraFps->pos.z;
			g_aDistance[nUseParticalNum] = x*x + y*y + z*z;
			g_aQuickSortID[nUseParticalNum] = g_Partical[nCnt][0].nID;                //ID登録
			nUseParticalNum++; //カウントアップ
		}
	}

	g_nUseParticalNum = nUseParticalNum;                   //総数を渡す
	//QuickSort(0, nUseParticalNum-1);                       //クイックソート
}

//----------------------------------
//クイックソート:引数1:START_ID 引数2:END_ID
//----------------------------------
void QuickSort(int nLeftID, int nRightID)
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

	int aQuickSortIDL[MAX_PARTICAL];                 //クイックソートID左配列
	int aQuickSortIDR[MAX_PARTICAL];                 //クイックソートID右配列
	float aDistanceL[MAX_PARTICAL];                    //パーティクルからカメラまでの距離左配列
	float aDistanceR[MAX_PARTICAL];                    //パーティクルからカメラまでの距離右配列

	//---------------------------
	//左と右分割
	//---------------------------
	for (int i = 1; i <= nLeftNum_BE; i++)
	{
		//---------------------------
		//中間値前のメンバ値チェック
		//---------------------------
		//左配列に入れる
		if (g_aDistance[nMiddleID - i] < g_aDistance[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = g_aDistance[nMiddleID - i];          //距離配列に入れる
			aQuickSortIDL[nLeftNum_AF] = g_aQuickSortID[nMiddleID - i];    //ID配列に入れる
			nLeftNum_AF++;                                         //メンバ数カウントアップ
		}
		//右配列に入れる
		else
		{
			aDistanceR[nRightNum_AF] = g_aDistance[nMiddleID - i];  //配列に入れる
			aQuickSortIDR[nRightNum_AF] = g_aQuickSortID[nMiddleID - i];    //ID配列に入れる
			nRightNum_AF++;                                         //メンバ数カウントアップ
		}

		//---------------------------
		//中間値後ろのメンバ値チェック
		//---------------------------
		//左配列に入れる
		if (g_aDistance[nMiddleID + i] < g_aDistance[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = g_aDistance[nMiddleID + i];          //距離配列に入れる
			aQuickSortIDL[nLeftNum_AF] = g_aQuickSortID[nMiddleID + i];    //ID配列に入れる
			nLeftNum_AF++;                                         //メンバ数カウントアップ
		}
		//右配列に入れる
		else
		{
			aDistanceR[nRightNum_AF] = g_aDistance[nMiddleID + i];  //配列に入れる
			aQuickSortIDR[nRightNum_AF] = g_aQuickSortID[nMiddleID + i];    //ID配列に入れる
			nRightNum_AF++;                                         //メンバ数カウントアップ
		}
	}

	//総数が偶数の場合
	if(nRightNum_BE > nLeftNum_BE)
	{
		//左配列に入れる
		if (g_aDistance[nRightID] < g_aDistance[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = g_aDistance[nRightID];          //距離配列に入れる
			aQuickSortIDL[nLeftNum_AF] = g_aQuickSortID[nRightID];    //ID配列に入れる
			nLeftNum_AF++;                                         //メンバ数カウントアップ
		}
		//右配列に入れる
		else
		{
			aDistanceR[nRightNum_AF] = g_aDistance[nRightID];  //配列に入れる
			aQuickSortIDR[nRightNum_AF] = g_aQuickSortID[nRightID];    //ID配列に入れる
			nRightNum_AF++;                                         //メンバ数カウントアップ
		}
	}

	//----------------------------
	//分割の後の配列を配列に反映する
	//----------------------------
	nCnt = nLeftID;              //カウント初期化
	float MIDistance = g_aDistance[nMiddleID];      //中間値保存
	int MIID = g_aQuickSortID[nMiddleID];      //中間値保存

	//左部分
	for(int j = 0; j < nLeftNum_AF; j++)
	{
		g_aDistance[nCnt] = aDistanceL[j];        //距離代入
		g_aQuickSortID[nCnt] = aQuickSortIDL[j];  //ID代入
		nCnt++;            //カウントアップ
	}

	//中間値
	g_aDistance[nCnt] = MIDistance;        //距離代入
	g_aQuickSortID[nCnt] = MIID;  //ID代入
	nCnt++;       //カウントアップ

	//右部分
	for (int j = 0; j < nRightNum_AF; j++)
	{
		g_aDistance[nCnt] = aDistanceR[j];        //距離代入
		g_aQuickSortID[nCnt] = aQuickSortIDR[j];  //ID代入
		nCnt++;            //カウントアップ
	}

	//------------------
	//再帰部分
	//------------------
	//左部分
	if(nLeftNum_AF > 1)
	{
		QuickSort(nLeftID, nLeftID+ nLeftNum_AF-1);
	}

	//右部分
	if (nRightNum_AF > 1)
	{
		QuickSort(nRightID- nRightNum_AF+1, nRightID);
	}
}