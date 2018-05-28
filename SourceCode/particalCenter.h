//-----------------------------------------------------------------------------------------
//�p�[�e�B�N���w�b�h�t�@�C��[ParticalCenter.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _PARTICALCENTER_H_
#define _PARTICALCENTER_H_

#include "main.h"

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define NUM_PARTICALCENTERVERTEX (4)                            //���_��
#define NUM_PARTICALCENTERPOLYGON (128)						    //��`��
#define NUM_PARTICALCENTERTEXTURE (2)                           //�e�N�X�`����

//�p�[�e�B�N���̕�����
#define PARTICALCENTER_WIDTH (20.0f)
#define PARTICALCENTER_HEIGHT (20.0f)

//�p�[�e�B�N���̃G�t�F�N�g�̔��a
#define MAIN_RADIUS (20.0f)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define PARTICALCENTERPOLYGON01_TEXTURENAME "data/TEXTURE/partical.png"
#define PARTICALCENTERPOLYGON02_TEXTURENAME "data/TEXTURE/particalEffect.jpg"

//-----------------------------------------------------------------------------------------
//�p�[�e�B�N���̍\����
//-----------------------------------------------------------------------------------------
//�p�[�e�B�N���̃G�t�F�N�g�I��
typedef enum
{
	EFFECTMODE_BASE = 0,
	EFFECTMODE_EXPLOSION,
	EFFECTMODE_FIRE,
	EFFECTMODE_MAX,
}EFFECTMODE;

typedef struct
{
	D3DXVECTOR3 pos;                               //�p�[�e�B�N���̃|�W�V����
	D3DXVECTOR2 speed;                             //�p�[�e�B�N���̃X�s�[�h
	D3DXVECTOR2 posOld;                            //�p�[�e�B�N���̃|�W�V����
	float fRangeAngel;                             //�p�[�e�B�N���̊g�U�p�x�̑傫��
	float fShootAngel;                             //�p�[�e�B�N���̔��ˊp�x
	float fRadius;                                 //�p�[�e�B�N���̔��a
	float fDistance;                               //�p�[�e�B�N���̔�ъ����
	bool bUse;                                     //�p�[�e�B�N�����g�����ǂ���
	EFFECTMODE EffectMode;                         //�G�t�F�N�g���[�h

	//�p�[�e�B�N�����͊�
	float fGravity;                //�d��
	float fWindPower;              //����
	float fWindAngel;              //���������p�x
}PARTICALCENTER;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitParticalCenter(void);
void UninitParticalCenter(void);
void UpdateParticalCenter(void);
void MakeVertexPolygonParticalCenter(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);
PARTICALCENTER *GetParticalCenter(void);

void SetParticalCenter( float fposX, float fposY, float fposZ, float fRadius, float fDistance, EFFECTMODE effectMode);  //�p�[�e�B�N���Z�b�g

#endif
