#include "CParticalSetting.h"
#include "particalManager.h"

#define SPAWN_PARTICAL_RADIUS (1.0f)
#define BLOOD_PARTICAL_RADIUS (0.5f)
#define EXPLOSION_PNUM (120)
/********************************************CPSetting‚Ì•”•ª**********************************************/
CPSetting::CPSetting()
{
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nFrame = 0;
	m_bUse = false;
}

CPSetting::~CPSetting()
{
	
}

void CPSetting::Init(const D3DXVECTOR3 *pos)
{
	m_pos = *pos;
	m_nFrame = 0;
	m_bUse = false;
}

void CPSetting::SetUse(bool bUse)
{
	m_bUse = bUse;
}

bool CPSetting::GetUse(void)
{
	return m_bUse;
}

/********************************************************************************************************/

/********************************************CSpawn‚Ì•”•ª**********************************************/
CSpawn::CSpawn()
{
	m_fRadius = 0;
	m_fSpeedY = 0;
	m_HorizonNum = 0;
}

CSpawn::~CSpawn()
{
	
}

void CSpawn::Init(const D3DXVECTOR3 *pos, float fRadius, float nSpeedY, int HorizonNum, const D3DXVECTOR3 *pColor)
{
	CPSetting::Init(pos);
	m_fRadius = fRadius;
	m_fSpeedY = nSpeedY;
	m_HorizonNum = HorizonNum;
	m_Color = *pColor;
}
void CSpawn::Update(void)
{
	float fPieceAngle = 2 * PI / m_HorizonNum;

	if (m_nFrame % 20 == 0) 
	{

		for (int nCnt = 0; nCnt < m_HorizonNum; nCnt++)
		{
			D3DXVECTOR3 ParticalPos = D3DXVECTOR3(m_fRadius*cos(fPieceAngle*nCnt) + m_pos.x, m_pos.y, m_fRadius*sin(fPieceAngle*nCnt) + m_pos.z);
			D3DXVECTOR3 ParticalSpeed = D3DXVECTOR3(0.0f, m_fSpeedY, 0.0f);
			CParticalManager::SetPartical(&ParticalPos, &ParticalSpeed, SPAWN_PARTICAL_RADIUS, 60, 0, CPartical::PARTICAL_NORMAL, &m_Color, 0);
			CParticalManager::SetParticalEF(&ParticalPos, &ParticalSpeed, SPAWN_PARTICAL_RADIUS * 2, 60, 1, CPartical::PARTICAL_NORMAL, &m_Color, 0);
		}
	}
	m_nFrame++;

	if (m_nFrame >= 10000)
	{
		m_nFrame = 0;
	}
}

D3DXVECTOR3 CSpawn::GetPos(void)
{
	return m_pos;
}
/********************************************************************************************************/

/********************************************CSpawn‚Ì•”•ª**********************************************/
CExplosion::CExplosion()
{
	
}

CExplosion::~CExplosion()
{
	
}

void CExplosion::Create(const D3DXVECTOR3 *pos, int nDistanceMax,const D3DXVECTOR3 *pColor)
{
	for(int nCnt = 0; nCnt < EXPLOSION_PNUM; nCnt++)
	{
		float fAngleHorizon = rand() % 628;
		fAngleHorizon = fAngleHorizon / 100.0f;                                                               //ƒ‰ƒ“ƒ_ƒ€Šp“x‚ÌŒvŽZ
		float fVertical = rand() % 628;
		fVertical = fVertical / 100.0f;
		float fSpeed = rand() % (int)nDistanceMax;
		fSpeed = fSpeed / 100.0f;
		D3DXVECTOR3 ParticalSpeed;
		ParticalSpeed.x = fSpeed*cos(fAngleHorizon)*cos(fVertical);
		ParticalSpeed.z = fSpeed*sin(fAngleHorizon)*cos(fVertical);
		ParticalSpeed.y = fSpeed*sin(fVertical);
		
		CParticalManager::SetPartical(pos, &ParticalSpeed, BLOOD_PARTICAL_RADIUS, 60, 0, CPartical::PARTICAL_GRAVITY, pColor, GRAVITY);
		CParticalManager::SetParticalEF(pos, &ParticalSpeed, BLOOD_PARTICAL_RADIUS * 2, 60, 1, CPartical::PARTICAL_GRAVITY, pColor, GRAVITY);
	}
}

/*****************************************************************************************************/