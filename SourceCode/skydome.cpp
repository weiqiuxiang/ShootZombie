//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "skydome.h"
#include "camera.h"
#include "matrix.h"

#define SKYDOME_POSX (0.0f)
#define SKYDOME_POSY (0.0f)
#define SKYDOME_POSZ (0.0f)

#define START_ANGEL_VERTICAL (0.0f)            //���������̊J�n�p�x
#define END_ANGEL_VERTICAL (PI)              //���������̏I���p�x

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureSkydome[2] = {};							//�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferSkydome = NULL;						//���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DINDEXBUFFER9 g_pIdxBufferFieldSkydome = NULL;					//�C���f�b�N�X�o�b�t�@�𑀍삷�邽�߂̃|�C���^
int g_nSkydomeVertexNum, g_nSkydomeIndexNum,g_nSkydomePolygonNum;       //���_��,�C���f�b�N�X��,�|���S����
SKYDOME g_Skydome;       //�X�J�C�h�[��

D3DXMATRIX g_SkydomeMtxWorld;                                   // ���[���h�}�g���N�X
WORLD_MATRIX g_SkydomeMatrix;                           //���[���h�}�g���N�X�̕ϊ��s��

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitSkydome(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;
	pD3DDevice = Get_Device();

	g_SkydomeMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_SkydomeMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SkydomeMatrix.pos = D3DXVECTOR3(0.0f, -22.0f, 0.0f);

	g_Skydome.fRadius = 4000.0f;                                         //�X�J�C�h�[�����a
	
	g_Skydome.nHorizon_PieceNum = 100;                                   //����������
	g_Skydome.nVertical_PieceNum = 100;                                  //����������
	
	g_Skydome.fStart_Angel_Vertical = START_ANGEL_VERTICAL;              //������]�p�x�̎n�_�w��
	g_Skydome.fEnd_Angel_Vertical = END_ANGEL_VERTICAL;                  //������]�p�x�̏I�_�w��
	//�P�ʉ�]�p�x�v�Z
	g_Skydome.fHorizon_PerAngel = 2*PI / g_Skydome.nHorizon_PieceNum;
	g_Skydome.fVertical_PerAngel = (g_Skydome.fEnd_Angel_Vertical- g_Skydome.fStart_Angel_Vertical) / g_Skydome.nVertical_PieceNum;

	//���_��,�C���f�b�N�X��,�|���S�����v�Z
	g_nSkydomeVertexNum = (g_Skydome.nHorizon_PieceNum+1)*(g_Skydome.nVertical_PieceNum + 1);
	g_nSkydomeIndexNum = (2+2* g_Skydome.nHorizon_PieceNum)*g_Skydome.nVertical_PieceNum + (g_Skydome.nVertical_PieceNum - 1)*2;
	g_nSkydomePolygonNum = g_nSkydomeIndexNum - 2;

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_3D)*g_nSkydomeVertexNum,D3DUSAGE_WRITEONLY,FVF_VERTEX3D,D3DPOOL_MANAGED,&g_pVtxBufferSkydome,NULL)))
	{
		return E_FAIL;
	}

	//�C���f�b�N�X�o�b�t�@�̐���
	HRESULT hr;
	hr = pD3DDevice->CreateIndexBuffer(
		sizeof(WORD)*g_nSkydomeIndexNum,     //�o�b�t�@��(2�o�C�g)
		D3DUSAGE_WRITEONLY,        //�g�p���@�t���O
		D3DFMT_INDEX16,              //�C���f�b�N�X�t�H�[�}�b�g(�d�����Ȃ����_����65536�͌��E)
		D3DPOOL_MANAGED,           //�������Ǘ����@
		&g_pIdxBufferFieldSkydome,        //�C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^�̃A�h���X
		NULL);
	
	if(FAILED(hr))
	{
		return E_FAIL;
	}

	//�e�N�X�`���ǂݍ���
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice, SKYDOOM_TEXTURENAME,&g_pTextureSkydome[0]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}
	
	//���b�V���E�H�[���ݒ�
	InitMeshSkydome_Y();

	return S_OK;
}

//�|���S���̍X�V
void UpdateSkydome(void)
{
	//��]
	g_SkydomeMatrix.rot.y += 0.001f;
	//�ړ�
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawSkydome(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();
	SetMatrix(&g_SkydomeMtxWorld, g_SkydomeMatrix);      //�}�g���N�X�ϊ��Z�b�g
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���

	//�R�x�j�����:���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0,g_pVtxBufferSkydome,0,sizeof(VERTEX_3D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetIndices(g_pIdxBufferFieldSkydome);                                                    //�C���f�b�N�X�o�b�t�@�̃Z�b�g
	pD3DDevice -> SetFVF(FVF_VERTEX3D);
	pD3DDevice -> SetTexture(0, g_pTextureSkydome[0]);												 //�e�N�X�`���̐ݒ�
	pD3DDevice -> DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0, g_nSkydomeVertexNum,0, g_nSkydomePolygonNum);              //�`��

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g���I���ɂ���
}

