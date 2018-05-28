/*
cpp名:titleManager.cpp
タイトルアニメーションに使うcppです
*/

#include "CScore.h"

//-----------------------------------------------------------------------------------------
//マクロ定義
//-----------------------------------------------------------------------------------------
#define NUMBER_TEX "data/TEXTURE/score/score.png"

#define NUMBER_WIDTH (25)
#define NUMBER_HEIGHT (24)
#define NUMBER_DISTANCE (25)

CScore::CScore()
{
	m_nScore = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

CScore::~CScore()
{

}

void CScore::Init(D3DXVECTOR3 pos)
{
	int Score;
	m_nScore = 0;
	Score = m_nScore;
	m_pos = pos;
	CNumber::LoadTex(NUMBER_TEX);

	for (int nCnt = 0;nCnt < DIGIT_NUM; nCnt++)
	{
		int nNumber = Score % 10;
		Score = Score / 10;
		m_Number[nCnt].Init(D3DXVECTOR3(m_pos.x+ (DIGIT_NUM -1 -nCnt)*NUMBER_DISTANCE,m_pos.y,0.0f),D3DXVECTOR3(NUMBER_WIDTH, NUMBER_HEIGHT,0.0f), D3DXVECTOR3(nNumber*0.1,0.0f,0.0f), D3DXVECTOR3(0.1f,1.0f,0.0f));
	}
}

void CScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < DIGIT_NUM; nCnt++)
	{
		m_Number[nCnt].Uninit();
	}
	
	CNumber::UnloadTex();
}

void CScore::Update(void)
{
	int Score = m_nScore;

	for (int nCnt = 0; nCnt < DIGIT_NUM; nCnt++)
	{
		int nNumber = Score % 10;
		Score = Score / 10;
		m_Number[nCnt].SetTexPos(&D3DXVECTOR3(nNumber*0.1, 0.0f, 0.0f));
	}

	for (int nCnt = 0; nCnt < DIGIT_NUM; nCnt++)
	{
		m_Number[nCnt].Update();
	}
}

void CScore::Draw(void)
{
	for (int nCnt = 0; nCnt < DIGIT_NUM; nCnt++)
	{
		m_Number[nCnt].Draw();
	}
}

int CScore::GetScore(void)
{
	return m_nScore;
}
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;
}