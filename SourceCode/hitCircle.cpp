//-----------------------------------------------------------------------------------------
//�w�b�_�t�@�C��
//-----------------------------------------------------------------------------------------
#include <Windows.h>
#include "matrix.h"
#include "hitCircle.h"
#include "input.h"
#include "model_aircraft.h"
#include "EnemyBall.h"

//-----------------------------------------------------------------------------------------
//�v���g�^�C�v
//-----------------------------------------------------------------------------------------
void InitVertexIndex(int nID);

//-----------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//-----------------------------------------------------------------------------------------
HITCIRCLE g_hitCircle[HITCIRCLE_MAX];       //�X�J�C�h�[��
#ifdef _DEBUG
bool g_DrawCheckHR;
#endif // 

////////////////////////////////////////////////
//�|���S��(���_)�̏�����
////////////////////////////////////////////////
HRESULT InitHitCircle(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;
	pD3DDevice = Get_Device();

	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		g_hitCircle[i].m_pVtxBuffer = NULL;
		g_hitCircle[i].m_pIdxBufferField = NULL;
		g_hitCircle[i].m_type = TYPE_OBJ;
		g_hitCircle[i].HitCheckType = HBTYPE_NULL;                           //���蔻��`�F�b�N
		g_hitCircle[i].bUse = false;
		g_hitCircle[i].m_offset = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}

#ifdef _DEBUG
	g_DrawCheckHR = false;
#endif //
	return S_OK;
}

//�|���S���̍X�V
void UpdateHitCircle(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();

	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		if (g_hitCircle[i].bUse == true)
		{
			g_hitCircle[i].m_posOld = g_hitCircle[i].m_Matrix.pos;

			if(g_hitCircle[i].m_type == TYPE_PLAYER)
			{
				g_hitCircle[i].m_Matrix.pos = pAircraft->m_Matrix.pos + g_hitCircle[i].m_offset;
			}

			if(g_hitCircle[i].m_type == TYPE_ENEMY)
			{
				ENEMYBALL *pEB = Get_EB(g_hitCircle[i].nEnemyID);
				if(pEB->bUse == false)
				{
					g_hitCircle[i].bUse = false;
					continue;
				}
				else
				{
					g_hitCircle[i].m_Matrix.pos = pEB->m_Matrix.pos + g_hitCircle[i].m_offset;
				}
			}
		}
	}
}

////////////////////////////////////////////////
//�|���S���̕`��
////////////////////////////////////////////////
void DrawHitCircle(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{
		if (g_DrawCheckHR == false) g_DrawCheckHR = true;
		else g_DrawCheckHR = false;
	}
	if (g_DrawCheckHR == true)
	{
		LPDIRECT3DDEVICE9 pD3DDevice;
		pD3DDevice = Get_Device();
		for (int i = 0; i < HITCIRCLE_MAX; i++)
		{
			pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);          //���C�g���I�t�ɂ���
			if (g_hitCircle[i].bUse == true)
			{
				SetMatrix(&g_hitCircle[i].m_MtxWorld, g_hitCircle[i].m_Matrix);      //�}�g���N�X�ϊ��Z�b�g

				pD3DDevice->SetStreamSource(0, g_hitCircle[i].m_pVtxBuffer, 0, sizeof(VERTEX_3D));                     //VRAM����GPU�Ƀf�[�^���X�g���[��
				pD3DDevice->SetIndices(g_hitCircle[i].m_pIdxBufferField);                                                    //�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pD3DDevice->SetFVF(FVF_VERTEX3D);
				pD3DDevice->SetTexture(0, NULL);												 //�e�N�X�`���̐ݒ�
				pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_hitCircle[i].m_nVertexNum, 0, g_hitCircle[i].m_nPolygonNum);              //�`��
			}
			pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);          //���C�g���I���ɂ���
		}
	}
#endif // DEBUG
}

