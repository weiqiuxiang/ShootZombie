#include "CGameUI.h"
#include "model_aircraft.h"

#define SURASHU_TEX "data/TEXTURE/game/mukendai.png"
#define MUKENDAI_TEX "data/TEXTURE/game/surashu.png"
#define BULLETNUM_TEX "data/TEXTURE/game/bullet_number.png"
#define HP_TEX "data/TEXTURE/game/HP.png"
#define SCORE_TEX "data/TEXTURE/score/score_font.png"
#define HANDGUN_TEX "data/TEXTURE/game/gun.png"
#define AK47_TEX "data/TEXTURE/game/gun02.png"
#define GRENADE_TEX "data/TEXTURE/game/Grenade.png"
#define HANDGUNEF_TEX "data/TEXTURE/game/gunSelect.png"
#define AK47EF_TEX "data/TEXTURE/game/gun02Select.png"
#define GRENADEEF_TEX "data/TEXTURE/game/GrenadeSelect.png"
#define RELOAD_TEX "data/TEXTURE/game/reload.png"
#define STAMINA_TEX "data/TEXTURE/game/stamina.png"

#define NUMBER_WIDTH (35)
#define NUMBER_HEIGHT (35)
#define NUMBER_DISTANCE (35)
#define MUKENDAI_WIDTH (70)
#define MUKENDAI_HEIGHT (35)

#define RELOAD_SHOWTIME (60) 

#define STAMINA_GAUGE_HEIGHT (10)
#define STAMINA_GAUGE_WIDTHMAX (250)

CGameUI::CGameUI()
{
	
}
CGameUI::~CGameUI()
{
	
}

void CGameUI::Init(void)
{
	//数値ロード部分
	CBullet::LoadTex(BULLETNUM_TEX);

	AIRCRAFT *pAircraft = Get_Aircraft();
	int HandGunBulletNow = pAircraft->m_HandGunNum;
	int BulletNow = pAircraft->m_AK47Num;
	int BulletStore = pAircraft->m_AK47Store;

	m_nFrame = 0;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		int nNumber = HandGunBulletNow % 10;
		HandGunBulletNow = HandGunBulletNow / 10;
		m_BulletNum[nCnt].Init(D3DXVECTOR3(SCREEN_WIDTH*0.75 + (2 - 1 - nCnt)*NUMBER_DISTANCE, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f), D3DXVECTOR3(nNumber*0.1, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 1.0f, 0.0f));
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		int nNumber = BulletStore % 10;
		BulletStore = BulletStore / 10;
		m_BulletStore[nCnt].Init(D3DXVECTOR3(SCREEN_WIDTH*0.9 + (2 - 1 - nCnt)*NUMBER_DISTANCE, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f), D3DXVECTOR3(nNumber*0.1, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 1.0f, 0.0f));
	}

	//UIOBJロード部分
	m_UIObj[0].Init(D3DXVECTOR3(SCREEN_WIDTH*0.85, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f), MUKENDAI_TEX, true);
	m_UIObj[1].Init(D3DXVECTOR3((SCREEN_WIDTH*0.9 + NUMBER_DISTANCE), SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(MUKENDAI_WIDTH, MUKENDAI_HEIGHT, 0.0f), SURASHU_TEX, true);

	for (int nCnt = 0; nCnt < LIFE_MAX; nCnt++)
	{
		m_UIObj[2+ nCnt].Init(D3DXVECTOR3(SCREEN_WIDTH*0.05 + nCnt*NUMBER_WIDTH, SCREEN_HEIGHT*0.05, 0.0f), D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT, 0.0f), HP_TEX, true);
	}

	m_UIObj[7].Init(D3DXVECTOR3(SCREEN_WIDTH*0.6, SCREEN_HEIGHT*0.05, 0.0f), D3DXVECTOR3(NUMBER_WIDTH*2, NUMBER_HEIGHT, 0.0f), SCORE_TEX, true);

	m_UIObj[8].Init(D3DXVECTOR3(SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH * 2.15, NUMBER_HEIGHT*1.5, 0.0f), HANDGUN_TEX, true);
	m_UIObj[9].Init(D3DXVECTOR3(SCREEN_WIDTH*0.1+ NUMBER_WIDTH*2.5, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH * 2.15, NUMBER_HEIGHT*1.5, 0.0f), AK47_TEX, true);
	m_UIObj[10].Init(D3DXVECTOR3(SCREEN_WIDTH*0.1 + NUMBER_WIDTH*5, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH * 2.15, NUMBER_HEIGHT*1.5, 0.0f), GRENADE_TEX, true);

	m_UIObj[11].Init(D3DXVECTOR3(SCREEN_WIDTH*0.1, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH * 2.15, NUMBER_HEIGHT*1.5, 0.0f), HANDGUNEF_TEX, true);
	m_UIObj[12].Init(D3DXVECTOR3(SCREEN_WIDTH*0.1 + NUMBER_WIDTH * 2.5, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH * 2.15, NUMBER_HEIGHT*1.5, 0.0f), AK47EF_TEX, true);
	m_UIObj[13].Init(D3DXVECTOR3(SCREEN_WIDTH*0.1 + NUMBER_WIDTH * 5, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(NUMBER_WIDTH * 2.15, NUMBER_HEIGHT*1.5, 0.0f), GRENADEEF_TEX, true);

	//色とアルファ調整
	m_UIObj[11].SetColor(0, 0, 255);
	m_UIObj[12].SetColor(0, 0, 255);
	m_UIObj[13].SetColor(0, 0, 255);
	m_UIObj[11].SetAlpha(255);
	m_UIObj[12].SetAlpha(255);
	m_UIObj[13].SetAlpha(255);

	m_UIObj[14].Init(D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.5, 0.0f), D3DXVECTOR3(130.0f, 35.0f, 0.0f), RELOAD_TEX, true);

	m_UIObj[15].Init(D3DXVECTOR3(SCREEN_WIDTH*0.01 + STAMINA_GAUGE_WIDTHMAX*0.5, SCREEN_HEIGHT*0.1, 0.0f), D3DXVECTOR3(STAMINA_GAUGE_WIDTHMAX, STAMINA_GAUGE_HEIGHT, 0.0f), STAMINA_TEX, true);
}

