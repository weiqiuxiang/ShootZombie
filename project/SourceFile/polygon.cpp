//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "TpsCamera.h"
#include "model_aircraft.h"
#include "motion.h"

#define TESTPOLYGON_POSX (100.0f)
#define TESTPOLYGON_POSY (100.0f)
#define TESTPOLYGON_WIDTH (100.0f)
#define TESTPOLYGON_HEIGHT (100.0f)

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePolygon[2] = {};          //�e�N�X�`���𑀍삷�邽�߂̃|�C���^(�e�N�X�`���o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPolygon = NULL;    //���_�o�b�t�@�𑀍삷�邽�߂̃|�C���^(���_�o�b�t�@�C���^�[�t�F�C�X�ɃA�N�Z�X���߂̃|�C���^)
D3DXMATRIX g_PolygonWorldMatrix;                        //�|���S���̃��[���h�}�g���N�X
WORLD_MATRIX g_PolygonMatrix;                           //�|���S���̃}�g���N�X�̕ϊ��s��

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���f�����[���h�ϊ��}�g���N�X�̏�����
	g_PolygonMatrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	g_PolygonMatrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_PolygonMatrix.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&g_pVtxBufferPolygon,NULL)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���ǂݍ���
	if ( FAILED( D3DXCreateTextureFromFile(pD3DDevice,POLYGON_TEXTURENAME,&g_pTexturePolygon[0]) ) )
	{
		MessageBox(NULL,"�e�N�X�`���̓ǂݍ��݂����s���܂���","�G���[",MB_OK|MB_ICONHAND);
		return E_FAIL;
	}

	//�Ə����W
	g_pVtxBufferPolygon -> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	MakeVertexPolygon(pD3DDevice,pVtx);

	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 - TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 - TESTPOLYGON_HEIGHT*0.5,0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 + TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 - TESTPOLYGON_HEIGHT*0.5,0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 - TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 + TESTPOLYGON_HEIGHT*0.5,0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 + TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 + TESTPOLYGON_HEIGHT*0.5,0.0f);

	g_pVtxBufferPolygon -> Unlock();                                     //�o�b�t�@�̃A�����b�N

	return S_OK;
}

//�|���S���̍X�V
void UpdatePolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;									   //�e�N�X�`���\���̂̃|�C���^

	//MakeVertexPolygonBG(pD3DDevice,pVtx);                      //�|���S���̐ݒ�

	/***************************���z�A�h���X���擾���o�b�t�@�����b�N����************************************/
	g_pVtxBufferPolygon -> Lock(0,0,(void**)&pVtx,0);

	//���[�J�����W
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 - TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 - TESTPOLYGON_HEIGHT*0.5, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 + TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 - TESTPOLYGON_HEIGHT*0.5, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 - TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 + TESTPOLYGON_HEIGHT*0.5, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH*0.5 + TESTPOLYGON_WIDTH*0.5, SCREEN_HEIGHT*0.5 + TESTPOLYGON_HEIGHT*0.5, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
	pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

	g_pVtxBufferPolygon -> Unlock();                             
	/***************************�o�b�t�@�̃A�����b�N************************************/
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawPolygon(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();

	if ( (pAircraft -> m_MotionNow == CModel::MOTION_SHOOT) || (pAircraft->m_MotionNow == CModel::MOTION_GUNMOVE) )
	{
		LPDIRECT3DDEVICE9 pD3DDevice;
		pD3DDevice = Get_Device();

		pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���

		//�R�x�j�����:���_�t�H�[�}�b�g�ݒ�
		pD3DDevice->SetStreamSource(0, g_pVtxBufferPolygon, 0, sizeof(VERTEX_2D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
		pD3DDevice->SetFVF(FVF_VERTEX2D);
		pD3DDevice->SetTexture(0, g_pTexturePolygon[0]);												 //�e�N�X�`���̐ݒ�
		pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);                                  //�e�N�X�`���������̊m��

		pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g���I���ɂ���
	}
}

void UninitPolygon(void)
{
	//�e�N�X�`���C���^�[�t�F�C�X���������
	SAFE_RELEASE (g_pTexturePolygon[0]);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��
	SAFE_RELEASE (g_pTexturePolygon[1]);                      //�e�N�X�`���C���^�[�t�F�C�X�̕Еt��

	//���_�o�b�t�@���������
	SAFE_RELEASE (g_pVtxBufferPolygon);                       //���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
}

//�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
{
	/*******************************************�f�o�C�X�ݒ�***********************************************/
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}