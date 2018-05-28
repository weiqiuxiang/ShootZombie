//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include "bullet.h"
#include "explosion.h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------------------
#define EXPLOSION_RADIUS (1.5f)
#define FRAME_MAX (21)
#define TEX_WIDTH (0.333333333)
#define TEX_HEIGHT (0.142857143)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureExplosion = {};          //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferExplosion = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
EXPLOSION_EF g_Explosion[BULLET_MAX];    //�����G�t�F�N�g

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���f�����[���h�ϊ��}�g���N�X�̏�����

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BULLET_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferExplosion, NULL)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, EXPLOSION_TEXTURENAME, &g_pTextureExplosion)))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//
	//�o���b�g���_���b�N
	//////////////////////
	g_pVtxBufferExplosion->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexExplosion(pD3DDevice, pVtx);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		int k = i * 4;
		pVtx[0 + k].pos = D3DXVECTOR3(-EXPLOSION_RADIUS*0.5, EXPLOSION_RADIUS*0.5, 0.0f);
		pVtx[1 + k].pos = D3DXVECTOR3(EXPLOSION_RADIUS*0.5, EXPLOSION_RADIUS*0.5, 0.0f);
		pVtx[2 + k].pos = D3DXVECTOR3(-EXPLOSION_RADIUS*0.5, -EXPLOSION_RADIUS*0.5, 0.0f);
		pVtx[3 + k].pos = D3DXVECTOR3(EXPLOSION_RADIUS*0.5, -EXPLOSION_RADIUS*0.5, 0.0f);

		pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 20);
		pVtx[1 + k].color = D3DCOLOR_RGBA(255, 255, 255, 20);
		pVtx[2 + k].color = D3DCOLOR_RGBA(255, 255, 255, 20);
		pVtx[3 + k].color = D3DCOLOR_RGBA(255, 255, 255, 20);

		//�p�����[�^������
		g_Explosion[i].m_ExplosionMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Explosion[i].m_ExplosionMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[i].m_ExplosionMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Explosion[i].bUse = false;
		g_Explosion[i].nFrame = 0;
	}

	g_pVtxBufferExplosion->Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}

//
//�|���S���̍X�V
/////////////////////////////
void UpdateExplosion(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^

	int k = 0;       //�v�Z�p
	float fTexPosU, fTexPosV;//UV�ʒu

	//
	//�o���b�g�G�t�F�N�g���_���b�N
	////////////////////////////////
	g_pVtxBufferExplosion->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexExplosion(pD3DDevice, pVtx);  //�ݒ�
	//���_�ݒ�
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Explosion[i].bUse == true)
		{
			k = i * 4;
			fTexPosU = (g_Explosion[i].nFrame % 3)*TEX_WIDTH;
			fTexPosV = (g_Explosion[i].nFrame / 3)*TEX_HEIGHT;
			pVtx[0 + k].pos = D3DXVECTOR3(-EXPLOSION_RADIUS*0.5, EXPLOSION_RADIUS*0.5, 0.0f);
			pVtx[1 + k].pos = D3DXVECTOR3(EXPLOSION_RADIUS*0.5, EXPLOSION_RADIUS*0.5, 0.0f);
			pVtx[2 + k].pos = D3DXVECTOR3(-EXPLOSION_RADIUS*0.5, -EXPLOSION_RADIUS*0.5, 0.0f);
			pVtx[3 + k].pos = D3DXVECTOR3(EXPLOSION_RADIUS*0.5, -EXPLOSION_RADIUS*0.5, 0.0f);

			pVtx[0 + k].tex = D3DXVECTOR2(fTexPosU, fTexPosV);
			pVtx[1 + k].tex = D3DXVECTOR2(fTexPosU + TEX_WIDTH, fTexPosV);
			pVtx[2 + k].tex = D3DXVECTOR2(fTexPosU, fTexPosV + TEX_HEIGHT);
			pVtx[3 + k].tex = D3DXVECTOR2(fTexPosU + TEX_WIDTH, fTexPosV + TEX_HEIGHT);

			pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�ω�
			g_Explosion[i].nFrame++;     //�t���[��������

			//���ʐ؂�
			if (g_Explosion[i].nFrame >= FRAME_MAX)
			{
				g_Explosion[i].bUse = false;
			}
		}
	}

	g_pVtxBufferExplosion->Unlock();              //�A�����b�N
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���

	//
	//���_�t�H�[�}�b�g�ݒ�
	///////////////////////////////
	pD3DDevice->SetStreamSource(0, g_pVtxBufferExplosion, 0, sizeof(VERTEX_3D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX3D);
	pD3DDevice->SetTexture(0, g_pTextureExplosion);                                       //�e�N�X�`���̐ݒ�

	//�`��
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�o���b�g���g�p��
		if (g_Explosion[i].bUse == true)
		{
			SetBillboardMatrix(&g_Explosion[i].m_ExplosionWorldMatrix, g_Explosion[i].m_ExplosionMatrix);      //�r���{�[�h�}�g���N�X�Z�b�g
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);                         //�`��
		}
	}

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g���I���ɂ���
}

void UninitExplosion(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE(g_pTextureExplosion);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��

	SAFE_RELEASE(g_pVtxBufferExplosion);                       //���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
}

//�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************�f�o�C�X�ݒ�***********************************************/
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//
//�����G�t�F�N�g�Z�b�g
//////////////////////////////
void SetExplosion(D3DXVECTOR3 pos)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Explosion[i].bUse == false)
		{
			//���[�J������
			g_Explosion[i].m_ExplosionMatrix.pos = pos;
			//�}�g���N�X����
			g_Explosion[i].m_ExplosionMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Explosion[i].m_ExplosionMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//���C�t�ɂ��ω��ʌv�Z
			g_Explosion[i].nFrame = 0;
			//�t���O
			g_Explosion[i].bUse = true;
			break;
		}
	}
}