////////////////////////////////////////////////
//�|���S���̏I��
////////////////////////////////////////////////
void UninitHitCircle(void)
{
	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		SAFE_RELEASE(g_hitCircle[i].m_pVtxBuffer);                       //���_�o�b�t�@�C���^�[�t�F�C�X�̕Еt��
		SAFE_RELEASE(g_hitCircle[i].m_pIdxBufferField);                  //�C���f�b�N�X�o�b�t�@�����
	}
}

//�|���S���̕\���ʒuX,�|���S���̕\���ʒuY,�|���S���̕�,�|���S���̍���
void MakeVertexHitCircle(LPDIRECT3DDEVICE9 pDevice, VERTEX_3D *pVtx)
{
	/*******************************************�f�o�C�X�ݒ�***********************************************/
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //����1:�ݒ薼�J�����O ����2:�ݒ�l(D3DCULL_CCW:�t���v���𗠖ʂ�����)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//�~���b�V���̒��_���b�N�ƃC���f�b�N�X���b�N
void InitVertexIndex(int nID)
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
	g_hitCircle[nID].m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	//���_�o�^
	for (i = 0; i < (g_hitCircle[nID].m_skydome.nVertical_PieceNum + 1); i++)
	{
		for (j = 0; j < (g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1); j++)
		{
			int k = i * (g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1) + j;
			//pos�Ɋւ���v�Z
			fHorizon_Angel = g_hitCircle[nID].m_skydome.fHorizon_PerAngel*j;                                       //�����p�x
			fVertical_Angel = g_hitCircle[nID].m_skydome.fStart_Angel_Vertical + g_hitCircle[nID].m_skydome.fVertical_PerAngel * i;   //�����p�x
			posX = g_hitCircle[nID].m_skydome.fRadius*sin(fVertical_Angel)*cos(fHorizon_Angel);            //���_��X���W�v�Z
			posY = g_hitCircle[nID].m_skydome.fRadius*cos(fVertical_Angel);            //���_��Y���W�v�Z
			posZ = g_hitCircle[nID].m_skydome.fRadius*sin(fVertical_Angel)*sin(fHorizon_Angel);            //���_��Z���W�v�Z

			pVtx[0 + k].pos = D3DXVECTOR3(posX, posY, posZ);
			pVtx[0 + k].normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			pVtx[0 + k].tex = D3DXVECTOR2(j*(1.0f / g_hitCircle[nID].m_skydome.nHorizon_PieceNum), i*(1.0f / g_hitCircle[nID].m_skydome.nVertical_PieceNum));
			pVtx[0 + k].color = g_hitCircle[nID].m_Color;
		}
	}

	g_hitCircle[nID].m_pVtxBuffer->Unlock();
	/***************************���_�o�b�t�@�̃A�����b�N************************************/

	int q;
	/***************************���z�A�h���X���擾���C���f�b�N�X�o�b�t�@�����b�N����************************************/
	g_hitCircle[nID].m_pIdxBufferField->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�o�^
	//�k��
	for (i = 0; i < g_hitCircle[nID].m_skydome.nVertical_PieceNum; i++)
	{
		//���ʕ���
		for (j = 0; j < g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1; j++)
		{
			int k = j * 2 + i*(g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 2) * 2;

			pIdx[0 + k] = (i + 1)*(g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1) + j;
			pIdx[1 + k] = pIdx[0 + k] - (g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1);
		}

		q = (2 + 2 * g_hitCircle[nID].m_skydome.nHorizon_PieceNum)*(i + 1) + ((i + 1) - 1) * 2;

		//�k�ޕ���
		if (q < g_hitCircle[nID].m_nIndexNum)
		{
			pIdx[q] = pIdx[q - 1];         //�k��1�̒��_�ԍ�
			pIdx[q + 1] = pIdx[q] + (g_hitCircle[nID].m_skydome.nHorizon_PieceNum + 1) + 1;     //�k��2�̒��_�ԍ�
		}
	}

	g_hitCircle[nID].m_pIdxBufferField->Unlock();
	/***************************�C���f�b�N�X�o�b�t�@�̃A�����b�N************************************/
}

