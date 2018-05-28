#pragma once
#ifndef _SHADOW_H_
#define _SHADOW_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define SHADOW_TEXTURENAME "data/TEXTURE/shadow.jpg"

//�\����
typedef struct
{
	D3DXMATRIX m_ShadowWorldMatrix; //�|���S���̃��[���h�}�g���N�X
	WORLD_MATRIX m_ShadowMatrix;    //�|���S���̃}�g���N�X�̕ϊ��s��
	float fStandRadius;                 //�n�ʂɂ҂����藧���̉e(����Ă��Ȃ�)
	float fRadiusValue;                 //���a�̕ω���(�P�ʋ����̕ω���)
	int nAlpha;                         //�n�ʂɂ҂����藧���̃A���t�@�l(����Ă��Ȃ�)
	int nAlphaValue;                    //�A���t�@�l�ω���(�P�ʋ����̕ω���)
}SHADOW;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitShadow(void);                                                     //�|���S��(���_)�̏�����
void DrawShadow(void);                                                        //�|���S���̕`��
void UpdateShadow(void);                                                      //�|���S���̍X�V
void UninitShadow(void);                                                      //�|���S���̏I��
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //�|���S���ݒ�

#endif