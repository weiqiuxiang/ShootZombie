//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include "model_aircraft.h"
#include "MeshField.h"
#include "shadow.h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------------------
#define STAND_RADIUS (4.0f)    //�W�����a
#define RADIUS_VALUE (0.5f)    //�P�ʋ����̔��a�ω���
#define ALPHA_VALUE (10)       //�P�ʋ����̔��a�ω���

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureShadow = {};          //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferShadow = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
SHADOW g_Shadow;    //�����G�t�F�N�g

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���f�����[���h�ϊ��}�g���N�X�̏�����

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferShadow, NULL)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, SHADOW_TEXTURENAME, &g_pTextureShadow)))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�p�����[�^������
	g_Shadow.m_ShadowMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_Shadow.m_ShadowMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Shadow.m_ShadowMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Shadow.fStandRadius = STAND_RADIUS;
	g_Shadow.fRadiusValue = RADIUS_VALUE;
	g_Shadow.nAlpha = 255;
	g_Shadow.nAlphaValue = ALPHA_VALUE;

	//
	//���_���b�N
	//////////////////////
	g_pVtxBufferShadow->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexShadow(pD3DDevice, pVtx);

	pVtx[0].pos = D3DXVECTOR3(-STAND_RADIUS*0.5, STAND_RADIUS*0.5, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(STAND_RADIUS*0.5, STAND_RADIUS*0.5, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-STAND_RADIUS*0.5, -STAND_RADIUS*0.5, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(STAND_RADIUS*0.5, -STAND_RADIUS*0.5, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	g_pVtxBufferShadow->Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}

//
//�|���S���̍X�V
/////////////////////////////
void UpdateShadow(void)
{
	//
	//�ϐ��錾
	////////////////////////////////
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^

	float fResultRadius;      //���ʔ��a
	int fResultAlpha;         //���ʃA���t�@
	float fDistanceM_F;       //���f���ƒn�ʂ̋���
	AIRCRAFT *pAircraft = Get_Aircraft();  //���f�����W�擾
	MESHWALL *pMeshwall = GetMesh();           //���b�V���̍��W�擾

	//�v�Z����
	fDistanceM_F = 0.0f;

	//���f���ƒn�ʂ̋����̓}�C�i�X�ɂ��Ȃ��悤��
	if(fDistanceM_F < 0)
	{ 
		fDistanceM_F = 0;
	}

	fResultRadius = g_Shadow.fStandRadius + fDistanceM_F * g_Shadow.fRadiusValue;  //���ʔ��a�v�Z
	fResultAlpha = g_Shadow.nAlpha - fDistanceM_F * g_Shadow.nAlphaValue;  //���ʃA���t�@�l�v�Z

	//�A���t�@�l����
	if(fResultAlpha < 0)
	{
		fResultAlpha = 0;
	}
	
	if (fResultAlpha > 255)
	{
		fResultAlpha = 255;
	}

	g_Shadow.m_ShadowMatrix.pos = D3DXVECTOR3(pAircraft->m_Matrix.pos.x, pAircraft->m_Matrix.pos.y - 4.0f, pAircraft->m_Matrix.pos.z);
	
	//
	//���_���b�N
	////////////////////////////////
	g_pVtxBufferShadow->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexShadow(pD3DDevice, pVtx);  //�ݒ�

	//���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fResultRadius*0.5, 0.0f , fResultRadius*0.5);
	pVtx[1].pos = D3DXVECTOR3(fResultRadius*0.5, 0.0f , fResultRadius*0.5);
	pVtx[2].pos = D3DXVECTOR3(-fResultRadius*0.5, 0.0f , -fResultRadius*0.5);
	pVtx[3].pos = D3DXVECTOR3(fResultRadius*0.5, 0.0f , -fResultRadius*0.5);

	pVtx[0].tex = D3DXVECTOR2(0.0f , 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f , 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f , 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f , 1.0f);

	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, fResultAlpha);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, fResultAlpha);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, fResultAlpha);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, fResultAlpha);

	g_pVtxBufferShadow->Unlock();              //�A�����b�N
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���

	//���Z�����ݒ�
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT/*���Z�����ݒ�*/);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//
	//���_�t�H�[�}�b�g�ݒ�
	///////////////////////////////
	pD3DDevice->SetStreamSource(0, g_pVtxBufferShadow, 0, sizeof(VERTEX_3D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX3D);
	pD3DDevice->SetTexture(0, g_pTextureShadow);                                       //�e�N�X�`���̐ݒ�
	
	SetMatrix(&g_Shadow.m_ShadowWorldMatrix, g_Shadow.m_ShadowMatrix);      //�r���{�[�h�}�g���N�X�Z�b�g
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);                         //�`��

	// �`��G�̓��l�œ����Ȃ��Ă���
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);     //�u�����h���[�h���A���t�@�u�����h�ɖ߂�

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g���I���ɂ���
}

void UninitShadow(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE(g_pTextureShadow);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��

	SAFE_RELEASE(g_pVtxBufferShadow);                       //���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
}

//�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************�f�o�C�X�ݒ�***********************************************/
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT/*���Z�����ݒ�*/);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
