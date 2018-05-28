#include "CGrenadeTrack.h"

//マクロ
#define TRACK_FRAME (5) //計測フレーム
#define BILLBOARD_P_TEX "data/TEXTURE/partical/partical.png"
#define BILLBOARD_PEF_TEX "data/TEXTURE/partical/base.jpg"

//static変数

//定番関数
CGrenadeTrack::CGrenadeTrack()
{
	
}
CGrenadeTrack::~CGrenadeTrack()
{
	
}

void CGrenadeTrack::Init(void)
{
	CObj3dBillboard::LoadTex(0, BILLBOARD_P_TEX);
	CObj3dBillboard::LoadTex(1, BILLBOARD_PEF_TEX);

	for(int nCnt = 0; nCnt < BILLBOARD_NUM;nCnt++)
	{
		m_ObjBillUse[nCnt] = false;
	}
}
void CGrenadeTrack::Uninit(void)
{
	CObj3dBillboard::UnloadTex();

	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		m_ObjBill[nCnt].Uninit();
		m_ObjBillEF[nCnt].Uninit();
	}
}
void CGrenadeTrack::Update(void)
{
	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		if (m_ObjBillUse[nCnt] == true)
		{
			m_ObjBill[nCnt].Update();
			m_ObjBillEF[nCnt].Update();
		}
	}
}
void CGrenadeTrack::Draw(void)
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

	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		if (m_ObjBillUse[nCnt] == true)
		{
			m_ObjBill[nCnt].Draw();
			m_ObjBillEF[nCnt].Draw();
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

//ファクション関数
void CGrenadeTrack::SetTrack(const D3DXVECTOR3 *pos/*開始位置*/, const D3DXVECTOR3 *speed/*スピード*/, float gravity/*グレネードの重力影響*/, const D3DXVECTOR3 *color/*軌跡のカラー*/)
{
	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		m_ObjBillUse[nCnt] = false;
	}

	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		D3DXVECTOR3 posResult;

		//第nCnt目の点の位置の計算(点と点のframe数はTRACK_FRAME)
		posResult.x = pos->x + speed->x * TRACK_FRAME * nCnt;
		posResult.y = speed->y * TRACK_FRAME * nCnt - 0.5 * gravity * (TRACK_FRAME * nCnt) * (TRACK_FRAME * nCnt) + pos->y;          //S = vt - 0.5*g*t^2;
		posResult.z = pos->z + speed->z * TRACK_FRAME * nCnt;
		m_ObjBill[nCnt].Init(&posResult,&D3DXVECTOR3(0.0f,0.0f,0.0f), &D3DXVECTOR2(50.0f, 50.0f),0);
		m_ObjBillEF[nCnt].Init(&posResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR2(100.0f, 100.0f), 1);
		m_ObjBillUse[nCnt] = true;

		if (posResult.y < pos->y) break;
	}
}