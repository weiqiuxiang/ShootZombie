//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "MeshField.h"
#include "camera.h"
#include "hitRectangle .h"
#include "matrix.h"
#include "textureManager.h"
#include "light.h"

#define SHUKUTAI_POSX (-25.0f)
#define SHUKUTAI_POSY (-2.0f)
#define SHUKUTAI_POSZ (10.0f)
#define SHUKUTAI_POSZ_UE (9.4375f)
#define SHUKUTAI_POSZ_SHITA (5.0f)
#define LENGTH_X (10.0f)    //長方形X方向の長さ
#define LENGTH_Y (SHUKUTAI_POSZ_UE-SHUKUTAI_POSZ_SHITA)    //長方形Y方向の長さ

//最大の限定値
#define YOKOMASU_MAX (200)  //横マス最大数
#define TATEMASU_MAX (200)  //縦マス最大数
#define INDEX_NUM_MAX ((2+2*YOKOMASU_MAX)*TATEMASU_MAX + (TATEMASU_MAX-1)*2) //インデックスの最大数
#define VERTEX_NUM_MAX ((YOKOMASU_MAX+1)*(TATEMASU_MAX+1))               //最大頂点数
#define MeshField_NUM_MAX (INDEX_NUM_MAX-2)                           //最大ポリゴン数

#define NUMWALL_MAX (128)

#define HIGHWAY_TEX "data/GAMEOBJ/TEX/highway.jpg"
#define FLOOR01_TEX "data/TEXTURE/floor01.png"
#define WALL01_TEX "data/TEXTURE/wallTex02.jpg"

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
MESHWALL g_MeshWall[NUMWALL_MAX];

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		//MeshFieldワールド変換マトリクスの初期化
		g_MeshWall[nCnt].m_MeshFieldMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_MeshWall[nCnt].m_MeshFieldMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].m_MeshFieldMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].bUse = false;
		g_MeshWall[nCnt].m_pIdxBufferFieldMeshField = NULL;
		g_MeshWall[nCnt].m_pVtxBufferMeshField = NULL;
		g_MeshWall[nCnt].m_TexNum = -1;

		//頂点バッファの生成
		if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*VERTEX_NUM_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_MeshWall[nCnt].m_pVtxBufferMeshField, NULL)))
		{
			return E_FAIL;

		}

		//インデックスバッファの生成
		HRESULT hr;
		hr = pD3DDevice->CreateIndexBuffer(
			sizeof(WORD)*INDEX_NUM_MAX,     //バッファ量(2バイト)
			D3DUSAGE_WRITEONLY,        //使用方法フラグ
			D3DFMT_INDEX16,              //インデックスフォーマット(重複しない頂点数は65536個は限界)
			D3DPOOL_MANAGED,           //メモリ管理方法
			&g_MeshWall[nCnt].m_pIdxBufferFieldMeshField,        //インデックスバッファインターフェースへのポインタのアドレス
			NULL);
		//失敗チェック
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}


	//真ん中高速道路
	InitMeshWall_XZ(
		D3DXVECTOR3(0.0f, -LENGTH_X * 3, 0.0f),      //ローカル座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //ローカル座標
		1,                                             //横マス数
		20,                                             //縦マス数
		LENGTH_X*20,                                             //マスごとのwidth
		LENGTH_X*6,                                             //マスごとのheight
		MESH_FLOOR,
		HIGHWAY_TEX
	);

	//両側地面
	InitMeshWall_XZ(
		D3DXVECTOR3(-LENGTH_X*17.5, -LENGTH_X * 3, 0.0f),      //ローカル座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //ローカル座標
		15,                                             //横マス数
		120,                                             //縦マス数
		LENGTH_X,                                             //マスごとのwidth
		LENGTH_X,                                             //マスごとのheight
		MESH_FLOOR,
		FLOOR01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(LENGTH_X * 17.5, -LENGTH_X * 3, 0.0f),      //ローカル座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //ローカル座標
		15,                                             //横マス数
		120,                                             //縦マス数
		LENGTH_X,                                             //マスごとのwidth
		LENGTH_X,                                             //マスごとのheight
		MESH_FLOOR,
		FLOOR01_TEX
	);

	//真ん中高速道路
	InitMeshWall_XZ(
		D3DXVECTOR3(0.0f, -LENGTH_X * 3, LENGTH_X*140),      //ローカル座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //ローカル座標
		1,                                             //横マス数
		20,                                             //縦マス数
		LENGTH_X * 20,                                             //マスごとのwidth
		LENGTH_X * 6,                                             //マスごとのheight
		MESH_FLOOR,
		HIGHWAY_TEX
	);

	//両側地面
	InitMeshWall_XZ(
		D3DXVECTOR3(-LENGTH_X*17.5, -LENGTH_X * 3, LENGTH_X* 140),      //ローカル座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //ローカル座標
		15,                                             //横マス数
		120,                                             //縦マス数
		LENGTH_X,                                             //マスごとのwidth
		LENGTH_X,                                             //マスごとのheight
		MESH_FLOOR,
		FLOOR01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(LENGTH_X * 17.5, -LENGTH_X * 3, LENGTH_X* 140),      //ローカル座標
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //ローカル座標
		15,                                             //横マス数
		120,                                             //縦マス数
		LENGTH_X,                                             //マスごとのwidth
		LENGTH_X,                                             //マスごとのheight
		MESH_FLOOR,
		FLOOR01_TEX
	);

	//回り壁
	InitMeshWall_XZ(
		D3DXVECTOR3(LENGTH_X * 24, -LENGTH_X * 3, LENGTH_X * 100),      //ローカル座標
		D3DXVECTOR3(0.5*PI, -0.5*PI, 0.0f),      //ローカル座標
		60,                                             //横マス数
		30,                                             //縦マス数
		LENGTH_X*6,                                             //マスごとのwidth
		LENGTH_X*2,                                             //マスごとのheight
		MESH_WALL,
		WALL01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(-LENGTH_X * 24, -LENGTH_X * 3, LENGTH_X * 100),      //ローカル座標
		D3DXVECTOR3(0.5*PI, 0.5*PI, 0.0f),      //ローカル座標
		60,                                             //横マス数
		30,                                             //縦マス数
		LENGTH_X * 6,                                             //マスごとのwidth
		LENGTH_X * 2,                                             //マスごとのheight
		MESH_WALL,
		WALL01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(0.0f, -LENGTH_X * 3, -200.0f),      //ローカル座標
		D3DXVECTOR3(0.5*PI, 0.0f, 0.0f),      //ローカル座標
		10,                                             //横マス数
		30,                                             //縦マス数
		LENGTH_X * 6,                                             //マスごとのwidth
		LENGTH_X * 2,                                             //マスごとのheight
		MESH_WALL,
		WALL01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(0.0f, -LENGTH_X * 3, 1400.0f),      //ローカル座標
		D3DXVECTOR3(0.5*PI, PI, 0.0f),      //ローカル座標
		10,                                             //横マス数
		30,                                             //縦マス数
		LENGTH_X * 6,                                             //マスごとのwidth
		LENGTH_X * 2,                                             //マスごとのheight
		MESH_WALL,
		WALL01_TEX
	);

	//判定ポリゴンセット
	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == true)
		{
			SetHitRect(&g_MeshWall[nCnt].m_MeshFieldMatrix, &g_MeshWall[nCnt].HW, g_MeshWall[nCnt].meshType);
		}
	}

	return S_OK;
}

