//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "model.h"
#include "camera.h"
#include "TpsCamera.h"
#include "textureManager.h"
#include "matrix.h"
#include "hitCircle.h"
#include "particalManager.h"
#include "CParticalSetting.h"

#define NUM_MATERIAL (3)
#define SPAWN_NUM (4)

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
int g_TextureNUM[NUM_MATERIAL];                       //�e�N�X�`�����s�ԍ�
LPD3DXMESH g_pMeshModel;                               //���b�V���C���^�[�t�F�[�X�|�C���^
LPD3DXBUFFER g_pBuffMatModel;                          //�}�e���A�����o�b�t�@(D3DX�ėp�o�b�t�@�C���^�[�t�F�[�X)
DWORD g_nNumMatModel;                                  //�}�e���A���̐�
D3DXMATRIX g_ModelMtxWorld;                                   // ���[���h�}�g���N�X
WORLD_MATRIX g_ModelMatrix;                           //���[���h�}�g���N�X�̕ϊ��s��

//�N���X�ϐ�
CSpawn g_CSpawn[SPAWN_NUM];

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitModel(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	g_nNumMatModel = NUM_MATERIAL;                               //�}�e���A���̐�

	//���f�����[���h�ϊ��}�g���N�X�̏�����
	g_ModelMatrix.scl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	g_ModelMatrix.rot = D3DXVECTOR3(0.0f, PI, 0.0f);
	g_ModelMatrix.pos = D3DXVECTOR3(0.0f, -1.0f, -3.0f);

	g_CSpawn[0].Init(&D3DXVECTOR3(40.0f, -22.0f, 0.0f), 6.0f,0.25f, 15,&D3DXVECTOR3(0.0f,100,0.0f));
	g_CSpawn[0].SetUse(true);

	g_CSpawn[1].Init(&D3DXVECTOR3(-40.0f, -22.0f, 1.0f), 6.0f, 0.25f, 15, &D3DXVECTOR3(0.0f, 100, 0.0f));
	g_CSpawn[1].SetUse(true);

	g_CSpawn[2].Init(&D3DXVECTOR3(0.0f, -22.0f, 55.0f), 6.0f, 0.25f, 15, &D3DXVECTOR3(0.0f, 100, 0.0f));
	g_CSpawn[2].SetUse(true);

	g_CSpawn[3].Init(&D3DXVECTOR3(0.0f, -22.0f, -55.0f), 6.0f, 0.25f, 15, &D3DXVECTOR3(0.0f, 100, 0.0f));
	g_CSpawn[3].SetUse(true);

	return S_OK;
}

//���f���̍X�V
void UpdateModel(void)
{
	for (int nCnt = 0; nCnt < SPAWN_NUM; nCnt++)
	{
		if (g_CSpawn[nCnt].GetUse() == true)
		{
			g_CSpawn[nCnt].Update();
		}
	}
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	D3DMATERIAL9 matDef;
	pD3DDevice = Get_Device();
	pD3DDevice->GetMaterial(&matDef);
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();
	SetMatrix(&g_ModelMtxWorld, g_ModelMatrix);    //�}�g���N�X�̌v�Z
	TEXTUREMANAGER *pTextureM = GetTextureManager();
	
	pD3DDevice->SetMaterial(&matDef);   //�}�e���A����߂�
}

void UninitModel(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE (g_pMeshModel);                      //���b�V���C���^�[�t�F�[�X�|�C���^�����

	//���_�o�b�t�@���������
	SAFE_RELEASE (g_pBuffMatModel);                       //�}�e���A�����o�b�t�@�����
}

WORLD_MATRIX *Get_ModelMatrix(void) 
{
	return &g_ModelMatrix;
}

CSpawn *GetSpawnPoint(void)
{
	return &g_CSpawn[0];
}