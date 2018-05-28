//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "inputMouse.h"
#include "model_aircraft.h"
#include "camera.h"
#include "TpsCamera.h"
#include "textureManager.h"
#include "bullet.h"
#include "MeshField.h"
#include "vector.h"
#include "sound.h"
#include "hitcheckMesh.h"
#include "hitCircleMesh.h"
#include "tpsAim.h"
#include "matrix.h"
#include "hitCircle.h"
#include "fade.h"
#include "CGrenadeTrack.h"
#include "particalManager.h"
#include "CGrenade.h"

#define NUM_MATERIAL (0)
#define AIRCRAFT_SPEED (0.05f)                //�퓬�@���x
#define ROT_SPEED (0.05f*PI)                    //�퓬�@��]���x
#define AIR_FRICTION (0.1f)                    //��C���C�W��(�X�s�[�h�����p)

#define HANDGUNBULLET_MAX (15)
#define AK47BULLET_MAX (30)
#define GRENADE_MAX (8)
#define GRENADE_THROWTIME (50)
#define AK47_STORE (90)
#define AK47_RELOAD (120.0f)
#define HANDGUN_RELOAD (60.0f)

#define GRENADE_TO_EXPLOSION_TIME (120)

#define TRACK_FRAME (5)

#define POWER_TO_THROW_GRENADE (1.5f)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
void AircraftScale(void);                              //���f���̊g��
void AircraftRotate(void);                             //���f���̉�]
void AircraftMove(void);                               //���f���̈ړ�
void DamageCheck(void);
void Shoot(void);              //�o���b�g����
void ReloadAction(void);
void PlayerInputAction(void);

//-----------------------------------------------------------------------------------------
//�\���̕ϐ�
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
//int g_AircraftTextureNUM[NUM_MATERIAL];                       //�e�N�X�`�����s�ԍ�
LPD3DXMESH g_pMeshAircraft;                               //���b�V���C���^�[�t�F�[�X�|�C���^
LPD3DXBUFFER g_pBuffMatAircraft;                          //�}�e���A�����o�b�t�@(D3DX�ėp�o�b�t�@�C���^�[�t�F�[�X)
DWORD g_nNumMatAircraft;                                  //�}�e���A���̐�

AIRCRAFT g_Aircraft;                            //�퓬�@�̍\����

CModel g_CModel;

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitAircraft(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();
	CAMERA_FPS *pCameraFps = GetCameraTps();            //�J�����擾
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //�J���������_�擾

	g_nNumMatAircraft = NUM_MATERIAL;                               //�}�e���A���̐�

	//���f�����[���h�ϊ��}�g���N�X�̏�����
	g_Aircraft.m_Matrix.scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	g_Aircraft.m_Matrix.rot = D3DXVECTOR3(0.0f, -pCameraFps->fAngleHorizon + PI, 0.0f);
	g_Aircraft.m_Matrix.pos = D3DXVECTOR3(0.0f, -22.0f, 100.0f);
	g_Aircraft.m_AirPosOld = g_Aircraft.m_Matrix.pos;
	g_Aircraft.m_Speed = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_Aircraft.m_ModelVec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);                          //���f���̌��������x�N�g��
	g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + PI;
	g_Aircraft.m_nActionFrame = 0;
	g_Aircraft.m_nIDHR = -1;
	g_Aircraft.m_nIDHC = -1;
	g_Aircraft.m_bModeChange = false;
	g_Aircraft.bJump = false;
	g_Aircraft.m_bmove = false;
	g_Aircraft.m_nLife = LIFE_MAX;
	g_Aircraft.m_NoDamageTime = 0;                                               //���G����
	g_Aircraft.m_fStamina = STAMINA_MAX;                                         //�̗̓Q�[�W�ő�l
	g_Aircraft.m_StaminaOver = false;

	g_Aircraft.m_GunType = GUNTYPE_HANDGUN;
	g_Aircraft.m_HandGunNum = 15;
	g_Aircraft.m_AK47Num = AK47BULLET_MAX;
	g_Aircraft.m_AK47Store = AK47_STORE;
	g_Aircraft.m_GrenadeNum = 1;
	g_Aircraft.m_GrenadeStore = 8;
	g_Aircraft.m_ReloadTimeHandGun = 0;
	g_Aircraft.m_nGrenadeThrowTime = 0;
	g_Aircraft.m_nGrenadeCreateTime = 0;
	g_Aircraft.m_bGrenadeCreate = false;
	g_Aircraft.m_ReloadTimeAK47 = 0;
	g_Aircraft.m_bReloadAK47 = false;
	g_Aircraft.m_bReloadHandGun = false;

	g_Aircraft.m_MotionNow = CModel::MOTION_NORMAL;
	g_Aircraft.m_MotionBefore = CModel::MOTION_NORMAL;

	SetHitCirclePlayer(&g_Aircraft.m_Matrix.pos,2.0f, &D3DXVECTOR3(0.0f, 2.0f,0.0f));                 //���蔻��~�̐ݒu
	SetHitCirclePlayer(&g_Aircraft.m_Matrix.pos, 2.0f, &D3DXVECTOR3(0.0f, 0.0f, 0.0f));                 //���蔻��~�̐ݒu
	SetHitCirclePlayer(&g_Aircraft.m_Matrix.pos, 2.0f, &D3DXVECTOR3(0.0f, -2.0f, 0.0f));                 //���蔻��~�̐ݒu

	g_CModel.Init("data/MODEL/motion.txt");                                                    //���[�V�����t�@�C���̓ǂݍ���

	return S_OK;
}

