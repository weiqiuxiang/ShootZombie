//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "TpsCamera.h"
#include "model_aircraft.h"
#include "motion.h"

#define TESTPOLYGON_POSX (100.0f)
#define TESTPOLYGON_POSY (100.0f)
#define TESTPOLYGON_WIDTH (100.0f)
#define TESTPOLYGON_HEIGHT (100.0f)

//-----------------------------------------------------------------------------------------
//プロトタイプ
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePolygon[2] = {};          //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPolygon = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
D3DXMATRIX g_PolygonWorldMatrix;                        //ポリゴンのワールドマトリクス
WORLD_MATRIX g_PolygonMatrix;                           //ポリゴンのマトリクスの変換行列

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	//モデルワールド変換マトリクスの初期化
	g_PolygonMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_PolygonMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_PolygonMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferPolygon,NULL)))
	{
		return E_FAIL;
	}

	//テクスチャ読み込む
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice,POLYGON_TEXTURENAME,&g_pTexturePolygon[0]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//照準座標
	g_pVtxBufferPolygon -> Lock(0,0,(void**)&pVtx,0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexPolygon(pD3DDevice,pVtx);

	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 - TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 - TESTPOLYGON_HEIGHT*0.5,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 + TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 - TESTPOLYGON_HEIGHT*0.5,0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 - TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 + TESTPOLYGON_HEIGHT*0.5,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 + TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 + TESTPOLYGON_HEIGHT*0.5,0.0f);

	g_pVtxBufferPolygon -> Unlock();                                     //バッファのアンロック

	return S_OK;
}

//ポリゴンの更新
void UpdatePolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;									   //テクスチャ構造体のポインタ

	//MakeVertexPolygonBG(pD3DDevice,pVtx);                      //ポリゴンの設定

	/***************************仮想アドレスを取得しバッファをロックする************************************/
	g_pVtxBufferPolygon -> Lock(0,0,(void**)&pVtx,0);

	//ローカル座標
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 - TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 - TESTPOLYGON_HEIGHT*0.5, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 + TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 - TESTPOLYGON_HEIGHT*0.5, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 - TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 + TESTPOLYGON_HEIGHT*0.5, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 + TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 + TESTPOLYGON_HEIGHT*0.5, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	g_pVtxBufferPolygon -> Unlock();                             
	/***************************バッファのアンロック************************************/
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawPolygon(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();

	if ( (pAircraft -> m_MotionNow == CModel::MOTION_SHOOT) || (pAircraft->m_MotionNow == CModel::MOTION_GUNMOVE) )
	{
		LPDIRECT3DDEVICE9 pD3DDevice;
		pD3DDevice = Get_Device();

		pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //ライトをオフにする

		//コベニちゃん:頂点フォーマット設定
		pD3DDevice->SetStreamSource(0, g_pVtxBufferPolygon, 0, sizeof(VERTEX_2D));                     //VRAMからGPUにデータをストリーム
		pD3DDevice->SetFVF(FVF_VERTEX2D);
		pD3DDevice->SetTexture(0, g_pTexturePolygon[0]);												 //テクスチャの設定
		pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);                                  //テクスチャメモリの確保

		pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトをオンにする
	}
}

void UninitPolygon(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE (g_pTexturePolygon[0]);                      //テクスチャインターフェイスの片付け
	SAFE_RELEASE (g_pTexturePolygon[1]);                      //テクスチャインターフェイスの片付け

	//頂点バッファを解放する
	SAFE_RELEASE (g_pVtxBufferPolygon);                       //頂点バッファインターフェイスの片付け
}

//ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
{
	/*******************************************デバイス設定***********************************************/
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}