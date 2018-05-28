#define _CRT_SECURE_NO_WARNINGS
#include "motion.h"
#include "textureManager.h"
#include "matrix.h"
#include "model_aircraft.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PART_NUM (15)

CPart CModel::m_partData[PART_MAX];

/************************************************************CPart�N���X����***************************************************************************/
/**********************************************************************�J�n***************************************************************************/
CPart::CPart()
{
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	for(int nCnt = 0; nCnt < MATTEX_NUMMAX; nCnt++)
	{
		m_TexSlot[nCnt] = NULL;
	}
}

CPart::~CPart()
{
	
}

HRESULT CPart::Init(DWORD nNumMatModel)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	//���f���f�[�^�̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(m_aModelpass, D3DXMESH_MANAGED/*�w��������*/, pD3DDevice/*�f�o�C�X*/, NULL/*�אڃo�b�t�@*/, &m_pBuffMat/*�}�e���A�������i�[����*/, NULL, &nNumMatModel/*�}�e���A����*/, &m_pMesh/*���b�V��*/)))
	{
		MessageBox(NULL, "���f��MODEL_NAME�̓ǂݍ��݂����s����", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();   //�}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾

	for (int nCnt = 0; nCnt < MATTEX_NUMMAX; nCnt++)
	{
		m_TexSlot[nCnt] = NULL;
	}
	
	//�e�N�X�`���ǂݍ���
	for (int nCnt = 0; nCnt < nNumMatModel; nCnt++)
	{
		if (pMat[nCnt].pTextureFilename != NULL)
		{
			if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, pMat[nCnt].pTextureFilename, &m_TexSlot[nCnt])))
			{
				MessageBox(NULL, "���f���p�[�c�̃e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
				return E_FAIL;
			}
		}
	}
	
	m_NumMat = nNumMatModel;
	m_nIndex = -1;
	m_nNumParent = -1;
	m_bUse = false;

	return S_OK;
}

void CPart::SetInit(const WORLD_MATRIX *offsetInit, int nIndex, int nNumParent)
{
	m_OffsetInit = *offsetInit;
	m_nIndex = nIndex;
	m_nNumParent = nNumParent;
	m_bUse = true;
}

void CPart::SetKeyBefore(const WORLD_MATRIX *keyBefore)
{
	m_keyBefore = *keyBefore;
}

void CPart::SetKeyAfter(const WORLD_MATRIX *keyAfter)
{
	m_keyAfter = *keyAfter;
}

void CPart::SetModelPass(char *Modelpass)
{
	strcpy(m_aModelpass, Modelpass);
}

D3DXMATRIX CPart::GetResultMtx(void)
{
	return m_MtxResult;
}

bool CPart::GetUse(void)
{
	return m_bUse;
}

void CPart::SetUse(bool bUse)
{
	m_bUse = bUse;
}

int CPart::GetIndex(void)
{
	return m_nIndex;
}

int CPart::GetParent(void)
{
	return m_nNumParent;
}

void CPart::SetParentID(int nID)
{
	m_nID = nID;
}

int CPart::GetParentID(void)
{
	return m_nID;
}

HRESULT CPart::ModelChange(char *Modelpass,DWORD nNumMatModel)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	VERTEX_3D *pVtx = NULL;                           //�e�N�X�`���\���̂̃|�C���^
	pD3DDevice = Get_Device();

	strcpy(m_aModelpass, Modelpass);
	m_NumMat = nNumMatModel;
	Uninit();

	//���f���f�[�^�̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(m_aModelpass, D3DXMESH_MANAGED/*�w��������*/, pD3DDevice/*�f�o�C�X*/, NULL/*�אڃo�b�t�@*/, &m_pBuffMat/*�}�e���A�������i�[����*/, NULL, &nNumMatModel/*�}�e���A����*/, &m_pMesh/*���b�V��*/)))
	{
		MessageBox(NULL, "���f��MODEL_NAME�̓ǂݍ��݂����s����", "�G���[", MB_OK | MB_ICONHAND);
		return E_FAIL;
	}

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();   //�}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾

	for (int nCnt = 0; nCnt < MATTEX_NUMMAX; nCnt++)
	{
		m_TexSlot[nCnt] = NULL;
	}

	//�e�N�X�`���ǂݍ���
	for (int nCnt = 0; nCnt < nNumMatModel; nCnt++)
	{
		if (pMat[nCnt].pTextureFilename != NULL)
		{
			if (FAILED(D3DXCreateTextureFromFile(pD3DDevice, pMat[nCnt].pTextureFilename, &m_TexSlot[nCnt])))
			{
				MessageBox(NULL, "���f���p�[�c�̃e�N�X�`���̓ǂݍ��݂����s���܂���", "�G���[", MB_OK | MB_ICONHAND);
				return E_FAIL;
			}
		}
	}
}

