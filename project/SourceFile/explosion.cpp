//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include "bullet.h"
#include "explosion.h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//マクロ
//-----------------------------------------------------------------------------------------
#define EXPLOSION_RADIUS (1.5f)
#define FRAME_MAX (21)
#define TEX_WIDTH (0.333333333)
#define TEX_HEIGHT (0.142857143)

//-----------------------------------------------------------------------------------------
//プロトタイプ
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureExplosion = {};          //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferExplosion = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
EXPLOSION_EF g_Explosion[BULLET_MAX];    //爆発エフェクト

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	//モデルワールド変換マトリクスの初期化

	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BULLET_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferExplosion, NULL)))
	{
		return E_FAIL;
	}

	//テクスチャ読み込む
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, EXPLOSION_TEXTURENAME, &g_pTextureExplosion)))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//
	//バレット頂点ロック
	//////////////////////
	g_pVtxBufferExplosion->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexExplosion(pD3DDevice, pVtx);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		int k = i * 4;
		pVtx[0 + k].pos = D3DXVECTOR3(-EXPLOSION_RADIUS*0.5, EXPLOSION_RADIUS*0.5, 0.0f);
		pVtx[1 + k].pos = D3DXVECTOR3(EXPLOSION_RADIUS*0.5, EXPLOSION_RADIUS*0.5, 0.0f);
		pVtx[2 + k].pos = D3DXVECTOR3(-EXPLOSION_RADIUS*0.5, -EXPLOSION_RADIUS*0.5, 0.0f);
		pVtx[3 + k].pos = D3DXVECTOR3(EXPLOSION_RADIUS*0.5, -EXPLOSION_RADIUS*0.5, 0.0f);

		pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 20);
		pVtx[1 + k].color = D3DCOLOR_RGBA(255, 255, 255, 20);
		pVtx[2 + k].color = D3DCOLOR_RGBA(255, 255, 255, 20);
		pVtx[3 + k].color = D3DCOLOR_RGBA(255, 255, 255, 20);

		//パラメータ初期化
		g_Explosion[i].m_ExplosionMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Explosion[i].m_ExplosionMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[i].m_ExplosionMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[i].bUse = false;
		g_Explosion[i].nFrame = 0;
	}

	g_pVtxBufferExplosion->Unlock();                                     //バッファのアンロック

	return S_OK;
}

//
//ポリゴンの更新
/////////////////////////////
void UpdateExplosion(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ

	int k = 0;       //計算用
	float fTexPosU, fTexPosV;//UV位置

	//
	//バレットエフェクト頂点ロック
	////////////////////////////////
	g_pVtxBufferExplosion->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする
	MakeVertexExplosion(pD3DDevice, pVtx);  //設定
	//頂点設定
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Explosion[i].bUse == true)
		{
			k = i * 4;
			fTexPosU = (g_Explosion[i].nFrame % 3)*TEX_WIDTH;
			fTexPosV = (g_Explosion[i].nFrame / 3)*TEX_HEIGHT;
			pVtx[0 + k].pos = D3DXVECTOR3(-EXPLOSION_RADIUS*0.5, EXPLOSION_RADIUS*0.5, 0.0f);
			pVtx[1 + k].pos = D3DXVECTOR3(EXPLOSION_RADIUS*0.5, EXPLOSION_RADIUS*0.5, 0.0f);
			pVtx[2 + k].pos = D3DXVECTOR3(-EXPLOSION_RADIUS*0.5, -EXPLOSION_RADIUS*0.5, 0.0f);
			pVtx[3 + k].pos = D3DXVECTOR3(EXPLOSION_RADIUS*0.5, -EXPLOSION_RADIUS*0.5, 0.0f);

			pVtx[0 + k].tex = D3DXVECTOR2(fTexPosU, fTexPosV);
			pVtx[1 + k].tex = D3DXVECTOR2(fTexPosU + TEX_WIDTH, fTexPosV);
			pVtx[2 + k].tex = D3DXVECTOR2(fTexPosU, fTexPosV + TEX_HEIGHT);
			pVtx[3 + k].tex = D3DXVECTOR2(fTexPosU + TEX_WIDTH, fTexPosV + TEX_HEIGHT);

			pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);

			//変化
			g_Explosion[i].nFrame++;     //フレーム数増加

			//効果切れ
			if (g_Explosion[i].nFrame >= FRAME_MAX)
			{
				g_Explosion[i].bUse = false;
			}
		}
	}

	g_pVtxBufferExplosion->Unlock();              //アンロック
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //ライトをオフにする

	//
	//頂点フォーマット設定
	///////////////////////////////
	pD3DDevice->SetStreamSource(0, g_pVtxBufferExplosion, 0, sizeof(VERTEX_3D));                     //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX3D);
	pD3DDevice->SetTexture(0, g_pTextureExplosion);                                       //テクスチャの設定

	//描画
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//バレットが使用中
		if (g_Explosion[i].bUse == true)
		{
			SetBillboardMatrix(&g_Explosion[i].m_ExplosionWorldMatrix, g_Explosion[i].m_ExplosionMatrix);      //ビルボードマトリクスセット
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);                         //描画
		}
	}

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトをオンにする
}

void UninitExplosion(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE(g_pTextureExplosion);                      //テクスチャインターフェイスの片付け

	SAFE_RELEASE(g_pVtxBufferExplosion);                       //頂点バッファインターフェイスの片付け
}

//ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************デバイス設定***********************************************/
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//
//爆発エフェクトセット
//////////////////////////////
void SetExplosion(D3DXVECTOR3 pos)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Explosion[i].bUse == false)
		{
			//ローカル初期
			g_Explosion[i].m_ExplosionMatrix.pos = pos;
			//マトリクス初期
			g_Explosion[i].m_ExplosionMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Explosion[i].m_ExplosionMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//ライフによる変化量計算
			g_Explosion[i].nFrame = 0;
			//フラグ
			g_Explosion[i].bUse = true;
			break;
		}
	}
}
