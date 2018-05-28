#ifndef _OBJ3D_H_
#define _OBJ3D_H_
//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <d3dx9.h>
#include "main.h"

//-----------------------------------------------------------------------------------------
//�}�O��
//-----------------------------------------------------------------------------------------
#define OBJ3D_TEX_MAX (5)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
class CObj3D
{
public:
	CObj3D();
	~CObj3D();

	//��Ԋ֐�
	HRESULT Init(const WORLD_MATRIX *pPosRotScl, char *ModelPass, DWORD nNumMatEB, bool bUse);
	void Draw(void);
	void Update(void);
	void Uninit(void);

	//�t�@�N�V�����֐�
	void SetPos(const D3DXVECTOR3 *pos);                                                   //�ʒu�ݒ�
	void SetRot(const D3DXVECTOR3 *rot);                                                   //������]�p�x�ݒ�
	void AddRotY(float fRot);                                                              //Y����]
	void SetMaterial(const D3DMATERIAL9 *Material);                                        //�}�e���A���ݒ�
	void AddPos(const D3DXVECTOR3 *pos);                                                   //Pos����
	bool GetUse(void);                                                                     //�g�p��Ԏ擾
	void SetUse(bool bUse);                                                                //�g�p��Ԑݒ�
	void SetMatrialUse(bool bUse);                                                         //�����p�}�e���A����Ԑݒ�
	static void MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx);             //�����_�X�e�[�^�X�ݒ�

protected:
	D3DXMATRIX m_WorldMatrix;                         //���[���h�}�g���N�X
	WORLD_MATRIX m_PosRotScl;                         //�I�u�W�FPos,Rot,Scl,���̎O���g���ă��f�����O�s��𐶐�����
	D3DMATERIAL9 m_Material;                          //�ω��p�}�e���A��(�I�u�W�F���̂��g���u���N���鎞�̑΍��p�}�e���A��)
	LPD3DXMESH m_pMesh;                               //���b�V���C���^�[�t�F�[�X�|�C���^
	LPD3DXBUFFER m_pBuffMat;                          //�}�e���A�����o�b�t�@(D3DX�ėp�o�b�t�@�C���^�[�t�F�[�X)
	DWORD m_nNumMat;                                //�}�e���A���̐�
	int m_TexNUM[OBJ3D_TEX_MAX];                    //�e�N�X�`�����s�ԍ�
	bool m_UseMatrial;                                //�����p�}�e���A���̎g�p���(true�͎g�p)
	bool m_bUse;                                      //�g�p��ԃt���O
};

#endif