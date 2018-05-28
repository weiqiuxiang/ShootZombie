//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "hitRectangle .h"
#include "camera.h"
#include "TpsCamera.h"
#include "hitRectangle .h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//マグロ
//-----------------------------------------------------------------------------------------
#define HR_COLOR_R (0)
#define HR_COLOR_G (120)
#define HR_COLOR_B (135)
#define HR_COLOR_ALPHA (100)

//-----------------------------------------------------------------------------------------
//プロトタイプ
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureHitRect = NULL;          //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferHitRect = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
HITRECT g_HitRect[HITRECT_MAX];                  //頂点当り判定配列

#ifdef _DEBUG
bool g_Check;
#endif

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitHitRect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_3D)*HITRECT_MAX *4,D3DUSAGE_WRITEONLY,FVF_VERTEX3D,D3DPOOL_MANAGED,&g_pVtxBufferHitRect,NULL)))
	{
		return E_FAIL;
	}

	//テクスチャ読み込む
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, HITRECT_TEXTURENAME,&g_pTextureHitRect) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	MakeVertexHitRect(pD3DDevice, pVtx);
	g_pVtxBufferHitRect->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

#ifdef _DEBUG
	g_Check = false;
#endif

	for (int i = 0; i < HITRECT_MAX; i++)
	{
		//モデルワールド変換マトリクスの初期化
		g_HitRect[i].m_HitRectMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_HitRect[i].m_HitRectMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_HitRect[i].m_HitRectMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_HitRect[i].bUse = false;

		//頂点データ初期化
		for(int j = 0; j < 4; j++)
		{
			g_HitRect[i].Vectex[j].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_HitRect[i].Vectex[j].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_HitRect[i].Vectex[j].tex = D3DXVECTOR2(0.0f,0.0f);
			g_HitRect[i].Vectex[j].color = D3DCOLOR_RGBA(255,255,255,255);
			g_HitRect[i].VectexC[j] = g_HitRect[i].Vectex[j];
		}

		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	g_pVtxBufferHitRect->Unlock();                                     //バッファのアンロック
	return S_OK;
}

//ポリゴンの更新
void UpdateHitRect(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ

													  //
													  //バレットエフェクト頂点ロック
													  ////////////////////////////////
	g_pVtxBufferHitRect->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexHitRect(pD3DDevice, pVtx);  //設定
	for (int i = 0; i < HITRECT_MAX; i++)
	{
		if (g_HitRect[i].bUse == true)
		{
			int k = i * 4;
			pVtx[0 + k].pos = g_HitRect[i].VectexC[0].pos;
			pVtx[1 + k].pos = g_HitRect[i].VectexC[1].pos;
			pVtx[2 + k].pos = g_HitRect[i].VectexC[2].pos;
			pVtx[3 + k].pos = g_HitRect[i].VectexC[3].pos;

			pVtx[0 + k].tex = g_HitRect[i].VectexC[0].tex;
			pVtx[1 + k].tex = g_HitRect[i].VectexC[1].tex;
			pVtx[2 + k].tex = g_HitRect[i].VectexC[2].tex;
			pVtx[3 + k].tex = g_HitRect[i].VectexC[3].tex;

			pVtx[0 + k].color = g_HitRect[i].VectexC[0].color;
			pVtx[1 + k].color = g_HitRect[i].VectexC[1].color;
			pVtx[2 + k].color = g_HitRect[i].VectexC[2].color;
			pVtx[3 + k].color = g_HitRect[i].VectexC[3].color;

			pVtx[0 + k].normal = g_HitRect[i].VectexC[0].normal;
			pVtx[1 + k].normal = g_HitRect[i].VectexC[1].normal;
			pVtx[2 + k].normal = g_HitRect[i].VectexC[2].normal;
			pVtx[3 + k].normal = g_HitRect[i].VectexC[3].normal;
		}
	}

	g_pVtxBufferHitRect->Unlock();              //アンロック
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawHitRect(void)
{
#ifdef _DEBUG
	if(GetKeyboardTrigger(DIK_F1))
	{
		if (g_Check == false) g_Check = true;
		else g_Check = false;
	}
	if (g_Check == true)
	{
		LPDIRECT3DDEVICE9 pD3DDevice;
		pD3DDevice = Get_Device();
		D3DXMATRIX IdentityMtx;
		pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //ライトをオフにする
		D3DXMatrixIdentity(&IdentityMtx);                    //単位行列で初期化
		pD3DDevice->SetTransform(D3DTS_WORLD, &IdentityMtx);                //ワールド変換行列

		//コベニちゃん:頂点フォーマット設定
		for (int i = 0; i < HITRECT_MAX; i++)
		{
			if (g_HitRect[i].bUse == true)
			{
				pD3DDevice->SetStreamSource(0, g_pVtxBufferHitRect, 0, sizeof(VERTEX_3D));                     //VRAMからGPUにデータをストリーム
				pD3DDevice->SetFVF(FVF_VERTEX3D);
				pD3DDevice->SetTexture(0, NULL);												 //テクスチャの設定
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);                                  //テクスチャメモリの確保
			}
		}
	}
#endif
}

