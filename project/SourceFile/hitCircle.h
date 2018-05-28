#pragma once
#ifndef _HITCIRCLE_H_
#define _HITCIRCLE_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "skydome.h"

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define HITCIRCLE_MAX (128)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define HITCIRCLE_TEXTURENAME "data/TEXTURE/hitRect.png"

typedef enum
{
	HBTYPE_NULL = 0,
	HBTYPE_HANDGUN,
	HBTYPE_AK47,
	HBTYPE_ENEMY,
}HITBULLETTYPE;

typedef enum
{
	TYPE_PLAYER,
	TYPE_ENEMY,
	TYPE_OBJ,
	TYPE_MAX
}HRTYPE;

typedef struct
{
	SKYDOME m_skydome;
	D3DXMATRIX m_MtxWorld;                                   // ���[���h�}�g���N�X
	WORLD_MATRIX m_Matrix;                           //���[���h�}�g���N�X�̕ϊ��s��
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_offset;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer = NULL;						//���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
	LPDIRECT3DINDEXBUFFER9 m_pIdxBufferField = NULL;					//�C���f�b�N�X�o�b�t�@�𑀍삷�邽�߂̃|�C���^
	int m_nVertexNum;                    //���_��
	int m_nIndexNum;                     //�C���f�b�N�X��
	int m_nPolygonNum;                   //�|���S����
	D3DCOLOR m_Color;
	int nID;
	int nEnemyID;
	int nCntExplosion;
	HRTYPE m_type;
	HITBULLETTYPE HitCheckType;
	bool bUse;
}HITCIRCLE;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitHitCircle(void);                                                     //�|���S��(���_)�̏�����
void DrawHitCircle(void);                                                        //�|���S���̕`��
void UpdateHitCircle(void);                                                      //�|���S���̍X�V
void UninitHitCircle(void);                                                      //�|���S���̏I��
void MakeVertexHitCircle(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //�|���S���ݒ�
HRESULT SetHitCirclePlayer(const D3DXVECTOR3 *pPos, float nRadius, const D3DXVECTOR3 *offset);    //����|���S���Z�b�g
HRESULT SetHitCircleEnemy(const D3DXVECTOR3 *pPos, float nRadius, const D3DXVECTOR3 *offset,int EnemyID);    //����|���S���Z�b�g

void SetCirclePos(const D3DXVECTOR3 *pos,int nID);                            //������ID�ԍ��̉~�̈ʒu���Z�b�g����
HITCIRCLE *Get_HitCircle(int nID);                                            //������ID�ԍ��̉~���b�V����Ԃ�

#endif