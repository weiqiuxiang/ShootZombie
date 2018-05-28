#ifndef _HITRECT_H_
#define _HITRECT_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"
#include "MeshField.h"

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define HITRECT_MAX (128)

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define HITRECT_TEXTURENAME "data/TEXTURE/hitRect.png"

typedef struct
{
	D3DXMATRIX m_HitRectWorldMatrix;                        //�|���S���̃��[���h�}�g���N�X
	WORLD_MATRIX m_HitRectMatrix;                           //�|���S���̃}�g���N�X�̕ϊ��s��
	VERTEX_3D    Vectex[4];                            //�l���_�̏��
	VERTEX_3D    VectexC[4];                           //�l���_�̓��蔻����
	D3DXVECTOR3 VecXZ;                    //�@���Ɛ�����XZ���ʂ̃x�N�g��
	int nID;                             //�g�p�ԍ�
	bool bUse;                           //�g�p��ԃt���O
	MESHTYPE meshType;
}HITRECT;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitHitRect(void);                                                     //�|���S��(���_)�̏�����
void DrawHitRect(void);                                                        //�|���S���̕`��
void UpdateHitRect(void);                                                      //�|���S���̍X�V
void UninitHitRect(void);                                                      //�|���S���̏I��
void MakeVertexHitRect(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);           //�|���S���ݒ�
void SetHitRect(const WORLD_MATRIX *pResultMatrix, const D3DXVECTOR2 *HW, MESHTYPE meshType);    //����|���S���Z�b�g
HITRECT *Get_HitRect(void);

#endif