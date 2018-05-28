//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include "model_aircraft.h"
#include "MeshField.h"
#include "shadow.h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------------------
#define STAND_RADIUS (4.0f)    //標準半径
#define RADIUS_VALUE (0.5f)    //単位距離の半径変化量
#define ALPHA_VALUE (10)       //単位距離の半径変化量

//-----------------------------------------------------------------------------------------
//プロトタイプ
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureShadow = {};          //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferShadow = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
SHADOW g_Shadow;    //爆発エフェクト

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	//モデルワールド変換マトリクスの初期化

	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferShadow, NULL)))
	{
		return E_FAIL;
	}

	//テクスチャ読み込む
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, SHADOW_TEXTURENAME, &g_pTextureShadow)))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//パラメータ初期化
	g_Shadow.m_ShadowMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Shadow.m_ShadowMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Shadow.m_ShadowMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Shadow.fStandRadius = STAND_RADIUS;
	g_Shadow.fRadiusValue = RADIUS_VALUE;
	g_Shadow.nAlpha = 255;
	g_Shadow.nAlphaValue = ALPHA_VALUE;

	//
	//頂点ロック
	//////////////////////
	g_pVtxBufferShadow->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexShadow(pD3DDevice, pVtx);

	pVtx[0].pos = D3DXVECTOR3(-STAND_RADIUS*0.5, STAND_RADIUS*0.5, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(STAND_RADIUS*0.5, STAND_RADIUS*0.5, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-STAND_RADIUS*0.5, -STAND_RADIUS*0.5, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(STAND_RADIUS*0.5, -STAND_RADIUS*0.5, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	g_pVtxBufferShadow->Unlock();                                     //バッファのアンロック

	return S_OK;
}

//
//ポリゴンの更新
/////////////////////////////
void UpdateShadow(void)
{
	//
	//変数宣言
	////////////////////////////////
	//デバイス取得
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ

	float fResultRadius;      //結果半径
	int fResultAlpha;         //結果アルファ
	float fDistanceM_F;       //モデルと地面の距離
	AIRCRAFT *pAircraft = Get_Aircraft();  //モデル座標取得
	MESHWALL *pMeshwall = GetMesh();           //メッシュの座標取得

	//計算部分
	fDistanceM_F = 0.0f;

	//モデルと地面の距離はマイナスにしないように
	if(fDistanceM_F < 0)
	{ 
		fDistanceM_F = 0;
	}

	fResultRadius = g_Shadow.fStandRadius + fDistanceM_F * g_Shadow.fRadiusValue;  //結果半径計算
	fResultAlpha = g_Shadow.nAlpha - fDistanceM_F * g_Shadow.nAlphaValue;  //結果アルファ値計算

	//アルファ値制限
	if(fResultAlpha < 0)
	{
		fResultAlpha = 0;
	}
	
	if (fResultAlpha > 255)
	{
		fResultAlpha = 255;
	}

	g_Shadow.m_ShadowMatrix.pos = D3DXVECTOR3(pAircraft->m_Matrix.pos.x, pAircraft->m_Matrix.pos.y - 4.0f, pAircraft->m_Matrix.pos.z);
	
	//
	//頂点ロック
	////////////////////////////////
	g_pVtxBufferShadow->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexShadow(pD3DDevice, pVtx);  //設定

	//頂点設定
	pVtx[0].pos = D3DXVECTOR3(-fResultRadius*0.5, 0.0f , fResultRadius*0.5);
	pVtx[1].pos = D3DXVECTOR3(fResultRadius*0.5, 0.0f , fResultRadius*0.5);
	pVtx[2].pos = D3DXVECTOR3(-fResultRadius*0.5, 0.0f , -fResultRadius*0.5);
	pVtx[3].pos = D3DXVECTOR3(fResultRadius*0.5, 0.0f , -fResultRadius*0.5);

	pVtx[0].tex = D3DXVECTOR2(0.0f , 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f , 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f , 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f , 1.0f);

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, fResultAlpha);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, fResultAlpha);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, fResultAlpha);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, fResultAlpha);

	g_pVtxBufferShadow->Unlock();              //アンロック
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //ライトをオフにする

	//減算合成設定
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT/*減算合成設定*/);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//
	//頂点フォーマット設定
	///////////////////////////////
	pD3DDevice->SetStreamSource(0, g_pVtxBufferShadow, 0, sizeof(VERTEX_3D));                     //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX3D);
	pD3DDevice->SetTexture(0, g_pTextureShadow);                                       //テクスチャの設定
	
	SetMatrix(&g_Shadow.m_ShadowWorldMatrix, g_Shadow.m_ShadowMatrix);      //ビルボードマトリクスセット
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);                         //描画

	// 描画絵はα値で透明なっていく
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);     //ブレンドモードをアルファブレンドに戻す

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトをオンにする
}

void UninitShadow(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE(g_pTextureShadow);                      //テクスチャインターフェイスの片付け

	SAFE_RELEASE(g_pVtxBufferShadow);                       //頂点バッファインターフェイスの片付け
}

//ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************デバイス設定***********************************************/
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT/*減算合成設定*/);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
