//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "hitRectangle .h"
#include "camera.h"
#include "TpsCamera.h"
#include "hitRectangle .h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//�}�O��
//-----------------------------------------------------------------------------------------
#define HR_COLOR_R (0)
#define HR_COLOR_G (120)
#define HR_COLOR_B (135)
#define HR_COLOR_ALPHA (100)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureHitRect = NULL;          //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferHitRect = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
HITRECT g_HitRect[HITRECT_MAX];                  //���_���蔻��z��

#ifdef _DEBUG
bool g_Check;
#endif

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitHitRect(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_3D)*HITRECT_MAX *4,D3DUSAGE_WRITEONLY,FVF_VERTEX3D,D3DPOOL_MANAGED,&g_pVtxBufferHitRect,NULL)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���ǂݍ���
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, HITRECT_TEXTURENAME,&g_pTextureHitRect) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	MakeVertexHitRect(pD3DDevice, pVtx);
	g_pVtxBufferHitRect->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

#ifdef _DEBUG
	g_Check = false;
#endif

	for (int i = 0; i < HITRECT_MAX; i++)
	{
		//���f�����[���h�ϊ��}�g���N�X�̏�����
		g_HitRect[i].m_HitRectMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_HitRect[i].m_HitRectMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_HitRect[i].m_HitRectMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_HitRect[i].bUse = false;

		//���_�f�[�^������
		for(int j = 0; j < 4; j++)
		{
			g_HitRect[i].Vectex[j].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_HitRect[i].Vectex[j].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_HitRect[i].Vectex[j].tex = D3DXVECTOR2(0.0f,0.0f);
			g_HitRect[i].Vectex[j].color = D3DCOLOR_RGBA(255,255,255,255);
			g_HitRect[i].VectexC[j] = g_HitRect[i].Vectex[j];
		}

		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	g_pVtxBufferHitRect->Unlock();                                     //�o�b�t�@�̃A�����b�N
	return S_OK;
}

