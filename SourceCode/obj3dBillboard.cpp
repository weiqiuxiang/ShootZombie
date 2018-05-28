#include "obj3dBillboard.h"
#include "matrix.h"
//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 CObj3dBillboard::m_pTexture[TEX_MAX] = {};

CObj3dBillboard::CObj3dBillboard()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_TexNum = -1;
	m_pVtxBuffer = NULL;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_Color[nCnt] = 255;
	}
}
CObj3dBillboard::~CObj3dBillboard()
{

}
HRESULT CObj3dBillboard::Init(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *speed, const D3DXVECTOR2 *psize, int TexNum)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	m_pos = *pos;
	m_speed = *speed;
	m_size = *psize;
	m_TexNum = TexNum;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_Color[nCnt] = 255;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &m_pVtxBuffer, NULL)))
	{
		MessageBox(NULL, "�p�[�e�B�N�����_�o�b�t�@�m�ێ��s", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	m_Color[1] = 0;
	m_Color[2] = 0;

	return S_OK;
}
void CObj3dBillboard::Uninit(void)
{
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}
}

void CObj3dBillboard::Update(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //���_�\���̂̃|�C���^

	MakeVertexSetting(pD3DDevice, &pVtx[0]);

	//�^�C�g���|���S��
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	pVtx[0].pos = D3DXVECTOR3(-m_size.x*0.5, m_size.y*0.5, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x*0.5, m_size.y*0.5, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x*0.5, -m_size.y*0.5, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x*0.5, -m_size.y*0.5, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[1].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[2].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[3].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

	m_pVtxBuffer->Unlock();                                     //�o�b�t�@�̃A�����b�N
}

void CObj3dBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	//���_�t�H�[�}�b�g�ݒ�
	WORLD_MATRIX Mtx;
	D3DXMATRIX WMtx;

	Mtx.pos = m_pos;
	Mtx.rot = m_rot;
	Mtx.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	SetBillboardMatrix(&WMtx, Mtx);

	pD3DDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));                   //VRAM����GPU�Ƀf�[�^���X�g���[��
	pD3DDevice->SetFVF(FVF_VERTEX3D);

	pD3DDevice->SetTexture(0, m_pTexture[m_TexNum]);
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

HRESULT CObj3dBillboard::LoadTex(int TexNum, char *Texpass)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	if ((TexNum < 0) || (TexNum >= TEX_MAX))
	{
		MessageBox(NULL, "�w�肷��e�N�X�`���X���b�g�ԍ��͎w��͈͂𒴂�,�e�N�X�`�����[�h���s", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	else
	{
		//�e�N�X�`���̓ǂݍ���
		if (m_pTexture[TexNum] == NULL)
		{
			if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, Texpass, &m_pTexture[TexNum])))
			{
				MessageBox(NULL, "�p�[�e�B�N���p�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
				return E_FAIL;
			}
		}

		else
		{
			MessageBox(NULL, "billboard�̃e�N�X�`���X���b�g���g���Ă���", "�G���[", MB_OK | MB_ICONHAND);
			return E_FAIL;
		}
	}

	return S_OK;
}

void CObj3dBillboard::UnloadTex(void)
{
	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

void CObj3dBillboard::SpriteRorate(VERTEX_3D *pVtx)
{

}

void CObj3dBillboard::MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CObj3dBillboard::SetAlpha(int nAlpha)
{
	m_Color[3] = nAlpha;

	if (m_Color[3] < 0)
	{
		m_Color[3] = 0;
	}

	if (m_Color[3] > 255)
	{
		m_Color[3] = 255;
	}
}

void CObj3dBillboard::SetColor(int R, int G, int B)
{
	m_Color[0] = R;
	m_Color[1] = G;
	m_Color[2] = B;

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_Color[nCnt] > 255)
		{
			m_Color[nCnt] = 255;
		}

		if (m_Color[nCnt] < 0)
		{
			m_Color[nCnt] = 0;
		}
	}
}

void CObj3dBillboard::AddAlpha(int nValue)
{
	m_Color[3] += nValue;                                    //�J���[�̐ݒ�

	if (m_Color[3] >= 255)
	{
		m_Color[3] = 255;
	}

	if (m_Color[3] < 0)
	{
		m_Color[3] = 0;
	}
}

void CObj3dBillboard::SetSize(const D3DXVECTOR2 *psize)
{
	m_size = *psize;                                    //�J���[�̐ݒ�
}