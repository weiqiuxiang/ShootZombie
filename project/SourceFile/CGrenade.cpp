#include "CGrenade.h"
#include "hitCircleMesh.h"
#include "CParticalSetting.h"
#include "EnemyBall.h"
#include "hitCircle.h"

//マクロ
#define GRENADE_MODELPASS "data/MODEL/16_Grenade.x"
#define EXPLOSION_RANGE (30)

//staticメンバ関数
CGrenade *CGrenade::m_aGrenade[GRENADE_MAX] = {};

CGrenade::CGrenade()
{
	
}
CGrenade::~CGrenade()
{
	
}

//定番関数
HRESULT CGrenade::Init(const WORLD_MATRIX *pPosRotScl , char *ModelPass, DWORD nNumMatEB, bool bUse)
{
	CObj3D::Init(pPosRotScl, ModelPass, nNumMatEB, bUse);

	return S_OK;
}

void CGrenade::Draw(void)
{
	CObj3D::Draw();
}

void CGrenade::Update(void)
{
	CObj3D::Update();

	

	m_posOld = m_PosRotScl.pos;                                //前フレームの座標保存

	m_PosRotScl.pos += m_Speed;
	m_Speed.y -= m_gravity;

	m_nCntExplosion--;
	if (m_nCntExplosion <= 0)
	{
		//EXPLOSIONセット
		CExplosion::Create(&m_PosRotScl.pos, EXPLOSION_RANGE+30, &D3DXVECTOR3(254, 206, 42));
		CGrenade::HitEnemy();
		Uninit();                     //オブジェ削除
		return;
	}

	if(m_bCollsion == true)
	{
		m_Speed.x -= m_Speed.x*0.30;
		m_Speed.z -= m_Speed.z*0.30;
	}

	else
	{
		m_PosRotScl.rot += D3DXVECTOR3(0.05f*PI, 0.0f ,0.0f);
	}

	//当り判定
	m_bCollsion = CircleAndRectRepel(&m_posOld, &m_PosRotScl.pos, 1 , 0.6,&m_Speed);


}
void CGrenade::Uninit(void)
{
	int WorkID;

	CObj3D::Uninit();
	WorkID = m_nID;
	delete m_aGrenade[m_nID];
	m_aGrenade[WorkID] = NULL;
}

//ファクション関数
void CGrenade::Create(WORLD_MATRIX *pPosRotScl, D3DXVECTOR3 *pSpeed, float gravity, int nCntExplosion)
{
	for (int nCnt = 0; nCnt < GRENADE_MAX; nCnt++)
	{
		if (m_aGrenade[nCnt] == NULL)
		{
			m_aGrenade[nCnt] = new CGrenade;
			m_aGrenade[nCnt]->Init(pPosRotScl, GRENADE_MODELPASS, 2, true);    //モデル読み込み
			m_aGrenade[nCnt]->InitGrenade(nCnt, pSpeed, gravity, nCntExplosion);                     //パラメータセット
			break;
		}
	}
}

void CGrenade::InitGrenade(const int nID, D3DXVECTOR3 *pSpeed, const float gravity, const int nCntExplosion)
{
	m_nID = nID;
	m_Speed = *pSpeed;
	m_gravity = gravity;
	m_nCntExplosion = nCntExplosion;
	m_bCollsion = false;
	m_posOld = m_PosRotScl.pos;
}

void CGrenade::UpdateAll(void)
{
	for (int nCnt = 0; nCnt < GRENADE_MAX; nCnt++)
	{
		if (m_aGrenade[nCnt] != NULL)
		{
			m_aGrenade[nCnt]->Update();
		}
	}
}

void CGrenade::DrawAll(void)
{
	for (int nCnt = 0; nCnt < GRENADE_MAX; nCnt++)
	{
		if (m_aGrenade[nCnt] != NULL)
		{
			m_aGrenade[nCnt]->Draw();
		}
	}
}

void CGrenade::UninitAll(void)
{
	for (int nCnt = 0; nCnt < GRENADE_MAX; nCnt++)
	{
		if (m_aGrenade[nCnt] != NULL)
		{
			m_aGrenade[nCnt]->Uninit();
		}
	}
}

void CGrenade::HitEnemy(void)
{
	for (int i = 0; i < ENEMYBALL_MAX; i++)
	{
		ENEMYBALL *pEnemy = Get_EB(i);

		bool bDamage = false;
		for (int j = 0; j < HITCIRCLE_MAX; j++)
		{
			HITCIRCLE *pHitCircle = Get_HitCircle(j);
			if (pHitCircle->bUse == true)
			{
				if (pHitCircle->m_type == TYPE_ENEMY)
				{
					if (pHitCircle->nEnemyID == pEnemy->m_nID)
					{
						float fDistance = D3DXVec3Length (&(m_PosRotScl.pos - pHitCircle->m_Matrix.pos));

						if(fDistance < (pHitCircle->m_skydome.fRadius + EXPLOSION_RANGE))
						{
							bDamage = true;
						}
					}
				}
			}
		}

		if(bDamage == true)
		{
			pEnemy->m_nLife -= 5;
			pEnemy->DamageEfTime = 10;
		}
	}
}