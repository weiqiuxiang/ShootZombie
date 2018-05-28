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
#include "obj3d.h"
#include "textureManager.h"
#include "matrix.h"

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////

CObj3D::CObj3D()
{
	D3DXMatrixIdentity(&m_WorldMatrix);
	m_PosRotScl.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosRotScl.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosRotScl.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	//�����p�}�e���A���ݒ�
	m_Material.Diffuse.r = 255;
	m_Material.Diffuse.g = 0;
	m_Material.Diffuse.b = 0;
	m_Material.Diffuse.a = 50;
	m_Material.Ambient = m_Material.Diffuse;
	m_Material.Specular.r = 0;
	m_Material.Specular.g = 0;
	m_Material.Specular.b = 0;
	m_Material.Specular.a = 0;
	m_Material.Emissive = m_Material.Specular;
	m_Material.Power = 0.0f;

	m_UseMatrial = false;
	m_bUse = false;

	m_pMesh = NULL;
	m_pBuffMat = NULL;

	m_nNumMat = 0;                               //�}�e���A����

	for (int nCnt = 0; nCnt < OBJ3D_TEX_MAX; nCnt++)
	{
		m_TexNUM[nCnt] = -1;
	}
}

CObj3D::~CObj3D()
{

}

HRESULT CObj3D::Init(const WORLD_MATRIX *pPosRotScl, char *ModelPass,DWORD nNumMatEB, bool bUse)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //���_�\���̂̃|�C���^
	pD3DDevice = Get_Device();

	m_PosRotScl = *pPosRotScl;                        //�f�[�^���
	m_nNumMat = nNumMatEB;                                     //�}�e���A�������
	m_UseMatrial = false;
	m_bUse = bUse;
	D3DXMatrixIdentity(&m_WorldMatrix);

	//���f���f�[�^�̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(ModelPass, D3DXMESH_MANAGED/*�w��������*/, pD3DDevice/*�f�o�C�X*/, NULL/*�אڃo�b�t�@*/, &m_pBuffMat/*�}�e���A�������i�[����*/, NULL, &m_nNumMat/*�}�e���A����*/, &m_pMesh/*���b�V��*/)))
	{
		MessageBox(NULL, "���f��MODEL_NAME�̓ǂݍ��݂����s����", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();   //�}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾

	//�e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < m_nNumMat; nCnt++)
	{
		SetTexture(&m_TexNUM[nCnt], pMat[nCnt].pTextureFilename);
	}

	//�����p�}�e���A���ݒ�
	m_Material = pMat[0].MatD3D;
	m_Material.Ambient.r = 1;
	m_Material.Ambient.g = 1;
	m_Material.Ambient.b = 1;
	m_Material.Ambient.a = 0.5;

	return S_OK;
}

//�|���S���̍X�V
void CObj3D::Update(void)
{
	
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void CObj3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	TEXTUREMANAGER *pTexManager = GetTextureManager();

	D3DMATERIAL9 matDef;
	pD3DDevice->GetMaterial(&matDef);                                                 //�f�o�C�X�̃}�e���A�����擾���A�ۑ�����

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	TEXTUREMANAGER *pTextureM = GetTextureManager();                                  //�e�N�X�`���}�l�[�W���[�擾

	if (m_bUse == true)
	{
		SetMatrix(&m_WorldMatrix, m_PosRotScl);                                            //�}�g���N�X�̌v�Z

		for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
		{
			if (m_UseMatrial == true) pD3DDevice->SetMaterial(&m_Material);
			else pD3DDevice->SetMaterial(&pMat[nCnt].MatD3D);
			if(pMat[nCnt].pTextureFilename != NULL) pD3DDevice->SetTexture(0, (pTexManager+ m_TexNUM[nCnt])->TextureSlot);
			else pD3DDevice->SetTexture(0,NULL);
			m_pMesh->DrawSubset(nCnt);
		}
	}
	pD3DDevice->SetMaterial(&matDef);                                                //�}�e���A����߂�
}

//�I���֐�
void CObj3D::Uninit(void)
{
	if (m_pMesh != NULL)
	{
		SAFE_RELEASE(m_pMesh);
	}

	if (m_pBuffMat != NULL)
	{
		SAFE_RELEASE(m_pBuffMat);
	}
}

//�w�i�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void CObj3D::MakeVertexSetting(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CObj3D::SetPos(const D3DXVECTOR3 *pos)
{
	m_PosRotScl.pos = *pos;
}

void CObj3D::SetRot(const D3DXVECTOR3 *rot)
{
	m_PosRotScl.rot = *rot;
}

void CObj3D::AddRotY(float rot)
{
	m_PosRotScl.rot.y += rot;
}

void CObj3D::SetMaterial(const D3DMATERIAL9 *Material)
{
	m_Material = *Material;
}

bool CObj3D::GetUse(void)
{
	return m_bUse;
}

void CObj3D::AddPos(const D3DXVECTOR3 *pos)
{
	m_PosRotScl.pos += *pos;
}

void CObj3D::SetUse(bool bUse)
{
	m_bUse = bUse;
}

void CObj3D::SetMatrialUse(bool bUse)
{
	m_UseMatrial = bUse;
}