void CPart::Update(int nMotionFrameNum, int nMotionFrameNow, const D3DXMATRIX *pParentMtx)
{
	D3DXMATRIX  MtxInit,mtxParent, mtxOffsetNow;
	D3DXMATRIX mtxRotOffset, mtxMoveOffset,mtxRotInit,mtxMoveInit;
	
	WORLD_MATRIX MtxOffset;

	MtxOffset.pos = m_keyAfter.pos - m_keyBefore.pos;
	MtxOffset.rot = m_keyAfter.rot - m_keyBefore.rot;
	MtxOffset.pos = m_keyBefore.pos + MtxOffset.pos*(float)nMotionFrameNow / (float)nMotionFrameNum;
	MtxOffset.rot = m_keyBefore.rot + MtxOffset.rot*(float)nMotionFrameNow / (float)nMotionFrameNum;

	D3DXMatrixRotationYawPitchRoll(&mtxRotOffset, MtxOffset.rot.y/*y��*/, MtxOffset.rot.x/*x��*/, MtxOffset.rot.z/*z��*/); //��]�s���ݒ�
	D3DXMatrixTranslation(&mtxMoveOffset, MtxOffset.pos.x, MtxOffset.pos.y, MtxOffset.pos.z);                         //�ړ��s��ݒ�
	mtxOffsetNow = mtxRotOffset*mtxMoveOffset;                                       //�O�L�[�t���[���̃}�g���N�X

	D3DXMatrixRotationYawPitchRoll(&mtxRotInit, m_OffsetInit.rot.y/*y��*/, m_OffsetInit.rot.x/*x��*/, m_OffsetInit.rot.z/*z��*/); //��]�s���ݒ�
	D3DXMatrixTranslation(&mtxMoveInit, m_OffsetInit.pos.x, m_OffsetInit.pos.y, m_OffsetInit.pos.z);                         //�ړ��s��ݒ�
	MtxInit = mtxRotInit*mtxMoveInit;                                       //�O�L�[�t���[���̃}�g���N�X

	mtxParent = *pParentMtx;

	m_MtxResultOld = m_MtxResult;                 //�t���[���ω��O�ɕۑ�

	m_MtxResult = MtxInit*mtxParent;         //�ŏI�̃��U���g�}�g���N�X�Z�o
	m_MtxResult = mtxOffsetNow*m_MtxResult;
}

