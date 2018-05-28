/*
cpp名:titleManager.cpp
タイトルアニメーションに使うcppです
*/

#include "result.h"
#include "input.h"
#include "inputMouse.h"
#include "fade.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define RESULT_BG_TEX "data/TEXTURE/result/resultBG.png"
#define RESULT_RETURN_TEX "data/TEXTURE/result/return.png"

#define RETURN_WIDTH (200)
#define RETURN_HEIGHT (100)

CResult::CResult()
{

}

CResult::~CResult()
{

}

void CResult::Init(void)
{
	
	m_ResultObj[0].Init(D3DXVECTOR3(SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.5, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), RESULT_BG_TEX, true);
	m_ResultObj[1].Init(D3DXVECTOR3(SCREEN_WIDTH*0.85, SCREEN_HEIGHT*0.85, 0.0f), D3DXVECTOR3(RETURN_WIDTH, RETURN_HEIGHT, 0.0f), RESULT_RETURN_TEX, true);
}

void CResult::Uninit(void)
{
	for (int nCnt = 0; nCnt < RESULTOBJ_NUM; nCnt++)
	{
		m_ResultObj[nCnt].Uninit();
	}
}

void CResult::Update(void)
{
	D3DXVECTOR3 MousePos = GetMousePosInWindow();             //マウス座標取得

	bool bReturn = m_ResultObj[1].PointInPolygon(&MousePos);

	//左アローアイコン
	if (bReturn == true)
	{
		m_ResultObj[1].SetColor(255, 0, 0);

		if (GetMouseTrigger(MOUSE_LEFT))
		{
			SetFade(FADE_OUT,MODE_TITLE);
		}
	}

	else
	{
		m_ResultObj[1].SetColor(255, 255, 255);
	}


	for (int nCnt = 0; nCnt < RESULTOBJ_NUM; nCnt++)
	{
		if (m_ResultObj[nCnt].GetUse() == true)
		{
			m_ResultObj[nCnt].Update();
		}
	}
}

void CResult::Draw(void)
{
	if(m_ResultObj[0].GetUse() == true)
	{
		m_ResultObj[0].Draw();
	}

	if (m_ResultObj[1].GetUse() == true)
	{
		m_ResultObj[1].Draw();
	}
}