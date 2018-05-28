/*
cpp名:titleManager.cpp
	タイトルアニメーションに使うcppです
*/

#include "titleManager.h"
#include "input.h"
#include "inputMouse.h"
#include "fade.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define TITLEBGL_TEX "data/TEXTURE/title/titleL.png"
#define TITLEBGR_TEX "data/TEXTURE/title/titleR.png"
#define TITLELOGO_TEX "data/TEXTURE/title/titleLogo.png"
#define TITLELOGOEF_TEX "data/TEXTURE/title/titleLogoEf.png"
#define TITLESELECTION01_TEX "data/TEXTURE/title/start.png"
#define TITLESELECTION02_TEX "data/TEXTURE/title/exit.png"

#define LOGO_WIDTH (500)
#define LOGO_HEIGHT (100)
#define SELE_ICON_WIDTH (250)
#define SELE_ICON_HEIGHT (85)

CTitle::CTitle()
{
	
}

CTitle::~CTitle()
{
	
}

void CTitle::Init(void)
{
	m_nFrame = 0;                                                //フレーム経過数初期化

	//タイトルオブジェ定義
	m_TitleObj[0].Init(D3DXVECTOR3(-SCREEN_WIDTH*0.25, SCREEN_HEIGHT*0.5,0.0f), D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT,0.0f), TITLEBGL_TEX,true);
	m_TitleObj[1].Init(D3DXVECTOR3(SCREEN_WIDTH*1.25, SCREEN_HEIGHT*0.5, 0.0f), D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT, 0.0f), TITLEBGL_TEX, true);

	m_TitleObj[2].Init(D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.3, 0.0f), D3DXVECTOR3(LOGO_WIDTH, LOGO_HEIGHT, 0.0f), TITLELOGO_TEX, true);
	m_TitleObj[2].SetAlpha(0);

	m_TitleObj[3].Init(D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.3, 0.0f), D3DXVECTOR3(LOGO_WIDTH, LOGO_HEIGHT, 0.0f), TITLELOGOEF_TEX, true);
	m_TitleObj[3].SetAlpha(0);

	m_TitleObj[4].Init(D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.6, 0.0f), D3DXVECTOR3(SELE_ICON_WIDTH, SELE_ICON_HEIGHT, 0.0f), TITLESELECTION01_TEX, true);
	m_TitleObj[4].SetAlpha(0);
	m_TitleObj[5].Init(D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.8, 0.0f), D3DXVECTOR3(SELE_ICON_WIDTH, SELE_ICON_HEIGHT, 0.0f), TITLESELECTION02_TEX, true);
	m_TitleObj[5].SetAlpha(0);

}

void CTitle::Uninit(void)
{
	for(int nCnt = 0; nCnt < TITLEOBJ_NUM; nCnt++)
	{
		m_TitleObj[nCnt].Uninit();
	}
}

void CTitle::Update(void)
{
	//ドア閉める
	if (m_nFrame <= 59)
	{
		float MoveSpeed = SCREEN_WIDTH*0.5 / 60;
		m_TitleObj[0].AddPos(&D3DXVECTOR3(MoveSpeed, 0.0f, 0.0f));
		m_TitleObj[1].AddPos(&D3DXVECTOR3(-MoveSpeed, 0.0f, 0.0f));
	}

	else if ((m_nFrame > 60) && (m_nFrame <= 100))
	{
		int addAlpha = 255 / 40 + 1;
		m_TitleObj[2].AddAlpha(addAlpha);
	}

	else if((m_nFrame > 100) && (m_nFrame <= 140))
	{
		int addAlpha = 255 / 40 + 1;
		m_TitleObj[3].AddAlpha(addAlpha);
	}

	else if ((m_nFrame > 140) && (m_nFrame <= 200))
	{
		int addAlpha = 255 / 40 + 1;
		m_TitleObj[4].AddAlpha(addAlpha);

		if(m_nFrame > 160)
		{
			int addAlpha2 = 255 / 40 + 1;
			m_TitleObj[5].AddAlpha(addAlpha);
		}
	}

	else if(m_nFrame > 200)
	{
		D3DXVECTOR3 MousePos = GetMousePosInWindow();
		bool bStart = m_TitleObj[4].PointInPolygon(&MousePos);
		bool bEnd = m_TitleObj[5].PointInPolygon(&MousePos);

		//スケール
		if(bStart == true)
		{
			m_TitleObj[4].SetColor(255,0,0);
		}

		else
		{
			m_TitleObj[4].SetColor(255, 255, 255);
		}

		if (bEnd == true)
		{
			m_TitleObj[5].SetColor(255, 0, 0);
		}

		else
		{
			m_TitleObj[5].SetColor(255, 255, 255);
		}
		
		if((bStart == true) && (bEnd == false) )
		{
			if(GetMouseTrigger(MOUSE_LEFT))
			{
				SetFade(FADE_OUT, MODE_TUTORIAL);
			}
		}

		if ((bStart == false) && (bEnd == true))
		{
			if (GetMouseTrigger(MOUSE_LEFT))
			{
				exit(0);
			}
		}
	}

	//titleobjの更新
	for (int nCnt = 0;nCnt < TITLEOBJ_NUM; nCnt++)
	{
		if (m_TitleObj[nCnt].GetUse() == true)
		{
			m_TitleObj[nCnt].Update();
		}
	}

	m_nFrame++;
	if (m_nFrame > 10000) m_nFrame = 201;
}

void CTitle::Draw(void)
{
	//titleobjの描画
	if (m_TitleObj[0].GetUse() == true)
	{
		m_TitleObj[0].Draw();
	}

	if (m_TitleObj[1].GetUse() == true)
	{
		m_TitleObj[1].Draw();
	}

	if (m_TitleObj[3].GetUse() == true)
	{
		m_TitleObj[3].Draw();
	}

	if (m_TitleObj[2].GetUse() == true)
	{
		m_TitleObj[2].Draw();
	}

	if (m_TitleObj[4].GetUse() == true)
	{
		m_TitleObj[4].Draw();
	}

	if (m_TitleObj[5].GetUse() == true)
	{
		m_TitleObj[5].Draw();
	}
}