////////////////////////////////////////////////
//�|���S���̏I��
////////////////////////////////////////////////
void UninitSkydome(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE (g_pTextureSkydome[0]);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE (g_pTextureSkydome[1]);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��

	//���_�o�b�t�@���������
	SAFE_RELEASE (g_pVtxBufferSkydome);                       //���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��

	//�C���f�b�N�X�o�b�t�@�����
	SAFE_RELEASE(g_pIdxBufferFieldSkydome);
}

//�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexSkydomeBG(LPDIRECT3DDEVICE9 pDevice,VERTEX_3D *pVtx)
{
	/*******************************************�f�o�C�X�ݒ�***********************************************/
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//�X�J�C�h�[��(Y����])
void InitMeshSkydome_Y(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;									   //�e�N�X�`���\���̂̃|�C���^
	WORD *pIdx = NULL;
	int i, j;                                                  //�J�E���g�p�ϐ�

	float fHorizon_Angel;
	float fVertical_Angel;
	float posX;
	float posY;
	float posZ;

	/***************************���z�A�h���X���擾�����_�o�b�t�@�����b�N����************************************/
	g_pVtxBufferSkydome->Lock(0, 0, (void**)&pVtx, 0);
	//���_�o�^
	for (i = 0; i < (g_Skydome.nVertical_PieceNum + 1); i++)
	{
		for (j = 0; j < (g_Skydome.nHorizon_PieceNum + 1) ; j++)
		{
			int k = i * (g_Skydome.nHorizon_PieceNum + 1) + j;
			//pos�Ɋւ���v�Z
			fHorizon_Angel = -g_Skydome.fHorizon_PerAngel*j;                                       //�����p�x
			fVertical_Angel = g_Skydome.fStart_Angel_Vertical + g_Skydome.fVertical_PerAngel * i;   //�����p�x
			posX = g_Skydome.fRadius*sin(fVertical_Angel)*cos(fHorizon_Angel);            //���_��X���W�v�Z
			posY = g_Skydome.fRadius*cos(fVertical_Angel);            //���_��Y���W�v�Z
			posZ = g_Skydome.fRadius*sin(fVertical_Angel)*sin(fHorizon_Angel);            //���_��Z���W�v�Z

			pVtx[0 + k].pos = D3DXVECTOR3(posX, posY, posZ);
			pVtx[0 + k].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			pVtx[0 + k].tex = D3DXVECTOR2(j*(1.0f/ g_Skydome.nHorizon_PieceNum), i*(1.0f/ g_Skydome.nVertical_PieceNum));
			pVtx[0 + k].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
	}
	
	g_pVtxBufferSkydome->Unlock();
	/***************************���_�o�b�t�@�̃A�����b�N************************************/

	int q;
	/***************************���z�A�h���X���擾���C���f�b�N�X�o�b�t�@�����b�N����************************************/
	g_pIdxBufferFieldSkydome->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�o�^
	//�k��
	for (i = 0; i < g_Skydome.nVertical_PieceNum; i++)
	{
		//���ʕ���
		for (j = 0; j < g_Skydome.nHorizon_PieceNum + 1; j++)
		{
			int k = j * 2 + i*(g_Skydome.nHorizon_PieceNum + 2) * 2;
			
			pIdx[0 + k] = (i + 1)*(g_Skydome.nHorizon_PieceNum + 1) + j;
			pIdx[1 + k] = pIdx[0 + k] - (g_Skydome.nHorizon_PieceNum + 1);
		}

		 q = (2 + 2 * g_Skydome.nHorizon_PieceNum)*(i + 1) + ((i + 1) - 1) * 2;

		//�k�ޕ���
		if (q < g_nSkydomeIndexNum)
		{
			pIdx[q] = pIdx[q - 1];         //�k��1�̒��_�ԍ�
			pIdx[q + 1] = pIdx[q] + (g_Skydome.nHorizon_PieceNum + 1) + 1;     //�k��2�̒��_�ԍ�
		}
	}

	g_pIdxBufferFieldSkydome->Unlock();
	/***************************�C���f�b�N�X�o�b�t�@�̃A�����b�N************************************/
}
