//-----------------------------------------------------------------------------------------
//�v���[���[,����Ȃǂ̏���[running02.cpp]
//author:syuusyou i
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------------------
#include <time.h>
#include "main.h"
#include "particalCenter.h"
#include "partical.h"
#include "camera.h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//�}�N��
//-----------------------------------------------------------------------------------------
#define PARTICALRADIUS (10)
#define PLUS_EFFECT_NUM (3)
//-----------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------------------------------------------------------
void UpdateParticalBase(void);
void UpdateParticalFire(void);
void QuickSort_Draw(void);                   //�N�C�b�N�\�[�g�@���g���ăp�[�e�B�N���̕`�揇�����߂�
void QuickSort(int nLeftID, int nRightID);                       //�ċA�I�ȃN���b�N�\�[�g�֐�
void DrawBase(void);
void DrawFire(void);

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureParticalPolygon[NUM_PARTICALTEXTURE] = {};           //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticalPolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFirePartical = NULL;      //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)

PARTICAL g_Partical[MAX_PARTICAL][PLUS_EFFECT_NUM];
PARTICAL g_ParticalFire[MAX_PARTICAL][PLUS_EFFECT_NUM];                     //���̃p�[�e�B�N��

//�N�C�b�N�\�[�g�p�ϐ�
int g_aQuickSortID[MAX_PARTICAL];                 //����̌�̃N�C�b�N�\�[�g
float g_aDistance[MAX_PARTICAL];                      //�p�[�e�B�N������J�����܂ł̋����z��
int g_nUseParticalNum;                             //�g�p���̃p�[�e�B�N������

//-----------------------------------------------------------------------------------------
//�d�v��Ԋ֐��B
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�e�ϐ��̏�����
//-----------------------------------------------------------------------------------------
HRESULT InitPartical(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();
	 
	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_PARTICALVERTEX * MAX_PARTICAL * PLUS_EFFECT_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferParticalPolygon, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_PARTICALVERTEX * MAX_PARTICAL * PLUS_EFFECT_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVtxBufferFirePartical, NULL)))
	{
		return E_FAIL;
	}

	//�p�[�e�B�N���̃e�N�X�`���擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALPOLYGON01_TEXTURENAME, &g_pTextureParticalPolygon[0])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}
	//���G�t�F�N�g�̃e�N�X�`���擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, PARTICALPOLYGONEFFECT_TEXTURENAME, &g_pTextureParticalPolygon[1])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}
	//���G�t�F�N�g�̃e�N�X�`���擾
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, FIREEFFECT_TEXTURENAME, &g_pTextureParticalPolygon[2])))
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//�v���[���[�̃|���S��
	g_pVtxBufferParticalPolygon->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPartical(pD3DDevice, pVtx);

	g_nUseParticalNum = 0;
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
		{
			//�p�[�e�B�N���̏��f�[�^�̏����l
			D3DXMatrixIdentity(&g_Partical[nCnt][nCnt2].MtxWorld);  //���[���h�}�g���N�X������

			g_Partical[nCnt][nCnt2].MtxSRT.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Partical[nCnt][nCnt2].MtxSRT.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Partical[nCnt][nCnt2].MtxSRT.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Partical[nCnt][nCnt2].speed.x = 0;
			g_Partical[nCnt][nCnt2].speed.y = 0;
			g_Partical[nCnt][nCnt2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_Partical[nCnt][nCnt2].nAlpha = 255;
			g_Partical[nCnt][nCnt2].nLife = 20;
			g_Partical[nCnt][nCnt2].bUse = false;
			g_Partical[nCnt][nCnt2].fRadius = 0;
			g_Partical[nCnt][nCnt2].fColorAlphaValue = 255 / g_Partical[nCnt][nCnt2].nLife;

			pVtx[0].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);

			pVtx += 4;
		}
	}

	g_pVtxBufferParticalPolygon->Unlock();                     //�o�b�t�@�̃A�����b�N

	//fire�p�[�e�B�N�������b�N
	g_pVtxBufferFirePartical->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	//fire�p�[�e�B�N��������
	for(int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
		{
			D3DXMatrixIdentity(&g_ParticalFire[nCnt][nCnt2].MtxWorld);  //���[���h�}�g���N�X������
			g_ParticalFire[nCnt][nCnt2].MtxSRT.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_ParticalFire[nCnt][nCnt2].MtxSRT.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_ParticalFire[nCnt][nCnt2].MtxSRT.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_ParticalFire[nCnt][nCnt2].speed.x = 0;
			g_ParticalFire[nCnt][nCnt2].speed.y = 0;
			g_ParticalFire[nCnt][nCnt2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_ParticalFire[nCnt][nCnt2].nAlpha = 255;
			g_ParticalFire[nCnt][nCnt2].nLife = 20;
			g_ParticalFire[nCnt][nCnt2].bUse = false;
			g_ParticalFire[nCnt][nCnt2].fRadius = 0;
			g_ParticalFire[nCnt][nCnt2].fColorAlphaValue = 255 / g_ParticalFire[nCnt][nCnt2].nLife;

			pVtx[0].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PARTICALRADIUS, PARTICALRADIUS, 0.0f);

			pVtx += 4;
		}
	}

	g_pVtxBufferFirePartical->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����
	
	return S_OK;
}