//���f���̍X�V
void UpdateAircraft(void)
{
	//�v���[���[�����Ă��鎞
	if (g_Aircraft.m_nLife > 0)
	{
		if (g_Aircraft.m_nGrenadeThrowTime <= 0)
		{
			AircraftRotate();          //���f����]
			AircraftMove();            //���f���ړ�
		}
		DamageCheck();             //�G�Ƃ̓��蔻��`�F�b�N
		Shoot();                   //�o���b�g����
		ReloadAction();
		PlayerInputAction();       //���͂ɂ���ă��[�h���ς��

		g_CModel.Update();

		CGrenade::UpdateAll();
	}

	//�v���[���[���S
	else
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

#ifdef _DEBUG
	if(GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}
#endif
}

//���f���g��
void AircraftScale(void)
{
	
}
//���f����]
void AircraftRotate(void) 
{
	CAMERA_FPS *pCameraFps = GetCameraTps();            //�J�����擾
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //�J���������_�擾
	TPS_SUBANGLE *pBefore = GetBeforeAngle();

	// -180�`180�x�Ɋۂ߂�
	if (g_Aircraft.m_Matrix.rot.y > PI) { g_Aircraft.m_Matrix.rot.y -= 2 * PI; }
	if (g_Aircraft.m_Matrix.rot.y < -PI) { g_Aircraft.m_Matrix.rot.y += 2 * PI; }
	
	if ( (g_Aircraft.m_MotionNow == CModel::MOTION_SHOOT) || (g_Aircraft.m_MotionNow == CModel::MOTION_GUNMOVE))
	{
		//
		//���f���̌����v�Z
		////////////////////////////////////
		//���C�̌����_������
		RayCheck(&g_Aircraft.m_ShootPoint, &pCameraFpsLookAt->pos, &pCameraFps->pos, &g_Aircraft.m_Matrix.pos);              //���C�ƃ��b�V���̌����_������
		D3DXVECTOR3 RayVec = g_Aircraft.m_ShootPoint - g_Aircraft.m_Matrix.pos;            //�����x�N�g��������
		D3DXVec3Normalize(&RayVec, &RayVec);                            //�P�ʃx�N�g����
																		//��]�ڕW�p�x�����߂�
		D3DXVECTOR2 RayVec2 = D3DXVECTOR2(RayVec.x, RayVec.z);
		D3DXVECTOR2 ModelVec2 = D3DXVECTOR2(g_Aircraft.m_ModelVec.x, g_Aircraft.m_ModelVec.z);
		g_Aircraft.m_fTargetAngle = Angle_TwoVector(ModelVec2, RayVec2);
		g_Aircraft.m_fTargetAngleV = asin(RayVec.y);                               //Y���̖ڕW�p�x�����߂�

		RotToTargetAngle(&g_Aircraft.m_Matrix.rot.y, -g_Aircraft.m_fTargetAngle + PI, ROT_SPEED);
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + PI;
	}

	else if ((g_Aircraft.m_MotionNow == CModel::MOTION_GSHOOT) || (g_Aircraft.m_MotionNow == CModel::MOTION_GGUNMOVE))
	{
		RotToTargetAngle(&g_Aircraft.m_Matrix.rot.y, -pCameraFps->fAngleHorizon +0.5* PI, ROT_SPEED);
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + PI;
	}

	else
	{
		RotToTargetAngle(&g_Aircraft.m_Matrix.rot.y, g_Aircraft.m_fTargetAngle - 0.5*PI, ROT_SPEED);
	}
}

