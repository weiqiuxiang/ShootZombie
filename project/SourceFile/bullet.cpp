//-----------------------------------------------------------------------------------------
//ヘッダファイル
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "bullet.h"
#include "hitCircle.h"
#include "hitCircleMesh.h"
#include "hitcheckMesh.h"
#include "explosion.h"
#include "camera.h"
#include "TpsCamera.h"
#include "sound.h"
#include "matrix.h"
#include "model_aircraft.h"

#define BULLET_WIDTH (0.6f)
#define BULLET_HEIGHT (0.6f)
#define BULLETEFFECT_WIDTH (1.2f)
#define BULLETEFFECT_HEIGHT (1.2f)

#define BORDERLINE (5000.0f)              //境界線の座標

//-----------------------------------------------------------------------------------------
//プロトタイプ
//-----------------------------------------------------------------------------------------
void UpdateBulletBody(void);    //バレット本体更新
void UpdateBulletEffect(void);  //バレットエフェクト更新

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet[2] = {};          //テクスチャを操作するためのポインタ(テクスチャバッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBulletEffect = NULL;    //頂点バッファを操作するためのポインタ(頂点バッファインターフェイスにアクセスためのポインタ)

BULLET g_Bullet[BULLET_MAX];                  //バレット構造体
BULLETEFFECT g_BulletEffect[BULLETEFFECT_MAX];            //バレットエフェクト

////////////////////////////////////////////////
//ポリゴン(頂点)の初期化
////////////////////////////////////////////////
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ
	pD3DDevice = Get_Device();

	//モデルワールド変換マトリクスの初期化

	//頂点バッファの生成
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLET_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferBullet, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLETEFFECT_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferBulletEffect, NULL)))
	{
		return E_FAIL;
	}

	//テクスチャ読み込む
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLET_TEXTURENAME, &g_pTextureBullet[0])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//テクスチャ読み込む
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLETEFFECT_TEXTURENAME, &g_pTextureBullet[1])))
	{
		MessageBox(NULL, "テクスチャの読み込みが失敗しました", "エラー", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//
	//バレット頂点ロック
	//////////////////////
	g_pVtxBufferBullet->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexBulletBG(pD3DDevice, pVtx);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		int k = i * 4;
		pVtx[0 + k].pos = D3DXVECTOR3(-BULLET_WIDTH*0.5, BULLET_HEIGHT*0.5, 0.0f);
		pVtx[1 + k].pos = D3DXVECTOR3(BULLET_WIDTH*0.5, BULLET_HEIGHT*0.5, 0.0f);
		pVtx[2 + k].pos = D3DXVECTOR3(-BULLET_WIDTH*0.5, -BULLET_HEIGHT*0.5, 0.0f);
		pVtx[3 + k].pos = D3DXVECTOR3(BULLET_WIDTH*0.5, -BULLET_HEIGHT*0.5, 0.0f);

		pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1 + k].color = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2 + k].color = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3 + k].color = D3DCOLOR_RGBA(255, 255, 255, 100);

		g_Bullet[i].m_BulletMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].m_BulletMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].m_BulletMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].bUse = false;
	}

	g_pVtxBufferBullet->Unlock();                                     //バッファのアンロック

	//
	//バレットエフェクト頂点ロック
	////////////////////////////////
	g_pVtxBufferBulletEffect->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexBulletBG(pD3DDevice, pVtx);

	for (int i = 0; i < BULLETEFFECT_MAX; i++)
	{
		int k = i * 4;
		pVtx[0 + k].pos = D3DXVECTOR3(-BULLETEFFECT_WIDTH*0.5, BULLETEFFECT_HEIGHT*0.5, 0.0f);
		pVtx[1 + k].pos = D3DXVECTOR3(BULLETEFFECT_WIDTH*0.5, BULLETEFFECT_HEIGHT*0.5, 0.0f);
		pVtx[2 + k].pos = D3DXVECTOR3(-BULLETEFFECT_WIDTH*0.5, -BULLETEFFECT_HEIGHT*0.5, 0.0f);
		pVtx[3 + k].pos = D3DXVECTOR3(BULLETEFFECT_WIDTH*0.5, -BULLETEFFECT_HEIGHT*0.5, 0.0f);

		pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx[0 + k].color = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1 + k].color = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2 + k].color = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3 + k].color = D3DCOLOR_RGBA(255, 0, 0, 255);

		g_BulletEffect[i].m_BulletMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_BulletEffect[i].m_BulletMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BulletEffect[i].m_BulletMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	g_pVtxBufferBulletEffect->Unlock();                                     //バッファのアンロック

	return S_OK;
}

//ポリゴンの更新
void UpdateBullet(void)
{
	UpdateBulletBody();  //バレット本体更新
	UpdateBulletEffect();//エフェクト更新
}