void CPart::Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3DDevice = Get_Device();

	D3DMATERIAL9 matDef;
	pD3DDevice->GetMaterial(&matDef);                                                 //�f�o�C�X�̃}�e���A�����擾���A�ۑ�����

	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	TEXTUREMANAGER *pTextureM = GetTextureManager();                                  //�e�N�X�`���}�l�[�W���[�擾
	
	AIRCRAFT *pAircraft = Get_Aircraft();

	D3DMATERIAL9 DamageMat;
	DamageMat.Diffuse.r = 0;
	DamageMat.Diffuse.g = 255;
	DamageMat.Diffuse.b = 0;
	DamageMat.Diffuse.a = 50;
	DamageMat.Ambient = DamageMat.Diffuse;
	DamageMat.Specular.r = 0;
	DamageMat.Specular.g = 0;
	DamageMat.Specular.b = 0;
	DamageMat.Specular.a = 0;
	DamageMat.Emissive = DamageMat.Specular;
	DamageMat.Power = 1.0f;

	if (m_bUse == true)
	{
		pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxResult);

		for (int nCnt = 0; nCnt < (int)m_NumMat; nCnt++)
		{
			pD3DDevice->SetMaterial(&pMat[nCnt].MatD3D);
			if (pAircraft->m_NoDamageTime > 0) pD3DDevice->SetMaterial(&DamageMat);          //���G���ԓ�
			pD3DDevice->SetTexture(0, m_TexSlot[nCnt]);
			m_pMesh->DrawSubset(nCnt);
		}
	}

	pD3DDevice->SetMaterial(&matDef);                                                //�}�e���A����߂�
}

