/*
cpp��:titleobj.cpp
	�^�C�g����2D�摜�����pcpp
*/

//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include <string.h>
#include "main.h"
#include "titleManager.h"
#include "input.h"
#include "obj2d.h"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////

CObj2D::CObj2D()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_rot = 0;
	m_pVtxBuffer = NULL;
	m_pTexture = NULL;
	m_bUse = false;
	for(int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_Color[nCnt] = 255;
	}
}

CObj2D::~CObj2D()
{
	
}

HRESULT CObj2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, char *TexPass,bool bUse)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	m_pos = pos;
	m_size = size;
	m_bUse = bUse;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_Color[nCnt] = 255;
	}

	//���_�o�b�t�@�̐���
	if(FAILED(pD3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D)*4,D3DUSAGE_WRITEONLY,FVF_VERTEX2D,D3DPOOL_MANAGED,&m_pVtxBuffer,NULL)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TexPass, &m_pTexture)))
	{
		MessageBox(NULL, "Obj2D�̃e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	//���_�ݒ�
	MakeVertexSetting(pD3DDevice, &pVtx[0]);

	//�^�C�g���|���S��
	m_pVtxBuffer-> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	SpriteRorate(&pVtx[0]);

	m_pVtxBuffer-> Unlock();                                     //�o�b�t�@�̃A�����b�N
	return S_OK;
}

//�|���S���̍X�V
void CObj2D::Update(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^

	MakeVertexSetting(pD3DDevice, &pVtx[0]);

	//�^�C�g���|���S��
	m_pVtxBuffer-> Lock(0,0,(void**)&pVtx,0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	SpriteRorate(&pVtx[0]);                                      //��]������z�u

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[1].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[2].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[3].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

	m_pVtxBuffer-> Unlock();                                     //�o�b�t�@�̃A�����b�N
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void CObj2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//���_�t�H�[�}�b�g�ݒ�
	pD3DDevice -> SetStreamSource(0, m_pVtxBuffer,0,sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice -> SetFVF(FVF_VERTEX2D);

	pD3DDevice -> SetTexture(0,m_pTexture);
	pD3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
}

//�I���֐�
void CObj2D::Uninit(void)
{
	if(m_pVtxBuffer != NULL)
	{
		SAFE_RELEASE(m_pVtxBuffer);
	}

	if(m_pTexture != NULL)
	{
		SAFE_RELEASE(m_pTexture);
	}
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void CObj2D::MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice,VERTEX_2D *pVtx)
{
	pDevice -> SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	pDevice -> SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

void CObj2D::SetPos(const D3DXVECTOR3 *pos)
{
	m_pos = *pos;
}

void CObj2D::SetSize(const D3DXVECTOR3 *size)
{
	m_size = *size;
}

void CObj2D::AddRot(const float rot)
{
	m_rot += rot;
}

void CObj2D::SetAlpha(int Alpha)
{
	m_Color[3] = Alpha;

	if(m_Color[3] < 0)
	{
		m_Color[3] = 0;
	}

	if(m_Color[3] > 255)
	{
		m_Color[3] = 255;
	}
}

bool CObj2D::GetUse(void)
{
	return m_bUse;
}

void CObj2D::AddAlpha(int nValue)
{
	m_Color[3] += nValue;                                    //�J���[�̐ݒ�

	if(m_Color[3] >= 255)
	{
		m_Color[3] = 255;
	}

	if (m_Color[3] < 0)
	{
		m_Color[3] = 0;
	}
}

void CObj2D::SpriteRorate(VERTEX_2D *pVtx)
{
	double X1, Y1, X2, Y2, X3, Y3, X4, Y4;             //����,�E�����W
	double Xc1, Yc1, Xc2, Yc2, Xc3, Yc3, Xc4, Yc4;     //��]������̍��W
	double XC, YC;                                     //���S���W

	X1 = m_pos.x - 0.5*m_size.x;
	Y1 = m_pos.y - 0.5*m_size.y;
	X2 = m_pos.x + 0.5*m_size.x;
	Y2 = m_pos.y - 0.5*m_size.y;
	X3 = m_pos.x - 0.5*m_size.x;
	Y3 = m_pos.y + 0.5*m_size.y;
	X4 = m_pos.x + 0.5*m_size.x;
	Y4 = m_pos.y + 0.5*m_size.y;

	XC = m_pos.x;
	YC = m_pos.y;

	Xc1 = (X1 - XC)*cos(m_rot) - (Y1 - YC)*sin(m_rot) + XC;
	Yc1 = (X1 - XC)*sin(m_rot) + (Y1 - YC)*cos(m_rot) + YC;

	Xc2 = (X2 - XC)*cos(m_rot) - (Y2 - YC)*sin(m_rot) + XC;
	Yc2 = (X2 - XC)*sin(m_rot) + (Y2 - YC)*cos(m_rot) + YC;

	Xc3 = (X3 - XC)*cos(m_rot) - (Y3 - YC)*sin(m_rot) + XC;
	Yc3 = (X3 - XC)*sin(m_rot) + (Y3 - YC)*cos(m_rot) + YC;

	Xc4 = (X4 - XC)*cos(m_rot) - (Y4 - YC)*sin(m_rot) + XC;
	Yc4 = (X4 - XC)*sin(m_rot) + (Y4 - YC)*cos(m_rot) + YC;

	pVtx[0].pos = D3DXVECTOR3(Xc1, Yc1, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Xc2, Yc2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Xc3, Yc3, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Xc4, Yc4, 0.0f);
}

void CObj2D::AddPos(const D3DXVECTOR3 *pos)
{
	m_pos += *pos;
}

bool CObj2D::PointInPolygon(const D3DXVECTOR3 *pos)
{
	float X1 = m_pos.x - 0.5*m_size.x;
	float X2 = m_pos.x + 0.5*m_size.x;
	float Y1 = m_pos.y - 0.5*m_size.y;
	float Y2 = m_pos.y + 0.5*m_size.y;

	if(
		((pos->x >= X1) && (pos->x <= X2)) &&
		((pos->y >= Y1) && (pos->y <= Y2))
		)
	{
		return true;
	}

	return false;
}

void CObj2D::SetColor(int R, int G, int B)
{
	m_Color[0] = R;
	m_Color[1] = G;
	m_Color[2] = B;

	for(int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_Color[nCnt] > 255)
		{
			m_Color[nCnt] = 255;
		}

		if(m_Color[nCnt] < 0)
		{
			m_Color[nCnt] = 0;
		}
	}
}

void CObj2D::SetUse(bool bUse)
{
	m_bUse = bUse;
}