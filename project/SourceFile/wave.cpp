#include "wave.h"
#include "CParticalSetting.h"
#include "model.h"
#include "EnemyBall.h"
#include "fade.h"

//---マクロ---

#define ENEMY_BASE_SPEED (0.2f)

//グローバル変数
WAVE g_Wave;

void InitWave(void)
{
	g_Wave.m_nCnt = 0;
	for (int nCnt = 0; nCnt < WAVE_NUM; nCnt++)
	{
		g_Wave.m_Wave[nCnt] = false;
		g_Wave.m_Clear[nCnt] = false;
	}
}

void UpdateWave(void)
{
	CSpawn *pSpawn = GetSpawnPoint();
	D3DXVECTOR3 pSpawnPos01 = (pSpawn + 0)->GetPos();
	D3DXVECTOR3 pSpawnPos02 = (pSpawn + 1)->GetPos();
	D3DXVECTOR3 pSpawnPos03 = (pSpawn + 2)->GetPos();
	D3DXVECTOR3 pSpawnPos04 = (pSpawn + 3)->GetPos();
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ENEMYBALL *pEB = Get_EB(0);


	//Wave01
	if(g_Wave.m_Wave[0] == false)
	{
		SetEB(&pSpawnPos01, &rot, ENEMY_BASE_SPEED+0.06f);
		SetEB(&pSpawnPos01, &rot, ENEMY_BASE_SPEED + 0.07f);
		SetEB(&pSpawnPos01, &rot, ENEMY_BASE_SPEED + 0.08f);
		SetEB(&pSpawnPos02, &rot, ENEMY_BASE_SPEED + 0.09f);

		g_Wave.m_Wave[0] = true;
		return;
	}

	else
	{
		if(g_Wave.m_Clear[0] == false)
		{
			g_Wave.m_Clear[0] = ClearFlag();
			return;
		}
	}

	//Wave02
	if (g_Wave.m_Wave[1] == false)
	{
		SetEB(&pSpawnPos01, &rot, ENEMY_BASE_SPEED+0.06f);
		SetEB(&pSpawnPos02, &rot, ENEMY_BASE_SPEED+0.06f);
		SetEB(&pSpawnPos03, &rot, ENEMY_BASE_SPEED + 0.07f);
		SetEB(&pSpawnPos04, &rot, ENEMY_BASE_SPEED + 0.08f);

		g_Wave.m_Wave[1] = true;
		return;
	}

	else
	{
		if (g_Wave.m_Clear[1] == false)
		{
			g_Wave.m_Clear[1] = ClearFlag();
			return;
		}
	}

	if (g_Wave.m_Wave[2] == false)
	{
		SetEB(&pSpawnPos01, &rot, ENEMY_BASE_SPEED + 0.07f);
		SetEB(&pSpawnPos02, &rot, ENEMY_BASE_SPEED + 0.07f);
		SetEB(&pSpawnPos03, &rot, ENEMY_BASE_SPEED + 0.07f);
		SetEB(&pSpawnPos04, &rot, ENEMY_BASE_SPEED + 0.07f);

		SetEB(&pSpawnPos01, &rot, ENEMY_BASE_SPEED + 0.09f);
		SetEB(&pSpawnPos02, &rot, ENEMY_BASE_SPEED + 0.09f);
		SetEB(&pSpawnPos03, &rot, ENEMY_BASE_SPEED + 0.09f);
		SetEB(&pSpawnPos04, &rot, ENEMY_BASE_SPEED + 0.09f);

		g_Wave.m_Wave[2] = true;
		return;
	}

	else
	{
		if (g_Wave.m_Clear[2] == false)
		{
			g_Wave.m_Clear[2] = ClearFlag();
			return;
		}
	}

	if (g_Wave.m_Wave[3] == false)
	{
		SetEB(&pSpawnPos01, &rot, ENEMY_BASE_SPEED + 0.1f);
		SetEB(&pSpawnPos02, &rot, ENEMY_BASE_SPEED + 0.1f);
		SetEB(&pSpawnPos03, &rot, ENEMY_BASE_SPEED + 0.1f);
		SetEB(&pSpawnPos04, &rot, ENEMY_BASE_SPEED + 0.1f);

		SetEB(&pSpawnPos01, &rot, ENEMY_BASE_SPEED + 0.1f);
		SetEB(&pSpawnPos02, &rot, ENEMY_BASE_SPEED + 0.1f);
		SetEB(&pSpawnPos03, &rot, ENEMY_BASE_SPEED + 0.1f);
		SetEB(&pSpawnPos04, &rot, ENEMY_BASE_SPEED + 0.1f);

		g_Wave.m_Wave[3] = true;
		return;
	}

	else
	{
		if (g_Wave.m_Clear[3] == false)
		{
			g_Wave.m_Clear[3] = ClearFlag();
			return;
		}
	}

	SetFade(FADE_OUT, MODE_RESULT);
}

bool ClearFlag(void)
{
	ENEMYBALL *pEB = Get_EB(0);
	bool clear = true;

	for(int nCnt = 0; nCnt < ENEMYBALL_MAX;nCnt++)
	{
		if( (pEB+ nCnt)->bUse == true )
		{
			clear = false;
			break;
		}
	}

	return clear;
}