void CGameUI::Uninit(void)
{
	CBullet::UnloadTex();

	for (int nCnt = 0; nCnt < UIOBJ_NUM; nCnt++)
	{
		m_UIObj[nCnt].Uninit();
	}
}

void CGameUI::Update(void)
{
	//数値アップデート部分
	AIRCRAFT *pAircraft = Get_Aircraft();
	int HandGunBulletNow = pAircraft->m_HandGunNum;
	int BulletNow = pAircraft->m_AK47Num;
	int BulletStore = pAircraft->m_AK47Store;
	int GrenadeNow = pAircraft->m_GrenadeNum;
	int GrenadeStore = pAircraft->m_GrenadeStore;

	m_nFrame++;

	if (pAircraft->m_GunType == GUNTYPE_HANDGUN)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			int nNumber = HandGunBulletNow % 10;
			HandGunBulletNow = HandGunBulletNow / 10;
			m_BulletNum[nCnt].SetTexPos(&D3DXVECTOR3(nNumber*0.1, 0.0f, 0.0f));
		}
	}

	if (pAircraft->m_GunType == GUNTYPE_AK47)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			int nNumber = BulletNow % 10;
			BulletNow = BulletNow / 10;
			m_BulletNum[nCnt].SetTexPos(&D3DXVECTOR3(nNumber*0.1, 0.0f, 0.0f));

			int nNumberStore = BulletStore % 10;
			BulletStore = BulletStore / 10;
			m_BulletStore[nCnt].SetTexPos(&D3DXVECTOR3(nNumberStore*0.1, 0.0f, 0.0f));
		}
	}

	if (pAircraft->m_GunType == GUNTYPE_GRENADE)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			int nNumber = GrenadeNow % 10;
			GrenadeNow = GrenadeNow / 10;
			m_BulletNum[nCnt].SetTexPos(&D3DXVECTOR3(nNumber*0.1, 0.0f, 0.0f));

			int nNumberStore = GrenadeStore % 10;
			GrenadeStore = GrenadeStore / 10;
			m_BulletStore[nCnt].SetTexPos(&D3DXVECTOR3(nNumberStore*0.1, 0.0f, 0.0f));
		}
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_BulletNum[nCnt].Update();
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_BulletStore[nCnt].Update();
	}

	//体力ゲージ表示更新
	float fStaminaGauge = pAircraft->m_fStamina / (float)STAMINA_MAX;
	m_UIObj[15].SetPos( &D3DXVECTOR3(SCREEN_WIDTH*0.01 + fStaminaGauge*STAMINA_GAUGE_WIDTHMAX*0.5, SCREEN_HEIGHT*0.1,0.0f) );
	m_UIObj[15].SetSize(&D3DXVECTOR3(STAMINA_GAUGE_WIDTHMAX*fStaminaGauge, STAMINA_GAUGE_HEIGHT, 0.0f));

	//背景オブジェアップデート部分
	for (int nCnt = 0; nCnt < UIOBJ_NUM; nCnt++)
	{
		if (m_UIObj[nCnt].GetUse() == true) m_UIObj[nCnt].Update();
	}

	if(m_nFrame >= 10000)
	{
		m_nFrame = 0;
	}
	
}

