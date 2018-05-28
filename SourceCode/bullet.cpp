//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "bullet.h"
#include "hitCircle.h"
#include "hitCircleMesh.h"
#include "hitcheckMesh.h"
#include "explosion.h"
#include "camera.h"
#include "TpsCamera.h"
#include "sound.h"
#include "matrix.h"
#include "model_aircraft.h"

#define BULLET_WIDTH (0.6f)
#define BULLET_HEIGHT (0.6f)
#define BULLETEFFECT_WIDTH (1.2f)
#define BULLETEFFECT_HEIGHT (1.2f)

#define BORDERLINE (5000.0f)              //���E���̍��W

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v
//-----------------------------------------------------------------------------------------
void UpdateBulletBody(void);    //�o���b�g�{�̍X�V
void UpdateBulletEffect(void);  //�o���b�g�G�t�F�N�g�X�V

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet[2] = {};          //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBulletEffect = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

BULLET g_Bullet[BULLET_MAX];                  //�o���b�g�\����
BULLETEFFECT g_BulletEffect[BULLETEFFECT_MAX];            //�o���b�g�G�t�F�N�g

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitBullet(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���f�����[���h�ϊ��}�g���N�X�̏�����

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLET_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferBullet, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * BULLETEFFECT_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferBulletEffect, NULL)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLET_TEXTURENAME, &g_pTextureBullet[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, BULLETEFFECT_TEXTURENAME, &g_pTextureBullet[1])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//
	//�o���b�g���_���b�N
	//////////////////////
	g_pVtxBufferBullet->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexBulletBG(pD3DDevice, pVtx);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		int k = i * 4;
		pVtx[0 + k].pos = D3DXVECTOR3(-BULLET_WIDTH*0.5, BULLET_HEIGHT*0.5, 0.0f);
		pVtx[1 + k].pos = D3DXVECTOR3(BULLET_WIDTH*0.5, BULLET_HEIGHT*0.5, 0.0f);
		pVtx[2 + k].pos = D3DXVECTOR3(-BULLET_WIDTH*0.5, -BULLET_HEIGHT*0.5, 0.0f);
		pVtx[3 + k].pos = D3DXVECTOR3(BULLET_WIDTH*0.5, -BULLET_HEIGHT*0.5, 0.0f);

		pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1 + k].color = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2 + k].color = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3 + k].color = D3DCOLOR_RGBA(255, 255, 255, 100);

		g_Bullet[i].m_BulletMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Bullet[i].m_BulletMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].m_BulletMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].bUse = false;
	}

	g_pVtxBufferBullet->Unlock();                                     //�o�b�t�@�̃A�����b�N

	//
	//�o���b�g�G�t�F�N�g���_���b�N
	////////////////////////////////
	g_pVtxBufferBulletEffect->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexBulletBG(pD3DDevice, pVtx);

	for (int i = 0; i < BULLETEFFECT_MAX; i++)
	{
		int k = i * 4;
		pVtx[0 + k].pos = D3DXVECTOR3(-BULLETEFFECT_WIDTH*0.5, BULLETEFFECT_HEIGHT*0.5, 0.0f);
		pVtx[1 + k].pos = D3DXVECTOR3(BULLETEFFECT_WIDTH*0.5, BULLETEFFECT_HEIGHT*0.5, 0.0f);
		pVtx[2 + k].pos = D3DXVECTOR3(-BULLETEFFECT_WIDTH*0.5, -BULLETEFFECT_HEIGHT*0.5, 0.0f);
		pVtx[3 + k].pos = D3DXVECTOR3(BULLETEFFECT_WIDTH*0.5, -BULLETEFFECT_HEIGHT*0.5, 0.0f);

		pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx[0 + k].color = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1 + k].color = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2 + k].color = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3 + k].color = D3DCOLOR_RGBA(255, 0, 0, 255);

		g_BulletEffect[i].m_BulletMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_BulletEffect[i].m_BulletMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BulletEffect[i].m_BulletMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	g_pVtxBufferBulletEffect->Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}

