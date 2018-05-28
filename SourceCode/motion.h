#pragma once
#ifndef _MOTION_H_
#define _MOTION_H_

//�w�b�_�t�@�C��
#include "main.h"

#define MATTEX_NUMMAX (10)
#define PART_MAX (20)

//�N���X�錾
/*
	���f���̃p�[�c�N���X
*/

class CPart
{
public:
	CPart();
	~CPart();

	HRESULT Init( DWORD nNumMatModel);
	void SetInit(const WORLD_MATRIX *offsetInit, int nIndex, int nNumPart);                      //�����̃I�t�Z�b�g�ʒu�Ɛe�̔ԍ��ƃp�[�c�ԍ��̐ݒ�
	void SetKeyBefore(const WORLD_MATRIX *keyBefore);                                                  //�O�̃L�[�t���[��
	void SetKeyAfter(const WORLD_MATRIX *keyAfter);                                                    //���̃L�[�t���[��
	void SetModelPass(char *Modelpass);
	D3DXMATRIX GetResultMtx(void);
	bool GetUse(void);
	void SetUse(bool bUse);
	void SetParentID(int nID);
	int GetParentID(void);
	int GetIndex(void);
	int GetParent(void);
	void Update(int nMotionFrameNum,int nMotionFrameNow, const D3DXMATRIX *pParent);
	void Draw(void);
	void Uninit(void);
	HRESULT ModelChange(char *Modelpass, DWORD nNumMatModel);                                                              //���f���`�F���W
private:
	WORLD_MATRIX m_keyBefore;        //�O�̃L�[�t���[���̃I�t�Z�b�g
	WORLD_MATRIX m_keyAfter;         //���̃L�[�t���[���̃I�t�Z�b�g
	D3DXMATRIX m_MtxResult;          //MtxParent*m_OffsetVec�̐���Mtx*m_MtxOffset(���f���}�g���N�X)
	D3DXMATRIX m_MtxResultOld;       //���蔻��pOld���W
	WORLD_MATRIX m_OffsetInit;       //�I�t�Z�b�g�𐶐����邽�߂̃x�N�g��(Init���ݒ�I�t�Z�b�g)
	char m_aModelpass[256];         //���f���p�X
	int m_nIndex;                   //�����̃p�[�c�̔ԍ�(Init���ǂݍ���)
	int m_nNumParent;               //�e�̃p�[�c�̔ԍ�(Init���ǂݍ���)
	int m_nID;                       //�e��ID�o�^
	bool m_bUse;                    //�g�p���Ă���t���O(�ŏ�false�ݒ�,����Init���ݒ�)

	LPD3DXMESH m_pMesh;                               //���b�V���C���^�[�t�F�[�X�|�C���^
	LPD3DXBUFFER m_pBuffMat;                          //�}�e���A�����o�b�t�@(D3DX�ėp�o�b�t�@�C���^�[�t�F�[�X)
	LPDIRECT3DTEXTURE9 m_TexSlot[MATTEX_NUMMAX];                      //�}�e���A���e�N�X�`���ԍ���ۑ�����z��
	DWORD m_NumMat;
};

/*
�d�l:�p�[�c���m�F��,�L�����ɑΉ����郂�[�V�����e�L�X�g��ǂݍ���,
CHARACT�\���̂́uMotionName�v���m�F��,�e�L�X�g���烂�[�V�����l�[�����Q�Ƃ������̃��[�V�����f�[�^��ǂݍ���,
�e�p�[�c�̃I�t�Z�b�g�f�[�^���e�p�[�c��OffsetVec�ɓǂݍ���,MtxOffset�����
�e�q�֌W�m�F,�e�q�֌W�ɂ���Đe��MtxResult��������MtxParent���,������MtxResult = MtxParent*MtxOffset���v�Z��
(MtxResultOld - MtxResult)���[�V�����������v�Z��,
�upD3DDevice->SetTransform(D3DTS_WORLD, &MtxResult);�v���g���Ď����̃��[���h�̈ʒu��\��
*/
class CModel
{
public:
	typedef enum
	{
		MOTION_NULL = 0,
		MOTION_NORMAL,
		MOTION_SHOOT,
		MOTION_NORMALMOVE,
		MOTION_GUNMOVE,
		MOTION_RUN,
		MOTION_GNORMAL,
		MOTION_GSHOOT,
		MOITON_GNORMALMOVE,
		MOTION_GGUNMOVE,
		MOTION_GRUN,
		MOTION_GTHROW,
		MOTION_MAX,
	}CHAR_MODE;

	CModel();
	~CModel();
	HRESULT Init(char *filepass);
	void Update(void);                                       //�e�p�[�c����A�b�v�f�[�g���Ȃ��Ƃ����Ȃ�
	void Draw(void);
	void Uninit(void);
	bool GetLoop(void);
	static D3DXMATRIX GetMtxParent(int nNumPart);                                           //�擾����e�}�g���N�X�̔ԍ�
private:
	char aMotionTextPass[256];        //�L�����ɑΉ����郂�[�V�����f�[�^�e�L�X�g�̑��΃p�X
	CHAR_MODE MotionName;             //���[�V������
	int m_nMotionFrameNum;                 //���[�V�����t���[����(�ǂݍ���)
	int m_nMotionFrameNow;              //�扽�t���[���ڂ����s���Ă���̂�?(�J�E���g�A�b�v)
	int m_nKeyNum;                      //�L�[�̐�(�ǂݍ���)
	int m_nKeyNow;                      //���̃L�[(�J�E���g�A�b�v)
	bool m_bLoop;                       //���[�v�t���O[0:���[�v���Ȃ� / 1:���[�v����]
	int m_nNumpart;                   //�p�[�c�̐�

	static CPart m_partData[PART_MAX];
};

#endif