void UninitHitRect(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE (g_pTextureHitRect);                      //テクスチャインターフェイスの片付け

	//頂点バッファを解放する
	SAFE_RELEASE (g_pVtxBufferHitRect);                       //頂点バッファインターフェイスの片付け
}

//ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexHitRect(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************デバイス設定***********************************************/
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//判定四角ポリゴンセット
void SetHitRect(const WORLD_MATRIX *pResultMatrix, const D3DXVECTOR2 *HW, MESHTYPE meshType)
{
	int nCnt;
	float fWidthHalf, fHeightHalf;
	D3DXMATRIX rot_VecXZ;

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//判定ポリゴン未使用なら
		if (g_HitRect[nCnt].bUse == false)
		{
			g_HitRect[nCnt].m_HitRectMatrix = *pResultMatrix;
			g_HitRect[nCnt].meshType = meshType;
			fWidthHalf = HW->x*0.5;
			fHeightHalf = HW->y*0.5;
			//
			//頂点データの初期化
			///////////////////////////////////////////
			g_HitRect[nCnt].Vectex[0].pos = D3DXVECTOR3(-fWidthHalf, 0.0f, fHeightHalf);
			g_HitRect[nCnt].Vectex[1].pos = D3DXVECTOR3(fWidthHalf, 0.0f, fHeightHalf);
			g_HitRect[nCnt].Vectex[2].pos = D3DXVECTOR3(-fWidthHalf, 0.0f, -fHeightHalf);
			g_HitRect[nCnt].Vectex[3].pos = D3DXVECTOR3(fWidthHalf, 0.0f, -fHeightHalf);

			g_HitRect[nCnt].Vectex[0].color = D3DCOLOR_RGBA(HR_COLOR_R, HR_COLOR_G, HR_COLOR_B, HR_COLOR_ALPHA);
			g_HitRect[nCnt].Vectex[1].color = D3DCOLOR_RGBA(HR_COLOR_R, HR_COLOR_G, HR_COLOR_B, HR_COLOR_ALPHA);
			g_HitRect[nCnt].Vectex[2].color = D3DCOLOR_RGBA(HR_COLOR_R, HR_COLOR_G, HR_COLOR_B, HR_COLOR_ALPHA);
			g_HitRect[nCnt].Vectex[3].color = D3DCOLOR_RGBA(HR_COLOR_R, HR_COLOR_G, HR_COLOR_B, HR_COLOR_ALPHA);

			g_HitRect[nCnt].Vectex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_HitRect[nCnt].Vectex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_HitRect[nCnt].Vectex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_HitRect[nCnt].Vectex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			g_HitRect[nCnt].Vectex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			g_HitRect[nCnt].Vectex[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			g_HitRect[nCnt].Vectex[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			g_HitRect[nCnt].Vectex[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			for (int i = 0; i < 4; i++)
			{
				g_HitRect[nCnt].VectexC[i] = g_HitRect[nCnt].Vectex[i];
				SetChangeVertex(&g_HitRect[nCnt].VectexC[i].pos,&g_HitRect[nCnt].m_HitRectMatrix);     //判定座標セット
				SetChangeNormarlize(&g_HitRect[nCnt].VectexC[i].normal, &g_HitRect[nCnt].m_HitRectMatrix);    //判定法線セット
				D3DXVec3Normalize(&g_HitRect[nCnt].VectexC[i].normal, &g_HitRect[nCnt].VectexC[i].normal); //単位ベクトル化
			}

			//プレーヤー衝突移動ベクトルの算出
			g_HitRect[nCnt].VecXZ = D3DXVECTOR3(g_HitRect[nCnt].VectexC[0].normal.x,0.0f, g_HitRect[nCnt].VectexC[0].normal.z);
			D3DXMatrixRotationYawPitchRoll(&rot_VecXZ, D3DX_PI*0.5,0.0f,0.0f);                     //Y軸を中心に90度回転
			D3DXVec3TransformCoord(&g_HitRect[nCnt].VecXZ, &g_HitRect[nCnt].VecXZ, &rot_VecXZ);            //プレーヤー衝突ベクトルの計算
			D3DXVec3Normalize(&g_HitRect[nCnt].VecXZ, &g_HitRect[nCnt].VecXZ);                       //単位ベクトル化
			//////////////////////////////////////////////////////

			g_HitRect[nCnt].nID = nCnt;
			g_HitRect[nCnt].bUse = true;
			break;
		}
	}

#ifdef _DEBUG
	if (nCnt >= HITRECT_MAX)
	{
		MessageBox(NULL, "使用可能の判定ポリゴン数は0以下です", "警告", MB_OK | MB_ICONHAND);  //判定ポリゴン使用数がMAXに超えた
	}
#endif
}

//判定四角ポリゴンのデータを取得
HITRECT *Get_HitRect(void)
{
	return &g_HitRect[0];
}