////////////////////////////////////////////////
//ポリゴンの描画
////////////////////////////////////////////////
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //ライトをオフにする

	pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);                               //zバッファを参照
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);                               //z情報残さない

	//加算合成設定
	// アルファ・ブレンディングを行う
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	// 描画絵ポリゴンはαで透明化になっていく
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// 先の絵は透明化しない
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//
	//バレット設定
	///////////////////////////////
	//頂点フォーマット設定
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBullet, 0, sizeof(VERTEX_3D));                     //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX3D);
	pD3DDevice->SetTexture(0, g_pTextureBullet[0]);												 //テクスチャの設定

	
	//バレット描画
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//バレットが使用中
		if (g_Bullet[i].bUse == true)
		{
			SetBillboardMatrix(&g_Bullet[i].m_BulletWorldMatrix, g_Bullet[i].m_BulletMatrix);      //ビルボードマトリクスセット
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, NUM_BULLET);                         //描画
		}
	}
	

	//
	//バレットエフェクト設定
	///////////////////////////////
	//頂点フォーマット設定
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBulletEffect, 0, sizeof(VERTEX_3D));                     //VRAMからGPUにデータをストリーム
	pD3DDevice->SetFVF(FVF_VERTEX3D);
	pD3DDevice->SetTexture(0, g_pTextureBullet[1]);												 //テクスチャの設定

	//バレットエフェクト描画
	for (int i = 0; i < BULLETEFFECT_MAX; i++)
	{
		//バレットが使用中
		if (g_BulletEffect[i].bUse == true)
		{
			SetBillboardMatrix(&g_BulletEffect[i].m_BulletWorldMatrix, g_BulletEffect[i].m_BulletMatrix);      //ビルボードマトリクスセット
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, NUM_BULLET);                         //描画
		}
	}

	//ブレンドモードをアルファブレンドに戻す
	// 先の絵はα値で不透明なっていく
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 描画絵はα値で透明なっていく
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      //z情報残す

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトをオンにする
}

void UninitBullet(void)
{
	//テクスチャインターフェイスを解放する
	SAFE_RELEASE(g_pTextureBullet[0]);                      //テクスチャインターフェイスの片付け
	SAFE_RELEASE(g_pTextureBullet[1]);                      //テクスチャインターフェイスの片付け

	//頂点バッファを解放する
	SAFE_RELEASE(g_pVtxBufferBullet);                       //頂点バッファインターフェイスの片付け
	SAFE_RELEASE(g_pVtxBufferBulletEffect);                 //エフェクト頂点バッファインターフェイスの片付け
}

//ポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexBulletBG(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************デバイス設定***********************************************/
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//弾のセット
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 speed, BULLETTYPE Btype)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].bUse == false)
		{
			g_Bullet[i].speed = speed;

			g_Bullet[i].m_BulletMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bullet[i].m_BulletMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Bullet[i].m_BulletMatrix.pos = pos;
			g_Bullet[i].m_Btype = Btype;
			g_Bullet[i].bUse = true;
			break;
		}
	}
}

//
//バレットエフェクトセット
//////////////////////////////
void SetBulletEffect(D3DXVECTOR3 pos, int nLife, BULLETTYPE Btype)
{
	for (int i = 0; i < BULLETEFFECT_MAX; i++)
	{
		if (g_BulletEffect[i].bUse == false)
		{
			//ローカル初期
			g_BulletEffect[i].m_BulletMatrix.pos = pos;
			//マトリクス初期
			g_BulletEffect[i].m_BulletMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_BulletEffect[i].m_BulletMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//ライフによる変化量計算
			g_BulletEffect[i].nLife = nLife;
			g_BulletEffect[i].nAlpha = 255;
			g_BulletEffect[i].fRadius = BULLETEFFECT_HEIGHT;
			g_BulletEffect[i].fRadiusValue = g_BulletEffect[i].fRadius / g_BulletEffect[i].nLife;
			g_BulletEffect[i].fColorAlphaValue = g_BulletEffect[i].nAlpha / g_BulletEffect[i].nLife;
			g_BulletEffect[i].m_Btype = Btype;
			//フラグ
			g_BulletEffect[i].bUse = true;
			break;
		}
	}
}

