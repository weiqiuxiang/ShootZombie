#include "CGrenadeTrack.h"

//�}�N��
#define TRACK_FRAME (5) //�v���t���[��
#define BILLBOARD_P_TEX "data/TEXTURE/partical/partical.png"
#define BILLBOARD_PEF_TEX "data/TEXTURE/partical/base.jpg"

//static�ϐ�

//��Ԋ֐�
CGrenadeTrack::CGrenadeTrack()
{
	
}
CGrenadeTrack::~CGrenadeTrack()
{
	
}

void CGrenadeTrack::Init(void)
{
	CObj3dBillboard::LoadTex(0, BILLBOARD_P_TEX);
	CObj3dBillboard::LoadTex(1, BILLBOARD_PEF_TEX);

	for(int nCnt = 0; nCnt < BILLBOARD_NUM;nCnt++)
	{
		m_ObjBillUse[nCnt] = false;
	}
}
void CGrenadeTrack::Uninit(void)
{
	CObj3dBillboard::UnloadTex();

	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		m_ObjBill[nCnt].Uninit();
		m_ObjBillEF[nCnt].Uninit();
	}
}
void CGrenadeTrack::Update(void)
{
	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		if (m_ObjBillUse[nCnt] == true)
		{
			m_ObjBill[nCnt].Update();
			m_ObjBillEF[nCnt].Update();
		}
	}
}
void CGrenadeTrack::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���

	pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);                               //z�o�b�t�@���Q��
	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);                               //z���c���Ȃ�

	//���Z�����ݒ�
	// �A���t�@�E�u�����f�B���O���s��
	pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	// �`��G�|���S���̓��œ������ɂȂ��Ă���
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// ��̊G�͓��������Ȃ�
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		if (m_ObjBillUse[nCnt] == true)
		{
			m_ObjBill[nCnt].Draw();
			m_ObjBillEF[nCnt].Draw();
		}
	}

	//�u�����h���[�h���A���t�@�u�����h�ɖ߂�
	// ��̊G�̓��l�ŕs�����Ȃ��Ă���
	pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// �`��G�̓��l�œ����Ȃ��Ă���
	pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      //z���c��
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g���I���ɂ���
}

//�t�@�N�V�����֐�
void CGrenadeTrack::SetTrack(const D3DXVECTOR3 *pos/*�J�n�ʒu*/, const D3DXVECTOR3 *speed/*�X�s�[�h*/, float gravity/*�O���l�[�h�̏d�͉e��*/, const D3DXVECTOR3 *color/*�O�Ղ̃J���[*/)
{
	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		m_ObjBillUse[nCnt] = false;
	}

	for (int nCnt = 0; nCnt < BILLBOARD_NUM; nCnt++)
	{
		D3DXVECTOR3 posResult;

		//��nCnt�ڂ̓_�̈ʒu�̌v�Z(�_�Ɠ_��frame����TRACK_FRAME)
		posResult.x = pos->x + speed->x * TRACK_FRAME * nCnt;
		posResult.y = speed->y * TRACK_FRAME * nCnt - 0.5 * gravity * (TRACK_FRAME * nCnt) * (TRACK_FRAME * nCnt) + pos->y;          //S = vt - 0.5*g*t^2;
		posResult.z = pos->z + speed->z * TRACK_FRAME * nCnt;
		m_ObjBill[nCnt].Init(&posResult,&D3DXVECTOR3(0.0f,0.0f,0.0f), &D3DXVECTOR2(50.0f, 50.0f),0);
		m_ObjBillEF[nCnt].Init(&posResult, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR2(100.0f, 100.0f), 1);
		m_ObjBillUse[nCnt] = true;

		if (posResult.y < pos->y) break;
	}
}