void CGameUI::Draw(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	if (pAircraft->m_GunType == GUNTYPE_HANDGUN)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			m_BulletNum[nCnt].Draw();
		}

		//
		if (m_UIObj[0].GetUse() == true) m_UIObj[0].Draw();

		//
		if (m_UIObj[1].GetUse() == true) m_UIObj[1].Draw();
	}

	if ( (pAircraft->m_GunType == GUNTYPE_AK47) || (pAircraft->m_GunType == GUNTYPE_GRENADE) )
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			m_BulletNum[nCnt].Draw();
		}

		//
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			m_BulletStore[nCnt].Draw();
		}

		//
		if (m_UIObj[0].GetUse() == true) m_UIObj[0].Draw();
	}

	if (m_UIObj[2].GetUse() == true)
	{
		for (int nCnt = 0; nCnt < pAircraft->m_nLife; nCnt++)
		{
			m_UIObj[2+nCnt].Draw();
		}
	}

	if(m_UIObj[7].GetUse() == true)
	{
		m_UIObj[7].Draw();
	}

	if (m_UIObj[8].GetUse() == true)
	{
		m_UIObj[8].Draw();
	}

	if (m_UIObj[9].GetUse() == true)
	{
		m_UIObj[9].Draw();
	}

	if (m_UIObj[10].GetUse() == true)
	{
		m_UIObj[10].Draw();
	}

	// アルファ・ブレンディングを行う
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	// 描画絵ポリゴンはαで透明化になっていく
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// 先の絵は透明化しない
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	if (pAircraft->m_GunType == GUNTYPE_HANDGUN)
	{
		if (m_UIObj[11].GetUse() == true)
		{
			m_UIObj[11].Draw();
			m_UIObj[11].Draw();
		}
	}

	if (pAircraft->m_GunType == GUNTYPE_AK47)
	{
		if (m_UIObj[12].GetUse() == true)
		{
			m_UIObj[12].Draw();
			m_UIObj[12].Draw();
		}
	}

	if (pAircraft->m_GunType == GUNTYPE_GRENADE)
	{
		if (m_UIObj[13].GetUse() == true)
		{
			m_UIObj[13].Draw();
			m_UIObj[13].Draw();
		}
	}

	// 先の絵はα値で不透明なっていく
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// 描画絵はα値で透明なっていく
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	if(
		((pAircraft->m_bReloadHandGun == true) && (pAircraft->m_GunType == GUNTYPE_HANDGUN)) ||
		((pAircraft->m_bReloadAK47 == true) && (pAircraft->m_GunType == GUNTYPE_AK47))
		)
	{
		int nShow = m_nFrame % RELOAD_SHOWTIME;

		if(nShow <= RELOAD_SHOWTIME /2)
		{
			nShow = nShow % (RELOAD_SHOWTIME/2);
			m_UIObj[14].SetAlpha( (float)nShow / (float)(RELOAD_SHOWTIME / 2) * 255 );
		}
		else
		{
			nShow = nShow % (RELOAD_SHOWTIME / 2);
			m_UIObj[14].SetAlpha((float)255 - (float)nShow / (float)(RELOAD_SHOWTIME / 2) * 255);
		}
	}
	else
	{
		m_UIObj[14].SetAlpha(0);
	}

	if (m_UIObj[14].GetUse() == true)
	{
		m_UIObj[14].Draw();
	}

	if (m_UIObj[15].GetUse() == true)
	{
		m_UIObj[15].Draw();
	}
}