//-----------------------------------------------------------------------------------------
//�I������
//-----------------------------------------------------------------------------------------
void UninitPartical(void)
{
	// �e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	for (int nCnt = 0; nCnt < NUM_PARTICALTEXTURE; nCnt++)
	{
		SAFE_RELEASE(g_pTextureParticalPolygon[nCnt]);
	}

	//���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE(g_pVtxBufferParticalPolygon);
	SAFE_RELEASE(g_pVtxBufferFirePartical);
}


//-----------------------------------------------------------------------------------------
//�p�[�e�B�N���X�V����
//-----------------------------------------------------------------------------------------
void UpdatePartical(void)
{
	UpdateParticalBase();
	UpdateParticalFire();
	UpdatePolygonPartical();
}
//-----------------------------------------------------------------------------------------
//BASE
//-----------------------------------------------------------------------------------------
void UpdateParticalBase(void)
{
	//�p�[�e�B�N�����˒��S�f�[�^�擾
	PARTICALCENTER *pParticalCenter;
	pParticalCenter = GetParticalCenter();

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//�����p�[�e�B�N���͎g�p�̏�ԂȂ�
		if (g_Partical[nCnt][0].bUse == true)
		{
			//���͌v�Z
			g_Partical[nCnt][0].speed.x += pParticalCenter->fWindPower * cos(pParticalCenter->fWindAngel);
			g_Partical[nCnt][0].speed.y += pParticalCenter->fWindPower * sin(pParticalCenter->fWindAngel);

			//�d�͌v�Z
			g_Partical[nCnt][0].speed.y -= pParticalCenter->fGravity;

			//�p�[�e�B�N���ړ�
			g_Partical[nCnt][0].MtxSRT.pos.x += g_Partical[nCnt][0].speed.x;
			g_Partical[nCnt][0].MtxSRT.pos.y += g_Partical[nCnt][0].speed.y;
			g_Partical[nCnt][0].MtxSRT.pos.z += g_Partical[nCnt][0].speed.z;

			//�p�[�e�B�N���I������
			g_Partical[nCnt][0].nLife--;                                   //Life������
			g_Partical[nCnt][0].nAlpha -= g_Partical[nCnt][0].fColorAlphaValue; //���l������

			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				//�G�t�F�N�g����
				g_Partical[nCnt][nCnt2].nAlpha -= g_Partical[nCnt][nCnt2].fColorAlphaValue;    //�G�t�F�N�g������

																							   //�K�v�Ȓl��U�ۑ�
				D3DCOLOR color = g_Partical[nCnt][nCnt2].color;
				float nAlpha = g_Partical[nCnt][nCnt2].nAlpha;
				float fColorAlphaValue = g_Partical[nCnt][nCnt2].fColorAlphaValue;

				g_Partical[nCnt][nCnt2] = g_Partical[nCnt][0];                    //�����G�t�F�N�g�ɗ^����

				g_Partical[nCnt][nCnt2].color = color;
				g_Partical[nCnt][nCnt2].nAlpha = nAlpha;
				g_Partical[nCnt][nCnt2].fColorAlphaValue = fColorAlphaValue;
				g_Partical[nCnt][nCnt2].fRadius = g_Partical[nCnt][1].fRadius * 2;
			}

			//life��0�ȉ��ɂȂ����疢�g�p�ɂ���
			if (g_Partical[nCnt][0].nLife <= 0)
			{
				g_Partical[nCnt][0].bUse = false;
				for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
				{
					g_Partical[nCnt][nCnt2].bUse = false;
				}
			}
		}
	}

	QuickSort_Draw();                   //�N�C�b�N�\�[�g�ŕ`�揇�ԍ쐬
}
//-----------------------------------------------------------------------------------------
//Fire
//-----------------------------------------------------------------------------------------
void UpdateParticalFire(void)
{
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//�p�[�e�B�N���ړ�
		g_ParticalFire[nCnt][0].MtxSRT.pos.x += g_ParticalFire[nCnt][0].speed.x;
		g_ParticalFire[nCnt][0].MtxSRT.pos.y += g_ParticalFire[nCnt][0].speed.y;
		g_ParticalFire[nCnt][0].MtxSRT.pos.z += g_ParticalFire[nCnt][0].speed.z;

		//�p�[�e�B�N���I������
		g_ParticalFire[nCnt][0].nLife--;                                   //Life������

		for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
		{
			//�G�t�F�N�g����
			g_ParticalFire[nCnt][nCnt2].nAlpha -= g_ParticalFire[nCnt][nCnt2].fColorAlphaValue;    //�G�t�F�N�g������

																						   //�K�v�Ȓl��U�ۑ�
			D3DCOLOR color = g_ParticalFire[nCnt][nCnt2].color;
			float nAlpha = g_ParticalFire[nCnt][nCnt2].nAlpha;
			float fColorAlphaValue = g_ParticalFire[nCnt][nCnt2].fColorAlphaValue;

			g_ParticalFire[nCnt][nCnt2] = g_ParticalFire[nCnt][0];                    //�����G�t�F�N�g�ɗ^����

			g_ParticalFire[nCnt][nCnt2].color = color;
			g_ParticalFire[nCnt][nCnt2].nAlpha = nAlpha;
			g_ParticalFire[nCnt][nCnt2].fColorAlphaValue = fColorAlphaValue;
			g_ParticalFire[nCnt][nCnt2].fRadius = g_ParticalFire[nCnt][1].fRadius * 2;

			//life��0�ȉ��ɂȂ����疢�g�p�ɂ���
			if (g_ParticalFire[nCnt][0].nLife <= 0)
			{
				g_ParticalFire[nCnt][0].bUse = false;
				for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
				{
					g_ParticalFire[nCnt][nCnt2].bUse = false;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
//�v���[���[�|���S���̃f�[�^�X�V
//-----------------------------------------------------------------------------------------
void UpdatePolygonPartical(void)
{
	VERTEX_3D *pVtx = NULL;
	VERTEX_3D *pVtxFire = NULL;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	g_pVtxBufferParticalPolygon -> Lock(0, 0, (void**)&pVtx, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����
	MakeVertexPolygonPartical(pD3DDevice, pVtx);

	//�p�[�e�B�N�����b�N�A�����b�N
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if(g_Partical[nCnt][0].bUse == true)
		{
			int k = nCnt * 4 * 2;
			pVtx[0 + k].pos = D3DXVECTOR3(-g_Partical[nCnt][0].fRadius,g_Partical[nCnt][0].fRadius, 0.0f);
			pVtx[1 + k].pos = D3DXVECTOR3(g_Partical[nCnt][0].fRadius,g_Partical[nCnt][0].fRadius, 0.0f);
			pVtx[2 + k].pos = D3DXVECTOR3(-g_Partical[nCnt][0].fRadius,-g_Partical[nCnt][0].fRadius, 0.0f);
			pVtx[3 + k].pos = D3DXVECTOR3(g_Partical[nCnt][0].fRadius,-g_Partical[nCnt][0].fRadius, 0.0f);

			g_Partical[nCnt][0].color = D3DCOLOR_RGBA(70,70,0,g_Partical[nCnt][0].nAlpha);

			pVtx[0 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[0+k].color = g_Partical[nCnt][0].color;
			pVtx[1+k].color = g_Partical[nCnt][0].color;
			pVtx[2+k].color = g_Partical[nCnt][0].color;
			pVtx[3+k].color = g_Partical[nCnt][0].color;

			pVtx[0+k].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1+k].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2+k].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3+k].tex = D3DXVECTOR2(1.0f, 1.0f);

			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				int p = k + 4 * nCnt2;

				//�G�t�F�N�g����
				pVtx[0 + p].pos = D3DXVECTOR3(-g_Partical[nCnt][nCnt2].fRadius, g_Partical[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[1 + p].pos = D3DXVECTOR3(g_Partical[nCnt][nCnt2].fRadius, g_Partical[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[2 + p].pos = D3DXVECTOR3(-g_Partical[nCnt][nCnt2].fRadius, -g_Partical[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[3 + p].pos = D3DXVECTOR3(g_Partical[nCnt][nCnt2].fRadius, -g_Partical[nCnt][nCnt2].fRadius, 0.0f);

				g_Partical[nCnt][nCnt2].color = D3DCOLOR_RGBA(5, 102, 184, 255);   //����RGB

				pVtx[0 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[1 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[2 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[3 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				pVtx[0 + p].color = g_Partical[nCnt][nCnt2].color;
				pVtx[1 + p].color = g_Partical[nCnt][nCnt2].color;
				pVtx[2 + p].color = g_Partical[nCnt][nCnt2].color;
				pVtx[3 + p].color = g_Partical[nCnt][nCnt2].color;

				pVtx[0 + p].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1 + p].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2 + p].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3 + p].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}

	g_pVtxBufferParticalPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	//���̃p�[�e�B�N�����_���b�N
	g_pVtxBufferFirePartical->Lock(0, 0, (void**)&pVtxFire, 0);          //���z�A�h���X���擾���o�b�t�@�����b�N����

	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if (g_ParticalFire[nCnt][0].bUse == true)
		{
			int k = nCnt * 4 * 2;
			pVtx[0 + k].pos = D3DXVECTOR3(-g_ParticalFire[nCnt][0].fRadius, g_ParticalFire[nCnt][0].fRadius, 0.0f);
			pVtx[1 + k].pos = D3DXVECTOR3(g_ParticalFire[nCnt][0].fRadius, g_ParticalFire[nCnt][0].fRadius, 0.0f);
			pVtx[2 + k].pos = D3DXVECTOR3(-g_ParticalFire[nCnt][0].fRadius, -g_ParticalFire[nCnt][0].fRadius, 0.0f);
			pVtx[3 + k].pos = D3DXVECTOR3(g_ParticalFire[nCnt][0].fRadius, -g_ParticalFire[nCnt][0].fRadius, 0.0f);

			g_ParticalFire[nCnt][0].color = D3DCOLOR_RGBA(70, 70, 0, g_ParticalFire[nCnt][0].nAlpha);

			pVtx[0 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3 + k].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[0 + k].color = g_ParticalFire[nCnt][0].color;
			pVtx[1 + k].color = g_ParticalFire[nCnt][0].color;
			pVtx[2 + k].color = g_ParticalFire[nCnt][0].color;
			pVtx[3 + k].color = g_ParticalFire[nCnt][0].color;

			pVtx[0 + k].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1 + k].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2 + k].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3 + k].tex = D3DXVECTOR2(1.0f, 1.0f);

			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				int p = k + 4 * nCnt2;

				//�G�t�F�N�g����
				pVtx[0 + p].pos = D3DXVECTOR3(-g_ParticalFire[nCnt][nCnt2].fRadius, g_ParticalFire[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[1 + p].pos = D3DXVECTOR3(g_ParticalFire[nCnt][nCnt2].fRadius, g_ParticalFire[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[2 + p].pos = D3DXVECTOR3(-g_ParticalFire[nCnt][nCnt2].fRadius, -g_ParticalFire[nCnt][nCnt2].fRadius, 0.0f);
				pVtx[3 + p].pos = D3DXVECTOR3(g_ParticalFire[nCnt][nCnt2].fRadius, -g_ParticalFire[nCnt][nCnt2].fRadius, 0.0f);

				g_ParticalFire[nCnt][nCnt2].color = D3DCOLOR_RGBA(5, 102, 184, 255);   //����RGB

				pVtx[0 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[1 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[2 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				pVtx[3 + p].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				pVtx[0 + p].color = g_ParticalFire[nCnt][nCnt2].color;
				pVtx[1 + p].color = g_ParticalFire[nCnt][nCnt2].color;
				pVtx[2 + p].color = g_ParticalFire[nCnt][nCnt2].color;
				pVtx[3 + p].color = g_ParticalFire[nCnt][nCnt2].color;

				pVtx[0 + p].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1 + p].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2 + p].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3 + p].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}
	g_pVtxBufferFirePartical->Unlock();                                     //�o�b�t�@�̃A�����b�N
}

//-----------------------------------------------------------------------------------------
//�p�[�e�B�N���`��
//-----------------------------------------------------------------------------------------
void DrawPartical(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���

	pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);                               //z�o�b�t�@���Q��
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);                               //z���c���Ȃ�

	DrawBase();//���p�[�e�B�N���`��

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //�u�����h���[�h���A���t�@�u�����h�ɖ߂�

	DrawFire();//���p�[�e�B�N���`��

	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //�u�����h���[�h���A���t�@�u�����h�ɖ߂�
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      //z���c��
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g���I���ɂ���
}

//�x�[�X
void DrawBase(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	for (int nCnt = g_nUseParticalNum - 1; nCnt >= 0; nCnt--)
	{
		//�p�[�e�B�N�����g���Ă���
		if (g_Partical[g_aQuickSortID[nCnt]][0].bUse == true)
		{
			int k = g_aQuickSortID[nCnt] * 4 * 2;

			SetBillboardMatrix(&g_Partical[g_aQuickSortID[nCnt]][0].MtxWorld, g_Partical[g_aQuickSortID[nCnt]][0].MtxSRT);

			//�p�[�e�B�N��
			pD3DDevice->SetStreamSource(0, g_pVtxBufferParticalPolygon, 0, sizeof(VERTEX_3D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
			pD3DDevice->SetFVF(FVF_VERTEX3D);
			pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[0]);				 //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, k, 2);                                            //�X�v���C�g�`��

																											 //�G�t�F�N�g
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                             //���Z�������s��!
			pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[1]);				 //�e�N�X�`���̐ݒ�
			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				int p = k + 4 * nCnt2;
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, p, 2);                                            //�G�t�F�N�g�`��
			}
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //�u�����h���[�h���A���t�@�u�����h�ɖ߂�
		}
	}
}

//��
void DrawFire(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//���p�[�e�B�N��
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		//�p�[�e�B�N�����g���Ă���
		if (g_ParticalFire[nCnt][0].bUse == true)
		{
			int k = nCnt * 4 * 2;

			SetBillboardMatrix(&g_ParticalFire[nCnt][0].MtxWorld, g_ParticalFire[nCnt][0].MtxSRT);

			//�p�[�e�B�N��
			pD3DDevice->SetStreamSource(0, g_pVtxBufferFirePartical, 0, sizeof(VERTEX_3D));              //VRAM����GPU�Ƀf�[�^���X�g���[��
			pD3DDevice->SetFVF(FVF_VERTEX3D);
			pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[0]);				 //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, k, 2);                                            //�X�v���C�g�`��

																											 //�G�t�F�N�g
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                       //���Z�������s��!
			pD3DDevice->SetTexture(0, g_pTextureParticalPolygon[2]);				 //�e�N�X�`���̐ݒ�
			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				int p = k + 4 * nCnt2;
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, p, 2);                                            //�G�t�F�N�g�`��
			}
			pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                   //�u�����h���[�h���A���t�@�u�����h�ɖ߂�
		}
	}
}


//�v���[���[�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygonPartical(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//----------------------------------
//�p�[�e�B�N����ݒu
//----------------------------------
void SetPartical(float fSpeedX,float fSpeedY, float fSpeedZ,float fRadius,float nLife)
{
	for(int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if(g_Partical[nCnt][0].bUse == false)
		{
			D3DXMatrixIdentity(&g_Partical[nCnt][0].MtxWorld);  //���[���h�}�g���N�X������
			g_Partical[nCnt][0].MtxSRT.pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_Partical[nCnt][0].MtxSRT.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Partical[nCnt][0].MtxSRT.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_Partical[nCnt][0].speed.x = fSpeedX;
			g_Partical[nCnt][0].speed.y = fSpeedY;
			g_Partical[nCnt][0].speed.z = fSpeedZ;
			g_Partical[nCnt][0].color = D3DCOLOR_RGBA(255,255,255,255);
			g_Partical[nCnt][0].fRadius = fRadius;
			g_Partical[nCnt][0].nLife = nLife;
			g_Partical[nCnt][0].nAlpha = 20;
			g_Partical[nCnt][0].fColorAlphaValue = g_Partical[nCnt][0].nAlpha / g_Partical[nCnt][0].nLife;
			g_Partical[nCnt][0].bUse = true;
			g_Partical[nCnt][0].nID = nCnt;

			//�G�t�F�N�g�̕���true�ɂ���
			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				g_Partical[nCnt][nCnt2] = g_Partical[nCnt][0];
				g_Partical[nCnt][nCnt2].nAlpha = 255;
				g_Partical[nCnt][nCnt2].fColorAlphaValue = g_Partical[nCnt][nCnt2].nAlpha / g_Partical[nCnt][nCnt2].nLife;
				g_Partical[nCnt][nCnt2].bUse = true; 
			}
			break;
		}
	}
}

//----------------------------------
//���p�[�e�B�N��
//----------------------------------
void SetFirePartical(PARTICALCENTER *pParticalCenter, float fFirePosX ,float fSpeedX, float fSpeedY, float fSpeedZ, float nLife)
{
	for (int nCnt = 0; nCnt < MAX_PARTICAL; nCnt++)
	{
		if (g_ParticalFire[nCnt][0].bUse == false)
		{
			D3DXMatrixIdentity(&g_ParticalFire[nCnt][0].MtxWorld);  //���[���h�}�g���N�X������
			g_ParticalFire[nCnt][0].MtxSRT.pos = D3DXVECTOR3(pParticalCenter->pos.x + fFirePosX, pParticalCenter->pos.y, pParticalCenter->pos.z);
			g_ParticalFire[nCnt][0].MtxSRT.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_ParticalFire[nCnt][0].MtxSRT.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_ParticalFire[nCnt][0].speed.x = fSpeedX;
			g_ParticalFire[nCnt][0].speed.y = fSpeedY;
			g_ParticalFire[nCnt][0].speed.z = fSpeedZ;
			g_ParticalFire[nCnt][0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			g_ParticalFire[nCnt][0].fRadius = pParticalCenter->fRadius;
			g_ParticalFire[nCnt][0].nLife = nLife;
			g_ParticalFire[nCnt][0].nAlpha = 20;
			g_ParticalFire[nCnt][0].fColorAlphaValue = g_ParticalFire[nCnt][0].nAlpha / g_ParticalFire[nCnt][0].nLife;
			g_ParticalFire[nCnt][0].bUse = true;
			g_ParticalFire[nCnt][0].nID = nCnt;

			//�G�t�F�N�g�̕���true�ɂ���
			for (int nCnt2 = 1; nCnt2 < PLUS_EFFECT_NUM; nCnt2++)
			{
				g_ParticalFire[nCnt][nCnt2] = g_ParticalFire[nCnt][0];
				g_ParticalFire[nCnt][nCnt2].nAlpha = 255;
				g_ParticalFire[nCnt][nCnt2].fColorAlphaValue = g_ParticalFire[nCnt][nCnt2].nAlpha / g_ParticalFire[nCnt][nCnt2].nLife;
				g_ParticalFire[nCnt][nCnt2].bUse = true;
			}

			break;
		}
	}
}

//----------------------------------
//�N�C�b�N�\�[�g�ŕ`�揇�Ԃ����߂�
//----------------------------------
void QuickSort_Draw(void)
{
	CAMERA_FPS *pCameraFps = GetCameraFps();            //�J�����擾
	int nUseParticalNum = 0;                          //�g�p���̃p�[�e�B�N����

	for (int nCnt = 0; nCnt < MAX_PARTICAL;nCnt++)
	{
		//������ID�̓o�^
		if (g_Partical[nCnt][0].bUse == true)
		{
			//�����v�Z
			float x = g_Partical[nCnt][0].MtxSRT.pos.x - pCameraFps->pos.x;
			float y = g_Partical[nCnt][0].MtxSRT.pos.y - pCameraFps->pos.y;
			float z = g_Partical[nCnt][0].MtxSRT.pos.z - pCameraFps->pos.z;
			g_aDistance[nUseParticalNum] = x*x + y*y + z*z;
			g_aQuickSortID[nUseParticalNum] = g_Partical[nCnt][0].nID;                //ID�o�^
			nUseParticalNum++; //�J�E���g�A�b�v
		}
	}

	g_nUseParticalNum = nUseParticalNum;                   //������n��
	//QuickSort(0, nUseParticalNum-1);                       //�N�C�b�N�\�[�g
}

//----------------------------------
//�N�C�b�N�\�[�g:����1:START_ID ����2:END_ID
//----------------------------------
void QuickSort(int nLeftID, int nRightID)
{
	//---------------------------
	//�ϐ��錾
	//---------------------------
	int nCnt = 0;           //�J�E���g
	int nTotalNum = nRightID - nLeftID + 1;              //����
	int nMiddleID = (nLeftID + nRightID) / 2;
	int nLeftNum_BE = nMiddleID - nLeftID;           //�����O�̒��Ԓl�̍����̔z�񃁃��o��
	int nRightNum_BE = nRightID - nMiddleID;         //�����O�̒��Ԓl�̉E���̔z�񃁃��o��
	int nLeftNum_AF = 0; //������̒��Ԓl�̍����̔z�񃁃��o��
	int nRightNum_AF = 0; //������̒��Ԓl�̉E���̔z�񃁃��o��

	int aQuickSortIDL[MAX_PARTICAL];                 //�N�C�b�N�\�[�gID���z��
	int aQuickSortIDR[MAX_PARTICAL];                 //�N�C�b�N�\�[�gID�E�z��
	float aDistanceL[MAX_PARTICAL];                    //�p�[�e�B�N������J�����܂ł̋������z��
	float aDistanceR[MAX_PARTICAL];                    //�p�[�e�B�N������J�����܂ł̋����E�z��

	//---------------------------
	//���ƉE����
	//---------------------------
	for (int i = 1; i <= nLeftNum_BE; i++)
	{
		//---------------------------
		//���Ԓl�O�̃����o�l�`�F�b�N
		//---------------------------
		//���z��ɓ����
		if (g_aDistance[nMiddleID - i] < g_aDistance[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = g_aDistance[nMiddleID - i];          //�����z��ɓ����
			aQuickSortIDL[nLeftNum_AF] = g_aQuickSortID[nMiddleID - i];    //ID�z��ɓ����
			nLeftNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
		//�E�z��ɓ����
		else
		{
			aDistanceR[nRightNum_AF] = g_aDistance[nMiddleID - i];  //�z��ɓ����
			aQuickSortIDR[nRightNum_AF] = g_aQuickSortID[nMiddleID - i];    //ID�z��ɓ����
			nRightNum_AF++;                                         //�����o���J�E���g�A�b�v
		}

		//---------------------------
		//���Ԓl���̃����o�l�`�F�b�N
		//---------------------------
		//���z��ɓ����
		if (g_aDistance[nMiddleID + i] < g_aDistance[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = g_aDistance[nMiddleID + i];          //�����z��ɓ����
			aQuickSortIDL[nLeftNum_AF] = g_aQuickSortID[nMiddleID + i];    //ID�z��ɓ����
			nLeftNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
		//�E�z��ɓ����
		else
		{
			aDistanceR[nRightNum_AF] = g_aDistance[nMiddleID + i];  //�z��ɓ����
			aQuickSortIDR[nRightNum_AF] = g_aQuickSortID[nMiddleID + i];    //ID�z��ɓ����
			nRightNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
	}

	//�����������̏ꍇ
	if(nRightNum_BE > nLeftNum_BE)
	{
		//���z��ɓ����
		if (g_aDistance[nRightID] < g_aDistance[nMiddleID])
		{
			aDistanceL[nLeftNum_AF] = g_aDistance[nRightID];          //�����z��ɓ����
			aQuickSortIDL[nLeftNum_AF] = g_aQuickSortID[nRightID];    //ID�z��ɓ����
			nLeftNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
		//�E�z��ɓ����
		else
		{
			aDistanceR[nRightNum_AF] = g_aDistance[nRightID];  //�z��ɓ����
			aQuickSortIDR[nRightNum_AF] = g_aQuickSortID[nRightID];    //ID�z��ɓ����
			nRightNum_AF++;                                         //�����o���J�E���g�A�b�v
		}
	}

	//----------------------------
	//�����̌�̔z���z��ɔ��f����
	//----------------------------
	nCnt = nLeftID;              //�J�E���g������
	float MIDistance = g_aDistance[nMiddleID];      //���Ԓl�ۑ�
	int MIID = g_aQuickSortID[nMiddleID];      //���Ԓl�ۑ�

	//������
	for(int j = 0; j < nLeftNum_AF; j++)
	{
		g_aDistance[nCnt] = aDistanceL[j];        //�������
		g_aQuickSortID[nCnt] = aQuickSortIDL[j];  //ID���
		nCnt++;            //�J�E���g�A�b�v
	}

	//���Ԓl
	g_aDistance[nCnt] = MIDistance;        //�������
	g_aQuickSortID[nCnt] = MIID;  //ID���
	nCnt++;       //�J�E���g�A�b�v

	//�E����
	for (int j = 0; j < nRightNum_AF; j++)
	{
		g_aDistance[nCnt] = aDistanceR[j];        //�������
		g_aQuickSortID[nCnt] = aQuickSortIDR[j];  //ID���
		nCnt++;            //�J�E���g�A�b�v
	}

	//------------------
	//�ċA����
	//------------------
	//������
	if(nLeftNum_AF > 1)
	{
		QuickSort(nLeftID, nLeftID+ nLeftNum_AF-1);
	}

	//�E����
	if (nRightNum_AF > 1)
	{
		QuickSort(nRightID- nRightNum_AF+1, nRightID);
	}
}