//���f���ړ�
void AircraftMove(void)
{
	//�J�����擾
	CAMERA_FPS *pCameraFps = GetCameraTps();            //�J�����擾
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //�J���������_�擾
	int nID_Air = -1;                     //���蔻�肪���݂���l�p�|���S���̔ԍ���ۑ�
	bool bMoveFlag = false;

	//���b�V���t�B�[���h�擾
	MESHWALL *pMeshwall = GetMesh();

	g_Aircraft.m_AirPosOld = g_Aircraft.m_Matrix.pos;       //���ݍ��W�ۑ�

	//
	//�ړ�����
	////////////////////////////////////
	if (GetKeyboardPress(DIK_A))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + 0.5*PI;
		bMoveFlag = true;
	}
	//�J�����E�ֈڍs�ړ�
	if (GetKeyboardPress(DIK_D))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon - 0.5*PI;
		bMoveFlag = true;
	}
	//�J�����O�ֈڍs�ړ�
	if (GetKeyboardPress(DIK_W))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + PI;
		bMoveFlag = true;
	}
	//�J�������ֈڍs�ړ�
	if (GetKeyboardPress(DIK_S))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon;
		bMoveFlag = true;
	}

	//����
	if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_A))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + 0.75*PI;
		bMoveFlag = true;
	}

	//�E��
	if(GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_D))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon - 0.75*PI;
		bMoveFlag = true;
	}

	//����
	if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_A))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon + 0.25*PI;
		bMoveFlag = true;
	}

	//�E��
	if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_D))
	{
		g_Aircraft.m_fTargetAngle = -pCameraFps->fAngleHorizon - 0.25*PI;
		bMoveFlag = true;
	}

#ifdef _DEBUG
	//test�p
	if(GetKeyboardPress(DIK_Q))
	{
		g_Aircraft.m_Matrix.pos.y += 0.3f;
	}

	if (GetKeyboardPress(DIK_E))
	{
		g_Aircraft.m_Matrix.pos.y -= 0.3f;
	}
