//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "model.h"
#include "camera.h"
#include "TpsCamera.h"
#include "textureManager.h"
#include "matrix.h"
#include "hitCircle.h"
#include "particalManager.h"
#include "CParticalSetting.h"

#define NUM_MATERIAL (3)
#define SPAWN_NUM (4)

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
int g_TextureNUM[NUM_MATERIAL];                       //テクスチャ発行番号
LPD3DXMESH g_pMeshModel;                               //メッシュインターフェースポインタ
LPD3DXBUFFER g_pBuffMatModel;                          //マテリアル情報バッファ(D3DX汎用バッファインターフェース)
DWORD g_nNumMatModel;                                  //マテリアルの数
D3DXMATRIX g_ModelMtxWorld;                                   // ワールドマトリクス
WORLD_MATRIX g_ModelMatrix;                           //ワールドマトリクスの変換行列

//クラス変数
CSpawn g_CSpawn[SPAWN_NUM];

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitModel(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	g_nNumMatModel = NUM_MATERIAL;                               //マテリアルの数

	//モデルワールド変換マトリクスの初期化
	g_ModelMatrix.scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	g_ModelMatrix.rot = D3DXVECTOR3(0.0f, PI, 0.0f);
	g_ModelMatrix.pos = D3DXVECTOR3(0.0f, -1.0f, -3.0f);

	g_CSpawn[0].Init(&D3DXVECTOR3(40.0f, -22.0f, 0.0f), 6.0f,0.25f, 15,&D3DXVECTOR3(0.0f,100,0.0f));
	g_CSpawn[0].SetUse(true);

	g_CSpawn[1].Init(&D3DXVECTOR3(-40.0f, -22.0f, 1.0f), 6.0f, 0.25f, 15, &D3DXVECTOR3(0.0f, 100, 0.0f));
	g_CSpawn[1].SetUse(true);

	g_CSpawn[2].Init(&D3DXVECTOR3(0.0f, -22.0f, 55.0f), 6.0f, 0.25f, 15, &D3DXVECTOR3(0.0f, 100, 0.0f));
	g_CSpawn[2].SetUse(true);

	g_CSpawn[3].Init(&D3DXVECTOR3(0.0f, -22.0f, -55.0f), 6.0f, 0.25f, 15, &D3DXVECTOR3(0.0f, 100, 0.0f));
	g_CSpawn[3].SetUse(true);

	return S_OK;
}

//モデルの更新
void UpdateModel(void)
{
	for (int nCnt = 0; nCnt < SPAWN_NUM; nCnt++)
	{
		if (g_CSpawn[nCnt].GetUse() == true)
		{
			g_CSpawn[nCnt].Update();
		}
	}
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	D3DMATERIAL9 matDef;
	pD3DDevice = Get_Device();
	pD3DDevice->GetMaterial(&matDef);
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();
	SetMatrix(&g_ModelMtxWorld, g_ModelMatrix);    //マトリクスの計算
	TEXTUREMANAGER *pTextureM = GetTextureManager();
	
	pD3DDevice->SetMaterial(&matDef);   //マテリアルを戻す
}

void UninitModel(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE (g_pMeshModel);                      //メッシュインターフェースポインタを解放

	//頂点バッファを解放する
	SAFE_RELEASE (g_pBuffMatModel);                       //マテリアル情報バッファを解放
}

WORLD_MATRIX *Get_ModelMatrix(void) 
{
	return &g_ModelMatrix;
}

CSpawn *GetSpawnPoint(void)
{
	return &g_CSpawn[0];
}