//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "skydome.h"
#include "camera.h"
#include "matrix.h"

#define SKYDOME_POSX (0.0f)
#define SKYDOME_POSY (0.0f)
#define SKYDOME_POSZ (0.0f)

#define START_ANGEL_VERTICAL (0.0f)            //垂直方向の開始角度
#define END_ANGEL_VERTICAL (PI)              //垂直方向の終了角度

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureSkydome[2] = {};							//テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferSkydome = NULL;						//頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DINDEXBUFFER9 g_pIdxBufferFieldSkydome = NULL;					//インデックスバッファを操作するためのポインタ
int g_nSkydomeVertexNum, g_nSkydomeIndexNum,g_nSkydomePolygonNum;       //頂点数,インデックス数,ポリゴン数
SKYDOME g_Skydome;       //スカイドーム

D3DXMATRIX g_SkydomeMtxWorld;                                   // ワールドマトリクス
WORLD_MATRIX g_SkydomeMatrix;                           //ワールドマトリクスの変換行列

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitSkydome(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;
	pD3DDevice = Get_Device();

	g_SkydomeMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_SkydomeMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SkydomeMatrix.pos = D3DXVECTOR3(0.0f, -22.0f, 0.0f);

	g_Skydome.fRadius = 4000.0f;                                         //スカイドーム半径
	
	g_Skydome.nHorizon_PieceNum = 100;                                   //水平分割数
	g_Skydome.nVertical_PieceNum = 100;                                  //垂直分割数
	
	g_Skydome.fStart_Angel_Vertical = START_ANGEL_VERTICAL;              //垂直回転角度の始点指定
	g_Skydome.fEnd_Angel_Vertical = END_ANGEL_VERTICAL;                  //垂直回転角度の終点指定
	//単位回転角度計算
	g_Skydome.fHorizon_PerAngel = 2*PI / g_Skydome.nHorizon_PieceNum;
	g_Skydome.fVertical_PerAngel = (g_Skydome.fEnd_Angel_Vertical- g_Skydome.fStart_Angel_Vertical) / g_Skydome.nVertical_PieceNum;

	//頂点数,インデックス数,ポリゴン数計算
	g_nSkydomeVertexNum = (g_Skydome.nHorizon_PieceNum+1)*(g_Skydome.nVertical_PieceNum + 1);
	g_nSkydomeIndexNum = (2+2* g_Skydome.nHorizon_PieceNum)*g_Skydome.nVertical_PieceNum + (g_Skydome.nVertical_PieceNum - 1)*2;
	g_nSkydomePolygonNum = g_nSkydomeIndexNum - 2;

	//頂点バッファの生成
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_3D)*g_nSkydomeVertexNum,D3DUSAGE_WRITEONLY,FVF_VERTEX3D,D3DPOOL_MANAGED,&g_pVtxBufferSkydome,NULL)))
	{
		return E_FAIL;
	}

	//インデックスバッファの生成
	HRESULT hr;
	hr = pD3DDevice->CreateIndexBuffer(
		sizeof(WORD)*g_nSkydomeIndexNum,     //バッファ量(2バイト)
		D3DUSAGE_WRITEONLY,        //使用方法フラグ
		D3DFMT_INDEX16,              //インデックスフォーマット(重複しない頂点数は65536個は限界)
		D3DPOOL_MANAGED,           //メモリ管理方法
		&g_pIdxBufferFieldSkydome,        //インデックスバッファインターフェースへのポインタのアドレス
		NULL);
	
	if(FAILED(hr))
	{
		return E_FAIL;
	}

	//テクスチャ読み込む
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, SKYDOOM_TEXTURENAME,&g_pTextureSkydome[0]) ) )
	{
		MessageBox(NULL,"テクスチャの読み込みが失敗しました","エラー",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}
	
	//メッシュウォール設定
	InitMeshSkydome_Y();

	return S_OK;
}