#endif
	
	//�X�s�[�h����
	if (bMoveFlag == true)
	{
		g_Aircraft.m_Speed.x += AIRCRAFT_SPEED*cos(g_Aircraft.m_fTargetAngle);
		g_Aircraft.m_Speed.z -= AIRCRAFT_SPEED*sin(g_Aircraft.m_fTargetAngle);

		if( (g_Aircraft.m_MotionNow == CModel::MOTION_RUN) || (g_Aircraft.m_MotionNow == CModel::MOTION_GRUN))
		{
			g_Aircraft.m_Speed.x += AIRCRAFT_SPEED*cos(g_Aircraft.m_fTargetAngle);
			g_Aircraft.m_Speed.z -= AIRCRAFT_SPEED*sin(g_Aircraft.m_fTargetAngle);
			g_Aircraft.m_fStamina -= 30;
		}
	}

	g_Aircraft.m_bmove = bMoveFlag;

	//�d�͂ɂ��Y���X�s�[�h�̑���
	g_Aircraft.m_Speed.y -= GRAVITY;

	//��C���C�ɂ���ăX�s�[�h����
	g_Aircraft.m_Speed.x += (0 - g_Aircraft.m_Speed.x)*AIR_FRICTION;
	g_Aircraft.m_Speed.z += (0 - g_Aircraft.m_Speed.z)*AIR_FRICTION;

	// -180�`180�x�Ɋۂ߂�
	if (g_Aircraft.m_fTargetAngle > PI) { g_Aircraft.m_fTargetAngle -= 2 * PI; }
	if (g_Aircraft.m_fTargetAngle < -PI) { g_Aircraft.m_fTargetAngle += 2 * PI; }

	//�ʒu�ω�
	g_Aircraft.m_Matrix.pos.x += g_Aircraft.m_Speed.x;
	g_Aircraft.m_Matrix.pos.y += g_Aircraft.m_Speed.y;
	g_Aircraft.m_Matrix.pos.z += g_Aircraft.m_Speed.z;

	g_Aircraft.m_GunPos = g_Aircraft.m_Matrix.pos;
	g_Aircraft.m_GunPos.y += 2.0f;

	//
	//���蔻��(��)
	////////////////////////////////////////////////////
	bool Check = false;
	bool Damage = false;
	D3DXVECTOR3 Start,End;                             //���蔻��p
	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		HITCIRCLE *pHitCircle = Get_HitCircle(i);
		if (pHitCircle->bUse == true)
		{
			if (pHitCircle->m_type == TYPE_PLAYER)
			{
				Start = g_Aircraft.m_AirPosOld + pHitCircle->m_offset;                              //���蔻��p
				End = g_Aircraft.m_Matrix.pos + pHitCircle->m_offset;                               //���蔻��p
				Check = CircleAndRect(&Start, &End, pHitCircle->m_skydome.fRadius);
				g_Aircraft.m_AirPosOld = Start - pHitCircle->m_offset;                             //�C�����old���W�Z�o
				g_Aircraft.m_Matrix.pos = End - pHitCircle->m_offset;                              //�C����̍��W�Z�o

				//���G���ԊO
				if (g_Aircraft.m_NoDamageTime <= 0)
				{
					if (Damage == false)
					{
						if (pHitCircle->HitCheckType == HBTYPE_ENEMY)
						{
							Damage = true;
							pHitCircle->HitCheckType = HBTYPE_NULL;
						}
					}

					else
					{
						pHitCircle->HitCheckType = HBTYPE_NULL;
					}
				}
				//���G���Ԓ�
				else
				{
					pHitCircle->HitCheckType = HBTYPE_NULL;
				}
			}
		}
	}

	if (Check == true)
	{
		g_Aircraft.bJump = false;
		g_Aircraft.m_Speed.y = 0;
	}

	if(Damage == true)
	{
		g_Aircraft.m_nLife--;
		g_Aircraft.m_NoDamageTime = 120;
	}

	if ((g_Aircraft.m_MotionNow != CModel::MOTION_RUN) && (g_Aircraft.m_MotionNow != CModel::MOTION_GRUN) )
	{
		g_Aircraft.m_fStamina += 15;
		if (g_Aircraft.m_fStamina > STAMINA_MAX)
		{
			g_Aircraft.m_fStamina = STAMINA_MAX;
		}
	}
	if(g_Aircraft.m_fStamina < 0)
	{
		g_Aircraft.m_fStamina = 0;
	}
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawAircraft(void)
{
	SetMatrix(&g_Aircraft.m_MtxWorld, g_Aircraft.m_Matrix);    //�}�g���N�X�̌v�Z

	g_CModel.Draw();                                       //���f���`��

	CGrenade::DrawAll();
}

void UninitAircraft(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE (g_pMeshAircraft);                      //���b�V���C���^�[�t�F�[�X�|�C���^�����

	//���_�o�b�t�@���������
	SAFE_RELEASE (g_pBuffMatAircraft);                       //�}�e���A�����o�b�t�@�����

	g_CModel.Uninit();

	CGrenade::UninitAll();                                //�S�ẴO���l�[�h�I�u�W�F�폜
}