//ポリゴンの更新
void UpdateMeshField(void)
{
	//回転
	//移動
	
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	TEXTUREMANAGER *pTexManager = GetTextureManager();                                  //テクスチャ管理
	SetMaterial(pD3DDevice);                                  //マテリアル設定

	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		//使用状態なら
		if (g_MeshWall[nCnt].bUse == true)
		{
			SetMatrix(&g_MeshWall[nCnt].m_MeshFieldMtxWorld, g_MeshWall[nCnt].m_MeshFieldMatrix);      //マトリクス変換セット

			//コベニちゃん:頂点フォーマット設定
			pD3DDevice->SetStreamSource(0, g_MeshWall[nCnt].m_pVtxBufferMeshField, 0, sizeof(VERTEX_3D));                     //VRAMからGPUにデータをストリーム
			pD3DDevice->SetIndices(g_MeshWall[nCnt].m_pIdxBufferFieldMeshField);                                                    //インデックスバッファのセット
			pD3DDevice->SetFVF(FVF_VERTEX3D);
			pD3DDevice->SetTexture(0, (pTexManager + g_MeshWall[nCnt].m_TexNum)->TextureSlot);                              //テクスチャの設定
			pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshWall[nCnt].m_nVertexNum, 0, g_MeshWall[nCnt].m_nPolygonNum);              //描画
		}
	}

}

////////////////////////////////////////////////
//ポリゴンの終了
////////////////////////////////////////////////
void UninitMeshField(void)
{
	//頂点バッファを解放する
	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		SAFE_RELEASE(g_MeshWall[nCnt].m_pVtxBufferMeshField);                       //頂点バッファインターフェイスの片付け

		SAFE_RELEASE(g_MeshWall[nCnt].m_pIdxBufferFieldMeshField);                  //インデックスバッファを解放
	}
}

//ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexMeshFieldBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_3D *pVtx)
{
	/*******************************************デバイス設定***********************************************/
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//メッシュウォール(XZ平面)
void InitMeshWall_XZ(D3DXVECTOR3 pos, D3DXVECTOR3 rot, UINT nNumBlock_Honrizon, UINT nNumBlock_Vertical, float fSizeBlock_Honrizon, float fSizeBlock_Vertical, MESHTYPE meshType, char *TexPass)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;									   //テクスチャ構造体のポインタ
	WORD *pIdx = NULL;
	int i, j;                                                  //カウント用変数

	//セット
	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == false)
		{
			g_MeshWall[nCnt].bUse = true;
			g_MeshWall[nCnt].meshType = meshType;
			g_MeshWall[nCnt].HW.x = nNumBlock_Honrizon*fSizeBlock_Honrizon;
			g_MeshWall[nCnt].HW.y = nNumBlock_Vertical*fSizeBlock_Vertical;
			g_MeshWall[nCnt].m_nVertexNum = (nNumBlock_Honrizon + 1)*(nNumBlock_Vertical + 1);         //頂点数
			g_MeshWall[nCnt].m_nIndexNum = (2 + 2 * nNumBlock_Honrizon)*nNumBlock_Vertical + (nNumBlock_Vertical - 1) * 2; //インデックス数
			g_MeshWall[nCnt].m_nPolygonNum = g_MeshWall[nCnt].m_nIndexNum - 2;       //ポリゴン数
			g_MeshWall[nCnt].m_MeshFieldMatrix.pos = pos;    //pos
			g_MeshWall[nCnt].m_MeshFieldMatrix.rot = rot;    //rot
			g_MeshWall[nCnt].fSlopeAngle = rot.x;

			SetTexture(&g_MeshWall[nCnt].m_TexNum,TexPass);                                     //テクスチャ読み込み

			/***************************仮想アドレスを取得し頂点バッファをロックする************************************/
			g_MeshWall[nCnt].m_pVtxBufferMeshField->Lock(0, 0, (void**)&pVtx, 0);
			//頂点登録
			for (i = 0; i < nNumBlock_Vertical + 1; i++)
			{
				for (j = 0; j < nNumBlock_Honrizon + 1; j++)
				{
					int k = i * (nNumBlock_Honrizon + 1) + j;
					pVtx[0 + k].pos = D3DXVECTOR3(j*fSizeBlock_Honrizon- g_MeshWall[nCnt].HW.x*0.5, 0.0f, -i*fSizeBlock_Vertical+ g_MeshWall[nCnt].HW.y*0.5);
					pVtx[0 + k].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[0 + k].tex = D3DXVECTOR2(j*1.0f, i*1.0f);
					pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
			}

			g_MeshWall[nCnt].m_pVtxBufferMeshField->Unlock();
			/***************************頂点バッファのアンロック************************************/

			/***************************仮想アドレスを取得しインデックスバッファをロックする************************************/
			g_MeshWall[nCnt].m_pIdxBufferFieldMeshField->Lock(0, 0, (void**)&pIdx, 0);

			//インデックスバッファ登録
			//縮退
			for (i = 0; i < nNumBlock_Vertical; i++)
			{
				//普通部分
				for (j = 0; j < nNumBlock_Honrizon + 1; j++)
				{
					int k = j * 2 + i*(nNumBlock_Honrizon + 2) * 2;
					pIdx[0 + k] = (i + 1)*(nNumBlock_Honrizon + 1) + j;
					pIdx[1 + k] = pIdx[0 + k] - (nNumBlock_Honrizon + 1);
				}

				int q = (2 + 2 * nNumBlock_Honrizon)*(i + 1) + ((i + 1) - 1) * 2;

				//縮退部分
				if (q < g_MeshWall[nCnt].m_nIndexNum)
				{
					pIdx[q] = pIdx[q - 1];         //縮退1の頂点番号
					pIdx[q + 1] = pIdx[q] + (nNumBlock_Honrizon + 1) + 1;     //縮退2の頂点番号
				}
			}

			g_MeshWall[nCnt].m_pIdxBufferFieldMeshField->Unlock();
			/***************************インデックスバッファのアンロック************************************/

			break;
		}
	}
}

//メッシュを取得
MESHWALL *GetMesh(void)
{
	return &g_MeshWall[0];
}