HRESULT SetHitCirclePlayer(const D3DXVECTOR3 *pPos, float nRadius,const D3DXVECTOR3 *offset)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;
	pD3DDevice = Get_Device();

	for(int i = 0; i < HITCIRCLE_MAX; i++)
	{
		if(g_hitCircle[i].bUse == false)
		{
			//
			//�X�J�h�[������
			//////////////////////////
			g_hitCircle[i].m_skydome.fRadius = nRadius;
			g_hitCircle[i].m_skydome.fStart_Angel_Vertical = 0;
			g_hitCircle[i].m_skydome.fEnd_Angel_Vertical = PI;
			g_hitCircle[i].m_skydome.nHorizon_PieceNum = 10;
			g_hitCircle[i].m_skydome.nVertical_PieceNum = 10;
			g_hitCircle[i].m_skydome.fHorizon_PerAngel = 2 * PI / g_hitCircle[i].m_skydome.nHorizon_PieceNum;
			g_hitCircle[i].m_skydome.fVertical_PerAngel = (g_hitCircle[i].m_skydome.fEnd_Angel_Vertical - g_hitCircle[i].m_skydome.fStart_Angel_Vertical) / g_hitCircle[i].m_skydome.nVertical_PieceNum;

			//���_��,�C���f�b�N�X��,�|���S�����v�Z
			g_hitCircle[i].m_nVertexNum = (g_hitCircle[i].m_skydome.nHorizon_PieceNum + 1)*(g_hitCircle[i].m_skydome.nVertical_PieceNum + 1);
			g_hitCircle[i].m_nIndexNum = (2 + 2 * g_hitCircle[i].m_skydome.nHorizon_PieceNum)*g_hitCircle[i].m_skydome.nVertical_PieceNum + (g_hitCircle[i].m_skydome.nVertical_PieceNum - 1) * 2;
			g_hitCircle[i].m_nPolygonNum = g_hitCircle[i].m_nIndexNum - 2;

			g_hitCircle[i].m_Matrix.pos = *pPos;
			g_hitCircle[i].m_posOld = g_hitCircle[i].m_Matrix.pos;
			g_hitCircle[i].m_offset = *offset;
			g_hitCircle[i].m_type = TYPE_PLAYER;
			g_hitCircle[i].m_Matrix.rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_hitCircle[i].m_Matrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_hitCircle[i].m_Color = D3DCOLOR_RGBA(200,10,10,100);
			g_hitCircle[i].nID = i;

			//���_�o�b�t�@�̐���
			if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*g_hitCircle[i].m_nVertexNum, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_hitCircle[i].m_pVtxBuffer, NULL)))
			{
				return E_FAIL;
			}

			//�C���f�b�N�X�o�b�t�@�̐���
			HRESULT hr;
			hr = pD3DDevice->CreateIndexBuffer(
				sizeof(WORD)*g_hitCircle[i].m_nIndexNum,     //�o�b�t�@��(2�o�C�g)
				D3DUSAGE_WRITEONLY,        //�g�p���@�t���O
				D3DFMT_INDEX16,              //�C���f�b�N�X�t�H�[�}�b�g(�d�����Ȃ����_����65536�͌��E)
				D3DPOOL_MANAGED,           //�������Ǘ����@
				&g_hitCircle[i].m_pIdxBufferField,        //�C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^�̃A�h���X
				NULL);

			if (FAILED(hr))
			{
				return E_FAIL;
			}

			InitVertexIndex(g_hitCircle[i].nID);                                      //���_�o�b�t�@�̃��b�N�ƒ��_����Ȃ��o�b�t�@�̃��b�N
			g_hitCircle[i].bUse = true;
			break;
		}
	}

	return S_OK;
}