//�o���b�g�̔���
void Shoot(void)
{
	D3DXVECTOR3 speedVec = D3DXVECTOR3(0.0f,0.0f,0.0f);    //�e�X�s�[�h
	D3DXMATRIX rotMatrix;     //��]�x�N�g��
	CAMERA_FPS *pCameraFps = GetCameraTps();            //�J�����擾
	CAMERA_LOOKAT *pCameraFpsLookAt = GetTpsCameraLookat();   //�J���������_�擾

	//�v�Z
	speedVec = g_Aircraft.m_ShootPoint - g_Aircraft.m_GunPos;
	D3DXVec3Normalize(&speedVec, &speedVec);

	if ((g_Aircraft.m_MotionNow == CModel::MOTION_SHOOT) || (g_Aircraft.m_MotionNow == CModel::MOTION_GUNMOVE))
	{
		if (g_Aircraft.m_GunType == GUNTYPE_HANDGUN)
		{
			//���N���b�N�Ŏˌ�
			if (GetMouseTrigger(MOUSE_LEFT))
			{
				if ((g_Aircraft.m_HandGunNum > 0) && (g_Aircraft.m_ReloadTimeHandGun <= 0))
				{
					//�o���b�g�Z�b�g
					SetBullet(g_Aircraft.m_GunPos, BULLET_SPEED*speedVec,BTYPE_HANDGUN);
					g_Aircraft.m_HandGunNum--;
					PlaySound(SOUND_LABEL_SE_SHOOT);
				}
			}
		}

		if (g_Aircraft.m_GunType == GUNTYPE_AK47)
		{
			//���N���b�N�Ŏˌ�
			if (GetMouseRepeat(MOUSE_LEFT))
			{
				if ((g_Aircraft.m_AK47Num > 0) && (g_Aircraft.m_ReloadTimeAK47 <= 0))
				{
					//�o���b�g�Z�b�g
					SetBullet(g_Aircraft.m_GunPos, BULLET_SPEED*speedVec, BTYPE_AK47);
					g_Aircraft.m_AK47Num--;
					PlaySound(SOUND_LABEL_SE_SHOOT);
				}
			}
		}
	}

	static D3DXVECTOR3 speed;
	static D3DXVECTOR3 posResult;

	if ((g_Aircraft.m_MotionNow == CModel::MOTION_GSHOOT) || (g_Aircraft.m_MotionNow == CModel::MOTION_GGUNMOVE) || (g_Aircraft.m_MotionNow == CModel::MOTION_GTHROW))
	{
		if (g_Aircraft.m_GunType == GUNTYPE_GRENADE)
		{
			if ( (g_Aircraft.m_nGrenadeThrowTime <= 0) && (g_Aircraft.m_GrenadeNum > 0))
			{
				CAMERA_FPS *pCamera = GetCameraTps();
				CAMERA_LOOKAT *pLookAt = GetTpsCameraLookat();

				speed.x = POWER_TO_THROW_GRENADE*cos(pCamera->fAngleHorizon)*cos(pCamera->fAngleVertical - PI*0.2);
				speed.z = POWER_TO_THROW_GRENADE*sin(pCamera->fAngleHorizon)*cos(pCamera->fAngleVertical - PI*0.2);
				speed.y = POWER_TO_THROW_GRENADE*sin(pCamera->fAngleVertical - PI*0.2);

				speed = -speed;

				for (int nCnt = 1; nCnt < BILLBOARD_NUM; nCnt++)
				{
					//��nCnt�ڂ̓_�̈ʒu�̌v�Z(�_�Ɠ_��frame����TRACK_FRAME)
					posResult.x = g_Aircraft.m_Matrix.pos.x + speed.x * TRACK_FRAME * nCnt;
					posResult.y = speed.y * TRACK_FRAME * nCnt - 0.5 * GRAVITY * (TRACK_FRAME * nCnt) * (TRACK_FRAME * nCnt) + g_Aircraft.m_Matrix.pos.y;          //S = vt - 0.5*g*t^2;
					posResult.z = g_Aircraft.m_Matrix.pos.z + speed.z * TRACK_FRAME * nCnt;
					CParticalManager::SetPartical(&posResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.5f, 1, 0, CPartical::PARTICAL_NORMAL, &D3DXVECTOR3(150, 0, 0), 0);
					CParticalManager::SetParticalEF(&posResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.5f, 1, 0, CPartical::PARTICAL_NORMAL, &D3DXVECTOR3(150, 0, 0), 0);

					if (posResult.y < (g_Aircraft.m_Matrix.pos.y - 50.0f)) break;
				}

				if (GetMouseTrigger(MOUSE_LEFT))
				{
					g_Aircraft.m_nGrenadeThrowTime = GRENADE_THROWTIME;
					g_Aircraft.m_nGrenadeCreateTime = GRENADE_THROWTIME - 25;
					g_Aircraft.m_bGrenadeCreate = true;
				}
			}

			if (g_Aircraft.m_bGrenadeCreate == true)
			{
				if (g_Aircraft.m_nGrenadeCreateTime <= 0)
				{
					if (g_Aircraft.m_GrenadeStore > 0)
					{
						CGrenade::Create(&g_Aircraft.m_Matrix, &speed, GRAVITY, GRENADE_TO_EXPLOSION_TIME);      //�O���l�[�h�����I
						g_Aircraft.m_GrenadeNum--;
						if (g_Aircraft.m_GrenadeNum <= 0)
						{
							g_Aircraft.m_GrenadeNum++;
							g_Aircraft.m_GrenadeStore--;
						}
					}

					else
					{
						if(g_Aircraft.m_GrenadeNum  > 0)
						{
							CGrenade::Create(&g_Aircraft.m_Matrix, &speed, GRAVITY, GRENADE_TO_EXPLOSION_TIME);      //�O���l�[�h�����I
							g_Aircraft.m_GrenadeNum--;
						}
					}

					g_Aircraft.m_bGrenadeCreate = false;
				}
			}
		}
	}

	//�e�����؂��,���������[�h���[�h�ɓ���
	if( (g_Aircraft.m_HandGunNum <= 0) && (g_Aircraft.m_bReloadHandGun == false) )
	{
		g_Aircraft.m_ReloadTimeHandGun = HANDGUN_RELOAD;
		g_Aircraft.m_bReloadHandGun = true;
	}

	if ((g_Aircraft.m_AK47Num <= 0) && (g_Aircraft.m_bReloadAK47 == false))
	{
		g_Aircraft.m_ReloadTimeAK47 = AK47_RELOAD;
		g_Aircraft.m_bReloadAK47 = true;
	}

	g_Aircraft.m_nGrenadeThrowTime--;                                //�O���l�[�h�����d�����Ԍ���
	if(g_Aircraft.m_nGrenadeThrowTime < 0)
	{
		g_Aircraft.m_nGrenadeThrowTime = 0;
	}
	g_Aircraft.m_nGrenadeCreateTime--;
	if(g_Aircraft.m_nGrenadeCreateTime < 0)
	{
		g_Aircraft.m_nGrenadeCreateTime = 0;
	}
}

