#include "particalManager.h"

#define PARTICAL_TEX "data/TEXTURE/partical/partical.png"
#define BASE_TEX "data/TEXTURE/partical/base.jpg"
#define WATER_TEX "data/TEXTURE/partical/water.jpg"
#define FIRE_TEX "data/TEXTURE/partical/fire.jpg"

//グローバル変数
CPartical *CParticalManager::m_aPartical[PARTICAL_MAX] = {};
CPartical *CParticalManager::m_aParticalEF[PARTICAL_EFFECT] = {};

CParticalManager::CParticalManager()
{
	
}

CParticalManager:: ~CParticalManager()
{

}
void CParticalManager::Init(void)
{
	CPartical::LoadTex(0, PARTICAL_TEX);
	CPartical::LoadTex(1, BASE_TEX);
	CPartical::LoadTex(2, WATER_TEX);
	CPartical::LoadTex(3, FIRE_TEX);
}
void CParticalManager::Uninit(void)
{
	ReleaseAll();
	CPartical::UnloadTex();
}

void CParticalManager::Update(void)
{
	//ライフ管理
	for (int nCnt = 0; nCnt < PARTICAL_MAX; nCnt++)
	{
		if (m_aPartical[nCnt] != NULL)
		{
			if(m_aPartical[nCnt]->GetLife() < 0)
			{
				m_aPartical[nCnt]->Uninit();
				delete m_aPartical[nCnt];
				m_aPartical[nCnt] = NULL;
			}
		}
	}

	for (int nCnt = 0; nCnt < PARTICAL_EFFECT; nCnt++)
	{
		if (m_aParticalEF[nCnt] != NULL)
		{
			if (m_aParticalEF[nCnt]->GetLife() < 0)
			{
				m_aParticalEF[nCnt]->Uninit();
				delete m_aParticalEF[nCnt];
				m_aParticalEF[nCnt] = NULL;
			}
		}
	}

	UpdateAll();                                                 //全てのパーティクル更新
}

void CParticalManager::Draw(void)
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

	DrawAll();

	//ブレンドモードをアルファブレンドに戻す
	// 先の絵はα値で不透明なっていく
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 描画絵はα値で透明なっていく
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      //z情報残す
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //ライトをオンにする
}

void CParticalManager::SetPartical(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *speed, float fRadius, int nLife, int TexNum, CPartical::PARTICAL_TYPE type, const D3DXVECTOR3 *color, float fGravity)
{
	for (int nCnt = 0; nCnt < PARTICAL_MAX; nCnt++)
	{
		if (m_aPartical[nCnt] == NULL)
		{
			m_aPartical[nCnt] = new CPartical;
			m_aPartical[nCnt]->Init(pos, speed, fRadius, nLife, TexNum, type, fGravity);
			m_aPartical[nCnt]->SetColor((int)color->x, (int)color->y, (int)color->z);
			break;
		}
	}
}

void CParticalManager::SetParticalEF(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *speed, float fRadius, int nLife, int TexNum, CPartical::PARTICAL_TYPE type, const D3DXVECTOR3 *color, float fGravity)
{
	for (int nCnt = 0; nCnt < PARTICAL_EFFECT; nCnt++)
	{
		if (m_aParticalEF[nCnt] == NULL)
		{
			m_aParticalEF[nCnt] = new CPartical;
			m_aParticalEF[nCnt]->Init(pos, speed, fRadius, nLife, TexNum, type, fGravity);
			m_aParticalEF[nCnt]->SetColor((int)color->x, (int)color->y, (int)color->z);
			break;
		}
	}
}

void CParticalManager::UpdateAll()
{
	for(int nCnt = 0; nCnt < PARTICAL_MAX; nCnt++)
	{
		if(m_aPartical[nCnt] != NULL)
		{
			m_aPartical[nCnt]->Update();
		}
	}

	for (int nCnt = 0; nCnt < PARTICAL_EFFECT; nCnt++)
	{
		if (m_aParticalEF[nCnt] != NULL)
		{
			m_aParticalEF[nCnt]->Update();
		}
	}
}

void CParticalManager::DrawAll()
{
	for (int nCnt = 0; nCnt < PARTICAL_MAX; nCnt++)
	{
		if (m_aPartical[nCnt] != NULL)
		{
			m_aPartical[nCnt]->Draw();
		}
	}

	for (int nCnt = 0; nCnt < PARTICAL_EFFECT; nCnt++)
	{
		if (m_aParticalEF[nCnt] != NULL)
		{
			m_aParticalEF[nCnt]->Draw();
		}
	}
}

void CParticalManager::ReleaseAll()
{
	for (int nCnt = 0; nCnt < PARTICAL_MAX; nCnt++)
	{
		if (m_aPartical[nCnt] != NULL)
		{
			m_aPartical[nCnt]->Uninit();
			delete m_aPartical[nCnt];
			m_aPartical[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < PARTICAL_EFFECT; nCnt++)
	{
		if (m_aParticalEF[nCnt] != NULL)
		{
			m_aParticalEF[nCnt]->Uninit();
			delete m_aParticalEF[nCnt];
			m_aParticalEF[nCnt] = NULL;
		}
	}
}