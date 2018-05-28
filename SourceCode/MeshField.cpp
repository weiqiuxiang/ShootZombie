//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "MeshField.h"
#include "camera.h"
#include "hitRectangle .h"
#include "matrix.h"
#include "textureManager.h"
#include "light.h"

#define SHUKUTAI_POSX (-25.0f)
#define SHUKUTAI_POSY (-2.0f)
#define SHUKUTAI_POSZ (10.0f)
#define SHUKUTAI_POSZ_UE (9.4375f)
#define SHUKUTAI_POSZ_SHITA (5.0f)
#define LENGTH_X (10.0f)    //�����`X�����̒���
#define LENGTH_Y (SHUKUTAI_POSZ_UE-SHUKUTAI_POSZ_SHITA)    //�����`Y�����̒���

//�ő�̌���l
#define YOKOMASU_MAX (200)  //���}�X�ő吔
#define TATEMASU_MAX (200)  //�c�}�X�ő吔
#define INDEX_NUM_MAX ((2+2*YOKOMASU_MAX)*TATEMASU_MAX + (TATEMASU_MAX-1)*2) //�C���f�b�N�X�̍ő吔
#define VERTEX_NUM_MAX ((YOKOMASU_MAX+1)*(TATEMASU_MAX+1))               //�ő咸�_��
#define MeshField_NUM_MAX (INDEX_NUM_MAX-2)                           //�ő�|���S����

#define NUMWALL_MAX (128)

#define HIGHWAY_TEX "data/GAMEOBJ/TEX/highway.jpg"
#define FLOOR01_TEX "data/TEXTURE/floor01.png"
#define WALL01_TEX "data/TEXTURE/wallTex02.jpg"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
MESHWALL g_MeshWall[NUMWALL_MAX];

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		//MeshField���[���h�ϊ��}�g���N�X�̏�����
		g_MeshWall[nCnt].m_MeshFieldMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_MeshWall[nCnt].m_MeshFieldMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].m_MeshFieldMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].bUse = false;
		g_MeshWall[nCnt].m_pIdxBufferFieldMeshField = NULL;
		g_MeshWall[nCnt].m_pVtxBufferMeshField = NULL;
		g_MeshWall[nCnt].m_TexNum = -1;

		//���_�o�b�t�@�̐���
		if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*VERTEX_NUM_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_MeshWall[nCnt].m_pVtxBufferMeshField, NULL)))
		{
			return E_FAIL;

		}

		//�C���f�b�N�X�o�b�t�@�̐���
		HRESULT hr;
		hr = pD3DDevice->CreateIndexBuffer(
			sizeof(WORD)*INDEX_NUM_MAX,     //�o�b�t�@��(2�o�C�g)
			D3DUSAGE_WRITEONLY,        //�g�p���@�t���O
			D3DFMT_INDEX16,              //�C���f�b�N�X�t�H�[�}�b�g(�d�����Ȃ����_����65536�͌��E)
			D3DPOOL_MANAGED,           //�������Ǘ����@
			&g_MeshWall[nCnt].m_pIdxBufferFieldMeshField,        //�C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^�̃A�h���X
			NULL);
		//���s�`�F�b�N
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}


	//�^�񒆍������H
	InitMeshWall_XZ(
		D3DXVECTOR3(0.0f, -LENGTH_X * 3, 0.0f),      //���[�J�����W
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //���[�J�����W
		1,                                             //���}�X��
		20,                                             //�c�}�X��
		LENGTH_X*20,                                             //�}�X���Ƃ�width
		LENGTH_X*6,                                             //�}�X���Ƃ�height
		MESH_FLOOR,
		HIGHWAY_TEX
	);

	//�����n��
	InitMeshWall_XZ(
		D3DXVECTOR3(-LENGTH_X*17.5, -LENGTH_X * 3, 0.0f),      //���[�J�����W
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //���[�J�����W
		15,                                             //���}�X��
		120,                                             //�c�}�X��
		LENGTH_X,                                             //�}�X���Ƃ�width
		LENGTH_X,                                             //�}�X���Ƃ�height
		MESH_FLOOR,
		FLOOR01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(LENGTH_X * 17.5, -LENGTH_X * 3, 0.0f),      //���[�J�����W
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //���[�J�����W
		15,                                             //���}�X��
		120,                                             //�c�}�X��
		LENGTH_X,                                             //�}�X���Ƃ�width
		LENGTH_X,                                             //�}�X���Ƃ�height
		MESH_FLOOR,
		FLOOR01_TEX
	);

	//�^�񒆍������H
	InitMeshWall_XZ(
		D3DXVECTOR3(0.0f, -LENGTH_X * 3, LENGTH_X*140),      //���[�J�����W
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //���[�J�����W
		1,                                             //���}�X��
		20,                                             //�c�}�X��
		LENGTH_X * 20,                                             //�}�X���Ƃ�width
		LENGTH_X * 6,                                             //�}�X���Ƃ�height
		MESH_FLOOR,
		HIGHWAY_TEX
	);

	//�����n��
	InitMeshWall_XZ(
		D3DXVECTOR3(-LENGTH_X*17.5, -LENGTH_X * 3, LENGTH_X* 140),      //���[�J�����W
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //���[�J�����W
		15,                                             //���}�X��
		120,                                             //�c�}�X��
		LENGTH_X,                                             //�}�X���Ƃ�width
		LENGTH_X,                                             //�}�X���Ƃ�height
		MESH_FLOOR,
		FLOOR01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(LENGTH_X * 17.5, -LENGTH_X * 3, LENGTH_X* 140),      //���[�J�����W
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),      //���[�J�����W
		15,                                             //���}�X��
		120,                                             //�c�}�X��
		LENGTH_X,                                             //�}�X���Ƃ�width
		LENGTH_X,                                             //�}�X���Ƃ�height
		MESH_FLOOR,
		FLOOR01_TEX
	);

	//����
	InitMeshWall_XZ(
		D3DXVECTOR3(LENGTH_X * 24, -LENGTH_X * 3, LENGTH_X * 100),      //���[�J�����W
		D3DXVECTOR3(0.5*PI, -0.5*PI, 0.0f),      //���[�J�����W
		60,                                             //���}�X��
		30,                                             //�c�}�X��
		LENGTH_X*6,                                             //�}�X���Ƃ�width
		LENGTH_X*2,                                             //�}�X���Ƃ�height
		MESH_WALL,
		WALL01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(-LENGTH_X * 24, -LENGTH_X * 3, LENGTH_X * 100),      //���[�J�����W
		D3DXVECTOR3(0.5*PI, 0.5*PI, 0.0f),      //���[�J�����W
		60,                                             //���}�X��
		30,                                             //�c�}�X��
		LENGTH_X * 6,                                             //�}�X���Ƃ�width
		LENGTH_X * 2,                                             //�}�X���Ƃ�height
		MESH_WALL,
		WALL01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(0.0f, -LENGTH_X * 3, -200.0f),      //���[�J�����W
		D3DXVECTOR3(0.5*PI, 0.0f, 0.0f),      //���[�J�����W
		10,                                             //���}�X��
		30,                                             //�c�}�X��
		LENGTH_X * 6,                                             //�}�X���Ƃ�width
		LENGTH_X * 2,                                             //�}�X���Ƃ�height
		MESH_WALL,
		WALL01_TEX
	);

	InitMeshWall_XZ(
		D3DXVECTOR3(0.0f, -LENGTH_X * 3, 1400.0f),      //���[�J�����W
		D3DXVECTOR3(0.5*PI, PI, 0.0f),      //���[�J�����W
		10,                                             //���}�X��
		30,                                             //�c�}�X��
		LENGTH_X * 6,                                             //�}�X���Ƃ�width
		LENGTH_X * 2,                                             //�}�X���Ƃ�height
		MESH_WALL,
		WALL01_TEX
	);

	//����|���S���Z�b�g
	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == true)
		{
			SetHitRect(&g_MeshWall[nCnt].m_MeshFieldMatrix, &g_MeshWall[nCnt].HW, g_MeshWall[nCnt].meshType);
		}
	}

	return S_OK;
}

