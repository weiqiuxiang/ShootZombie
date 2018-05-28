#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//���_�t�H�[�}�b�g
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�}�N����`
//-----------------------------------------------------------------------------------------
#define MESHFIELD_TEXTURENAME "data/TEXTURE/863.jpg"

//-----------------------------------------------------------------------------------------
//���b�V��wall�\����
//-----------------------------------------------------------------------------------------
typedef enum
{
	MESH_NULL = 0,
	MESH_WALL,
	MESH_FLOOR,
	MESH_SLOPE,
	MESH_MAX,
}MESHTYPE;

typedef struct
{
	D3DXMATRIX m_MeshFieldMtxWorld;                           // ���[���h�}�g���N�X
	WORLD_MATRIX m_MeshFieldMatrix;                           //���[���h�}�g���N�X�̕ϊ��s��
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBufferMeshField;           //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
	LPDIRECT3DINDEXBUFFER9 m_pIdxBufferFieldMeshField;       //�C���f�b�N�X�o�b�t�@�𑀍삷�邽�߂̃|�C���^
	D3DXVECTOR2 HW;
	float fSlopeAngle;          //���b�V���̑�����MESH_SLOPE�̎�,�Ζʂ̎Η�
	int m_nVertexNum;     //���_��
	int m_nIndexNum;      //�C���f�b�N�X��
	int m_nPolygonNum;    //�|���S����
	int m_TexNum;         //�g�p�e�N�X�`���ԍ�
	int nID_HR;             //�g�p���Ă���l�p����|���S���̔ԍ�
	bool bUse;              //�g�p�t���O
	MESHTYPE meshType;
}MESHWALL;

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
HRESULT InitMeshField(void);                                                     //�|���S��(���_)�̏�����
void DrawMeshField(void);                                                        //�|���S���̕`��
void UpdateMeshField(void);                                                      //�|���S���̍X�V
void UninitMeshField(void);                                                      //�|���S���̏I��
void MakeVertexMeshFieldBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_3D *pVtx);           //�|���S���ݒ�
//XZ���ʂ̃��b�V���E�H�[��
void InitMeshWall_XZ(D3DXVECTOR3 pos, D3DXVECTOR3 rot, UINT nNumBlock_Honrizon, UINT nNumBlock_Vertical, float fSizeBlock_Honrizon, float fSizeBlock_Vertical,  MESHTYPE meshType,char *TexPass);
MESHWALL *GetMesh(void);    //���b�V����Ԃ�
#endif