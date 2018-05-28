#include "CPartical.h"
#include "matrix.h"
//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 CPartical::m_pTexture[TEX_MAX] = {};

CPartical::CPartical()
{
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadius = 0;
	m_fRadiusSpeed = 0;
	m_nLife = 0;
	m_TexNum = -1;
	m_nAlphaSpeed = 0;
	m_fGravity = 0;
	m_type = PARTICAL_NORMAL;
	m_pVtxBuffer = NULL;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_Color[nCnt] = 255;
	}
}
CPartical::~CPartical()
{
	
}
HRESULT CPartical::Init(const D3DXVECTOR3 *pos, const D3DXVECTOR3 *speed, float fRadius, int nLife, int TexNum, PARTICAL_TYPE type, float fGravity)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	m_pos = *pos;
	m_speed = *speed;
	m_fRadius = fRadius;
	m_nLife = nLife;
	m_TexNum = TexNum;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = type;
	m_nAlphaSpeed = 255 / nLife;
	m_fRadiusSpeed = fRadius / nLife;
	m_fGravity = fGravity;

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

	return S_OK;
}
void CPartical::Uninit(void)
{
	if(m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}
}

void CPartical::Update(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = Get_Device();
	VERTEX_3D *pVtx = NULL;                           //���_�\���̂̃|�C���^

	MakeVertexSetting(pD3DDevice, &pVtx[0]);

	//�^�C�g���|���S��
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);                    //���z�A�h���X���擾���o�b�t�@�����b�N����

	pVtx[0].pos = D3DXVECTOR3(-m_fRadius, m_fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fRadius, m_fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fRadius, -m_fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fRadius, -m_fRadius, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[1].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[2].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	pVtx[3].color = D3DCOLOR_RGBA(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);

	m_pVtxBuffer->Unlock();                                     //�o�b�t�@�̃A�����b�N

	switch (m_type)
	{
		case PARTICAL_NORMAL:
			m_pos += m_speed;                           //�ʒu�ړ�
			m_nLife--;                                  //���C�t����
			AddAlpha(-m_nAlphaSpeed);
			AddRadius(-m_fRadiusSpeed);
			break;
		case PARTICAL_GRAVITY:
			m_pos += m_speed;                           //�ʒu�ړ�
			m_speed.y -= m_fGravity;
			m_nLife--;                                  //���C�t����
			AddAlpha(-m_nAlphaSpeed);
			AddRadius(-m_fRadiusSpeed);
			break;
		default:
			m_pos += m_speed;                           //�ʒu�ړ�
			m_nLife--;                                  //���C�t����
			AddAlpha(-m_nAlphaSpeed);
			AddRadius(-m_fRadiusSpeed);
			break;
	}
}

void CPartical::Draw(void)
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
int CPartical::GetLife(void)
{
	return m_nLife;
}

HRESULT CPartical::LoadTex(int TexNum, char *Texpass)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	if( (TexNum < 0) || (TexNum >= TEX_MAX) )
	{
		MessageBox(NULL, "�w�肷��e�N�X�`���X���b�g�ԍ��͎w��͈͂𒴂�,�e�N�X�`�����[�h���s", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	else
	{
		//�e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, Texpass, &m_pTexture[TexNum])))
		{
			MessageBox(NULL, "�p�[�e�B�N���p�e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
			return E_FAIL;
		}
	}

	return S_OK;
}

void CPartical::UnloadTex(void)
{
	for(int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		if(m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

void CPartical::SpriteRorate(VERTEX_3D *pVtx)
{
	
}

void CPartical::MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CPartical::SetAlpha(int nAlpha)
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

void CPartical::SetColor(int R, int G, int B)
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

void CPartical::AddAlpha(int nValue)
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

void CPartical::AddRadius(float fValue)
{
	m_fRadius += fValue;                                    //�J���[�̐ݒ�

	if (m_fRadius < 0)
	{
		m_fRadius = 0;
	}
}