#ifndef _AIRCRAFT_AIRCRAFT_H_
#define _AIRCRAFT_AIRCRAFT_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "motion.h"

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define NUM_AIRCRAFTVERTEX (4)                        //���_��
#define NUM_AIRCRAFTPOLYGON (2)                       //�|���S����

#define STAMINA_MAX (2000)                             //�̗̓Q�[�W�ő�l
#define STAMINA_MIN (50)

#define LIFE_MAX (5)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define Aircraft_NAME "data/player.x"

//-----------------------------------------------------------------------------------------
//�\���̕ϐ�
//-----------------------------------------------------------------------------------------
typedef enum
{
	GUNTYPE_HANDGUN = 0,
	GUNTYPE_AK47,
	GUNTYPE_GRENADE,
	GUNTYPE_MAX
}GUNTYPE;

typedef struct
{
	D3DXMATRIX m_MtxWorld;                                   // ���[���h�}�g���N�X
	WORLD_MATRIX m_Matrix;                           //���[���h�}�g���N�X�̕ϊ��s��
	D3DXVECTOR3 m_AirPosOld;                                //���蔻��p�Â����W
	D3DXVECTOR3 m_Speed;                                //���x
	D3DXVECTOR3 m_ModelVec;                            //���f���̌����x�N�g��
	D3DXVECTOR3 m_GunPos;
	D3DXVECTOR3 m_ShootPoint;
	float m_fTargetAngle;                                   //��]�ڕW�̊p�x
	float m_fTargetAngleV;                                   //�����ڕW�̊p�x
	int m_nActionFrame;                                   //�A�N�V�����̃t���[��
	bool m_bModeChange;                                   //���[�h�`�F���W�t���O
	bool bJump;                                           //�W�����v�t���O
	bool m_bmove;
	int m_nIDHR;                                           //�g�p���Ă��郁�b�V�����蔻���ID
	int m_nIDHC;                                           //�g�p���Ă��铖�蔻��~��ID
	int m_nLife;
	int m_NoDamageTime;
	int m_nGrenadeThrowTime;
	int m_nGrenadeCreateTime;
	bool m_bGrenadeCreate;
	float m_fStamina;                                        //�X�^�~�i�̒l
	bool m_StaminaOver;

	int m_ReloadTimeHandGun;
	int m_ReloadTimeAK47;
	int m_HandGunNum;
	int m_AK47Num;
	int m_AK47Store;
	int m_GrenadeNum;
	int m_GrenadeStore;
	bool m_bReloadHandGun;
	bool m_bReloadAK47;
	GUNTYPE m_GunType;
	GUNTYPE m_GunTypeOld;

	CModel::CHAR_MODE m_MotionNow;
	CModel::CHAR_MODE m_MotionBefore;
}AIRCRAFT;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾

//-----------------------------------------------------------------------------------------
HRESULT InitAircraft(void);                                                     //�|���S��(���_)�̏�����
void DrawAircraft(void);                                                        //�|���S���̕`��
void UpdateAircraft(void);                                                      //�|���S���̍X�V
void UninitAircraft(void);                                                      //�|���S���̏I��

AIRCRAFT *Get_Aircraft(void);    //�퓬�@�̃f�[�^���擾

#endif