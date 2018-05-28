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
#include "input.h"
#include "CNumber.h"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;                         //���̉�
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////

CNumber::CNumber()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TexPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TexSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = 0;
	m_pVtxBuffer = NULL;
}

CNumber::~CNumber()
{

}

HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 TexPos, D3DXVECTOR3 TexSize)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();

	m_rot = 0;
	m_pos = pos;
	m_size = size;
	m_TexPos = TexPos;
	m_TexSize = TexSize;
	
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &m_pVtxBuffer, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//�|���S���̍X�V
void CNumber::Update(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_2D *pVtx = NULL;                           //���_�\���̂̃|�C���^

	CObj2D::MakeVertexSetting(pD3DDevice, &pVtx[0]);

	//�^�C�g���|���S��
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

		SpriteRorate(&pVtx[0]);                                      //��]������z�u

		pVtx[0].tex = D3DXVECTOR2(m_TexPos.x, m_TexPos.y);
		pVtx[1].tex = D3DXVECTOR2(m_TexPos.x + m_TexSize.x, m_TexPos.y);
		pVtx[2].tex = D3DXVECTOR2(m_TexPos.x, m_TexPos.y + m_TexSize.y);
		pVtx[3].tex = D3DXVECTOR2(m_TexPos.x + m_TexSize.x, m_TexPos.y + m_TexSize.y);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

		m_pVtxBuffer->Unlock();                                     //�o�b�t�@�̃A�����b�N
	}
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	pD3DDevice->SetTexture(0, m_pTexture);
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//�I���֐�
void CNumber::Uninit(void)
{
	
}

void CNumber::SetTexPos(const D3DXVECTOR3 *pos)
{
	m_TexPos = *pos;
}

void CNumber::SetTexSize(const D3DXVECTOR3 *size)
{
	m_TexSize = *size;
}

void CNumber::SpriteRorate(VERTEX_2D *pVtx)
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

void CNumber::MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_2D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

HRESULT CNumber::LoadTex(char *TexPass)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TexPass, &m_pTexture)))
	{
		MessageBox(NULL, "�X�R�A�̃e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}
	return S_OK;
}
void CNumber::UnloadTex(void)
{
	if(m_pTexture != NULL)
	{
		SAFE_RELEASE(m_pTexture);
	}
}

/*************************************************************************************************************/
CBullet::CBullet()
{
	
}

CBullet::~CBullet()
{
	
}

HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 TexPos, D3DXVECTOR3 TexSize)
{
	CNumber::Init(pos, size, TexPos, TexSize);

	return S_OK;
}

void CBullet::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//���_�t�H�[�}�b�g�ݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX2D);

	pD3DDevice->SetTexture(0, m_pTexture);
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
void CBullet::Update(void)
{
	CNumber::Update();
}
void CBullet::Uninit(void)
{
	CNumber::Uninit();
}

HRESULT CBullet::LoadTex(char *TexPass)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, TexPass, &m_pTexture)))
	{
		MessageBox(NULL, "�X�R�A�̃e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}
	return S_OK;
}
void CBullet::UnloadTex(void)
{
	if (m_pTexture != NULL)
	{
		SAFE_RELEASE(m_pTexture);
	}
}