/*
cpp名:titleManager.cpp
タイトルアニメーションに使うcppです
*/

#include "tutorial.h"
#include "input.h"
#include "inputMouse.h"
#include "fade.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define TUTORIAL_BG_TEX "data/TEXTURE/tutorial/TutorialBG.png"
#define TUTORIAL_IMG01_TEX "data/TEXTURE/tutorial/tutorial01.jpg"
#define TUTORIAL_IMG02_TEX "data/TEXTURE/tutorial/tutorial02.jpg"
#define TUTORIAL_IMG03_TEX "data/TEXTURE/tutorial/tutorial03.jpg"
#define TUTORIAL_IMG04_TEX "data/TEXTURE/tutorial/tutorial04.jpg"
#define TUTORIAL_IMG05_TEX "data/TEXTURE/tutorial/tutorial05.jpg"
#define TUTORIAL_SKIP_TEX "data/TEXTURE/tutorial/skip.png"
#define TUTORIAL_ARROWL_TEX "data/TEXTURE/tutorial/arrowL.png"
#define TUTORIAL_ARROWR_TEX "data/TEXTURE/tutorial/arrowR.png"

#define ARROW_WIDTH (70)
#define ARROW_HEIGHT (70)
#define TUTORIALBG_WIDTH (600)
#define TUTORIALBG_HEIGHT (450)
#define SKIP_ICON_WIDTH (150)
#define SKIP_ICON_HEIGHT (75)

CTutorial::CTutorial()
{

}

CTutorial::~CTutorial()
{

}

void CTutorial::Init(void)
{
	m_nDrawNum = 1;
	m_TutorialObj[0].Init(D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.5, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), TUTORIAL_BG_TEX, true);

	m_TutorialObj[1].Init(D3DXVECTOR3(TUTORIALBG_WIDTH*0.5 + 5.0f, TUTORIALBG_HEIGHT*0.5 + 5.0f, 0.0f), D3DXVECTOR3(TUTORIALBG_WIDTH, TUTORIALBG_HEIGHT, 0.0f), TUTORIAL_IMG01_TEX, true);
	m_TutorialObj[2].Init(D3DXVECTOR3(TUTORIALBG_WIDTH*0.5 + 5.0f, TUTORIALBG_HEIGHT*0.5 + 5.0f, 0.0f), D3DXVECTOR3(TUTORIALBG_WIDTH, TUTORIALBG_HEIGHT, 0.0f), TUTORIAL_IMG02_TEX, true);
	m_TutorialObj[3].Init(D3DXVECTOR3(TUTORIALBG_WIDTH*0.5 + 5.0f, TUTORIALBG_HEIGHT*0.5 + 5.0f, 0.0f), D3DXVECTOR3(TUTORIALBG_WIDTH, TUTORIALBG_HEIGHT, 0.0f), TUTORIAL_IMG03_TEX, true);
	m_TutorialObj[4].Init(D3DXVECTOR3(TUTORIALBG_WIDTH*0.5 + 5.0f, TUTORIALBG_HEIGHT*0.5 + 5.0f, 0.0f), D3DXVECTOR3(TUTORIALBG_WIDTH, TUTORIALBG_HEIGHT, 0.0f), TUTORIAL_IMG04_TEX, true);
	m_TutorialObj[5].Init(D3DXVECTOR3(TUTORIALBG_WIDTH*0.5 + 5.0f, TUTORIALBG_HEIGHT*0.5 + 5.0f, 0.0f), D3DXVECTOR3(TUTORIALBG_WIDTH, TUTORIALBG_HEIGHT, 0.0f), TUTORIAL_IMG05_TEX, true);

	m_TutorialObj[6].Init(D3DXVECTOR3(SCREEN_WIDTH*0.75, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(ARROW_WIDTH, ARROW_HEIGHT, 0.0f), TUTORIAL_ARROWL_TEX, true);
	m_TutorialObj[7].Init(D3DXVECTOR3(SCREEN_WIDTH*0.90, SCREEN_HEIGHT*0.9, 0.0f), D3DXVECTOR3(ARROW_WIDTH, ARROW_HEIGHT, 0.0f), TUTORIAL_ARROWR_TEX, true);
	m_TutorialObj[8].Init(D3DXVECTOR3(SCREEN_WIDTH*0.83, SCREEN_HEIGHT*0.78, 0.0f), D3DXVECTOR3(SKIP_ICON_WIDTH, SKIP_ICON_HEIGHT, 0.0f), TUTORIAL_SKIP_TEX, true);
}

void CTutorial::Uninit(void)
{
	for (int nCnt = 0; nCnt < TUTORIALOBJ_NUM; nCnt++)
	{
		m_TutorialObj[nCnt].Uninit();
	}
}

void CTutorial::Update(void)
{
	D3DXVECTOR3 MousePos = GetMousePosInWindow();             //マウス座標取得

	bool bLeftArrow = m_TutorialObj[6].PointInPolygon(&MousePos);
	bool bRightArrow = m_TutorialObj[7].PointInPolygon(&MousePos);
	bool bSkip = m_TutorialObj[8].PointInPolygon(&MousePos);

	//左アローアイコン
	if(bLeftArrow == true)
	{
		m_TutorialObj[6].SetColor(255,0,0);

		if(GetMouseTrigger(MOUSE_LEFT))
		{
			m_nDrawNum--;
		}
	}

	else
	{
		m_TutorialObj[6].SetColor(255, 255, 255);
	}

	//右アローアイコン
	if (bRightArrow == true)
	{
		m_TutorialObj[7].SetColor(255, 0, 0);

		if (GetMouseTrigger(MOUSE_LEFT))
		{
			m_nDrawNum++;
		}
	}

	else
	{
		m_TutorialObj[7].SetColor(255, 255, 255);
	}

	//スキップアイコン
	if (bSkip == true)
	{
		m_TutorialObj[8].SetColor(255, 0, 0);

		if (GetMouseTrigger(MOUSE_LEFT))
		{
			SetFade(FADE_OUT, MODE_GAME);
		}
	}

	else
	{
		m_TutorialObj[8].SetColor(255, 255, 255);
	}

	for (int nCnt = 0; nCnt < TUTORIALOBJ_NUM; nCnt++)
	{
		if (m_TutorialObj[nCnt].GetUse() == true)
		{
			m_TutorialObj[nCnt].Update();
		}
	}

	if(m_nDrawNum < 1)
	{
		m_nDrawNum = 1;
	}
	if(m_nDrawNum > 5)
	{
		m_nDrawNum = 5;
	}
}

void CTutorial::Draw(void)
{
	if (m_TutorialObj[0].GetUse() == true)
	{
		m_TutorialObj[0].Draw();
	}

	if ((m_nDrawNum >= 1) && (m_nDrawNum < 6))
	{
		if (m_TutorialObj[m_nDrawNum].GetUse() == true)
		{
			m_TutorialObj[m_nDrawNum].Draw();
		}
	}

	if (m_nDrawNum != 1)
	{
		if (m_TutorialObj[6].GetUse() == true)
		{
			m_TutorialObj[6].Draw();
		}
	}

	if (m_nDrawNum != 5)
	{
		if (m_TutorialObj[7].GetUse() == true)
		{
			m_TutorialObj[7].Draw();
		}
	}

	
	if (m_TutorialObj[8].GetUse() == true)
	{
		m_TutorialObj[8].Draw();
	}
}