void CPart::Uninit(void)
{
	if(m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if(m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	for (int nCnt = 0; nCnt < MATTEX_NUMMAX; nCnt++)
	{
		if (m_TexSlot[nCnt] != NULL)
		{
			m_TexSlot[nCnt]->Release();
			m_TexSlot[nCnt] = NULL;
		}
	}
}

/**********************************************************************�I��***************************************************************************/
CModel::CModel()
{
	for (int nCnt = 0; nCnt < PART_MAX; nCnt++)
	{
		m_partData[nCnt] = {};
	}
}

CModel::~CModel()
{
	
}

HRESULT CModel::Init(char *filepass)
{
	FILE *fp;
	char buf[256];
	int nReal = 0;

	m_nMotionFrameNum = 0;
	m_nMotionFrameNow = 0;
	m_nKeyNum = 0;
	m_nKeyNow = 0;

	//������
	for (int nCnt = 0; nCnt < PART_MAX; nCnt++)
	{
		m_partData[nCnt].SetUse(false);
	}

	strcpy(aMotionTextPass, filepass);                           //�t�@�C���p�X�R�s�[

	if( (fp = fopen(aMotionTextPass,"r")) == NULL)
	{
		MessageBox(NULL, "���[�V�����e�L�X�g�̓ǂݍ��ݎ��s", "�G���[", MB_OK | MB_ICONHAND);
	}

	//�p�[�c���̓ǂݍ���
	do
	{
		fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

		//�ǂݔ�΂�����
		nReal = 0;
		if( (strncmp(buf, "#", 1) == 0) || (strcmp(buf,"\n") == 0) ) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
		for(int nCnt = 0;nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;                                  //�f�[�^�����܂œǂݍ��ނƃ��[�v���甲��
				break;
			}
		}
		
		if (strncmp((buf+ nReal), "NUM_MODEL =", 11) == 0)
		{
			sscanf((buf + nReal), "%*11c%d", &m_nNumpart);                    //�p�[�c����ǂݍ���
			break;
		}

	} while(strncmp((buf + nReal), "NUM_MODEL =", 11) != 0);

	//�p�[�c�̃��f���p�X�̓ǂݍ���
	for (int nCnt = 0; nCnt < m_nNumpart;nCnt++)
	{
		char agomi[256];                                           //�ǂݎ̂ėp
		char aData[256];

		do
		{
			fgets(buf, sizeof(buf), fp);

			//�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf+nReal), "MODEL_FILENAME =", 16) == 0) break;
		} while (strncmp((buf + nReal), "MODEL_FILENAME =", 16) != 0);

		sscanf(buf, "%*16c%s%s", &aData[0], &agomi[0]);
		m_partData[nCnt].SetModelPass(&aData[0]);                                                      //���f���p�X�ǂݍ���
	}

	//���f���̃f�[�^�������ƃ}�e���A�����ݒ�
	for (int nCnt = 0; nCnt < PART_NUM; nCnt++)
	{
		m_partData[nCnt].Init(1);
	}

	//���[�V���������f�[�^�̓ǂݍ���
	for (int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		int nIndex = -2;
		int nParent = -2;
		WORLD_MATRIX mTx;
		mTx.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		//PARTSSET��ǂݍ��ނ��Ƃ��m�F�ł���܂œǂݍ��ݑ���
		do
		{
			fgets(buf, sizeof(buf), fp);

			//�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;

				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf+ nReal), "PARTSSET", 8) == 0)
			{
				fgets(buf, sizeof(buf), fp);                  //���̍s��̃f�[�^��ǂݍ���
				break;
			}
		} while (strncmp((buf + nReal), "PARTSSET", 8) != 0);

		//�C���f�b�N�X�ԍ��ǂݍ���
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}
		if((strncmp(buf + nReal, "INDEX =", 7) == 0) )
		{
			sscanf(buf + nReal, "%*7c%d", &nIndex);                                       //�����̔ԍ��ǂݍ���(�C���f�b�N�X)
			fgets(buf, sizeof(buf), fp);                  //���̍s��̃f�[�^��ǂݍ���
		}
		else
		{
			MessageBox(NULL, "INDEX�̓ǂݍ��ݎ��s", "�G���[", MB_OK | MB_ICONHAND);
		}

		//�e�̃p�[�c�ԍ��ǂݍ���
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}
		if ((strncmp(buf+ nReal, "PARENT =", 8) == 0))
		{
			sscanf(buf+ nReal, "%*8c%d", &nParent);                                      //�e�̔ԍ��ǂݍ���
			fgets(buf, sizeof(buf), fp);                  //���̍s��̃f�[�^��ǂݍ���
		}
		else
		{
			MessageBox(NULL, "PARENT�̓ǂݍ��ݎ��s", "�G���[", MB_OK | MB_ICONHAND);
		}

		//POS
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}
		if((strncmp(buf + nReal, "POS =", 5) == 0))
		{
			sscanf(buf + nReal, "%*5c%f%f%f", &mTx.pos.x,&mTx.pos.y, &mTx.pos.z);
			fgets(buf, sizeof(buf), fp);                  //���̍s��̃f�[�^��ǂݍ���
		}
		else
		{
			MessageBox(NULL, "POS�̓ǂݍ��ݎ��s", "�G���[", MB_OK | MB_ICONHAND);
		}

		//ROT
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}
		if ((strncmp(buf + nReal, "ROT =", 5) == 0))
		{
			sscanf(buf + nReal, "%*5c%f%f%f", &mTx.rot.x, &mTx.rot.y,&mTx.rot.z);
			fgets(buf, sizeof(buf), fp);                  //���̍s��̃f�[�^��ǂݍ���
		}
		else
		{
			MessageBox(NULL, "ROT�̓ǂݍ��ݎ��s", "�G���[", MB_OK | MB_ICONHAND);
		}

		m_partData[nCnt].SetInit(&mTx, nIndex, nParent);                          //�����I�t�Z�b�g�Ȃǂ̃f�[�^���Z�b�g
	}

	fclose(fp);

	//ID�o�^
	for(int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nNumpart; nCnt2++)
		{
			if (nCnt != 0)
			{
				if(m_partData[nCnt].GetParent() == m_partData[nCnt2].GetIndex())
				{
					m_partData[nCnt].SetParentID(nCnt2);
					break;
				}
			}
		}
	}

	return S_OK;
}