void DamageCheck(void)
{
	g_Aircraft.m_NoDamageTime--;

	if(g_Aircraft.m_NoDamageTime < 0)
	{
		g_Aircraft.m_NoDamageTime = 0;
	}
}

void ReloadAction(void)
{
	//�����[�h�A�N�V����
	if(GetKeyboardTrigger(DIK_R))
	{
		if(g_Aircraft.m_GunType == GUNTYPE_HANDGUN)
		{
			if((g_Aircraft.m_HandGunNum != HANDGUNBULLET_MAX) && (g_Aircraft.m_bReloadHandGun == false))
			{
				g_Aircraft.m_ReloadTimeHandGun = HANDGUN_RELOAD;
				g_Aircraft.m_bReloadHandGun = true;
			}
		}

		if (g_Aircraft.m_GunType == GUNTYPE_AK47)
		{
			if ((g_Aircraft.m_AK47Num != AK47BULLET_MAX) && (g_Aircraft.m_bReloadAK47 == false) && (g_Aircraft.m_AK47Store != 0) )
			{
				g_Aircraft.m_ReloadTimeAK47 = AK47_RELOAD;
				g_Aircraft.m_bReloadAK47 = true;
			}
		}
	}

	//�����[�h�ߒ�
	if(g_Aircraft.m_GunType == GUNTYPE_HANDGUN)
	{
		if(g_Aircraft.m_bReloadHandGun == true)
		{
			g_Aircraft.m_ReloadTimeHandGun--;

			if(g_Aircraft.m_ReloadTimeHandGun <= 0)
			{
				g_Aircraft.m_ReloadTimeHandGun = 0;
				g_Aircraft.m_HandGunNum = HANDGUNBULLET_MAX;
				g_Aircraft.m_bReloadHandGun = false;
			}
		}
	}

	if (g_Aircraft.m_GunType == GUNTYPE_AK47)
	{
		if (g_Aircraft.m_bReloadAK47 == true)
		{
			g_Aircraft.m_ReloadTimeAK47--;

			if (g_Aircraft.m_ReloadTimeAK47 <= 0)
			{
				g_Aircraft.m_ReloadTimeAK47 = 0;
				g_Aircraft.m_bReloadAK47 = false;

				if ( (g_Aircraft.m_AK47Store+ g_Aircraft.m_AK47Num) >= AK47BULLET_MAX)
				{
					int NowBulletNum = g_Aircraft.m_AK47Num;
					g_Aircraft.m_AK47Num = AK47BULLET_MAX;
					g_Aircraft.m_AK47Store -= AK47BULLET_MAX - NowBulletNum;
				}

				else
				{
					g_Aircraft.m_AK47Num = g_Aircraft.m_AK47Store + g_Aircraft.m_AK47Num;
					g_Aircraft.m_AK47Store = 0;
				}
			}
		}
	}
}