//�|���S���̍X�V
void UpdateBullet(void)
{
	UpdateBulletBody();  //�o���b�g�{�̍X�V
	UpdateBulletEffect();//�G�t�F�N�g�X�V
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���

	pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);                               //z�o�b�t�@���Q��
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);                               //z���c���Ȃ�

	//���Z�����ݒ�
	// �A���t�@�E�u�����f�B���O���s��
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	// �`��G�|���S���̓��œ������ɂȂ��Ă���
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// ��̊G�͓��������Ȃ�
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//
	//�o���b�g�ݒ�
	///////////////////////////////
	//���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBullet, 0, sizeof(VERTEX_3D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX3D);
	pD3DDevice->SetTexture(0, g_pTextureBullet[0]);												 //�e�N�X�`���̐ݒ�

	
	//�o���b�g�`��
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�o���b�g���g�p��
		if (g_Bullet[i].bUse == true)
		{
			SetBillboardMatrix(&g_Bullet[i].m_BulletWorldMatrix, g_Bullet[i].m_BulletMatrix);      //�r���{�[�h�}�g���N�X�Z�b�g
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, NUM_BULLET);                         //�`��
		}
	}
	

	//
	//�o���b�g�G�t�F�N�g�ݒ�
	///////////////////////////////
	//���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, g_pVtxBufferBulletEffect, 0, sizeof(VERTEX_3D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX3D);
	pD3DDevice->SetTexture(0, g_pTextureBullet[1]);												 //�e�N�X�`���̐ݒ�

	//�o���b�g�G�t�F�N�g�`��
	for (int i = 0; i < BULLETEFFECT_MAX; i++)
	{
		//�o���b�g���g�p��
		if (g_BulletEffect[i].bUse == true)
		{
			SetBillboardMatrix(&g_BulletEffect[i].m_BulletWorldMatrix, g_BulletEffect[i].m_BulletMatrix);      //�r���{�[�h�}�g���N�X�Z�b�g
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, NUM_BULLET);                         //�`��
		}
	}

	//�u�����h���[�h���A���t�@�u�����h�ɖ߂�
	// ��̊G�̓��l�ŕs�����Ȃ��Ă���
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// �`��G�̓��l�œ����Ȃ��Ă���
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      //z���c��

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g���I���ɂ���
}

void UninitBullet(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE(g_pTextureBullet[0]);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pTextureBullet[1]);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��

	//���_�o�b�t�@���������
	SAFE_RELEASE(g_pVtxBufferBullet);                       //���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferBulletEffect);                 //�G�t�F�N�g���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
}

//�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexBulletBG(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************�f�o�C�X�ݒ�***********************************************/
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//�e�̃Z�b�g
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 speed, BULLETTYPE Btype)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].bUse == false)
		{
			g_Bullet[i].speed = speed;

			g_Bullet[i].m_BulletMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Bullet[i].m_BulletMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Bullet[i].m_BulletMatrix.pos = pos;
			g_Bullet[i].m_Btype = Btype;
			g_Bullet[i].bUse = true;
			break;
		}
	}
}

//
//�o���b�g�G�t�F�N�g�Z�b�g
//////////////////////////////
void SetBulletEffect(D3DXVECTOR3 pos, int nLife, BULLETTYPE Btype)
{
	for (int i = 0; i < BULLETEFFECT_MAX; i++)
	{
		if (g_BulletEffect[i].bUse == false)
		{
			//���[�J������
			g_BulletEffect[i].m_BulletMatrix.pos = pos;
			//�}�g���N�X����
			g_BulletEffect[i].m_BulletMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_BulletEffect[i].m_BulletMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//���C�t�ɂ��ω��ʌv�Z
			g_BulletEffect[i].nLife = nLife;
			g_BulletEffect[i].nAlpha = 255;
			g_BulletEffect[i].fRadius = BULLETEFFECT_HEIGHT;
			g_BulletEffect[i].fRadiusValue = g_BulletEffect[i].fRadius / g_BulletEffect[i].nLife;
			g_BulletEffect[i].fColorAlphaValue = g_BulletEffect[i].nAlpha / g_BulletEffect[i].nLife;
			g_BulletEffect[i].m_Btype = Btype;
			//�t���O
			g_BulletEffect[i].bUse = true;
			break;
		}
	}
}