//
//バレット更新
////////////////////////
void UpdateBulletBody(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].bUse == true)
		{
			g_Bullet[i].posOld = g_Bullet[i].m_BulletMatrix.pos;

			//速度増加
			g_Bullet[i].m_BulletMatrix.pos.x += g_Bullet[i].speed.x;
			g_Bullet[i].m_BulletMatrix.pos.y += g_Bullet[i].speed.y;
			g_Bullet[i].m_BulletMatrix.pos.z += g_Bullet[i].speed.z;

			//範囲外判定
			if (
				(g_Bullet[i].m_BulletMatrix.pos.x >= BORDERLINE || g_Bullet[i].m_BulletMatrix.pos.x <= -BORDERLINE) ||
				(g_Bullet[i].m_BulletMatrix.pos.z >= BORDERLINE || g_Bullet[i].m_BulletMatrix.pos.z <= -BORDERLINE)
				)
			{
				g_Bullet[i].bUse = false;
			}

			//弾とメッシュの当り判定
			if (VectorAndRect(&g_Bullet[i].posOld, &g_Bullet[i].m_BulletMatrix.pos))
			{
				//爆発効果セット
				SetExplosion(g_Bullet[i].m_BulletMatrix.pos);
				g_Bullet[i].bUse = false;
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//弾と当り判定円の当り判定
			AIRCRAFT *pAircraft = Get_Aircraft();                                      //プレーヤーのデータを取得
			if(BulletCircleAndCircleSSV(&g_Bullet[i].m_BulletMatrix.pos,&g_Bullet[i].posOld,&g_Bullet[i].m_BulletMatrix.pos,0.001f, g_Bullet[i].m_Btype) )
			{
				//爆発効果セット
				SetExplosion(g_Bullet[i].m_BulletMatrix.pos);
				g_Bullet[i].bUse = false;
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//エフェクトセット
			SetBulletEffect(g_Bullet[i].m_BulletMatrix.pos, 20, g_Bullet[i].m_Btype);
		}
	}
}

//
//バレットエフェクト更新
////////////////////////
void UpdateBulletEffect(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //テクスチャ構造体のポインタ

	AIRCRAFT *pAircraft = Get_Aircraft();

	//
	//バレットエフェクト頂点ロック
	////////////////////////////////
	g_pVtxBufferBulletEffect->Lock(0, 0, (void**)&pVtx, 0);                    //仮想アドレスを取得しバッファをロックする

	MakeVertexBulletBG(pD3DDevice, pVtx);  //設定
	for (int i = 0; i < BULLETEFFECT_MAX; i++)
	{
		if (g_BulletEffect[i].bUse == true)
		{
			int k = i * 4;
			pVtx[0 + k].pos = D3DXVECTOR3(-BULLETEFFECT_WIDTH*0.5, g_BulletEffect[i].fRadius*0.5, 0.0f);
			pVtx[1 + k].pos = D3DXVECTOR3(BULLETEFFECT_WIDTH*0.5, g_BulletEffect[i].fRadius*0.5, 0.0f);
			pVtx[2 + k].pos = D3DXVECTOR3(-BULLETEFFECT_WIDTH*0.5, -g_BulletEffect[i].fRadius*0.5, 0.0f);
			pVtx[3 + k].pos = D3DXVECTOR3(BULLETEFFECT_WIDTH*0.5, -g_BulletEffect[i].fRadius*0.5, 0.0f);

			pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

			if (g_BulletEffect[i].m_Btype == BTYPE_HANDGUN)
			{
				pVtx[0 + k].color = D3DCOLOR_RGBA(255, 0, 0, g_BulletEffect[i].nAlpha);
				pVtx[1 + k].color = D3DCOLOR_RGBA(255, 0, 0, g_BulletEffect[i].nAlpha);
				pVtx[2 + k].color = D3DCOLOR_RGBA(255, 0, 0, g_BulletEffect[i].nAlpha);
				pVtx[3 + k].color = D3DCOLOR_RGBA(255, 0, 0, g_BulletEffect[i].nAlpha);
			}

			if (g_BulletEffect[i].m_Btype == BTYPE_AK47)
			{
				pVtx[0 + k].color = D3DCOLOR_RGBA(0, 0, 255, g_BulletEffect[i].nAlpha);
				pVtx[1 + k].color = D3DCOLOR_RGBA(0, 0, 255, g_BulletEffect[i].nAlpha);
				pVtx[2 + k].color = D3DCOLOR_RGBA(0, 0, 255, g_BulletEffect[i].nAlpha);
				pVtx[3 + k].color = D3DCOLOR_RGBA(0, 0, 255, g_BulletEffect[i].nAlpha);
			}

			//変化
			g_BulletEffect[i].fRadius -= g_BulletEffect[i].fRadiusValue;    //半径変化
			g_BulletEffect[i].nAlpha -= g_BulletEffect[i].fColorAlphaValue; //アルファ変化
			g_BulletEffect[i].nLife--;  //ライフ変化

			//効果切れ
			if (g_BulletEffect[i].nLife < 0)
			{
				g_BulletEffect[i].bUse = false;
			}
		}
	}

	g_pVtxBufferBulletEffect->Unlock();              //アンロック
}