//���[�V�����t���O�Ǘ�
void PlayerInputAction(void)
{
	bool bShootModeFlag = false;
	bool bRunModeFlag = false;

	g_Aircraft.m_MotionBefore = g_Aircraft.m_MotionNow;                                  //���[�V�����ۑ�

	//
	if (g_Aircraft.m_GunType != GUNTYPE_GRENADE)
	{
		g_Aircraft.m_MotionNow = CModel::MOTION_NORMAL;                                      //�������Ȃ��Ȃ�MOTION_NORMAl�ɂ���

		if (GetMousePress(MOUSE_RIGHT))
		{
			bShootModeFlag = true;
		}

		if (bShootModeFlag == true)
		{
			g_Aircraft.m_MotionNow = CModel::MOTION_SHOOT;                                   //�V���b�g���[�V�����ɂȂ�
		}

		if (GetKeyboardTrigger(DIK_LSHIFT))
		{
			g_Aircraft.m_StaminaOver = true;
		}

		if (g_Aircraft.m_fStamina < STAMINA_MIN)
		{
			g_Aircraft.m_StaminaOver = false;
		}

		if (g_Aircraft.m_bmove == true)
		{
			if (g_Aircraft.m_MotionNow == CModel::MOTION_NORMAL) g_Aircraft.m_MotionNow = CModel::MOTION_NORMALMOVE;
			if (g_Aircraft.m_MotionNow == CModel::MOTION_SHOOT) g_Aircraft.m_MotionNow = CModel::MOTION_GUNMOVE;

			if (GetKeyboardPress(DIK_LSHIFT))
			{
				if (g_Aircraft.m_fStamina >= STAMINA_MIN)
				{
					bRunModeFlag = true;
				}
			}

			if ((bRunModeFlag == true) && (g_Aircraft.m_StaminaOver == true))
			{
				g_Aircraft.m_MotionNow = CModel::MOTION_RUN;                                   //�_�b�V�����[�V�����ɂȂ�
			}
		}
	}

	//
	else
	{
		if (g_Aircraft.m_nGrenadeThrowTime <= 0)
		{
			g_Aircraft.m_MotionNow = CModel::MOTION_GNORMAL;                                      //�������Ȃ��Ȃ�MOTION_NORMAl�ɂ���

			if (GetMousePress(MOUSE_RIGHT))
			{
				bShootModeFlag = true;
			}

			if (bShootModeFlag == true)
			{
				g_Aircraft.m_MotionNow = CModel::MOTION_GSHOOT;                                   //�V���b�g���[�V�����ɂȂ�
			}

			if (GetKeyboardTrigger(DIK_LSHIFT))
			{
				g_Aircraft.m_StaminaOver = true;
			}

			if (g_Aircraft.m_fStamina < STAMINA_MIN)
			{
				g_Aircraft.m_StaminaOver = false;
			}

			if (g_Aircraft.m_bmove == true)
			{
				if (g_Aircraft.m_MotionNow == CModel::MOTION_GNORMAL) g_Aircraft.m_MotionNow = CModel::MOITON_GNORMALMOVE;
				if (g_Aircraft.m_MotionNow == CModel::MOTION_GSHOOT) g_Aircraft.m_MotionNow = CModel::MOTION_GGUNMOVE;

				if (GetKeyboardPress(DIK_LSHIFT))
				{
					if (g_Aircraft.m_fStamina >= STAMINA_MIN)
					{
						bRunModeFlag = true;
					}
				}

				if ((bRunModeFlag == true) && (g_Aircraft.m_StaminaOver == true))
				{
					g_Aircraft.m_MotionNow = CModel::MOTION_GRUN;                                   //�_�b�V�����[�V�����ɂȂ�
				}
			}
		}

		else
		{
			g_Aircraft.m_MotionNow = CModel::MOTION_GTHROW;
		}
	}

	if (g_Aircraft.m_MotionNow != CModel::MOTION_GTHROW)
	{
		g_Aircraft.m_GunTypeOld = g_Aircraft.m_GunType;

		if (GetKeyboardTrigger(DIK_1))
		{
			g_Aircraft.m_GunType = GUNTYPE_HANDGUN;
		}

		if (GetKeyboardTrigger(DIK_2))
		{
			g_Aircraft.m_GunType = GUNTYPE_AK47;
		}

		if (GetKeyboardTrigger(DIK_3))
		{
			g_Aircraft.m_GunType = GUNTYPE_GRENADE;
		}
	}
}

AIRCRAFT *Get_Aircraft(void)
{
	return &g_Aircraft;
}