//-----------------------------------------------------------------------------------------
//パーティクル,道具などの処理[ParticalCenter.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "partical.h"
#include "particalCenter.h"
#include "input.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define FRAMEPARTICAL (4)                //1フレーム発射するパーティクルの数

//-----------------------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------------------
PARTICALCENTER g_ParticalCenter[NUM_PARTICALCENTERPOLYGON];
int g_ParticalFunction = 0;                //パーティクルメニュー階層コントロール変数

//-----------------------------------------------------------------------------------------
//プロトタイプ宣言
//-----------------------------------------------------------------------------------------
//パーティクル効果
void ParticalBase(PARTICALCENTER *);                         //パーティクル基本
void ParticalBaseControl(PARTICALCENTER *pParticalCenter);   //パーティクル基本パラメータのコントロール
void ParticalExplosion(PARTICALCENTER *);                    //パーティクル爆発エフェクト
void ParticalFire(PARTICALCENTER *pParticalCenter);          //パーティクル炎

//-----------------------------------------------------------------------------------------
//重要定番関数達
//-----------------------------------------------------------------------------------------

//各変数の初期化
HRESULT InitParticalCenter(void)
{
	srand((unsigned)time(NULL));        //乱数の種を生成する

	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		//パーティクルの初期位置
		g_ParticalCenter[nCnt].pos.x = 0;
		g_ParticalCenter[nCnt].pos.y = 0;
		//パーティクルの初期スピード
		g_ParticalCenter[nCnt].speed.x = 10;
		g_ParticalCenter[nCnt].speed.y = 9;
		//パーティクルの古い位置
		g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
		g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;
		//パーティクルの集中発射角度と拡散角度
		g_ParticalCenter[nCnt].fShootAngel = PI *0.5;
		g_ParticalCenter[nCnt].fRangeAngel = PI * 0.01;
		//パーティクルの飛び基準距離と半径
		g_ParticalCenter[nCnt].fDistance = 200;
		g_ParticalCenter[nCnt].fRadius = 0;
		//プレーヤー使用
		g_ParticalCenter[nCnt].bUse = false;

		//パーティクル周囲環境
		g_ParticalCenter[nCnt].fGravity = 0;                 //重力
		g_ParticalCenter[nCnt].fWindPower = 0;               //風力
		g_ParticalCenter[nCnt].fWindAngel = 0;               //風が吹く角度
	}

	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //頂点構造体のポインタ
	pD3DDevice = Get_Device();

	

	return S_OK;
}
//終了処理
void UninitParticalCenter(void)
{
	
}

//パーティクルデータの更新処理
void UpdateParticalCenter(void)
{
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON;nCnt++)
	{
		//もしパーティクル中心は使用可の状態なら
		if (g_ParticalCenter[nCnt].bUse == true)
		{
			//パーティクルの前の座標を記録
			g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
			g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;

			//-----------------------------------------------------------------------------------------
			//パーティクルの移動処理
			//-----------------------------------------------------------------------------------------
			//プレーヤーの位置を受け取る

			//-----------------------------------------------------------------------------------------
			//パーティクルのエフェクトリスト
			//-----------------------------------------------------------------------------------------
			switch(g_ParticalCenter[nCnt].EffectMode)
			{
			case EFFECTMODE_BASE:
				//パーティクル:基本
				ParticalBase(&g_ParticalCenter[nCnt]);
				break;
			case EFFECTMODE_EXPLOSION:
				//パーティクル:爆発
				ParticalExplosion(&g_ParticalCenter[nCnt]);
				break;
			case EFFECTMODE_FIRE:
				ParticalFire(&g_ParticalCenter[nCnt]);
				break;
			default:
				//パーティクル:基本
				ParticalBase(&g_ParticalCenter[nCnt]);
				break;
			}

			//-----------------------------------------------------------------------------------------
			//パーティクル
			//-----------------------------------------------------------------------------------------
			//移動範囲制限
			if ((g_ParticalCenter[nCnt].pos.x < 0) || ( (g_ParticalCenter[nCnt].pos.x + PARTICALCENTER_WIDTH) > SCREEN_WIDTH))
			{
				g_ParticalCenter[nCnt].pos.x = g_ParticalCenter[nCnt].posOld.x;
			}

			if ((g_ParticalCenter[nCnt].pos.y < 0) || ((g_ParticalCenter[nCnt].pos.y + PARTICALCENTER_HEIGHT) > SCREEN_HEIGHT))
			{
				g_ParticalCenter[nCnt].pos.y = g_ParticalCenter[nCnt].posOld.y;
			}
		}
	}
}