HRESULT SetHitCircleEnemy(const D3DXVECTOR3 *pPos, float nRadius,const D3DXVECTOR3 *offset,int nEnemyID)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;
	pD3DDevice = Get_Device();

	for (int i = 0; i < HITCIRCLE_MAX; i++)
	{
		if (g_hitCircle[i].bUse == false)
		{
			//
			//�X�J�h�[������
			//////////////////////////
			g_hitCircle[i].m_skydome.fRadius = nRadius;
			g_hitCircle[i].m_skydome.fStart_Angel_Vertical = 0;
			g_hitCircle[i].m_skydome.fEnd_Angel_Vertical = PI;
			g_hitCircle[i].m_skydome.nHorizon_PieceNum = 10;
			g_hitCircle[i].m_skydome.nVertical_PieceNum = 10;
			g_hitCircle[i].m_skydome.fHorizon_PerAngel = 2 * PI / g_hitCircle[i].m_skydome.nHorizon_PieceNum;
			g_hitCircle[i].m_skydome.fVertical_PerAngel = (g_hitCircle[i].m_skydome.fEnd_Angel_Vertical - g_hitCircle[i].m_skydome.fStart_Angel_Vertical) / g_hitCircle[i].m_skydome.nVertical_PieceNum;

			//���_��,�C���f�b�N�X��,�|���S�����v�Z
			g_hitCircle[i].m_nVertexNum = (g_hitCircle[i].m_skydome.nHorizon_PieceNum + 1)*(g_hitCircle[i].m_skydome.nVertical_PieceNum + 1);
			g_hitCircle[i].m_nIndexNum = (2 + 2 * g_hitCircle[i].m_skydome.nHorizon_PieceNum)*g_hitCircle[i].m_skydome.nVertical_PieceNum + (g_hitCircle[i].m_skydome.nVertical_PieceNum - 1) * 2;
			g_hitCircle[i].m_nPolygonNum = g_hitCircle[i].m_nIndexNum - 2;

			g_hitCircle[i].m_Matrix.pos = *pPos;
			g_hitCircle[i].m_offset = *offset;
			g_hitCircle[i].m_type = TYPE_ENEMY;
			g_hitCircle[i].m_Matrix.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_hitCircle[i].m_Matrix.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_hitCircle[i].m_Color = D3DCOLOR_RGBA(200, 10, 10, 100);
			g_hitCircle[i].nID = i;

			//���_�o�b�t�@�̐���
			if (FAILED(pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*g_hitCircle[i].m_nVertexNum, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_hitCircle[i].m_pVtxBuffer, NULL)))
			{
				return E_FAIL;
			}

			//�C���f�b�N�X�o�b�t�@�̐���
			HRESULT hr;
			hr = pD3DDevice->CreateIndexBuffer(
				sizeof(WORD)*g_hitCircle[i].m_nIndexNum,     //�o�b�t�@��(2�o�C�g)
				D3DUSAGE_WRITEONLY,        //�g�p���@�t���O
				D3DFMT_INDEX16,              //�C���f�b�N�X�t�H�[�}�b�g(�d�����Ȃ����_����65536�͌��E)
				D3DPOOL_MANAGED,           //�������Ǘ����@
				&g_hitCircle[i].m_pIdxBufferField,        //�C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^�̃A�h���X
				NULL);

			if (FAILED(hr))
			{
				return E_FAIL;
			}

			InitVertexIndex(g_hitCircle[i].nID);                                      //���_�o�b�t�@�̃��b�N�ƒ��_����Ȃ��o�b�t�@�̃��b�N
			g_hitCircle[i].nEnemyID = nEnemyID;                                             //�g�p���ID��n��
			g_hitCircle[i].bUse = true;
			break;
		}
	}

	return S_OK;
}

void SetCirclePos(const D3DXVECTOR3 *pos, int nID)
{
	g_hitCircle[nID].m_Matrix.pos = *pos;
}

HITCIRCLE *Get_HitCircle(int nID)
{
	return &g_hitCircle[nID];
}