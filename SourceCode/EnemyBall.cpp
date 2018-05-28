//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "Enemyball.h"
#include "camera.h"
#include "TpsCamera.h"
#include "textureManager.h"
#include "matrix.h"
#include "hitCircle.h"
#include "hitCircleMesh.h"
#include "model_aircraft.h"
#include "vector.h"
#include "CScore.h"
#include "CParticalSetting.h"

#define ENEMYBALL_PASS "data/zombie.x"
#define ENEMY_SPEED (0.1f)
#define ENEMY_FICTION (0.1f)
#define ROT_SPEED (0.05f*PI)                    //��]���x
//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
ENEMYBALL g_EB[ENEMYBALL_MAX];                    //�G�̔z��

LPD3DXMESH g_pMeshEB;                               //���b�V���C���^�[�t�F�[�X�|�C���^
LPD3DXBUFFER g_pBuffMatEB;                          //�}�e���A�����o�b�t�@(D3DX�ėp�o�b�t�@�C���^�[�t�F�[�X)
DWORD g_nNumMatEB;                                  //�}�e���A���̐�
//int g_TexNUMEB[NUM_EB_MATERIAL];                    //�e�N�X�`�����s�ԍ�

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitEB(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	g_nNumMatEB = NUM_EB_MATERIAL;                               //�}�e���A���̐�

	//���f�����[���h�ϊ��}�g���N�X�̏�����
	for(int nCnt = 0; nCnt < ENEMYBALL_MAX; nCnt++)
	{
		g_EB[nCnt].m_Matrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EB[nCnt].m_Matrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EB[nCnt].m_Matrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_EB[nCnt].m_DirectVec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		g_EB[nCnt].m_fSpeedY = 0.0f;
		g_EB[nCnt].m_nLife = 10;
		g_EB[nCnt].m_nID = -1;
		g_EB[nCnt].bUse = false;
		g_EB[nCnt].DamageEfTime = 0;
	}

	//���f���f�[�^�̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(ENEMYBALL_PASS, D3DXMESH_MANAGED/*�w��������*/, pD3DDevice/*�f�o�C�X*/, NULL/*�אڃo�b�t�@*/, &g_pBuffMatEB/*�}�e���A�������i�[����*/, NULL, &g_nNumMatEB/*�}�e���A����*/, &g_pMeshEB/*���b�V��*/)))
	{
		MessageBox(NULL, "���f��MODEL_NAME�̓ǂݍ��݂����s����", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatEB->GetBufferPointer();   //�}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾

	//�e�N�X�`���ǂݍ���
	for(int nCnt = 0; nCnt < g_nNumMatEB; nCnt++)
	{
		//SetTexture(&g_TexNUMEB[nCnt], pMat[nCnt].pTextureFilename);
	}

	return S_OK;
}

//���f���̍X�V
void UpdateEB(void)
{
	CScore *pCScore = GetScore();

	for (int i = 0; i < ENEMYBALL_MAX; i++)
	{
		if (g_EB[i].bUse == true)
		{
			if (g_EB[i].m_nLife > 0)
			{
				EBRotate(g_EB[i].m_nID);          //���f����]
				EBMove(g_EB[i].m_nID);            //���f���ړ�

				g_EB[i].DamageEfTime--;
				if(g_EB[i].DamageEfTime < 0)
				{
					g_EB[i].DamageEfTime = 0;
				}
			}

			else
			{
				for(int nCnt = 0;nCnt < HITCIRCLE_MAX;nCnt++)
				{
					HITCIRCLE *pHitCircle = Get_HitCircle(nCnt);

					if (pHitCircle->bUse == true)
					{
						if (pHitCircle->m_type == TYPE_ENEMY)
						{
							if (pHitCircle->nEnemyID == g_EB[i].m_nID)
							{
								pHitCircle->bUse = false;
							}
						}
					}
				}

				CExplosion::Create(&g_EB[i].m_Matrix.pos,25,&D3DXVECTOR3(170,4,3));
				g_EB[i].bUse = false;
				pCScore->AddScore(300);
			}
		}
	}
}

//���f����]
void EBRotate(int EnemyID) 
{
	AIRCRAFT *pAircraft = Get_Aircraft();
	D3DXVECTOR2 DirectVec;
	DirectVec.x = pAircraft->m_Matrix.pos.x - g_EB[EnemyID].m_Matrix.pos.x;
	DirectVec.y = pAircraft->m_Matrix.pos.z - g_EB[EnemyID].m_Matrix.pos.z;

	D3DXVECTOR2 DefaultVec;
	DefaultVec.x = g_EB[EnemyID].m_DirectVec.x;
	DefaultVec.y = g_EB[EnemyID].m_DirectVec.z;

	g_EB[EnemyID].m_fTargetAngle = Angle_TwoVector(DefaultVec, DirectVec);

	if (g_EB[EnemyID].m_Matrix.rot.y > PI) { g_EB[EnemyID].m_Matrix.rot.y -= 2 * PI; }
	if (g_EB[EnemyID].m_Matrix.rot.y < -PI) { g_EB[EnemyID].m_Matrix.rot.y += 2 * PI; }

	RotToTargetAngle(&g_EB[EnemyID].m_Matrix.rot.y, -g_EB[EnemyID].m_fTargetAngle + PI, ROT_SPEED);
}
//���f���ړ�
void EBMove(int EnemyID)
{
	AIRCRAFT *pAircraft = Get_Aircraft();
	
	g_EB[EnemyID].m_SpeedVec = pAircraft->m_Matrix.pos - g_EB[EnemyID].m_Matrix.pos;
	D3DXVECTOR2 MoveSpeed = D3DXVECTOR2(g_EB[EnemyID].m_SpeedVec.x, g_EB[EnemyID].m_SpeedVec.z);
	g_EB[EnemyID].m_fSpeedY -= GRAVITY;
	D3DXVec2Normalize(&MoveSpeed, &MoveSpeed);

	//���W���Z
	g_EB[EnemyID].m_posOld = g_EB[EnemyID].m_Matrix.pos;
	g_EB[EnemyID].m_Matrix.pos.x += g_EB[EnemyID].m_fSpeed*MoveSpeed.x;
	g_EB[EnemyID].m_Matrix.pos.y += g_EB[EnemyID].m_fSpeedY;
	g_EB[EnemyID].m_Matrix.pos.z += g_EB[EnemyID].m_fSpeed*MoveSpeed.y;

	//���蔻��v�Z
	//
	//���蔻��(��)
	////////////////////////////////////////////////////
	bool Check = false;
	bool Damage = false;
	int nCntHit = 0;
	D3DXVECTOR3 Start, End;                             //���蔻��p
	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		HITCIRCLE *pHitCircle = Get_HitCircle(i);
		if (pHitCircle->bUse == true)
		{
			if (pHitCircle->m_type == TYPE_ENEMY)
			{
				if (pHitCircle->nEnemyID == EnemyID)
				{
					//���b�V������
					Start = g_EB[EnemyID].m_posOld + pHitCircle->m_offset;                              //���蔻��p
					End = g_EB[EnemyID].m_Matrix.pos + pHitCircle->m_offset;                               //���蔻��p
					Check = CircleAndRect(&Start, &End, pHitCircle->m_skydome.fRadius);
					g_EB[EnemyID].m_posOld = Start - pHitCircle->m_offset;                             //�C�����old���W�Z�o
					g_EB[EnemyID].m_Matrix.pos = End - pHitCircle->m_offset;                              //�C����̍��W�Z�o

					//�v���[���[����
					for (int j = 0; j < HITCIRCLE_MAX; j++)
					{
						HITCIRCLE *pHitCirclePlayer = Get_HitCircle(j);
						if (pHitCirclePlayer->bUse == true)
						{
							if (pHitCirclePlayer->m_type == TYPE_PLAYER)
							{
								D3DXVECTOR3 fDistanceVec = pHitCirclePlayer->m_Matrix.pos - pHitCircle->m_Matrix.pos;
								float fDistance = D3DXVec3Length(&fDistanceVec);
								if (fDistance < (pHitCirclePlayer->m_skydome.fRadius + pHitCircle->m_skydome.fRadius)) pHitCirclePlayer->HitCheckType = HBTYPE_ENEMY;
							}
						}
					}

					//�v���[���[�̒e����󂯂��_�[���W����
					if (Damage == false)
					{
						if (pHitCircle->HitCheckType == HBTYPE_HANDGUN)
						{
							Damage = true;
							g_EB[EnemyID].m_nLife -= 1;
							pHitCircle->HitCheckType = HBTYPE_NULL;
							g_EB[EnemyID].DamageEfTime = 10;
						}

						if (pHitCircle->HitCheckType == HBTYPE_AK47)
						{
							Damage = true;
							g_EB[EnemyID].m_nLife -= 2;
							pHitCircle->HitCheckType = HBTYPE_NULL;
							g_EB[EnemyID].DamageEfTime = 10;
						}
					}
				}
			}
		}
	}

	if(Check == true)
	{
		g_EB[EnemyID].m_fSpeedY = 0.0f;
	}
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawEB(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	D3DMATERIAL9 matDef;
	pD3DDevice->GetMaterial(&matDef);                                                 //�f�o�C�X�̃}�e���A�����擾���A�ۑ�����

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatEB->GetBufferPointer();

	TEXTUREMANAGER *pTextureM = GetTextureManager();                                  //�e�N�X�`���}�l�[�W���[�擾

	D3DMATERIAL9 DamageMat;
	DamageMat.Diffuse.r = 255;
	DamageMat.Diffuse.g = 0;
	DamageMat.Diffuse.b = 0;
	DamageMat.Diffuse.a = 50;
	DamageMat.Ambient = DamageMat.Diffuse;
	DamageMat.Specular.r = 0;
	DamageMat.Specular.g = 0;
	DamageMat.Specular.b = 0;
	DamageMat.Specular.a = 0;
	DamageMat.Emissive = DamageMat.Specular;
	DamageMat.Power = 1.0f;

	for (int i = 0; i < ENEMYBALL_MAX; i++)
	{
		if (g_EB[i].bUse == true)
		{
			SetMatrix(&g_EB[i].m_Mtxworld, g_EB[i].m_Matrix);                                            //�}�g���N�X�̌v�Z

			for (int nCnt = 0; nCnt < (int)g_nNumMatEB; nCnt++)
			{
				if (g_EB[i].DamageEfTime > 0)
				{
					pD3DDevice->SetMaterial(&DamageMat);
				}
				else 
				{
					pD3DDevice->SetMaterial(&pMat[nCnt].MatD3D);
				}
				pD3DDevice->SetTexture(0, NULL);
				g_pMeshEB->DrawSubset(nCnt);
			}
		}
	}
	pD3DDevice->SetMaterial(&matDef);                                                //�}�e���A����߂�
}

void UninitEB(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE(g_pMeshEB);                      //���b�V���C���^�[�t�F�[�X�|�C���^�����

	//���_�o�b�t�@���������
	SAFE_RELEASE(g_pBuffMatEB);                       //�}�e���A�����o�b�t�@�����
}

ENEMYBALL *Get_EB(int nID)
{
	return &g_EB[nID];
}

void SetEB(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pRot, float fSpeed)
{
	for (int i = 0; i < ENEMYBALL_MAX; i++)
	{
		if (g_EB[i].bUse == false)
		{
			g_EB[i].m_Matrix.pos = *pPos;
			g_EB[i].m_Matrix.rot = *pRot;
			g_EB[i].m_Matrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_EB[i].m_posOld = g_EB[i].m_Matrix.pos;
			g_EB[i].m_fSpeed = fSpeed;
			g_EB[i].m_fSpeedY = 0.0f;
			g_EB[i].m_SpeedVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_EB[i].DamageEfTime = 0;
			g_EB[i].m_nID = i;
			g_EB[i].m_nLife = 10;
			SetHitCircleEnemy(&g_EB[i].m_Matrix.pos,2.0f, &D3DXVECTOR3(0.0f,2.0f,0.0f),g_EB[i].m_nID);                                //���蔻��~�̃Z�b�g
			SetHitCircleEnemy(&g_EB[i].m_Matrix.pos, 2.0f, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_EB[i].m_nID);                                //���蔻��~�̃Z�b�g
			SetHitCircleEnemy(&g_EB[i].m_Matrix.pos, 2.0f, &D3DXVECTOR3(0.0f, -2.0f, 0.0f), g_EB[i].m_nID);                                //���蔻��~�̃Z�b�g
			g_EB[i].bUse = true;
			break;
		}
	}
}