//パーティクルポリゴンの表示位置X,ポリゴンの表示位置Y,ポリゴンの幅,ポリゴンの高さ
void MakeVertexPolygonParticalCenter(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //引数1:設定名カリング 引数2:設定値(D3DCULL_CCW:逆時計回りを裏面をする)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//ポリゴン描画する前にテクスチャをセット
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

PARTICALCENTER *GetParticalCenter(void)
{
	return &g_ParticalCenter[0];
}

//-----------------------
//パーティクル:基本
//-----------------------
void ParticalBase(PARTICALCENTER *pParticalCenter)
{
		//発射角度計算
		float fHantenAngle = 2 * PI * ( (pParticalCenter->fShootAngel / (2*PI) ) - (int)(pParticalCenter->fShootAngel / (2*PI) ) );

		ParticalBaseControl(pParticalCenter);      //パーティクルパラメータのコントロール

		//-----------------------------------------------------------------------------------------
		//パーティクルの発射
		//-----------------------------------------------------------------------------------------
		float fDistance = 0;                //パーティクルの飛び距離
		float fDistanceZ = 0;               //パーティクルZ軸の飛び距離
		float fAngelXY = 0;                   //角度
		D3DXVECTOR3 speed;                  //パーティクルのスピード

		fDistance = pParticalCenter->fDistance + rand()%50 + 50;       //飛び距離計算
		fDistance = fDistance*0.001;//(0.05-0.1)
		fDistanceZ = 0.001*0.5* (pParticalCenter->fDistance + rand() % 50 + 50);//(0.025-0.05)
		fAngelXY = pParticalCenter->fShootAngel + ( rand()%(int)(pParticalCenter->fRangeAngel*2*100) - (int)(pParticalCenter->fRangeAngel*100) ) * 0.01; //飛び角度計算
		speed.x = fDistance*cos(fAngelXY);        //x方向のスピード
		speed.y = fDistance*sin(fAngelXY);        //y方向のスピード
		speed.z = fDistanceZ;

		//パーティクルを発射
		SetPartical(speed.x,speed.y, speed.z,pParticalCenter->fRadius,120);
}

//パーティクル基本コントロール
void ParticalBaseControl(PARTICALCENTER *pParticalCenter)
{
	//発射角度が逆時計回りに沿いて回る
	if (GetKeyboardPress(DIK_LEFT))
	{
		pParticalCenter->fShootAngel -= 0.01*PI;
	}

	//発射角度が時計回りに沿いて回る
	if (GetKeyboardPress(DIK_RIGHT))
	{
		pParticalCenter->fShootAngel += 0.01*PI;
	}

	//発射角度が0以上確保(最大値は10*PI)
	if (pParticalCenter->fShootAngel <= -10 * PI)
	{
		pParticalCenter->fShootAngel = -10 * PI;
	}

	if (pParticalCenter->fShootAngel >= 10 * PI)
	{
		pParticalCenter->fShootAngel = 10 * PI;
	}

	//拡散角度拡大
	if (GetKeyboardPress(DIK_R))
	{
		pParticalCenter->fRangeAngel += 0.01*PI;
	}

	//拡散角度縮小
	if (GetKeyboardPress(DIK_F))
	{
		pParticalCenter->fRangeAngel -= 0.01*PI;
	}

	//拡散角度の範囲を0~PIに収まる
	if (pParticalCenter->fRangeAngel < 0.01)
	{
		pParticalCenter->fRangeAngel = 0.01;
	}

	if (pParticalCenter->fRangeAngel > PI)
	{
		pParticalCenter->fRangeAngel = PI;
	}

	//パーティクルの飛び基準距離をコントロール
	if (GetKeyboardPress(DIK_T))  //飛び距離伸びる
	{
		pParticalCenter->fDistance += 3;
	}

	if (GetKeyboardPress(DIK_G))  //飛び距離縮む
	{
		pParticalCenter->fDistance -= 3;
	}

	//飛び距離範囲制限
	if (pParticalCenter->fDistance < 0)
	{
		pParticalCenter->fDistance = 0;
	}

	if (pParticalCenter->fDistance > 100)
	{
		pParticalCenter->fDistance = 100;
	}
	/*
	//-----------------------------------------------------------------------------------------
	//風力計算
	//-----------------------------------------------------------------------------------------
	//風の角度計算
	if (GetKeyboardPress(DIK_I))
	{
		pParticalCenter->fWindAngel += 0.01*PI;
	}

	if (GetKeyboardPress(DIK_J))
	{
		pParticalCenter->fWindAngel -= 0.01*PI;
	}

	//角度範囲制限(-2*PI ~ 2*PI)
	if (pParticalCenter->fWindAngel < -2 * PI)
	{
		pParticalCenter->fWindAngel = -2 * PI;
	}

	if (pParticalCenter->fWindAngel > 2 * PI)
	{
		pParticalCenter->fWindAngel = 2 * PI;
	}

	//風力計算
	if (GetKeyboardPress(DIK_U))
	{
		pParticalCenter->fWindPower += 0.001;
	}

	if (GetKeyboardPress(DIK_H))
	{
		pParticalCenter->fWindPower -= 0.001;
	}

	//風力制限(0 ~ 10)
	if (pParticalCenter->fWindPower < 0)
	{
		pParticalCenter->fWindPower = 0;
	}

	if (pParticalCenter->fWindPower > 0.05)
	{
		pParticalCenter->fWindPower = 0.05;
	}
	*/
	//-----------------------------------------------------------------------------------------
	//重力計算
	//-----------------------------------------------------------------------------------------
	if (GetKeyboardPress(DIK_Y))
	{
		pParticalCenter->fGravity += 0.0001;
	}

	if (GetKeyboardPress(DIK_H))
	{
		pParticalCenter->fGravity -= 0.0001;
	}

	//重力制限(0 ~ 15)
	if (pParticalCenter->fGravity < 0)
	{
		pParticalCenter->fGravity = 0;
	}

	if (pParticalCenter->fGravity > 0.005 )
	{
		pParticalCenter->fGravity = 0.005;
	}
}

//----------------------------------------
//パーティクル爆発効果
//----------------------------------------
void ParticalExplosion(PARTICALCENTER *pParticalCenter)
{
	float fDistance = 0;                //パーティクルの飛び距離
	float fAngel = 0;                   //角度
	D3DXVECTOR3 speed;                  //パーティクルのスピード
	D3DXVECTOR3 test;

	//飛び距離範囲制限
	if(pParticalCenter->fDistance < 0)
	{
		pParticalCenter->fDistance = 0;
	}

	if(pParticalCenter->fDistance > 20)
	{
		pParticalCenter->fDistance = 20;
	}

	
	for(int nCnt = 0; nCnt < 120; nCnt++)
	{
		//-----------------------------------------------------------------------------------------
		//パーティクルの発射
		//-----------------------------------------------------------------------------------------
		fDistance = pParticalCenter->fDistance + rand()%20;       //飛び距離計算
		fAngel = pParticalCenter->fShootAngel + ( rand()%(int)(pParticalCenter->fRangeAngel*2*100) - (int)(pParticalCenter->fRangeAngel*100) ) * 0.01; //飛び角度計算
		speed.x = fDistance*cos(fAngel)/20;        //x方向のスピード
		speed.y = fDistance*sin(fAngel)/20;        //y方向のスピード

		test.x = (fDistance+10)*cos(fAngel)/20;        //x方向のスピード
		test.y = (fDistance+10)*sin(fAngel)/20;        //y方向のスピード

		//パーティクルを発射
		SetPartical(speed.x,speed.y,speed.z,pParticalCenter->fRadius,20);
	}

	pParticalCenter -> bUse = false;
}

//----------------------------------------
//パーティクル炎効果
//----------------------------------------
void ParticalFire(PARTICALCENTER *pParticalCenter)
{
	//発射角度計算
	float FirePosX = (rand()%100)*0.01;                //パーティクルの飛び距離
	D3DXVECTOR3 speed;                  //パーティクルのスピード
	
	speed.x = 0;
	speed.y = 0.01*(rand()%50 + 50);  //0.5-1
	speed.z = 0;

	//パーティクルを発射
	SetFirePartical(pParticalCenter,FirePosX,speed.x, speed.y, speed.z, 40);
}

//----------------------------------------
//パーティクル中心セット
//----------------------------------------
void SetParticalCenter(float fposX,float fposY,float fposZ,float fRadius,float fDistance, EFFECTMODE effectMode)
{
	for(int nCnt = 0; nCnt < NUM_PARTICALCENTERPOLYGON; nCnt++)
	{
		if(g_ParticalCenter[nCnt].bUse == false)
		{
			//パーティクルの初期位置
			g_ParticalCenter[nCnt].pos.x = fposX;
			g_ParticalCenter[nCnt].pos.y = fposY;
			g_ParticalCenter[nCnt].pos.z = fposZ;
			//パーティクルの初期スピード
			g_ParticalCenter[nCnt].speed.x = 1;
			g_ParticalCenter[nCnt].speed.y = 1;
			//パーティクルの古い位置
			g_ParticalCenter[nCnt].posOld.x = g_ParticalCenter[nCnt].pos.x;
			g_ParticalCenter[nCnt].posOld.y = g_ParticalCenter[nCnt].pos.y;
			//パーティクルの集中発射角度と拡散角度
			g_ParticalCenter[nCnt].fShootAngel = PI *0.5;
			g_ParticalCenter[nCnt].fRangeAngel = PI * 0.01;
			//パーティクルの飛び基準距離と半径
			g_ParticalCenter[nCnt].fDistance = fDistance;
			g_ParticalCenter[nCnt].fRadius = fRadius;
			g_ParticalCenter[nCnt].EffectMode = effectMode;
			//プレーヤー使用
			g_ParticalCenter[nCnt].bUse = true;
			g_ParticalCenter[nCnt].fGravity = 0.002;//重力
			break;
		}
	}
}