//
//�o���b�g�X�V
////////////////////////
void UpdateBulletBody(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].bUse == true)
		{
			g_Bullet[i].posOld = g_Bullet[i].m_BulletMatrix.pos;

			//���x����
			g_Bullet[i].m_BulletMatrix.pos.x += g_Bullet[i].speed.x;
			g_Bullet[i].m_BulletMatrix.pos.y += g_Bullet[i].speed.y;
			g_Bullet[i].m_BulletMatrix.pos.z += g_Bullet[i].speed.z;

			//�͈͊O����
			if (
				(g_Bullet[i].m_BulletMatrix.pos.x >= BORDERLINE || g_Bullet[i].m_BulletMatrix.pos.x <= -BORDERLINE) ||
				(g_Bullet[i].m_BulletMatrix.pos.z >= BORDERLINE || g_Bullet[i].m_BulletMatrix.pos.z <= -BORDERLINE)
				)
			{
				g_Bullet[i].bUse = false;
			}

			//�e�ƃ��b�V���̓��蔻��
			if (VectorAndRect(&g_Bullet[i].posOld, &g_Bullet[i].m_BulletMatrix.pos))
			{
				//�������ʃZ�b�g
				SetExplosion(g_Bullet[i].m_BulletMatrix.pos);
				g_Bullet[i].bUse = false;
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//�e�Ɠ��蔻��~�̓��蔻��
			AIRCRAFT *pAircraft = Get_Aircraft();                                      //�v���[���[�̃f�[�^���擾
			if(BulletCircleAndCircleSSV(&g_Bullet[i].m_BulletMatrix.pos,&g_Bullet[i].posOld,&g_Bullet[i].m_BulletMatrix.pos,0.001f, g_Bullet[i].m_Btype) )
			{
				//�������ʃZ�b�g
				SetExplosion(g_Bullet[i].m_BulletMatrix.pos);
				g_Bullet[i].bUse = false;
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//�G�t�F�N�g�Z�b�g
			SetBulletEffect(g_Bullet[i].m_BulletMatrix.pos, 20, g_Bullet[i].m_Btype);
		}
	}
}

//
//�o���b�g�G�t�F�N�g�X�V
////////////////////////
void UpdateBulletEffect(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^

	AIRCRAFT *pAircraft = Get_Aircraft();

	//
	//�o���b�g�G�t�F�N�g���_���b�N
	////////////////////////////////
	g_pVtxBufferBulletEffect->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexBulletBG(pD3DDevice, pVtx);  //�ݒ�
	for (int i = 0; i < BULLETEFFECT_MAX; i++)
	{
		if (g_BulletEffect[i].bUse == true)
		{
			int k = i * 4;
			pVtx[0 + k].pos = D3DXVECTOR3(-BULLETEFFECT_WIDTH*0.5, g_BulletEffect[i].fRadius*0.5, 0.0f);
			pVtx[1 + k].pos = D3DXVECTOR3(BULLETEFFECT_WIDTH*0.5, g_BulletEffect[i].fRadius*0.5, 0.0f);
			pVtx[2 + k].pos = D3DXVECTOR3(-BULLETEFFECT_WIDTH*0.5, -g_BulletEffect[i].fRadius*0.5, 0.0f);
			pVtx[3 + k].pos = D3DXVECTOR3(BULLETEFFECT_WIDTH*0.5, -g_BulletEffect[i].fRadius*0.5, 0.0f);

			pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

			if (g_BulletEffect[i].m_Btype == BTYPE_HANDGUN)
			{
				pVtx[0 + k].color = D3DCOLOR_RGBA(255, 0, 0, g_BulletEffect[i].nAlpha);
				pVtx[1 + k].color = D3DCOLOR_RGBA(255, 0, 0, g_BulletEffect[i].nAlpha);
				pVtx[2 + k].color = D3DCOLOR_RGBA(255, 0, 0, g_BulletEffect[i].nAlpha);
				pVtx[3 + k].color = D3DCOLOR_RGBA(255, 0, 0, g_BulletEffect[i].nAlpha);
			}

			if (g_BulletEffect[i].m_Btype == BTYPE_AK47)
			{
				pVtx[0 + k].color = D3DCOLOR_RGBA(0, 0, 255, g_BulletEffect[i].nAlpha);
				pVtx[1 + k].color = D3DCOLOR_RGBA(0, 0, 255, g_BulletEffect[i].nAlpha);
				pVtx[2 + k].color = D3DCOLOR_RGBA(0, 0, 255, g_BulletEffect[i].nAlpha);
				pVtx[3 + k].color = D3DCOLOR_RGBA(0, 0, 255, g_BulletEffect[i].nAlpha);
			}

			//�ω�
			g_BulletEffect[i].fRadius -= g_BulletEffect[i].fRadiusValue;    //���a�ω�
			g_BulletEffect[i].nAlpha -= g_BulletEffect[i].fColorAlphaValue; //�A���t�@�ω�
			g_BulletEffect[i].nLife--;  //���C�t�ω�

			//���ʐ؂�
			if (g_BulletEffect[i].nLife < 0)
			{
				g_BulletEffect[i].bUse = false;
			}
		}
	}

	g_pVtxBufferBulletEffect->Unlock();              //�A�����b�N
}