//ポリゴンの更新
void UpdateSkydome(void)
{
	//回転
	g_SkydomeMatrix.rot.y += 0.001f;
	//移動
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawSkydome(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	SetMatrix(&g_SkydomeMtxWorld, g_SkydomeMatrix);      //マトリクス変換セット
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //ライトをオフにする

	//コベニちゃん:頂点フォーマット設定
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferSkydome,0,sizeof(VERTEX_3D));                     //VRAMからGPUにデータをストリーム
	pD3DDevice->SetIndices(g_pIdxBufferFieldSkydome);                                                    //インデックスバッファのセット
	pD3DDevice -> SetFVF(FVF_VERTEX3D);
	pD3DDevice -> SetTexture(0, g_pTextureSkydome[0]);												 //テクスチャの設定
	pD3DDevice -> DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0, g_nSkydomeVertexNum,0, g_nSkydomePolygonNum);              //描画

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトをオンにする
}

////////////////////////////////////////////////
//ポリゴンの終了
////////////////////////////////////////////////
void UninitSkydome(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE (g_pTextureSkydome[0]);                      //テクスチャインターフェイスの片付け
	SAFE_RELEASE (g_pTextureSkydome[1]);                      //テクスチャインターフェイスの片付け

	//頂点バッファを解放する
	SAFE_RELEASE (g_pVtxBufferSkydome);                       //頂点バッファインターフェイスの片付け

	//インデックスバッファを解放
	SAFE_RELEASE(g_pIdxBufferFieldSkydome);
}

//ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexSkydomeBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_3D *pVtx)
{
	/*******************************************デバイス設定***********************************************/
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//スカイドーム(Y軸回転)
void InitMeshSkydome_Y(void)
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
	g_pVtxBufferSkydome->Lock(0, 0, (void**)&pVtx, 0);
	//頂点登録
	for (i = 0; i < (g_Skydome.nVertical_PieceNum + 1); i++)
	{
		for (j = 0; j < (g_Skydome.nHorizon_PieceNum + 1) ; j++)
		{
			int k = i * (g_Skydome.nHorizon_PieceNum + 1) + j;
			//posに関する計算
			fHorizon_Angel = -g_Skydome.fHorizon_PerAngel*j;                                       //水平角度
			fVertical_Angel = g_Skydome.fStart_Angel_Vertical + g_Skydome.fVertical_PerAngel * i;   //垂直角度
			posX = g_Skydome.fRadius*sin(fVertical_Angel)*cos(fHorizon_Angel);            //頂点のX座標計算
			posY = g_Skydome.fRadius*cos(fVertical_Angel);            //頂点のY座標計算
			posZ = g_Skydome.fRadius*sin(fVertical_Angel)*sin(fHorizon_Angel);            //頂点のZ座標計算

			pVtx[0 + k].pos = D3DXVECTOR3(posX, posY, posZ);
			pVtx[0 + k].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			pVtx[0 + k].tex = D3DXVECTOR2(j*(1.0f/ g_Skydome.nHorizon_PieceNum), i*(1.0f/ g_Skydome.nVertical_PieceNum));
			pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
	}
	
	g_pVtxBufferSkydome->Unlock();
	/***************************頂点バッファのアンロック************************************/

	int q;
	/***************************仮想アドレスを取得しインデックスバッファをロックする************************************/
	g_pIdxBufferFieldSkydome->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファ登録
	//縮退
	for (i = 0; i < g_Skydome.nVertical_PieceNum; i++)
	{
		//普通部分
		for (j = 0; j < g_Skydome.nHorizon_PieceNum + 1; j++)
		{
			int k = j * 2 + i*(g_Skydome.nHorizon_PieceNum + 2) * 2;
			
			pIdx[0 + k] = (i + 1)*(g_Skydome.nHorizon_PieceNum + 1) + j;
			pIdx[1 + k] = pIdx[0 + k] - (g_Skydome.nHorizon_PieceNum + 1);
		}

		 q = (2 + 2 * g_Skydome.nHorizon_PieceNum)*(i + 1) + ((i + 1) - 1) * 2;

		//縮退部分
		if (q < g_nSkydomeIndexNum)
		{
			pIdx[q] = pIdx[q - 1];         //縮退1の頂点番号
			pIdx[q + 1] = pIdx[q] + (g_Skydome.nHorizon_PieceNum + 1) + 1;     //縮退2の頂点番号
		}
	}

	g_pIdxBufferFieldSkydome->Unlock();
	/***************************インデックスバッファのアンロック************************************/
}