//�|���S���̍X�V
void UpdateHitRect(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^

													  //
													  //�o���b�g�G�t�F�N�g���_���b�N
													  ////////////////////////////////
	g_pVtxBufferHitRect->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexHitRect(pD3DDevice, pVtx);  //�ݒ�
	for (int i = 0; i < HITRECT_MAX; i++)
	{
		if (g_HitRect[i].bUse == true)
		{
			int k = i * 4;
			pVtx[0 + k].pos = g_HitRect[i].VectexC[0].pos;
			pVtx[1 + k].pos = g_HitRect[i].VectexC[1].pos;
			pVtx[2 + k].pos = g_HitRect[i].VectexC[2].pos;
			pVtx[3 + k].pos = g_HitRect[i].VectexC[3].pos;

			pVtx[0 + k].tex = g_HitRect[i].VectexC[0].tex;
			pVtx[1 + k].tex = g_HitRect[i].VectexC[1].tex;
			pVtx[2 + k].tex = g_HitRect[i].VectexC[2].tex;
			pVtx[3 + k].tex = g_HitRect[i].VectexC[3].tex;

			pVtx[0 + k].color = g_HitRect[i].VectexC[0].color;
			pVtx[1 + k].color = g_HitRect[i].VectexC[1].color;
			pVtx[2 + k].color = g_HitRect[i].VectexC[2].color;
			pVtx[3 + k].color = g_HitRect[i].VectexC[3].color;

			pVtx[0 + k].normal = g_HitRect[i].VectexC[0].normal;
			pVtx[1 + k].normal = g_HitRect[i].VectexC[1].normal;
			pVtx[2 + k].normal = g_HitRect[i].VectexC[2].normal;
			pVtx[3 + k].normal = g_HitRect[i].VectexC[3].normal;
		}
	}

	g_pVtxBufferHitRect->Unlock();              //�A�����b�N
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawHitRect(void)
{
#ifdef _DEBUG
	if(GetKeyboardTrigger(DIK_F1))
	{
		if (g_Check == false) g_Check = true;
		else g_Check = false;
	}
	if (g_Check == true)
	{
		LPDIRECT3DDEVICE9 pD3DDevice;
		pD3DDevice = Get_Device();
		D3DXMATRIX IdentityMtx;
		pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���
		D3DXMatrixIdentity(&IdentityMtx);                    //�P�ʍs��ŏ�����
		pD3DDevice->SetTransform(D3DTS_WORLD, &IdentityMtx);                //���[���h�ϊ��s��

		//�R�x�j�����:���_�t�H�[�}�b�g�ݒ�
		for (int i = 0; i < HITRECT_MAX; i++)
		{
			if (g_HitRect[i].bUse == true)
			{
				pD3DDevice->SetStreamSource(0, g_pVtxBufferHitRect, 0, sizeof(VERTEX_3D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
				pD3DDevice->SetFVF(FVF_VERTEX3D);
				pD3DDevice->SetTexture(0, NULL);												 //�e�N�X�`���̐ݒ�
				pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);                                  //�e�N�X�`���������̊m��
			}
		}
	}
#endif
}

void UninitHitRect(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE (g_pTextureHitRect);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��

	//���_�o�b�t�@���������
	SAFE_RELEASE (g_pVtxBufferHitRect);                       //���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
}

//�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexHitRect(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************�f�o�C�X�ݒ�***********************************************/
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//����l�p�|���S���Z�b�g
void SetHitRect(const WORLD_MATRIX *pResultMatrix, const D3DXVECTOR2 *HW, MESHTYPE meshType)
{
	int nCnt;
	float fWidthHalf, fHeightHalf;
	D3DXMATRIX rot_VecXZ;

	for (nCnt = 0; nCnt < HITRECT_MAX; nCnt++)
	{
		//����|���S�����g�p�Ȃ�
		if (g_HitRect[nCnt].bUse == false)
		{
			g_HitRect[nCnt].m_HitRectMatrix = *pResultMatrix;
			g_HitRect[nCnt].meshType = meshType;
			fWidthHalf = HW->x*0.5;
			fHeightHalf = HW->y*0.5;
			//
			//���_�f�[�^�̏�����
			///////////////////////////////////////////
			g_HitRect[nCnt].Vectex[0].pos = D3DXVECTOR3(-fWidthHalf, 0.0f, fHeightHalf);
			g_HitRect[nCnt].Vectex[1].pos = D3DXVECTOR3(fWidthHalf, 0.0f, fHeightHalf);
			g_HitRect[nCnt].Vectex[2].pos = D3DXVECTOR3(-fWidthHalf, 0.0f, -fHeightHalf);
			g_HitRect[nCnt].Vectex[3].pos = D3DXVECTOR3(fWidthHalf, 0.0f, -fHeightHalf);

			g_HitRect[nCnt].Vectex[0].color = D3DCOLOR_RGBA(HR_COLOR_R, HR_COLOR_G, HR_COLOR_B, HR_COLOR_ALPHA);
			g_HitRect[nCnt].Vectex[1].color = D3DCOLOR_RGBA(HR_COLOR_R, HR_COLOR_G, HR_COLOR_B, HR_COLOR_ALPHA);
			g_HitRect[nCnt].Vectex[2].color = D3DCOLOR_RGBA(HR_COLOR_R, HR_COLOR_G, HR_COLOR_B, HR_COLOR_ALPHA);
			g_HitRect[nCnt].Vectex[3].color = D3DCOLOR_RGBA(HR_COLOR_R, HR_COLOR_G, HR_COLOR_B, HR_COLOR_ALPHA);

			g_HitRect[nCnt].Vectex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_HitRect[nCnt].Vectex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_HitRect[nCnt].Vectex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			g_HitRect[nCnt].Vectex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			g_HitRect[nCnt].Vectex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			g_HitRect[nCnt].Vectex[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			g_HitRect[nCnt].Vectex[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			g_HitRect[nCnt].Vectex[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			for (int i = 0; i < 4; i++)
			{
				g_HitRect[nCnt].VectexC[i] = g_HitRect[nCnt].Vectex[i];
				SetChangeVertex(&g_HitRect[nCnt].VectexC[i].pos,&g_HitRect[nCnt].m_HitRectMatrix);     //������W�Z�b�g
				SetChangeNormarlize(&g_HitRect[nCnt].VectexC[i].normal, &g_HitRect[nCnt].m_HitRectMatrix);    //����@���Z�b�g
				D3DXVec3Normalize(&g_HitRect[nCnt].VectexC[i].normal, &g_HitRect[nCnt].VectexC[i].normal); //�P�ʃx�N�g����
			}

			//�v���[���[�Փˈړ��x�N�g���̎Z�o
			g_HitRect[nCnt].VecXZ = D3DXVECTOR3(g_HitRect[nCnt].VectexC[0].normal.x,0.0f, g_HitRect[nCnt].VectexC[0].normal.z);
			D3DXMatrixRotationYawPitchRoll(&rot_VecXZ, D3DX_PI*0.5,0.0f,0.0f);                     //Y���𒆐S��90�x��]
			D3DXVec3TransformCoord(&g_HitRect[nCnt].VecXZ, &g_HitRect[nCnt].VecXZ, &rot_VecXZ);            //�v���[���[�Փ˃x�N�g���̌v�Z
			D3DXVec3Normalize(&g_HitRect[nCnt].VecXZ, &g_HitRect[nCnt].VecXZ);                       //�P�ʃx�N�g����
			//////////////////////////////////////////////////////

			g_HitRect[nCnt].nID = nCnt;
			g_HitRect[nCnt].bUse = true;
			break;
		}
	}

#ifdef _DEBUG
	if (nCnt >= HITRECT_MAX)
	{
		MessageBox(NULL, "�g�p�\�̔���|���S������0�ȉ��ł�", "�x��", MB_OK | MB_ICONHAND);  //����|���S���g�p����MAX�ɒ�����
	}
#endif
}

//����l�p�|���S���̃f�[�^���擾
HITRECT *Get_HitRect(void)
{
	return &g_HitRect[0];
}