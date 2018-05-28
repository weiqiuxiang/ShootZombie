//-----------------------------------------------------------------------------------------
//�v���[���[�w�b�h�t�@�C��[enemy_running02.h]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#ifndef _PARTICAL_H_
#define _PARTICAL_H_

#include <d3dx9.h>
#include "main.h"
#include "particalCenter.h"
//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define MAX_PARTICAL (256)
#define NUM_PARTICALVERTEX (4)                          //���_��
#define NUM_PARTICALTEXTURE (3)                         //�e�N�X�`����

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define PARTICALPOLYGON01_TEXTURENAME "data/TEXTURE/partical.png"
#define PARTICALPOLYGONEFFECT_TEXTURENAME "data/TEXTURE/water.jpg"
#define FIREEFFECT_TEXTURENAME "data/TEXTURE/fire.jpg"

//-----------------------------------------------------------------------------------------
//�v���[���[�̍\����
//-----------------------------------------------------------------------------------------
typedef struct
{
	D3DXMATRIX MtxWorld;                                   // ���[���h�}�g���N�X
	WORLD_MATRIX MtxSRT;                           //���[���h�}�g���N�X�̕ϊ��s��
	D3DXVECTOR3 speed;       //�ړ���(���x)
	D3DXCOLOR color;         //�J���[
	float fRadius;           //���a
	int nAlpha;              //���l
	float fColorAlphaValue;  //�F(��)�ω���
	int nLife;               //����
	bool bUse;               //�g�p�t���O
	int nID;                 //�����Ă���ID�ԍ�
}PARTICAL;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitPartical(void);
void UninitPartical(void);
void UpdatePartical(void);
void DrawPartical(void);
void UpdatePolygonPartical(void);
void MakeVertexPolygonPartical(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);
void SetPartical(float fSpeedX,float fSpeedY,float fSpeedZ,float fRadius,float nLife);
void SetFirePartical(PARTICALCENTER *pParticalCenter, float fFirePosX, float fSpeedX, float fSpeedY, float fSpeedZ, float nLife);

#endif
