#pragma once
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define EXPLOSION_TEXTURENAME "data/TEXTURE/explosion_Tex.png"

//�\����
typedef struct
{
	D3DXMATRIX m_ExplosionWorldMatrix;         //�|���S���̃��[���h�}�g���N�X
	WORLD_MATRIX m_ExplosionMatrix;  //�|���S���̃}�g���N�X�̕ϊ��s��
	int nFrame;                   //�i�s�t���[����
	bool bUse;                    //�g�p�t���O
}EXPLOSION_EF;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitExplosion(void);                                                     //�|���S��(���_)�̏�����
void DrawExplosion(void);                                                        //�|���S���̕`��
void UpdateExplosion(void);                                                      //�|���S���̍X�V
void UninitExplosion(void);                                                      //�|���S���̏I��
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //�|���S���ݒ�
void SetExplosion(D3DXVECTOR3);       //�����e�N�X�`���I�u�W�F�Z�b�g

#endif