void CModel::Update(void)
{
	AIRCRAFT *pAircraft = Get_Aircraft();

	D3DXMATRIX WorldMtx = pAircraft->m_MtxWorld;

	FILE *fp;
	char buf[256];
	int nReal = 0;

	MotionName = pAircraft->m_MotionNow;                    //���[�V�����擾

	if(pAircraft->m_GunType != pAircraft->m_GunTypeOld)
	{
		if (pAircraft->m_GunType == GUNTYPE_GRENADE)
		{
			m_partData[14].ModelChange("data/MODEL/16_Grenade.x",2);
		}

		else
		{
			m_partData[14].ModelChange("data/MODEL/15_gun.x", 1);
		}
	}

	if (pAircraft->m_MotionNow != pAircraft->m_MotionBefore)
	{
		m_nKeyNow = 0;                                //���[�V�����ϊ��̎��J�n�L�[��0�ɂ���
		m_nMotionFrameNow = 0;
	}

	//�e�L�X�g�ǂݍ���
	if ((fp = fopen(aMotionTextPass, "r")) == NULL)
	{
		MessageBox(NULL, "���[�V�����e�L�X�g�̓ǂݍ��ݎ��s", "�G���[", MB_OK | MB_ICONHAND);
	}

	//���[�V�����ԍ��ɂ�胂�[�V�����̓ǂݍ���
	for (int nCnt = 0; nCnt < (int)MotionName; nCnt++)
	{
		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

																									   //�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "MOTIONSET", 9) == 0)
			{
				break;
			}

		} while (strncmp((buf + nReal), "MOTIONSET", 9) != 0);
	}

	//LOOP���邩?
	do
	{
		fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

		//�ǂݔ�΂�����
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}

		if (strncmp((buf + nReal), "LOOP =", 6) == 0)
		{
			int nLoop = -2;
			sscanf((buf + nReal), "%*6c%d", &nLoop);                    //�p�[�c����ǂݍ���
			if (nLoop = 0) m_bLoop = false;
			else m_bLoop = true;
			break;
		}

	} while (strncmp((buf + nReal), "LOOP =", 6) != 0);

	//���[�V�����̃L�[��
	do
	{
		fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

																								   //�ǂݔ�΂�����
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}

		if (strncmp((buf + nReal), "NUM_KEY =", 9) == 0)
		{
			sscanf((buf + nReal), "%*9c%d", &m_nKeyNum);                    //�p�[�c����ǂݍ���
			break;
		}

	} while (strncmp((buf + nReal), "NUM_KEY =", 9) != 0);

	//���[�V�����̌��݂̃L�[��ǂݍ���
	for (int nCnt = 0; nCnt < m_nKeyNow+1; nCnt++)
	{
		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

																									   //�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "KEYSET", 6) == 0)
			{
				break;
			}

		} while (strncmp((buf + nReal), "KEYSET", 6) != 0);
	}
	
	//���݃L�[�̃t���[������ǂݍ���
	do
	{
		fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

		//�ǂݔ�΂�����
		nReal = 0;
		if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
		for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
		{
			if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
			else
			{
				nReal = nCnt;
				break;
			}
		}

		if (strncmp((buf + nReal), "FRAME =", 7) == 0)
		{
			sscanf(buf + nReal, "%*7c%d", &m_nMotionFrameNum);
			break;
		}

	} while (strncmp((buf + nReal), "FRAME =", 7) != 0);

	//�L�[�̑Ή��p�[�c�ǂݍ���**
	for (int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		WORLD_MATRIX KeyBefore;
		KeyBefore.scl = D3DXVECTOR3(1.0f,1.0f,1.0f);

		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

			//�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "POS =", 5) == 0)
			{
				sscanf(buf + nReal, "%*5c%f%f%f", &KeyBefore.pos.x, &KeyBefore.pos.y, &KeyBefore.pos.z);
				break;
			}

		} while (strncmp((buf + nReal), "POS =", 5) != 0);

		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

			//�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "ROT =", 5) == 0)
			{
				sscanf(buf + nReal, "%*5c%f%f%f", &KeyBefore.rot.x, &KeyBefore.rot.y, &KeyBefore.rot.z);
				break;
			}

		} while (strncmp((buf + nReal), "ROT =", 5) != 0);

		m_partData[nCnt].SetKeyBefore(&KeyBefore);
	}

	fclose(fp);                                                                                                     //�t�@�C�������

	int KeyAfterNum = (m_nKeyNow+1) % m_nKeyNum;

	//�e�L�X�g�ǂݍ���
	if ((fp = fopen(aMotionTextPass, "r")) == NULL)
	{
		MessageBox(NULL, "���[�V�����e�L�X�g�̓ǂݍ��ݎ��s", "�G���[", MB_OK | MB_ICONHAND);
	}

	//���[�V�����ԍ��ɂ�胂�[�V�����̓ǂݍ���
	for (int nCnt = 0; nCnt < (int)MotionName; nCnt++)
	{
		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

																									   //�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "MOTIONSET", 9) == 0)
			{
				break;
			}

		} while (strncmp((buf + nReal), "MOTIONSET", 9) != 0);
	}

	//���[�V�����̎��̃L�[��ǂݍ���
	for (int nCnt = 0; nCnt < KeyAfterNum + 1; nCnt++)
	{
		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

			//�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "KEYSET", 6) == 0)
			{
				break;
			}

		} while (strncmp((buf + nReal), "KEYSET", 6) != 0);
	}

	//�L�[�̑Ή��p�[�c�ǂݍ���
	for (int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		WORLD_MATRIX KeyAfter;
		KeyAfter.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

																									   //�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "POS =", 5) == 0)
			{
				sscanf(buf + nReal, "%*5c%f%f%f", &KeyAfter.pos.x, &KeyAfter.pos.y, &KeyAfter.pos.z);
				break;
			}

		} while (strncmp((buf + nReal), "POS =", 5) != 0);

		do
		{
			fgets(buf, sizeof(buf), fp);                                                               //�s���Ɠǂݍ���

																									   //�ǂݔ�΂�����
			nReal = 0;
			if ((strncmp(buf, "#", 1) == 0) || (strcmp(buf, "\n") == 0)) continue;                          //�R�����g�A�E�g�Ɖ��s��ǂݔ�΂�
			for (int nCnt = 0; nCnt < 256; nCnt++)                                                            //�󔒂ƃ^�u��ǂݔ�΂�
			{
				if ((buf[nCnt] == ' ') || (buf[nCnt] == '\t')) continue;
				else
				{
					nReal = nCnt;
					break;
				}
			}

			if (strncmp((buf + nReal), "ROT =", 5) == 0)
			{
				sscanf(buf + nReal, "%*5c%f%f%f", &KeyAfter.rot.x, &KeyAfter.rot.y, &KeyAfter.rot.z);
				break;
			}

		} while (strncmp((buf + nReal), "ROT =", 5) != 0);

		m_partData[nCnt].SetKeyAfter(&KeyAfter);
	}

	for (int nCnt = 0; nCnt < m_nNumpart; nCnt++)
	{
		D3DXMATRIX ParentMtx;

		if(m_partData[nCnt].GetParent() == -1)
		{
			ParentMtx = WorldMtx;
		}

		else
		{
			int nNumparent = m_partData[nCnt].GetParentID();
			ParentMtx = m_partData[nNumparent].GetResultMtx();
		}
		m_partData[nCnt].Update(m_nMotionFrameNum,m_nMotionFrameNow,&ParentMtx);
	}

	m_nMotionFrameNow++;
	if(m_nMotionFrameNow >= m_nMotionFrameNum)
	{
		m_nMotionFrameNow = 0;
		m_nKeyNow = (m_nKeyNow + 1) % m_nKeyNum;
	}

	fclose(fp);
}

void CModel::Draw(void)
{
	for (int nCnt = 0; nCnt < PART_MAX; nCnt++)
	{
		if (m_partData[nCnt].GetUse() == true)
		{
			m_partData[nCnt].Draw();
		}
	}
}

void CModel::Uninit(void)
{
	for(int nCnt = 0; nCnt < PART_MAX; nCnt++)
	{
		m_partData[nCnt].Uninit();
	}
}

D3DXMATRIX CModel::GetMtxParent(int nNumPart)
{
	if (m_partData[nNumPart].GetUse() == true)
	{
		MessageBox(NULL, "���̃p�[�c�͎g���Ă���܂���", "�G���[", MB_OK | MB_ICONHAND);
	}

	return m_partData[nNumPart].GetResultMtx();
}

bool CModel::GetLoop(void)
{
	return m_bLoop;
}