//�|���S���̍X�V
void UpdateMeshField(void)
{
	//��]
	//�ړ�
	
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	TEXTUREMANAGER *pTexManager = GetTextureManager();                                  //�e�N�X�`���Ǘ�
	SetMaterial(pD3DDevice);                                  //�}�e���A���ݒ�

	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		//�g�p��ԂȂ�
		if (g_MeshWall[nCnt].bUse == true)
		{
			SetMatrix(&g_MeshWall[nCnt].m_MeshFieldMtxWorld, g_MeshWall[nCnt].m_MeshFieldMatrix);      //�}�g���N�X�ϊ��Z�b�g

			//�R�x�j�����:���_�t�H�[�}�b�g�ݒ�
			pD3DDevice->SetStreamSource(0, g_MeshWall[nCnt].m_pVtxBufferMeshField, 0, sizeof(VERTEX_3D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
			pD3DDevice->SetIndices(g_MeshWall[nCnt].m_pIdxBufferFieldMeshField);                                                    //�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3DDevice->SetFVF(FVF_VERTEX3D);
			pD3DDevice->SetTexture(0, (pTexManager + g_MeshWall[nCnt].m_TexNum)->TextureSlot);                              //�e�N�X�`���̐ݒ�
			pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshWall[nCnt].m_nVertexNum, 0, g_MeshWall[nCnt].m_nPolygonNum);              //�`��
		}
	}

}

////////////////////////////////////////////////
//�|���S���̏I��
////////////////////////////////////////////////
void UninitMeshField(void)
{
	//���_�o�b�t�@���������
	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		SAFE_RELEASE(g_MeshWall[nCnt].m_pVtxBufferMeshField);                       //���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��

		SAFE_RELEASE(g_MeshWall[nCnt].m_pIdxBufferFieldMeshField);                  //�C���f�b�N�X�o�b�t�@�����
	}
}

//�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexMeshFieldBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_3D *pVtx)
{
	/*******************************************�f�o�C�X�ݒ�***********************************************/
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//���b�V���E�H�[��(XZ����)
void InitMeshWall_XZ(D3DXVECTOR3 pos, D3DXVECTOR3 rot, UINT nNumBlock_Honrizon, UINT nNumBlock_Vertical, float fSizeBlock_Honrizon, float fSizeBlock_Vertical, MESHTYPE meshType, char *TexPass)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;									   //�e�N�X�`���\���̂̃|�C���^
	WORD *pIdx = NULL;
	int i, j;                                                  //�J�E���g�p�ϐ�

	//�Z�b�g
	for (int nCnt = 0; nCnt < NUMWALL_MAX; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == false)
		{
			g_MeshWall[nCnt].bUse = true;
			g_MeshWall[nCnt].meshType = meshType;
			g_MeshWall[nCnt].HW.x = nNumBlock_Honrizon*fSizeBlock_Honrizon;
			g_MeshWall[nCnt].HW.y = nNumBlock_Vertical*fSizeBlock_Vertical;
			g_MeshWall[nCnt].m_nVertexNum = (nNumBlock_Honrizon + 1)*(nNumBlock_Vertical + 1);         //���_��
			g_MeshWall[nCnt].m_nIndexNum = (2 + 2 * nNumBlock_Honrizon)*nNumBlock_Vertical + (nNumBlock_Vertical - 1) * 2; //�C���f�b�N�X��
			g_MeshWall[nCnt].m_nPolygonNum = g_MeshWall[nCnt].m_nIndexNum - 2;       //�|���S����
			g_MeshWall[nCnt].m_MeshFieldMatrix.pos = pos;    //pos
			g_MeshWall[nCnt].m_MeshFieldMatrix.rot = rot;    //rot
			g_MeshWall[nCnt].fSlopeAngle = rot.x;

			SetTexture(&g_MeshWall[nCnt].m_TexNum,TexPass);                                     //�e�N�X�`���ǂݍ���

			/***************************���z�A�h���X���擾�����_�o�b�t�@�����b�N����************************************/
			g_MeshWall[nCnt].m_pVtxBufferMeshField->Lock(0, 0, (void**)&pVtx, 0);
			//���_�o�^
			for (i = 0; i < nNumBlock_Vertical + 1; i++)
			{
				for (j = 0; j < nNumBlock_Honrizon + 1; j++)
				{
					int k = i * (nNumBlock_Honrizon + 1) + j;
					pVtx[0 + k].pos = D3DXVECTOR3(j*fSizeBlock_Honrizon- g_MeshWall[nCnt].HW.x*0.5, 0.0f, -i*fSizeBlock_Vertical+ g_MeshWall[nCnt].HW.y*0.5);
					pVtx[0 + k].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pVtx[0 + k].tex = D3DXVECTOR2(j*1.0f, i*1.0f);
					pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
			}

			g_MeshWall[nCnt].m_pVtxBufferMeshField->Unlock();
			/***************************���_�o�b�t�@�̃A�����b�N************************************/

			/***************************���z�A�h���X���擾���C���f�b�N�X�o�b�t�@�����b�N����************************************/
			g_MeshWall[nCnt].m_pIdxBufferFieldMeshField->Lock(0, 0, (void**)&pIdx, 0);

			//�C���f�b�N�X�o�b�t�@�o�^
			//�k��
			for (i = 0; i < nNumBlock_Vertical; i++)
			{
				//���ʕ���
				for (j = 0; j < nNumBlock_Honrizon + 1; j++)
				{
					int k = j * 2 + i*(nNumBlock_Honrizon + 2) * 2;
					pIdx[0 + k] = (i + 1)*(nNumBlock_Honrizon + 1) + j;
					pIdx[1 + k] = pIdx[0 + k] - (nNumBlock_Honrizon + 1);
				}

				int q = (2 + 2 * nNumBlock_Honrizon)*(i + 1) + ((i + 1) - 1) * 2;

				//�k�ޕ���
				if (q < g_MeshWall[nCnt].m_nIndexNum)
				{
					pIdx[q] = pIdx[q - 1];         //�k��1�̒��_�ԍ�
					pIdx[q + 1] = pIdx[q] + (nNumBlock_Honrizon + 1) + 1;     //�k��2�̒��_�ԍ�
				}
			}

			g_MeshWall[nCnt].m_pIdxBufferFieldMeshField->Unlock();
			/***************************�C���f�b�N�X�o�b�t�@�̃A�����b�N************************************/

			break;
		}
	}
}

//���b�V�����擾
MESHWALL *GetMesh(void)
{
	return &g_MeshWall[0];
}