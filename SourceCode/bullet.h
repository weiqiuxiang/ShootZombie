#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define NUM_VERTEX (4)                        //���_��
#define NUM_BULLET (2)						  //�|���S����
#define BULLET_MAX (128)
#define BULLETEFFECT_MAX (4096)
#define BULLET_SPEED (10.0f)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define BULLET_TEXTURENAME "data/TEXTURE/bullet.png"
#define BULLETEFFECT_TEXTURENAME "data/TEXTURE/bulletEffect.jpg"

typedef enum
{
	BTYPE_HANDGUN = 0,
	BTYPE_AK47,
	BTYPE_MAX,
}BULLETTYPE;

//�\����
typedef struct
{
	D3DXVECTOR3 posOld;           //���W
	D3DXVECTOR3 speed;         //�X�s�[�h
	D3DXMATRIX m_BulletWorldMatrix;         //�|���S���̃��[���h�}�g���N�X
	WORLD_MATRIX m_BulletMatrix;            //�|���S���̃}�g���N�X�̕ϊ��s��
	BULLETTYPE m_Btype;
	bool bUse;                 //�g�p�t���O
}BULLET;

typedef struct
{
	D3DXMATRIX m_BulletWorldMatrix;         //�|���S���̃��[���h�}�g���N�X
	WORLD_MATRIX m_BulletMatrix;            //�|���S���̃}�g���N�X�̕ϊ��s��
	float fRadius;           //���a
	float fRadiusValue;      //���a�̕ω���
	int nAlpha;              //���l
	float fColorAlphaValue;  //�F(��)�ω���
	int nLife;               //����
	BULLETTYPE m_Btype;
	bool bUse;                 //�g�p�t���O
}BULLETEFFECT;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitBullet(void);                                                     //�|���S��(���_)�̏�����
void DrawBullet(void);                                                        //�|���S���̕`��
void UpdateBullet(void);                                                      //�|���S���̍X�V
void UninitBullet(void);                                                      //�|���S���̏I��
void MakeVertexBulletBG(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //�|���S���ݒ�
void SetBullet(D3DXVECTOR3, D3DXVECTOR3, BULLETTYPE Btype);
void SetBulletEffect(D3DXVECTOR3 pos, int nLife, BULLETTYPE Btype);                              //

#endif