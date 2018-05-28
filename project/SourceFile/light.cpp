/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ライト処理[light.cpp]

Author :
Data   :
----------------------------------------------------------------
■Update : 2016/04/18
ライトの操作
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include "light.h"

/////////////////////////////////グロバール変数/////////////////////////////////
D3DLIGHT9 g_Light;                         //ライト

/////////////////////////////////ライト初期化関数/////////////////////////////////
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = Get_Device();               //デバイス取得

	//ライトの設定
	ZeroMemory(&g_Light, sizeof(D3DLIGHT9));                      //ライト初期化
	g_Light.Type = D3DLIGHT_DIRECTIONAL;                          //ライトのタイプを指定
	g_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);          //ディレクショナルライトの色
	g_Light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);          //アンビエントライト
	D3DXVECTOR3 vecDir(1.0f, -1.0f, 1.0f);                         //ライト方向
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light.Direction, &vecDir); //ライト方向を設定したライトに入れる
}

/////////////////////////////////ライト終了関数/////////////////////////////////
void UninitLight(void) 
{
	
}

/////////////////////////////////ライト更新関数/////////////////////////////////
void UpdateLight(void) 
{
	LPDIRECT3DDEVICE9 pDevice = Get_Device();               //デバイス取得
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトの設定

	pDevice->SetLight(0, &g_Light);                               //デバイスのライトスロット0番に格納
	pDevice->LightEnable(0, TRUE);                                //ライトスロット0番ライト起用
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f) );           // アンビエントライト設定
}

void SetMaterial (LPDIRECT3DDEVICE9 pDevice)
{
	D3DMATERIAL9 mtrl;                               //マテリアル
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse.r = 1.0f;
	mtrl.Diffuse.g = 1.0f;
	mtrl.Diffuse.b = 1.0f;
	mtrl.Diffuse.a = 1.0f;
	mtrl.Ambient = mtrl.Diffuse;
	pDevice->SetMaterial(&mtrl);
}