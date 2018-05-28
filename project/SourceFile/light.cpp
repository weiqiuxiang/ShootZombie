/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
���C�g����[light.cpp]

Author :
Data   :
----------------------------------------------------------------
��Update : 2016/04/18
���C�g�̑���
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include "light.h"

/////////////////////////////////�O���o�[���ϐ�/////////////////////////////////
D3DLIGHT9 g_Light;                         //���C�g

/////////////////////////////////���C�g�������֐�/////////////////////////////////
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = Get_Device();               //�f�o�C�X�擾

	//���C�g�̐ݒ�
	ZeroMemory(&g_Light, sizeof(D3DLIGHT9));                      //���C�g������
	g_Light.Type = D3DLIGHT_DIRECTIONAL;                          //���C�g�̃^�C�v���w��
	g_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);          //�f�B���N�V���i�����C�g�̐F
	g_Light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);          //�A���r�G���g���C�g
	D3DXVECTOR3 vecDir(1.0f, -1.0f, 1.0f);                         //���C�g����
	D3DXVec3Normalize((D3DXVECTOR3*)&g_Light.Direction, &vecDir); //���C�g������ݒ肵�����C�g�ɓ����
}

/////////////////////////////////���C�g�I���֐�/////////////////////////////////
void UninitLight(void) 
{
	
}

/////////////////////////////////���C�g�X�V�֐�/////////////////////////////////
void UpdateLight(void) 
{
	LPDIRECT3DDEVICE9 pDevice = Get_Device();               //�f�o�C�X�擾
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g�̐ݒ�

	pDevice->SetLight(0, &g_Light);                               //�f�o�C�X�̃��C�g�X���b�g0�ԂɊi�[
	pDevice->LightEnable(0, TRUE);                                //���C�g�X���b�g0�ԃ��C�g�N�p
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f) );           // �A���r�G���g���C�g�ݒ�
}

void SetMaterial (LPDIRECT3DDEVICE9 pDevice)
{
	D3DMATERIAL9 mtrl;                               //�}�e���A��
	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse.r = 1.0f;
	mtrl.Diffuse.g = 1.0f;
	mtrl.Diffuse.b = 1.0f;
	mtrl.Diffuse.a = 1.0f;
	mtrl.Ambient = mtrl.Diffuse;
	pDevice->SetMaterial(&mtrl);
}